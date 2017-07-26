/*##########################################################
## COP4610 – Principles of Operating Systems – Summer C 2017
## Prof. Jose F. Osorio
## Student Name: Alejandro Palacios – 5156050
## Project: Virtual Memory Management
## Specs: Creates the timer module for the new algorithm and 
## retrieves information from the zone and page structs.
## Due Date: 07/22/2017 by 11:55pm
## Module Name: memstats.c
##
## I Certify that this program code has been written by me
## and no part of it has been taken from any sources.
##########################################################*/

#include<linux/linkage.h>
#include<linux/kernel.h>
#include<linux/memstats.h>
#include<linux/sched.h>
#include<asm/uaccess.h>
#include<linux/mm.h>
#include<linux/mmzone.h>
#include<linux/vmstat.h>
#include<linux/page-flags.h>
#include<linux/timer.h>
#include<linux/page-flags.h>

struct memstats *kernel_memstats;
struct timer_list ref_timer;

static void mutate_ref_count(struct page* page)
{
        //Gets reference bit of page
        int referenced = TestClearPageReferenced(page);

        //Stores what the page size value will be.
        int next_page_size = page->ref_count + referenced;

        //If page is referenced and the next page size will be under the max size of long, increment.
        if(next_page_size > 0 && next_page_size <= sizeof(long))
        {
                page->ref_count += referenced;

        }
}

static void ref_timer_routine()
{
	//Iterate through active and inactive lists, calling an update to the refcount.
        struct zone* current_zone;
        for_each_zone(current_zone)
        {
		spin_lock_irq(&current_zone->lru_lock);
                struct page* curr_page;
                list_for_each_entry(curr_page, &current_zone->active_list, lru)
                {
                        mutate_ref_count(curr_page);
                }

                list_for_each_entry(curr_page, &current_zone->inactive_list, lru)
                {
                        mutate_ref_count(curr_page);
                }
		spin_unlock_irq(&current_zone->lru_lock);
	}
}

static int timer_init(void)
{

        //Setup kernel timer.
        setup_timer(&ref_timer, ref_timer_routine, 0);
        return 0;
}


static void timer_exit(void) {

        del_timer(&ref_timer);
	//Call timer again in 30 seconds.
	mod_timer(&ref_timer, jiffies + msecs_to_jiffies(30000));
        printk(KERN_WARNING "Starting timer in 30 seconds.\n");
}

//Init and exit timer module.
module_init(timer_init);
module_exit(timer_exit);

asmlinkage long sys_memstats(struct memstats *stats)
{
	printk("Mem info module loaded\n");
	//Allocate memory to struct.
	kernel_memstats = kmalloc(sizeof(struct memstats), GFP_KERNEL);
	//Declare zone for iteration in for_each_zone
	struct zone* current_zone;

	//Init all values to 0.
	kernel_memstats->num_free_pages = 0;
	kernel_memstats->num_pages_slab = 0;
 	kernel_memstats->num_active_pages = 0;
	kernel_memstats->num_inactive_pages = 0;
	kernel_memstats->num_active_ref = 0;
	kernel_memstats->num_inactive_ref = 0;
	kernel_memstats->cumm_active_to_inactive = 0;
	kernel_memstats->cumm_evicted_pages = 0;
	//Iterate over zones.
	for_each_zone(current_zone)
	{
		//Get num of free pages.
		kernel_memstats->num_free_pages += current_zone->free_pages;
		//Get number of pages in slab allocator.
		kernel_memstats->num_pages_slab += zone_page_state(current_zone, NR_SLAB);
		//Get count sent from active to inactive.
		kernel_memstats->cumm_active_to_inactive += current_zone->active_inactive_count;
		//Get count sent from inactive to evicted.
		kernel_memstats->cumm_evicted_pages += current_zone->inactive_evicted_count;
		//Iterate over active pages.
		struct page* curr_page; 
		list_for_each_entry(curr_page, &current_zone->active_list, lru)
		{
			kernel_memstats->num_active_pages++;

			if(PageReferenced(curr_page))
				kernel_memstats->num_active_ref++;	
		}
		//Iterate over inactive pages.
		list_for_each_entry(curr_page, &current_zone->inactive_list, lru)
		{
			kernel_memstats->num_inactive_pages++;

			if(PageReferenced(curr_page))
				kernel_memstats->num_inactive_ref++;
		
		}
	}
	//Print results
	printk("Total number of free pages: %ld\n", kernel_memstats->num_free_pages);
	printk("Total number of pages used in slab %ld\n", kernel_memstats->num_pages_slab);
	printk("Total number of active pages: %ld\n", kernel_memstats->num_active_pages);
	printk("Total number of inactive pages: %ld\n", kernel_memstats->num_inactive_pages);
	printk("Total number of active pages with set referenced bits: %ld\n", kernel_memstats->num_active_ref);
	printk("Total number of inactive pages with set referenced bits: %ld\n", kernel_memstats->num_inactive_ref);
	printk("Pages sent from active to inactive list: %ld\n", kernel_memstats->cumm_active_to_inactive);
	printk("Pages evicted from inactive list: %ld\n", kernel_memstats->cumm_evicted_pages);

	//Attempt to copy memstats object back to user space.
	int copy_code = copy_to_user(stats, kernel_memstats, sizeof(struct memstats));
	
	//Copy to user space pinfo fails, then return EFAULT error code.
	if(copy_code) 
	{
		return -EFAULT;
	}

	return 0;
}
