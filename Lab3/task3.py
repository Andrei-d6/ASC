from concurrent.futures import ThreadPoolExecutor
from concurrent.futures import as_completed
from threading import current_thread
import time, random, string

def randomString(stringLength=10000):
    """Generate a random string of fixed length """
    letters = 'ATGCC'
    return ''.join(random.choice(letters) for i in range(stringLength))


subDNA = 'ATG'
times = 0
data = ["lab1", "lab2", "lab3"]
DNA = []

def threadFunction(index, l):
    #time.sleep(random.randint(1, 5))
    print(l)
    if subDNA in DNA[index]:
        return "DNA sequence found in sample " + str(index)
    else:
        return ":("



def main():
   with ThreadPoolExecutor(max_workers = 30) as executor:
      results = executor.map(lambda x: threadFunction(x, ["duru duru"]), random.sample(range(0, 100), 100))

   for result in results:
    print(result)

    #print("found sample " + str(len(result)) + " times")


if __name__ == "__main__":
    random.seed(1)

    for i in range(100):
        DNA.append(randomString())

    main()

