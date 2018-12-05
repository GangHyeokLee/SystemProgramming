#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define total_array 2000000

void parallel(int *);
void calculator(int *, int);
void sequential(int *);

void main(int argc, char * argv[])
{
    int array[total_array];
    struct timeval start_time, end_time;
    double operating_time;

    for (int i=0; i<total_array;i++)
    {
        array[i] =i;
    }

    if(argc < 1)
    {
        fprintf(stderr, "argc < 1\n");
        exit(1);
    }

    if(strcmp(argv[1] ,"0")==0) //parallel mode
    {
        gettimeofday(&start_time, NULL);

        parallel(array);

        gettimeofday(&end_time, NULL);

        operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) - (double)(start_time.tv_usec)/1000000.0;

        printf("Elapsed: %f seconds\n", (double)(operating_time));

        while(wait(0)!=-1);
    }
    else //sequential mode
    {
        gettimeofday(&start_time, NULL);

        sequential(array);

        gettimeofday(&end_time, NULL);

        operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) - (double)(start_time.tv_usec)/1000000.0;

        printf("Elapsed: %f seconds\n", (double)(operating_time));
    }

    return;
}

void parallel(int *array)
{
    int  parentpid = getpid(), i, newpid;
    long even = 0, odd = 0, even_sum = 0, odd_sum=0;

    for (i = 0; i < total_array; i++)
    {
        if (!(array[i] % 2))
        {
            even++;
        }
    }
    printf("Total even numbers are : %ld\n", even);

    if (getpid() == parentpid)
    {
        if((newpid = fork()) == 0)
            calculator(array, parentpid);
    }
    if (getpid() == parentpid)
    {
        if((newpid = fork()) == 0)
            calculator(array, parentpid);
    }
    if (getpid() == parentpid)
    {
        if((newpid = fork()) == 0)
            calculator(array, parentpid);
    }


    while (wait(0) != -1);

    return;
}

void calculator(int * array, int parentpid)
{
    int i;
    long temp = 0;
   if(getpid() == (parentpid+1))//first child process
   {
       for (i = 0; i < total_array; i++)
           if (array[i] % 2) //odd count
               temp++;
       printf("Total odd  numbers are : %ld\n", temp);

       exit(1);
   }
   else if(getpid() == (parentpid+2)) //second child process
   {
       for (i = 0; i < total_array; i++)
           if (!(array[i] % 2)) //even sum
               temp += array[i];
       printf("Sum of even numbers is : %ld\n", temp);

       exit(1);
   }
   else if(getpid() == parentpid+3)//third child process
   {
       for (i = 0; i < total_array; i++)
           if (array[i] % 2) //odd sum
               temp+= array[i];
       printf("Sum of odd  numbers is : %ld\n", temp);
   }   exit(1);

   return;
}

void sequential(int * array)
{
    int i, even = 0, odd = 0;
    long even_sum = 0, odd_sum=0;

    for(i = 0;i<total_array;i++)
    {
        if(array[i]%2)
        {
            odd++;
            odd_sum+=array[i];
        }
        else
        {
            even++;
            even_sum+=array[i];
        }
    }

    printf("Total even numbers are : %d\n", even);
    printf("Total odd  numbers are : %d\n", odd);
    printf("Sum of even numbers is : %ld\n", even_sum);
    printf("Sum of odd  numbers is : %ld\n", odd_sum);

    return;
}