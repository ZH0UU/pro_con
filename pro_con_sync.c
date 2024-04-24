#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define BUFFER_SIZE 4

int buffer[BUFFER_SIZE]; // buffer
int counter = 0;         // counter
int loop;                // loop

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // init mutex
sem_t empty;                                       // empty buffer semaphore
sem_t full;                                        // full buffer semaphore

void *producer(void *arg)
{
    int in = 0;
    for (int i = 0; i < loop; i++)
    {
        sem_wait(&empty);           // wait buffer empty
        pthread_mutex_lock(&mutex); // lock
        buffer[in] = i;             // produce
        printf("Produced: %d\n", i);
        in = (in + 1) % BUFFER_SIZE;  // circular buffer
        counter++;                    // increase counter
        pthread_mutex_unlock(&mutex); // unlock
        sem_post(&full);              // add full buffer semaphore
    }
    in = (in + BUFFER_SIZE - 1) % BUFFER_SIZE; // last_in
    printf("producer: last_in[%d] = %d\n", in, buffer[in]);
    return EXIT_SUCCESS;
}

void *consumer(void *arg)
{
    int out = 0;
    for (int i = 0; i < loop; i++)
    {
        sem_wait(&full);                 // wait buffer full
        pthread_mutex_lock(&mutex);      // lock
        int next_consumed = buffer[out]; // consume
        printf("\t\tConsumed: %d\n", next_consumed);
        out = (out + 1) % BUFFER_SIZE; // circular buffer
        counter--;                     // decrease counter
        pthread_mutex_unlock(&mutex);  // unlock
        sem_post(&empty);              // add empty buffer semaphore
    }
    out = (out + BUFFER_SIZE - 1) % BUFFER_SIZE; // last_out
    printf("\t\tconsumer: last_out[%d] = %d\n", out, buffer[out]);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[], char *envp[])
{
    loop = atoi(argv[1]);             // loop
    sem_init(&empty, 0, BUFFER_SIZE); // init empty buffer semaphore as BUFFER_SIZE
    sem_init(&full, 0, 0);            // init full buffer semaphore as 0
    pthread_t tid[2];
    pthread_create(&tid[0], NULL, producer, NULL); // create producer thread
    pthread_create(&tid[1], NULL, consumer, NULL); // create consumer thread
    for (int i = 0; i < 2; i++)
        pthread_join(tid[i], NULL); // wait for thread termination
    if (counter == 0)
        printf("congratulations! last_counter = %d, it's thread safety!\n", counter);
    else
        printf("last_counter = %d, it's not thread safety!\n", counter);
    pthread_mutex_destroy(&mutex); // destroy mutex
    sem_destroy(&empty);           // destroy empty buffer semaphore
    sem_destroy(&full);            // destroy full buffer semaphore
    return EXIT_SUCCESS;
}
// 20223102 저우뤄펑
// 2024/4/24