/** File:       bench.c
 ** Author:     Dongli Zhang
 ** Contact:    dongli.zhang0129@gmail.com
 **
 ** Copyright (C) Dongli Zhang 2013
 **
 ** This program is free software;  you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation; either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY;  without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 ** the GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program;  if not, write to the Free Software 
 ** Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

double mysecond()
{
  	struct timeval tp;
 	struct timezone tzp;
 	int i;
 
 	i = gettimeofday(&tp,&tzp);
 	return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}


int num_thread;
#define PAGE_SIZE 4096
pthread_t *tids;

void* do_thread(void *arg)
{
	int my_id = *(int *)arg;
	pthread_t my_tid = pthread_self();
	printf("New Thread %d:%lx\n", my_id, my_tid);

	char *c;
	char *d;
 	unsigned long i;
 	unsigned long size = 32 * 1024 * 1024;
	int tmp;

 	c = malloc(size*sizeof(double));
	d = malloc(size*sizeof(double));
  
    /*for(i=0; i<size/PAGE_SIZE; i+=PAGE_SIZE)
 	{
 		c[i] = 'a';
 	}
 
 	back:
 	for(i=0; i<size; i++)
 	{
		tmp = (c[i]+1) % 65536;
 		c[i] = tmp;
 	}*/
	

	for(i=0; i<size; i++)
	{
		c[i] = mysecond();
	}
back:
	for(i=0; i<size; i++)
	{
		d[i] = c[i];
	}
 
	goto back;

	printf("Thread %lx died\n", my_tid);
}

int main(int argc, char **argv)
{
	if(argc == 1)
	{
		printf("usage: ./bench num_thread\n");
		return;
	}

	int count;
	int err;

	num_thread = atoi(argv[1]);
	printf("Total : %d\n", num_thread);
	tids = (pthread_t *)malloc(sizeof(pthread_t)*num_thread);

	for(count=0; count<num_thread; count++)
	{
		printf("Creating thread %d\n", count);
		err = pthread_create(&tids[count], NULL, do_thread, &count);	
		if(err != 0)
		{
			printf("Create new thread failed\n");
			return 0;
		}
		
		usleep(200000);
	}

	for(count=0; count<num_thread; count++)
	{
		err = pthread_join(tids[count], NULL);
	}

	printf("Main sleep\n");
}
