/** File:       bench-single.c
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
#include <unistd.h>
#include <stdlib.h>

#define PAGE_SIZE 4096

int main(int argc, char **argv)
{
	char *c;
	unsigned long i;
	unsigned long size = 64 * 1024 * 1024;

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
}
