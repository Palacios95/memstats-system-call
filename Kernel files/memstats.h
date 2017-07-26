/*##########################################################
## COP4610 – Principles of Operating Systems – Summer C 2017
## Prof. Jose F. Osorio
## Student Name: Alejandro Palacios – 5156050
## Project: Virtual Memory Management
## Specs: defines the memstats structure
## Due Date: 07/22/2017 by 11:55pm
## Module Name: memstats.h
##
## I Certify that this program code has been written by me
## and no part of it has been taken from any sources.
##########################################################*/

struct memstats {
	long num_free_pages;			/*Number of ree pages over all memory zones*/
	long num_pages_slab;			/*Number of pages used by slab allocator*/
	long num_active_pages;			/*Number of pages in the active list*/
	long num_inactive_pages;		/*Number of pages in the inactive list*/
	long num_active_ref;			/*Number of pages in the active list whose reference bits are set*/
	long num_inactive_ref;			/*Number of inactive pages in the inactive list whose reference bits are set*/
	long cumm_active_to_inactive;		/*Cummulative number of pages moved from the active list to the inactive list*/
	long cumm_evicted_pages;		/*Cummulative number of pages evicted from the inactive list*/
};
