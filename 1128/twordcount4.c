#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

struct arg_set{
    char *fname;
    int count;
};

struct arg_set *mailbox = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag  = PTHREAD_COND_INITIALIZER;

void *count_words(void *);

void main(int argc, char * argv[])
{

    pthread_t t1, t2;
    struct arg_set args1, args2;

    int reports_in = 0;
    int total_words = 0;

    if(argc != 3)
    {
        printf("usage: %s file1 file2\n", argv[0]);
        exit(1);
    }

    pthread_mutex_lock(&lock);

    args1.fname = argv[1];
    args1.count = 0;
    pthread_create(&t1, NULL, count_words, (void*)&args1);

    args2.fname = argv[2];
    args2.count = 0;
    pthread_create(&t2, NULL, count_words, (void*)&args2);


    while(reports_in<2)
    {
        printf("MAIN : waiting for flag to go up\n");
        pthread_cond_wait(&flag, &lock);

        printf("MAIN : Wow! flag was raised, I have the lock\n");
        printf("%7d: %s\n", mailbox->count, mailbox->fname);
        total_words += mailbox->count;

        if(mailbox == &args1)
        {
            pthread_join(t1, NULL);
            printf("done %s\n", args1.fname);
        }

        if(mailbox == &args2)
        {
            pthread_join(t2, NULL);
            printf("done %s\n", args2.fname);
        }

        mailbox = NULL;
        pthread_cond_signal(&flag);
        reports_in++;
        printf("%d\n", reports_in);


    }

    printf("%7d: total words\n", total_words);
    return;
}

void *count_words(void *a)
{
    struct arg_set *args = a;
    FILE *fp;
    int c, prevc = '\0';

    if((fp = fopen(args->fname, "r"))!=NULL)
    {
        while((c = getc(fp))!=EOF)
        {
            if(!isalnum(c) && isalnum(prevc))
                args->count++;
            prevc=c;
        }
        fclose(fp);
    }
    else
    {
        perror(args->fname);
    }

    printf("COUNT %s : waiting to get lock\n", args->fname);
    pthread_mutex_lock(&lock);
    printf("COUNT %s : have lock, storing data\n", args->fname);
    if(mailbox!=NULL)
    {
        pthread_cond_wait(&flag, &lock);
    }

    printf("start %s\n", args->fname);

    mailbox = args;
    printf("COUNT %s : raising flag\n", args->fname);
    pthread_cond_signal(&flag);
    printf("COUNT %s : unlocking box\n", args->fname);
    pthread_mutex_unlock(&lock);
    return NULL;
}