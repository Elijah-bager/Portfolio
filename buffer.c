// NOTE: DO NOT CHANGE THIS COMMENT!! CSE 2431 Lab 2 buffer.c AU 25 052103 

// STUDENT NAME: Elijah Baugher

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 7

typedef unsigned int buffer_item;

/* all data declared below at file scope is shared by all threads */

buffer_item buffer[BUFFER_SIZE]; /* shared data buffer */

/* Declaration of pthread mutex locks here; these are shared by threads. */
/* See the lab instructions to find out how to initialize the locks in main, */
/* and also how to acquire the lock before entering a critical section, */
/* and how to release the lock after exiting a critical section. */

pthread_mutex_t prod_mutex; /* shared lock used to ensure only one producer thread */
			    /* can access buffer at any given time. */

pthread_mutex_t cons_mutex; /* shared lock used to ensure only one consumer thread */
			    /* can access buffer at any given time. */

pthread_mutex_t output_mutex; /* shared lock used to ensure only one thread */
			     /* can write output at any given time. */

/* See the lab instructions to find out how to initialize */
/* the semaphores in main. */
sem_t empty;	/* shared data empty counting semaphore */

sem_t full;	/* shared data full counting semaphore */

/* shared data buffer indexes */

int in = 0;	/* shared data/index to be used by producers for next position in */
		/* buffer to insert produced item */

int out = 0;	/* shared data/index to be used by consumers for next position in */
		/* buffer from which to consume item */

unsigned int seed = 110; 	/* seed value for rand_r; initialized to 110 */

unsigned int *seedp = &seed; 	/* pointer to seed for rand_r(), a  */
				/* re-entrant, and therefore thread-safe, */
				/* version of rand() */

void *producer (void *param);
void *consumer (void *param);
void insert_item (buffer_item);
void remove_item (buffer_item *);

/* function shared by all producer threads; DO NOT CHANGE THIS CODE */
void *producer (void *param) {
	buffer_item rand;
	while (1) {
		/* generate a random number between 0 and 100 */
		rand = rand_r(seedp) % 101;
		/* sleep for rand * 1000 microseconds */
		usleep(rand * 1000);
		/* insert item into buffer */
		insert_item(rand);
	}
}

/* function shared by all consumer threads; DO NOT CHANGE THIS CODE */
void *consumer (void *param) {
	buffer_item rand;
	while (1) {
		/* generate a random number between 0 and 100 */
		rand = rand_r(seedp) % 101;
		/* sleep for rand * 1000 microseconds */
		usleep(rand * 1000);
		/* remove item from buffer */
		remove_item(&rand);
	}
}

/* function shared by all producer threads; COMPLETE THIS CODE */
void insert_item(buffer_item item) {
	sem_wait(&empty); /* decrement empty semaphore */
	pthread_mutex_lock(&prod_mutex); /* acquire prod_mutex lock */
	buffer[in] = item; /* insert item into buffer at position in */
	in = (in + 1) % BUFFER_SIZE; /* update in to next position in circular buffer */
	/* output_mutex lock should be held before writing output */
	printf("producer produced %d\n", item);
	/* output_mutex lock should be released after writing output */
	pthread_mutex_unlock(&prod_mutex); /* release prod_mutex lock */
	sem_post(&full); /* increment full semaphore */
}

/* function shared by all consumer threads; COMPLETE THIS CODE */
void remove_item(buffer_item *item) {
	sem_wait(&full); /* decrement full semaphore */
	pthread_mutex_lock(&cons_mutex); /* acquire cons_mutex lock */
	*item = buffer[out]; /* remove item from buffer at position out */
	out = (out + 1) % BUFFER_SIZE; /* update out to next position in circular buffer */
	/* output_mutex lock should be held before writing output */
	printf("\tconsumer consumed %d\n", *item);
	/* output_mutex lock should be released after writing output */
	pthread_mutex_unlock(&cons_mutex); /* release cons_mutex lock */
	sem_post(&empty); /* increment empty semaphore */
}

/* COMPLETE CODE FOR MAIN - SEE COMMENTS BELOW */
int main(int argc, char *argv[]) {
	int sleep_time = atoi(argv[1]);
	int num_prod_threads = atoi(argv[2]);
	int num_cons_threads = atoi(argv[3]);
	int i; /* loop counter for for-loop to create threads */
	pthread_mutex_init(&prod_mutex, NULL); /* initialize prod_mutex lock */
	pthread_mutex_init(&cons_mutex, NULL); /* initialize cons_mutex lock */
	pthread_mutex_init(&output_mutex, NULL); /* initialize output_mutex lock */
	
	sem_init(&full,0,0); /* initialize full semaphore using sem_init */

	sem_init(&empty,0,BUFFER_SIZE); /* initialize empty semaphore using sem_init */

		
	/* Create single thread id (tid) and attribute variable (attr) for the multiple threads */
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	/* create num_prod_threads with for-loop */
	for(int i = 0; i < num_prod_threads; i++) {
		pthread_create(&tid, &attr, producer, NULL);
	}

	/* create num_cons_threads with for-loop */	
	for(int i = 0; i < num_cons_threads; i++) {
		pthread_create(&tid, &attr, consumer, NULL);
	}

	/* sleep for number of seconds equal to second param on command line */
	sleep(sleep_time);

	return 0;
}

