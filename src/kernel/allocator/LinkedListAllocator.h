#ifndef __LinkedListAllocator_include__
#define __LinkedListAllocator_include__

#include "kernel/Allocator.h"


// Format eines freien Blocks
typedef struct _heap_block {
    uint32_t size; // size of memory stored in the block (excluding the struct itself)
	bool is_free;
    struct _heap_block *next;
	struct _heap_block *prev;
} heap_block;


class LinkedListAllocator : Allocator {

private:
      heap_block *list_start;

      LinkedListAllocator(Allocator &copy); // Verhindere Kopieren

public:
      LinkedListAllocator () { }

      virtual void init ();
      virtual void dump_free_memory ();
      virtual void* alloc (uint32_t req_size);
      virtual void free (void *ptr);

};

#endif
