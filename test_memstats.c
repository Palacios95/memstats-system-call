/*##########################################################
## COP4610 – Principles of Operating Systems – Summer C 2017
## Prof. Jose F. Osorio
## Student Name: Alejandro Palacios – 5156050
## Project: Virtual Memory Management
## Specs: calls the sys_memstats system call.
## Due Date: 07/22/2017 by 11:55pm
## Module Name: test_memstats.c
##
## I Certify that this program code has been written by me
## and no part of it has been taken from any sources.
##########################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <omp.h>

struct memstats {
        long num_free_pages;             /*Number of ree pages over all memory zones*/
        long num_pages_slab;             /*Number of pages used by slab allocator*/
        long num_active_pages;           /*Number of pages in the active list*/
        long num_inactive_pages;         /*Number of pages in the inactive list*/
        long num_active_ref;             /*Number of pages in the active list whose reference bits are set*/
        long num_inactive_ref;           /*Number of inactive pages in the inactive list whose reference bits are set*/
        long cumm_active_to_inactive;    /*Cummulative number of pages moved from the active list to the inactive list*/
        long cumm_evicted_pages;         /*Cummulative number of pages evicted from the inactive list*/
};

void call_memstats();
void mem_change();

int main()
{
	call_memstats();
	return 0;

}

void call_memstats()
{
	struct memstats mem_stats;
	long int return_val = syscall(301, &mem_stats);
	printf("Number of free pages: %ld\n", mem_stats.num_free_pages);
	printf("Number of pages in slab allocator: %ld\n", mem_stats.num_pages_slab);
	printf("Number of active pages: %ld\n", mem_stats.num_active_pages);
	printf("Number of inactive pages %ld\n", mem_stats.num_inactive_pages);
	printf("Number of active pages with set reference bits: %ld\n", mem_stats.num_active_ref);
	printf("Number of inactive pages with set reference bits: %ld\n", mem_stats.num_inactive_ref);
	printf("Number of pages sent from the active to inactive list: %ld\n", mem_stats.cumm_active_to_inactive);
	printf("Number of pages sent from inactive from evicted: %ld\n", mem_stats.cumm_evicted_pages);
	printf("\nReturned %d\n", return_val);

}
