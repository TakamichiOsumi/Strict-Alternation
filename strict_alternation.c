#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dispatch/dispatch.h>

static pthread_t handlers[2];
static dispatch_semaphore_t semaphore;

typedef enum { T1, T2 } thread_id;

static void*
thread_fn_callback(void *arg){
    uintptr_t count = 1, tid = (uintptr_t) arg;

    assert(tid == T1 || tid == T2);

    /* Let T2 wait until T1 sends signal */
    if (tid == T2){
	count = 2;
	dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
    }

    for (; count <= 15; count = count + 2){
	printf("[%s] i=%lu\n", tid == T1 ? "T1" : "T2", count);
	dispatch_semaphore_signal(semaphore);
	dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
    }

    /*
     * T2 breaks above for loop first.
     * In that case (only), send a signal from T2 to T1
     * to let T1 break the last wait.
     */
    if (tid == T2)
	dispatch_semaphore_signal(semaphore);

    return NULL;
}

static void
thread_create(pthread_t *handler, void *arg){
    if (pthread_create(handler, NULL,
		       thread_fn_callback, arg) != 0){
	perror("pthread_create");
	exit(-1);
    }
}

int
main(int argc, char **argv){

    semaphore = dispatch_semaphore_create(0);

    thread_create(&handlers[0], (void *) T1);
    thread_create(&handlers[1], (void *) T2);
    pthread_join(handlers[0], NULL);
    pthread_join(handlers[1], NULL);

    dispatch_release(semaphore);

    return 0;
}
