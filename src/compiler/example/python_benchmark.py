#!/usr/bin/env python3

from random import randint
from timeit import default_timer as time

def fib (n):
  if (n <= 1):
    return n;

  return fib (n - 2) + fib (n - 1)


def fibonacci (arr, size):
  for i in range (size): 
    arr[i] = fib (i)


def insertionSort (arr, n):
  for i in range (1, n):
    key = arr[i]
    j = i - 1;

    while (j >= 0 and arr[j] > key):
      arr[j+1] = arr[j]
      j = j - 1

    arr[j + 1] = key


def merge (arr, start, mid, end):
  left_len = mid - start + 1;
  right_len = end - mid;
  left = [arr[start + i] for i in range (left_len)]
  right = [arr[mid + j + 1] for j in range (right_len)]

  i = 0;
  j = 0;
  k = start;

  while (i < left_len and j < right_len):
    if (left[i] <= right[j]):
      arr[k] = left[i]
      i = i + 1
    else:
      arr[k] = right[j]
      j = j + 1

    k = k + 1

  while (i < left_len):
    arr[k] = left[i]
    i = i + 1
    k = k + 1

  while (j < right_len):
    arr[k] = right[j]
    j = j + 1
    k = k + 1


def mergeSort (arr, start, end):
  if (start >= end):
    return

  mid = (start + end) // 2
  mergeSort (arr, start, mid)
  mergeSort (arr, mid + 1, end)
  merge (arr, start, mid, end)


def printArr (arr, size):
  out = "";
  for i in range (size):
    out = out + str (arr[i]) + " "

  print (out)


def main ():
  print ("--------------------------------------------")
  print ("           Benchmarks from python           ")
  print ("--------------------------------------------")
  # fibonacci
  size = 25
  arr = [0] * size
  print ("Running Fibonacci sequence from 0 to " + str (size))
  start = time ()
  fibonacci (arr, size)
  duration = time () - start

  # printArr (arr, size)
  print ("Duration: " + str (duration) + " seconds")
  print ("")
  
  # insertion sort
  size = 1000;
  arr = [randint (0, 100000) for i in range (size)]
  print ("Running Insertion sort for array size " + str (size))

  start = time ()
  insertionSort (arr, size)
  duration = time () - start

  # printArr (arr, size);
  print ("Duration: " + str (duration) + " seconds")
  print ("")

  # merge sort
  size = 10000;
  arr = [randint (0, 100000) for i in range (size)]
  print ("Running merge sort for array size " + str (size))

  start = time ();
  mergeSort (arr, 0, size - 1)
  duration = time () - start

  # printArr (arr, size);
  print ("Duration: " + str (duration) + " seconds")


if __name__ == '__main__':
  main ()
