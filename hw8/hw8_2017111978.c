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

#define MAXTHREAD 8

/* structure for passing multiple arguments to thread*/
struct thread_data
{
    char *buff;                 /* memory buffer pointer */
    long long start;            /* start index in the buffer */
    long long block_size;       /* block size */
    long long counter;          /* counter variable */
};


void * count_words(void*);

/* initialize global variables here if needed */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag  = PTHREAD_COND_INITIALIZER;
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
    if((fp=fopen(argv[2],"r"))==NULL)
    {
        printf("Error opening file!\n");
        exit(-1);
    }

    threads = (pthread_t *)calloc(nthreads, sizeof(pthread_t));
    

    /* check the file size*/
    fseek(fp,0,SEEK_END);
    len = ftell(fp);

    printf("total buff size : %lld\n", len);
    /* copy the contents of file to a memory buffer */
    char* buff=(char*)calloc(len, sizeof(char));
    fread(buff,1,len, fp);

    /* calculate size of the block that will be assigned to each thread */
    block_size = len/nthreads;
    fseek(fp,SEEK_SET,0);
    

    data = (struct thread_data*)calloc(nthreads, sizeof(struct thread_data));
    /* store the block information in the argument structure */
    for(i=0;i<nthreads;i++)
    {
        data[i].buff = buff;
        data[i].start = block_size*i;
        data[i].block_size = block_size;
        data[i].counter=0;
    }



    /* record the current time */
    struct timeval start_time,end_time;
	gettimeofday(&start_time, NULL);	

	for(i = 0;i<nthreads;i++) //create thread
    {
        pthread_create(&threads[i], NULL, count_words, &data[i]);
        printf("Created Thread %d : %lld\n", i, data[i].start);
    }

	while(reports_in < nthreads)
    {
        printf("MAIN : waiting for flag to go up\n");
	    pthread_cond_wait(&flag, &lock);

        printf("MAIN : Wow! flag was raised, I have the lock\n");
        printf("%7lld: %lld\n", mail_box->counter, mail_box->start);
	    total_words+=mail_box->counter;

	    for(i = 0;i<nthreads;i++)
        {
	        if(mail_box->start == data[i].start) //check certificated thread by starting point
            {
	            pthread_join(threads[i], NULL);
                printf("done %lld\n", data[i].start);
            }
        }

	    mail_box = NULL;
	    pthread_cond_signal(&flag);
	    reports_in++;
        printf("%d\n", reports_in);
    }

    fclose(fp);
    free(buff);
    free(threads);
    free(data);

    /* calculate the elapsed time */
	gettimeofday(&end_time, NULL); 
	double operating_time = (double)(end_time.tv_sec)+(double)(end_time.tv_usec)/1000000.0-(double)(start_time.tv_sec)-(double)(start_time.tv_usec)/1000000.0;

    /* print the total number of words in the file */
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


    printf("Start %lld\n", data->start);
    /* count words in the block assigned to each thread */
    for(i = start;i<blocksize;i++)
    {
        c = data->buff[i];

        if(!isalnum(c) && isalnum(prevc))
        {
            data->counter++;
            printf("%lld\n", data->counter);
        }
        prevc = c;
    }

    printf("Thread start %lld : waiting to get lock\n", data->start);
    pthread_mutex_lock(&lock);
    printf("Thread start %lld : have lock, storing data\n", data->start);

    if(mail_box != NULL)
    {
        printf("Wait for other thread %lld\n", data->start);
        pthread_cond_wait(&flag, &lock);
    }

    printf("start %lld\n", data->start);

    mail_box = data;

    printf("Thread start %lld : raising flag\n", data->start);
    pthread_cond_signal(&flag);
    printf("Thread start %lld : unlocking box\n", data->start);
    pthread_mutex_unlock(&lock);

    return NULL;
}



