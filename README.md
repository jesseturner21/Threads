# Threads

This program demonstrates a producer-consumer pattern by creating a producer thread and a consumer thread with synchronized access to a buffer.

## Authors

- Jesse Turner
- Logan Magee

## Description

The program simulates a producer-consumer scenario where one thread produces items and another thread consumes them. It uses a fixed-size buffer to store the items and synchronizes access to the buffer using mutexes and conditional variables.

## Compilation

To compile the program, use the following command:

```bash
gcc threads.c -o threads
```
## Run

To run the program, use the following command:

```bash
./threads
```
