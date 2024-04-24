# Producer-Consumer Problem Solution

This code implements a solution to the producer-consumer problem using threads, mutex, and semaphores to synchronize the operations between producer and consumer threads, ensuring they can safely access a shared circular buffer.

## Producer-Consumer Problem

The producer-consumer problem is a classic concurrency problem involving producers putting data into a buffer while consumers retrieve data from it. The key challenge is to ensure that producers do not add data when the buffer is full, and consumers do not attempt to retrieve data when the buffer is empty. This requires effective thread synchronization and mutual exclusion.

## Implementation Details

### Data Structures

- `buffer`: A circular buffer used to store data produced by the producer.
- `counter`: Tracks the current number of elements in the buffer.

### Synchronization Mechanisms

- `pthread_mutex_t mutex`: Mutex lock used to protect access to the buffer, ensuring that only one thread can access the buffer at a time.
- `sem_t empty`: Semaphore tracking the number of empty slots in the buffer.
- `sem_t full`: Semaphore tracking the number of filled slots in the buffer.

### Producer Function

- `producer`: Producer thread function responsible for generating data and placing it into the buffer.
- The producer waits for the empty buffer semaphore to ensure there are available empty slots in the buffer.
- Access to the buffer is protected using a mutex lock to prevent multiple producers from modifying the buffer simultaneously.
- Data is generated and placed into the buffer, then the counter is updated.
- Finally, the full buffer semaphore is incremented to notify consumers that new data is available.

### Consumer Function

- `consumer`: Consumer thread function responsible for retrieving data from the buffer and consuming it.
- The consumer waits for the full buffer semaphore to ensure there is data available in the buffer to consume.
- Access to the buffer is protected using a mutex lock to prevent multiple consumers from modifying the buffer simultaneously.
- Data is retrieved from the buffer and consumed, then the counter is updated.
- Finally, the empty buffer semaphore is incremented to notify producers that new empty slots are available.

### Main Function

- `main`: Main function responsible for creating producer and consumer threads and waiting for them to complete execution.
- Before creating threads, the mutex lock and semaphores are initialized.
- After the producer and consumer threads complete execution, the mutex lock and semaphores are destroyed, and the final counter value is outputted.
