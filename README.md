# Strict-Alternation

Make a pair of threads to execute in strict alternation manner.

Implement the idea by zero semaphores, which is made by `sem_init(&sem, 0, 0);`. With this semaphore, `sem_wait(&sem);` immediately blocks the calling thread, since the internal counter 0, set by the last parameter of `sem_init`, gets decremented. Also, `sem_post(&sem);` wakes up any blocked thread, if any, similarly because it increments the internal count of semaphore.

As an example, write a program which create two threads T1 and T2. T1 thread prints odd numbers between 1 to 15. T2 prints even numbers between 2 to 15. The outputs are expected to be sequential. On Mac OS X, DispatchSemaphore can be used for this program safely.
