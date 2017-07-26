#SYSTEM CALL INSTRUCTIONS
	- The files that need to be updated for the system call to work are in the Kernel files/UpdateForSysCall folder.
	- You must include the memstats folder in /usr/src/linux for the system call to work.

#MODIFIED KERNEL SOURCE
	- The modified kernel source files are mmzone.h, mm.h, and vmscan.c. Please replace these for the memstats timer module and new algorithm to work properly.
	
#USER LEVEL PROGRAMS
	- The binaries are already provided for your convenience, but feel free to recompile them.
	- test_memstats.c calls the sys_memstats system call. Compile with gcc test_memstats.c -o tmem
	- stress_system.c performs mutations on very large arrays to force virtual memory management. Compile with gcc stress_system.c -o stress_system