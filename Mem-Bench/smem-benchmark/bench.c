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

int num_thread;
#define PAGE_SIZE 4096
pthread_t *tids;

void* do_thread(void *arg)
{
	int my_id = *(int *)arg;
	pid_t my_pid = getpid();
	printf("New Thread %d:%lx\n", my_id, my_pid);

	char *c;
 	unsigned long i;
 	unsigned long size = 512 * 1024 * 1024;
 
 	c = malloc(size*sizeof(char));
  
    for(i=0; i<size/PAGE_SIZE; i+=PAGE_SIZE)
 	{
 		c[i] = 'a';
 	}
 
 	back:
 	for(i=0; i<size; i++)
 	{
 		c[i] = (c[i]+1) % 100;
 	}
 
	goto back;

	printf("Thread %lx died\n", my_pid);
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
		if(fork() == 0)
		{
			printf("Creating thread %d\n", count);
			do_thread(&count);
		}
		
		sleep(1);
	}

	while(1);

	printf("Main sleep\n");
}
