// Authors: Jesse Turner & Logan Magee
//
// This program demonstrates a producer-consumer pattern by creating a producer thread and a
// consumer thread with synchronized access to a buffer and a counter for how many items in the
// buffer are filled. The producer thread first acquires the buffer mutex and fills the buffer with
// new items, signaling to the consumer thread and releasing the mutex once it completes. The
// consumer thread then acquires the buffer mutex and consumes the produced items in a FIFO fashion,
// signaling to the producer thread and releasing the mutex once it completes.
//
// This process is repeated ad infinitum until the program is forcefully closed.

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 5

// A buffer for items to be produced in and consumed from
static int buffer[BUFFER_SIZE];
// The number of items in the buffer currently filled
static int count = 0;

// The mutex over the above buffer and count variables
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Conditional variables for signaling between threads when their respective tasks are completed
static pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
static pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

static void *producer(void *arg) {
    int i = 0;
    while (1) {
        // Acquire exclusive access to the buffer and count variables
        pthread_mutex_lock(&mutex);

        // Wait for the consumer thread to signal us when the buffer is full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_producer, &mutex);
        }

        // Produce an item and print it
        buffer[count++] = ++i;
        printf("Produced: %d\n", i);

        sleep(1);

        // Signal to the consumer thread that it can continue
        pthread_cond_signal(&cond_consumer);

        // Release exclusive access to the buffer and count, allowing the consumer to acquire access
        pthread_mutex_unlock(&mutex);
    }
}

static void *consumer(void *arg) {
    while (1) {
        // Acquire exclusive access to the buffer and count variables
        pthread_mutex_lock(&mutex);

        // Wait for the producer thread to signal us when the buffer is empty
        while (count == 0) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        // Consume an item and print it
        int i = buffer[BUFFER_SIZE - count--];
        printf("Consumed: %d\n", i);

        sleep(1);

        // Signal to the producer thread that it can continue
        pthread_cond_signal(&cond_producer);

        // Release exclusive access to the buffer and count, allowing the producer to acquire access
        pthread_mutex_unlock(&mutex);
    }
}

int main(void) {
    pthread_t producer_thread, consumer_thread;

    // Create the producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Join the producer and consumer threads to the main thread so that we wait for them to
    // complete
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}