#!/bin/bash

echo "Starting Test 1"
echo -e "--Running Test 1--\n1000000 elements 1 insert\n" > results.txt
./gpu_hashtable 1000000 1 >> results.txt
echo -e "\n\n" >> results.txt
echo "Test 1 Done"


echo "Starting Test 2"
echo -e "--Running Test 2--\n2000000 elements 1 insert\n\n" >> results.txt
./gpu_hashtable 2000000 1 >> results.txt
echo -e "\n\n" >> results.txt
echo "Test 2 Done"


echo "Starting Test 3"
echo -e "--Running Test 3--\n4000000 elements 2 insert\n\n" >> results.txt
./gpu_hashtable 4000000 2 >> results.txt
echo -e "\n\n" >> results.txt
echo "Test 3 Done"


echo "Starting Test 4"
echo -e "--Running Test 4--\n10000000 elements 4 inserts\n\n" >> results.txt
./gpu_hashtable 10000000 4 >> results.txt
echo -e "\n\n" >> results.txt
echo "Test 4 Done"
