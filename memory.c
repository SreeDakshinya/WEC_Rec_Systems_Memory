#include <stdio.h>
#include <stdlib.h>

int align_mod=1;

struct allocated{
    int id;
    int start;
    int end;
}alloc[1000];
int last_alloc=0;

void show_alloc()
{
    int i;
    printf("Allocated till now: \n");
    printf("Id Start End\n");
    for(i=0;i<last_alloc;i++)
        printf("%d   %d    %d \n", alloc[i].id, alloc[i].start, alloc[i].end);
}

union each{
    int num;
    char c;
};
typedef union each each;

struct pair{
    int id;
    each value;
    int alloc;
    int req_id;
};
typedef struct pair pair;

struct arena_t{
    pair* space;
    int size;
};
typedef struct arena_t arena_t;

arena_t* arena_create(size_t size)
{
    arena_t* ans;
    int i;
    ans=(arena_t*)malloc(sizeof(arena_t));
    ans->size=size;
    ans->space=(pair*)malloc(size*sizeof(pair));
    for (i=0;i<size;i++)
        {
            (ans->space)[i].alloc=0;
            (ans->space)[i].id=i;
            (ans->space)[i].req_id=-1;
        }
    return ans;
}

void arena_alloc(arena_t *arena, size_t size)
{
    int i, j, success=1;
    for(i=0;i<(arena->size)/align_mod;i=i+1)
    {
        if ((arena->space)[i*align_mod].alloc==0)
                {   if (i+size>=arena->size) break;
                    for(j=0;j<size;j++)
                        if((arena->space)[i*align_mod+j].alloc!=0)
                            success=0;
                    if (success==1)
                        {
                            for (j=0;j<size;j++)
                                {
                                    (arena->space)[i*align_mod+j].alloc=1;
                                    (arena->space)[i*align_mod+j].req_id=last_alloc;
                                }
                            alloc[last_alloc].id=last_alloc;
                            alloc[last_alloc].start=i*align_mod;
                            alloc[last_alloc].end=i*align_mod+size-1;
                            last_alloc=last_alloc+1;
                            printf("Memory allocation successful. \n");
                            return;
                }
            }
    }
    printf("Memory allocation unsuccessful due to insufficient space. \n");
}

void arena_dealloc(arena_t *arena, int id)
{
    int i, s, e, f=0;
    for(i=0;i<last_alloc;i++)
        if (alloc[i].id==id)
            {
                s=alloc[i].start;
                e=alloc[i].end;
                f=1;
                break;
            }
    if (f==0)
    {
        printf("PID not allocated before. \n");
        return;
    }
    for(i=s;i<e+1;i++)
        {
            (arena->space)[i].alloc=0;
            (arena->space)[i].req_id=-1;
        }
}

void arena_reset(arena_t *arena)
{
    int i, s=arena->size;
    for (i=0;i<s;i++)
        {
            (arena->space)[i].alloc=0;
            (arena->space)[i].id=i;
            (arena->space)[i].req_id=-1;
        }
}

void arena_destroy(arena_t *arena)
{
    free(arena);
}

void show_arena(arena_t* arena)
{
    int i;
    printf("Index Alloc  PID\n");
    for(i=0;i<arena->size;i++)
        printf("%d      %d     %d \n", i, (arena->space)[i].alloc, (arena->space)[i].req_id);
}

void arena_realloc(arena_t *arena, int id, size_t new_size)
{
    int i, s, e, f=0;
    for(i=0;i<last_alloc;i++)
        if (alloc[i].id==id)
            {
                s=alloc[i].start;
                e=alloc[i].end;
                f=1;
                break;
            }
    if (f==0)
    {
        printf("PID not allocated before. \n");
        return;
    }
    for(i=e+1;i<e+new_size-(e-s+1);i++)
        {
            if ((arena->space)[i].alloc!=0)
                {
                    printf("Cannot resize the allocation. \n");
                    return;
                }
        }
    for(i=e+1;i<e+new_size-(e-s);i++)
        {
            (arena->space)[i].alloc=1;
            (arena->space)[i].req_id=id;
        }
    printf("Extension of the allocation is successful. \n");
}

int main()
{
    int i, choice=1, id;
    size_t total, req_size;
    arena_t* Arena;
    printf("Enter the total memory size: \n");
    scanf("%d", &total);
    printf("Enter the alignment value: \n");
    scanf("%d", &align_mod);
    Arena=arena_create(total);
    while(choice!=0)
    {
        printf("Enter 1 for allocating memory, 2 for deallocating a particular process, 3 for extending an old allocation, 4 for resetting the arena, 5 for destroying the arena and exiting and 0 for exiting:");
        scanf("%d", &choice);
        switch(choice)
        {
            case 1: printf("Enter the size of memory to be allocated: ");
                    scanf("%d", &req_size);
                    arena_alloc(Arena, req_size);
                    show_arena(Arena);
                    show_alloc();
                    printf("Enter 1 for continuing and 0 for exiting:");
                    scanf("%d", &choice);
                    break;

            case 2: printf("Enter the process id to be de-allocated: ");
                    scanf("%d", &id);
                    arena_dealloc(Arena, id);
                    show_arena(Arena);
                    show_alloc();
                    printf("Enter 1 for continuing and 0 for exiting:");
                    scanf("%d", &choice);
                    break;

            case 3: printf("Enter the process id to be resized and enter the new size: ");
                    scanf("%d %d", &id, &req_size);
                    arena_realloc(Arena, id, req_size);
                    show_arena(Arena);
                    show_alloc();
                    printf("Enter 1 for continuing and 0 for exiting:");
                    scanf("%d", &choice);
                    break;

            case 4: arena_reset(Arena);
                    printf("Enter 1 for continuing and 0 for exiting:");
                    scanf("%d", &choice);
                    break;

            case 5: arena_destroy(Arena);
                    choice=0;

            default: break;
        }
    }
}

