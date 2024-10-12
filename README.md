# WEC_Rec_Systems_Memory

This repository contains all of the files used to solve a given memory-allocation-based question for the Web Enthusiasts' Club Systems Recruitment.

## Description

This file is an implementation of the Arena Memory Allocation method. The following aspects have been covered in the code:
* Implemented aligned allocation and minimized fragmentation (**using a combination of aligned allocation and first fit**)
* Handled error cases like out-of-memory conditions and others.
* Added support for extending previously allocated sections of memory.

The following functions have been implemented:
  * ```arena_t* arena_create(size_t size)```  -  To create the arena of the given size 
  * ```void arena_alloc(arena_t *arena, size_t size)```  -  To allocate memory to a process
  * ```void arena_dealloc(arena_t *arena, int id)```  -  To deallocate the memory associated with a process
  * ```void arena_reset(arena_t *arena)```  -  To reset the arena
  * ```void arena_destroy(arena_t *arena)```  -  To destroy the arena
  * ```void show_arena(arena_t* arena)```  -  To show the arena
  * ```void arena_realloc(arena_t *arena, int id, size_t new_size)```  -  To extend the allocation of a given process: Checks whether the extra locations required are met by the locations following the current allocated section
  * ```void show_alloc()```  -  To show the allocations made till now
    
The implementation assumes that the memory is not circular. 
