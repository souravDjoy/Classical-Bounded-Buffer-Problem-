
//adding the libraries needed for this program
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include<unistd.h>


//defining some variables
#define BUFFER_SIZE 5
typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE];

//defining the semaphore and mutex
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

//to keep trace of the index in the buffer
int insertIndex = 0;
int removeIndex = 0;

//these are the global variables to use the number of producer and consumer threads defined by the user
int producerNo=0;
int consumerNo=0;

//defining the condition for the while loops in the threads
#define true 1


//prototyping other methods we are gonna use
int insertItem(buffer_item item);
void *producer(void *param);
int removeItem(buffer_item *item);
void *consumer(void *param);

unsigned int pid_list[100], cid_list[100];//to hold the pids of the threads in the arrays to give them unique ids in sequence like 0,1,2,3,etc.



int main(int argc, char *argv[]){//the main takes argc number of arguments and they are stores in argv[]
	int timeUser, producerthreads, consumerthreads;//three variables to store three arguments from the users

	//three arguments from std input are being stored for later execution of the program and threads
    timeUser = atoi(argv[1]);
	producerthreads = atoi(argv[2]);
	consumerthreads = atoi(argv[3]);

	//storing the number of producer and consumer threads for global use of the threads
	producerNo=producerthreads;
	consumerNo=consumerthreads;

	//initializing all the elements of the buffer to -1
    for(int i=0;i<BUFFER_SIZE; i++){
    buffer[i]= -1;
    }

	//initializing semaphores and mutex lock
	sem_init(&full, 0, BUFFER_SIZE);
	sem_init(&empty, 0, 0);
	pthread_mutex_init(&mutex, NULL);

	pthread_t pid, cid;//defining the producer and consumer id

	for(int i = 0; i < producerthreads; i++){//creating these many producer threads
		pthread_create(&pid,NULL,&producer,NULL);//creating the producer thread

		pid_list[i]=pid;//adding the pids to the list
	}

	for(int j = 0; j < consumerthreads; j++){//creating these many consumer threads
		pthread_create(&cid,NULL,&consumer,NULL);//creating the consumer thread

		cid_list[j]=cid;//adding the cids to the list
	}

	sleep(timeUser);//sleep the program

    //destroying the mutex and semaphores
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

	exit(0);//terminating the program

	return 0;
}


//this method inserts item into the buffer
int insertItem(buffer_item item)
{
	buffer[insertIndex] = item;//inserting the item in the argument
	insertIndex = (insertIndex + 1) % BUFFER_SIZE;//adjusting the insertPointer
	return 0;
}

//this method is the producer thread
void *producer(void *param) {
buffer_item item;//declaring some variables
int time2;

while (true) {

time2=rand()%4;// sleep for a random period of time: 0-4 seconds */
sleep(time2);//thread sleeps for this long
sem_wait(&full);//to wait on the semaphore
pthread_mutex_lock(&mutex);//locking the mutex


item = rand();//generate a random number

insertItem(item);//insert an item

int index=0;//just a variable to print the id of this process to the screen

unsigned int indexCom=(unsigned int)pthread_self();//retrieving the pid of this process

for(int i=0;i<producerNo; i++){
    if(pid_list[i]==indexCom)//comparing if this pid exists in the array
        index=i;//locking that index if there is a match
}


printf("Producer %d inserted item %d into buffer[%d]\n", index, item, insertIndex);//prints this

pthread_mutex_unlock(&mutex);//unlocking the mutex
sem_post(&empty);//waking up the blocked semaphore
}
}

//this method removes item from the buffer
int removeItem(buffer_item *item)
{
	*item = buffer[removeIndex];//removing an item from the buffer
	removeIndex = (removeIndex + 1) % BUFFER_SIZE;//adjusting the removePointer
	return 0;
}

//this method is the consumer thread
void *consumer(void *param) {
buffer_item item;//declaring some variables
int time3;

while (true) {

time3=rand()%4;/* sleep for a random period of time: 0-4 seconds */
sleep(time3);//thread sleeps for this long

sem_wait(&empty);//to wait on the semaphore
pthread_mutex_lock(&mutex);//locking the mutex


removeItem(&item);//removing an item

int index2=0;//just a variable to print the id of this process

unsigned int indexCom2=(unsigned int)pthread_self();//retrieving the cid of this process

for(int j=0;j<consumerNo; j++){
    if(cid_list[j]==indexCom2)//comparing the cid to the items of the list to see if that exists
        index2=j;//locking the index if there is a match
}

printf("Consumer %d removed item %d from buffer[%d]\n", index2, item, removeIndex);//prints this

pthread_mutex_unlock(&mutex);//unlocking the mutex
sem_post(&full);//waking up the blocked semaphore
}
}
