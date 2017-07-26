/*##########################################################
## COP4610 – Principles of Operating Systems – Summer C 2017
## Prof. Jose F. Osorio
## Student Name: Alejandro – 5156050
## Project: Virtual Memory Management
## Specs: Force Virtual Memory allocation to occur
## Due Date: 07/22/2017 by 11:55pm
## Module Name: stress_system.c
##
## I Certify that this program code has been written by me
## and no part of it has been taken from any sources.
##########################################################*/


#include<stdio.h>
#include<stdlib.h>       

int main()
{
	//Array size is 100 million.
	long arr_size = 100000000;

        int i;
	int j;
	printf("Stressing the system..\n");
	//Array of long arrays.
	long** arr[2];

	for(i = 0; i < 2; i++)
	{
		
		arr[i] = malloc(arr_size * sizeof(long));
		//Set every array value to 0. 200 million memory accesses in total.
		for(j = 0; j < arr_size; j++)
        	{

                        	arr[i][j] = 0;

        	}
	}
	//Clean up.
	for(i = 0; i < 2; i++)
	{
		free(arr[i]);
	}

	printf("Stress test completed..\n");
}
