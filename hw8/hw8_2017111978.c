/* This is a skeleton C code for the 8th homework 
 *
 * You can use any parts of this code for the homework
 * Notice! you have to use pthread_cond_signal(), pthread_cond_wait(), pthread_mutex_unlock(), and pthread_mutex_lock()
 *
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>
#include <ctype.h>
#include <fcntl.h>

#define MAXTHREAD 8

/* structure for passing multiple arguments to thread*/
struct thread_data
{
    int thread_id;
    char *buff;                 /* memory buffer pointer */
    long long start;            /* start index in the buffer */
    long long block_size;       /* block size */
    long long counter;          /* counter variable */
};


void * count_words(void*);

/* initialize global variables here if needed */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag1  = PTHREAD_COND_INITIALIZER;
pthread_cond_t flag2  = PTHREAD_COND_INITIALIZER;

int count1=0, count2=0, wait_count=0;

struct thread_data * mail_box = NULL;

/* main function*/
void main(int argc, char **argv)
{
    FILE* fp;
    int nthreads, x, id;
    long long block_size, len;
    int i, reports_in = 0, total_words = 0;
    struct thread_data * data;
    pthread_t *threads;

    /* handle the program arguments */
    if(argc < 3)
    {
        fprintf(stderr, "Usage: ./program_name <the number of thread> <file_path>");
        exit(-1);
    }
    nthreads = atoi(argv[1]);


    /* open file */
    if((fp=fopen(argv[2],"r+"))==NULL)
    {
        printf("Error opening file!\n");
        exit(-1);
    }

    threads = malloc(nthreads*sizeof(pthread_t));


    /* check the file size*/
    fseek(fp,0,SEEK_END);
    len = ftell(fp);

    printf("total length : %lld\n", len);

    /* copy the contents of file to a memory buffer */
    fseek(fp,0,SEEK_SET);
    char* buff=(char*)malloc(sizeof(char)*len);
    fread(buff,1,len, fp);

    /* calculate size of the block that will be assigned to each thread */
    block_size = len/nthreads;
    fseek(fp,SEEK_SET,0);


    data = (struct thread_data*)calloc(nthreads, sizeof(struct thread_data));
    /* store the block information in the argument structure */
    for(i=0;i<nthreads;i++)
    {
        data[i].thread_id = i;
        data[i].buff = buff;
        data[i].start = block_size*i;
        data[i].block_size = block_size;
        data[i].counter=0;
    }

    /* record the current time */


	pthread_mutex_lock(&lock);

	for(i = 0;i<nthreads;i++) //create thread
    {
        pthread_create(&threads[i], NULL, count_words, ((void*)&data[i]));
        printf("Created Thread %d : %lld\n", i, data[i].start);
    }

	printf("Wait scheduler for 10s\n");
	sleep(10); //스케줄러가 thread 다 만들 때까지 기다립니다.


    struct timeval start_time,end_time;
    gettimeofday(&start_time, NULL);
	while(reports_in < nthreads)
    {
        printf("MAIN : waiting for flag to go up\n");
	    pthread_cond_wait(&flag1, &lock);

        printf("MAIN : Wow! flag was raised, I have the lock\n");
	    total_words+=mail_box->counter;

	    for(i = 0;i<nthreads;i++)
        {
	        if(mail_box == &data[i]) //check certificated thread by starting point
            {
	            pthread_join(threads[i], NULL);
            }
        }

	    mail_box = NULL;
	    pthread_cond_signal(&flag2);
	    reports_in++;
    }

    fclose(fp);
    free(buff);
    free(threads);
    free(data);

    /* calculate the elapsed time */
	gettimeofday(&end_time, NULL);
	double operating_time = (double)(end_time.tv_sec)+(double)(end_time.tv_usec)/1000000.0-(double)(start_time.tv_sec)-(double)(start_time.tv_usec)/1000000.0;

    /* print the total number of words in the file */
    printf("\n\n[MAIN]Total Word Count: %d \n", total_words);
	printf("[MAIN]Elapsed: %f seconds\n", (double)operating_time);

    return;
}


/* function that will be threaded */
void *count_words(void *a)
{
    struct thread_data *data = (struct thread_data *) a;
    long long start=data->start;
    long long blocksize=data->block_size;
    long long i;
    char c, prevc = '\0';

    /* count words in the block assigned to each thread */
    for(i = start;i < (start + blocksize);i++)
    {
        c = data->buff[i];
        //printf("loop %s\n", c);

        if(!isalnum(c) && isalnum(prevc))
        {
            data->counter++;
        }
        prevc = c;
    }

    pthread_mutex_lock(&lock);
    printf("Thread start %d : have lock, storing data\n", data->thread_id);


    if(mail_box != NULL)
    {
        printf("Wait for other thread %d\n", data->thread_id);
        pthread_cond_wait(&flag2, &lock);
    }

    mail_box = data;

    printf("Thread start %d : raising flag\n", data->thread_id);
    pthread_cond_signal(&flag1);
    printf("Thread start %d : unlocking box\n\n", data->thread_id);
    pthread_mutex_unlock(&lock);
    count2++;

    return NULL;
}



