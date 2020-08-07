#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>


void *thread_function(void *arg);

void job1(long id);
void job2(long id);

void producer(long arg);
void consumer(long arg);
bool isEmpty();
bool isFull();
void append(int v);
int consume();

int A;
int B;
sem_t sum_lock;
sem_t sum_lock2;
sem_t s, n, e;
int buffer[3];
size_t bufsize = 3;
int itemCount = 0;
int front;
int rear;

int main(int argc, char **argv) {

    int num_threads;
    pthread_t *threads;
    long *tid;

    const int sizeofbuffer = bufsize;

    if(sem_init(&s, 0, 1) || sem_init(&n, 0, 0) || sem_init(&e, 0, sizeofbuffer)) {

        printf("Error initializing semaphores\n");

        return 0;

    }

    A = 0;
    B = 0;

    if(argc < 2) {

        printf("Usage: %s [NUMBER IF THREADS]\n", argv[0]);

        return 0;
    }

    num_threads = atoi(argv[1]);
    printf("Launching %i threads.\n", num_threads);

    threads = (pthread_t*) calloc(num_threads, sizeof(pthread_t));

    if(sem_init(&sum_lock, 0, 1) || sem_init(&sum_lock2, 0, 1)) {

        printf("Error initializing semaphore\n");
        perror("sem_init");

    }

    int pcr;
    for(long i=0; i < num_threads; i++) {

        if( (pcr = pthread_create(&threads[i], NULL, &thread_function,
                                 (void*)i))) {

            printf("Create Failed: %ld", i);

        }

    }

    for (int i=0; i<num_threads; i++) {

        pthread_join(threads[i], NULL);

    }

    printf("A=%d\n", A);
    printf("B=%d\n", B);

    free(threads);

}

void *thread_function(void * arg) {

    long my_id = (long) arg;
    if(my_id%2 == 0 ){
        job1(my_id);
        producer(my_id);

    } else {

        job2(my_id);
        consumer(my_id);
    }
}

//methods

void job1(long id) {

    printf("I am a producer with ID %ld\n", id);
    for(int i = 0; i < 5; i++) {
        sem_wait(&sum_lock);
        A+=1;
        sem_post(&sum_lock);
        usleep(rand()%10 + 1);
        sem_wait(&sum_lock2);
        B+=2;
        sem_post(&sum_lock2);
        usleep(rand()%10 + 1);
    }
}

void job2(long id) {

    printf("I am a consumer with ID %ld\n", id);
    for(int i = 0; i < 5; i++) {
        sem_wait(&sum_lock2);
        B+=3;
        sem_post(&sum_lock2);
        usleep(rand()%10 + 1);
        sem_wait(&sum_lock);
        A+=1;
        sem_post(&sum_lock);
        usleep(rand()%10 + 1);
    }
}

void producer(long arg) {

    long id = (long) arg;
    int m;
    int check = 20;
    sem_getvalue(&n, &m);
    while(check != 0) {

        //produce
        int v = rand() % 10 + 1;
        int sem;
        /*sem_getvalue(&e, &sem);
        printf("Producer before append and semaphores sem_wait(e) %d\n", sem);*/

        sem_wait(&e);
        sem_wait(&s);

        //append
        append(v);
        printf("%ld Produces: %d\n", id, v);
        sem_post(&s);
        sem_post(&n);
        check--;
        if(check == 0) {
            break;
        }
    }
}


void consumer(long arg) {

    long id = (long) arg;
    int check = 20;
    int m;
    sem_getvalue(&n, &m);
    while(check != 0) {

        sem_wait(&n);
        sem_wait(&s);
        int c = consume();
        sem_post(&s);
        sem_post(&e);
        printf("%ld Consumes %d\n", id, c);
        check--;
        if(check == 0) {
            break;
        }
    }

}

bool isEmpty() {

    return itemCount == 0;

}

bool isFull() {

    return itemCount == bufsize;

}

void append(int v) {

    if(!isFull()) {

        if(rear == bufsize - 1) {

            rear = -1;

        }

        buffer[++rear] = v;


    }

}

int consume() {

    int data = buffer[front++];

    if(front == bufsize) {

        front = 0;

    }

    itemCount--;
    return data;

}