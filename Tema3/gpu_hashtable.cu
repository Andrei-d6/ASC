#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <ctime>
#include <sstream>
#include <string>

#include<math.h>
#include "gpu_hashtable.hpp"

#define PERFORMACE_VIEW 1

#define MIN_LOAD_FACTOR 0.9
#define EMPTY_KEY 	0
#define EMPTY_VALUE 0

#define NUM_BLOCKS(n) (((n) + 255) / 256)
#define NUM_THREADS 256


/* Functie de hash */
__device__ int hash_func(int k, int htable_size)
{
	k = ((k >> 16) ^ k) * 0x45d9f3b;
    k = ((k >> 16) ^ k) * 0x45d9f3b;
	k = (k >> 16) ^ k;
	
	/* rezultatul final trebuie
	 * sa se incadreze in limita data - size
	 */
    return k % htable_size;
}

/* Initializarea hashtable-ului */
__global__ void gpu_init_hashTable(entry_t *htable, const int size)
{
	unsigned int threadId = blockIdx.x * blockDim.x + threadIdx.x;

	/* initializarea se face in limitele dimensiunii */
	if (threadId < size) {
		htable[threadId].key = EMPTY_KEY;
		htable[threadId].value = EMPTY_VALUE;
	}
}

/* INIT HASH
 */
GpuHashTable::GpuHashTable(int size) {

	/* alocare de memorie pentru htable */
	cudaMalloc(&htable, size * sizeof(entry_t));
	DIE(htable == 0, "cudaMalloc htable");

	/* alocare de memorie pentru contorul elementelor din hashtable */
	cudaMallocManaged(&count, sizeof(unsigned int));
	DIE(count == 0, "cudaMallocManaged count");

	/* initializarea valorilor din hashtable */
	gpu_init_hashTable<<<NUM_BLOCKS(size), NUM_THREADS>>>(htable, size);
	cudaDeviceSynchronize();

	/* initializarea marimii hashtable-ului */
	htable_size = size;
	/* initializarea contorului de elemente din hashtable */
	*count = 0;
}

/* DESTROY HASH
 */
GpuHashTable::~GpuHashTable() {
	/* eliberarea memoriei pentru hashtable */
	if (htable != 0)
		cudaFree(htable);

	/* eliberarea memoriei pentru contor */
	if (count != 0)
		cudaFree(count);
}

/* RESHAPE HASH
 */
void GpuHashTable::reshape(int numBucketsReshape) {
	int size = numBucketsReshape;

	if (htable != 0)
		cudaFree(htable);
	
	/* alocare de memorie pentru htable */
	cudaMalloc(&htable, size * sizeof(entry_t));
	DIE(htable == 0, "cudaMalloc htable");

	if (count != 0)
		cudaFree(count);
	
	/* alocare de memorie pentru contorul elementelor din hashtable */
	cudaMallocManaged(&count, sizeof(unsigned int));
	DIE(count == 0, "cudaMallocManaged count");

	/* initializarea valorilor din hashtable */
	gpu_init_hashTable<<<NUM_BLOCKS(size), NUM_THREADS>>>(htable, size);
	cudaDeviceSynchronize();

	/* initializarea marimii hashtable-ului */
	htable_size = size;
	/* initializarea contorului de elemente din hashtable */
	*count = 0;
}

/* Construirea unui hashtable pe baza unui alt hashtable */
__global__ void gpu_hashtable_copy(entry_t *old_htable, entry_t *new_htable, const int old_htable_size, const int new_htable_size)
{
	unsigned int threadId = blockIdx.x * blockDim.x + threadIdx.x;

	/* vream sa adaugam elemente care
	 * se gasesc in limitele vechiului hashtable
	 */
	if (threadId >= old_htable_size)
		return;

	/* cheia pentru thread-ul curent din vechiul hashtable */
	int key = old_htable[threadId].key;

	/* pentru perechile goale
	 * nu avem ce aduaga in noul hashtable
	 */
	if (key == EMPTY_KEY)
		return;
		
	int current_key;
	int index = hash_func(key, new_htable_size);	
	
	/* cautarea unui slot valabil pentru adaugarea unei noi perechi cheie:valoare */
	while (1) {
		/* verificam (si actualizam) atomic faptul ca slot-ul este liber */
		current_key = atomicCAS(&new_htable[index].key, EMPTY_KEY, key);
		
		/* daca a fost gasit un slot liber este folosit acesta */
		if (current_key == EMPTY_KEY || current_key == key) {
			new_htable[index].value = old_htable[threadId].value;
			return;
		}

		/* daca slot-ul curent este ocupat cautam in continuare */
		index = (index + 1) % new_htable_size;
	}
}

/* Adugarea unei noi perechi in hashtable */
__global__ void gpu_hashtable_insert(entry_t *htable, unsigned int *count, const int htable_size, const int *keys, const int *values, const int numKeys)
{
	unsigned int threadId = blockIdx.x * blockDim.x + threadIdx.x;

	/* id-ul trebuie sa se afle in limitele date */
	if (threadId >= numKeys)
		return;

	int key = keys[threadId];
	int value = values[threadId];
	int current_key;
	int index = hash_func(key, htable_size);

	/* pentru cheia curenta cautam un slot liber pentru a o adauga */
	while (1) {
		/* verificam (si actualizam) atomic faptul ca slot-ul este liber */
		current_key = atomicCAS(&htable[index].key, EMPTY_KEY, key);

		/* daca a fost gasit un slot liber este folosit acesta */
		if (current_key == EMPTY_KEY || current_key == key) {
			htable[index].value = value;
			
			/* daca spatiul era liber inseamna ca a fost adaugat un nou element 
			 * alternativ, aceasta inserare ar fi presupus actualizarea unei valori
			 * caz in care nu ar creste numarul de elemente din hashmap
			 */
			if (current_key == EMPTY_KEY)
				atomicAdd(count, 1);
			return;
		}

		/* daca slot-ul curent este ocupat cautam in continuare */
		index = (index + 1) % htable_size;
	}
}

/* INSERT BATCH
 */
bool GpuHashTable::insertBatch(int *keys, int* values, int numKeys) {
	
	int *device_keys;
	int *device_values;
	int old_htable_size;
	entry_t *new_htable;

	/* verificam faptul ca avem suficient loc pentru noile elemente */
	if (*count + numKeys > MIN_LOAD_FACTOR * htable_size) {

		/* //////////////////////////////////////////////////////////////
		 * ///////////// Redimensionarea hashthable-ului  ///////////////
		 * //////////////////////////////////////////////////////////////
		 */

		old_htable_size = htable_size;
		/* noua marime se caluleaza in functie de gradul de ocupare dorit */
		htable_size = (*count + numKeys) / MIN_LOAD_FACTOR;
		
		/* alocarea memoriei pentru noul hashtable */
		cudaMalloc(&new_htable, htable_size * sizeof(entry_t));
		DIE(new_htable == 0, "cudaMalloc new_htable");

		/* initializarea noului hashtable */
		gpu_init_hashTable<<<NUM_BLOCKS(htable_size), NUM_THREADS>>>(new_htable, htable_size);
		cudaDeviceSynchronize();

		/* introducerea datelor existente in noul hashtable */
		gpu_hashtable_copy<<<NUM_BLOCKS(htable_size), NUM_THREADS>>>(htable, new_htable, old_htable_size, htable_size);
		cudaDeviceSynchronize();

		/* eliberarea memoriei vechiului hashtable */
		cudaFree(htable);

		/* actualizarea noului hashtable */
		htable = new_htable;
	}

	/* alocare de memorie pentru parametrii pentru kernel */
	cudaMalloc((void **) &device_keys, numKeys * sizeof(int));	
	DIE(device_keys == 0, "cudaMalloc device_keys");

	cudaMalloc((void **) &device_values, numKeys * sizeof(int));
	DIE(device_values == 0, "cudaMalloc device_keys");
	
	/* copierea datelor pentru chei si valori */
	cudaMemcpy(device_keys, keys, numKeys * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(device_values, values, numKeys * sizeof(int), cudaMemcpyHostToDevice);

#if PERFORMACE_VIEW
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start);
#endif

	/* inserarea noilor elemente in hashtable */
	gpu_hashtable_insert<<<NUM_BLOCKS(numKeys), NUM_THREADS>>>(htable, count, htable_size, device_keys, device_values, numKeys);
	cudaDeviceSynchronize();

#if PERFORMACE_VIEW
	cudaEventRecord(stop);
	cudaEventSynchronize(stop);

	float time = 0;
	cudaEventElapsedTime(&time, start, stop);
	float seconds = time / 1000.0f;
	printf("Inserted %d elements in %f ms (%f million keys/second)\n", numKeys, time, numKeys / (double)seconds / 1000000.0f);
#endif

	/* eliberarea memoriei pentru parametrii kernel-ului */
	cudaFree(device_keys);
	cudaFree(device_values);

	return true;
}

/* Returnarea valorilor pentru un set de chei */
__global__ void gpu_hashtable_lookup(entry_t *htable, const int htable_size, const int *keys, int *values, const int numKeys)
{
	unsigned int threadId = blockIdx.x * blockDim.x + threadIdx.x;

	/* id-ul trebuie sa se afle in limitele date */
	if (threadId >= numKeys)
		return;

	int key; 
	int index;
	int timeout = 0;
	key = keys[threadId];
	index = hash_func(key, htable_size);

	/* se cauta valoarea asociata cheii date */
	while (true) {

		/* daca a fost parcurs intreg hashtable-ul
		 * atunci cheia data nu se regaseste in acesta
		 */
		if (timeout == htable_size) {
			values[threadId] = EMPTY_VALUE;
			return;
		}

		/* daca a fost gasita o potrivire
		 * atunci intoarcem valoarea de la acel index
		 */
		if (htable[index].key == key) {
			values[threadId] = htable[index].value;
			return;
		}

		/* daca index-ul curent nu avea cheia cautata 
		 * atunci continuam cautarea
		 */
		index = (index + 1) % htable_size;
		timeout += 1;
	}
}

/* GET BATCH
 */
int* GpuHashTable::getBatch(int* keys, int numKeys) {
	int *values;
	int *device_keys;
	int *device_values;

	/* alocare de memorie pentru a retine valorile cautate */
	values = (int *)malloc(numKeys * sizeof(int));

	/* alocare de memorie pentru parametrii functiei de cautare */
	cudaMalloc((void **) &device_keys, numKeys * sizeof(int));	
	DIE(device_keys == 0, "cudaMalloc device_keys");

	cudaMalloc((void **) &device_values, numKeys * sizeof(int));
	DIE(device_values == 0, "cudaMalloc device_keys");

	/* copierea datelor pentru cheile cautate */
	cudaMemcpy(device_keys, keys, numKeys * sizeof(int), cudaMemcpyHostToDevice);

#if PERFORMACE_VIEW
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start);
#endif

	/* cautarea valorilor asociate cheilor date */
	gpu_hashtable_lookup<<<NUM_BLOCKS(numKeys), NUM_THREADS>>>(htable, htable_size, device_keys, device_values, numKeys);
	cudaDeviceSynchronize();

#if PERFORMACE_VIEW
	cudaEventRecord(stop);
	cudaEventSynchronize(stop);

	float time = 0;
	cudaEventElapsedTime(&time, start, stop);
	float seconds = time / 1000.0f;
	printf("Got %d elements in %f ms (%f million keys/second)\n", numKeys, time, numKeys / (double)seconds / 1000000.0f);
#endif

	/* copierea valorilor gasite pe Host */
	cudaMemcpy(values, device_values, numKeys * sizeof(int), cudaMemcpyDeviceToHost);

	return values;
}

/* GET LOAD FACTOR
 * num elements / hash total slots elements
 */
float GpuHashTable::loadFactor() {
	/*
	 * count - numarul de elemente aflate in hashtable
	 * htable_size - spatiul total alocat pentru hashtable
	 */
	return (float)*count /(float)htable_size; // no larger than 1.0f = 100%
}

/*********************************************************/

#define HASH_INIT GpuHashTable GpuHashTable(1);
#define HASH_RESERVE(size) GpuHashTable.reshape(size);

#define HASH_BATCH_INSERT(keys, values, numKeys) GpuHashTable.insertBatch(keys, values, numKeys)
#define HASH_BATCH_GET(keys, numKeys) GpuHashTable.getBatch(keys, numKeys)

#define HASH_LOAD_FACTOR GpuHashTable.loadFactor()

#include "test_map.cpp"
