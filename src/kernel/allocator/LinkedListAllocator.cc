
//#include <stddef.h>
#include "kernel/Globals.h"
#include "kernel/allocator/LinkedListAllocator.h"


#define HEAP_MIN_FREE_BLOCK_SIZE 64         // min. Groesse eines freien Blocks

void LinkedListAllocator::init() {
	list_start = (heap_block*)heap_start;
	list_start->size = heap_size - sizeof(heap_block);
	list_start->is_free = true;
	list_start->prev = NULL;
	list_start->next = NULL;

	list_end = list_start;
}



void LinkedListAllocator::dump_free_memory() {
	lock.waitForAcquire();
	kout << hex << "LinkedListAllocator start:"<<heap_start<<" end:"<<heap_end<<" list_start:"<<list_start<<" list_end:"<<list_end<<endl;
	for (heap_block* current = list_start; current != NULL; current = current->next) {
		kout << "block:"<<current<<" size:"<<current->size<<" free:"<<current->is_free<<" prev:"<<(current->prev)<<" next:"<<(current->next)<<endl;
	}
	lock.release();
}


void * LinkedListAllocator::alloc(uint32_t req_size) {
	lock.waitForAcquire();
	char* prevDbg = dbgString;
    dbgString = "Alloc";
	if (req_size < HEAP_MIN_FREE_BLOCK_SIZE) req_size = HEAP_MIN_FREE_BLOCK_SIZE;

	heap_block* current = list_start;
	while (current != NULL) {
		if (current->is_free && current->size >= req_size) {//if it fits, reserve it
			current->is_free = false;

			//if there's enough free space left, create a block for that space
			if (current->size - req_size >= sizeof(heap_block) + HEAP_MIN_FREE_BLOCK_SIZE) {

				heap_block* new_block = (heap_block*)(((uint8_t*)current) + sizeof(heap_block) + req_size);
				new_block->size = current->size - req_size - sizeof(heap_block);
				new_block->is_free = true;
				new_block->prev = current;
				new_block->next = current->next;
				if (new_block->next != NULL) new_block->next->prev = new_block;



				//link the list
				current->size = req_size;
				if (current == list_end) list_end = new_block;

				current->next = new_block;
			}
			dbgString = prevDbg;
			lock.release();
			return (void*)(((uint8_t*)current) + sizeof(heap_block));
		}

		current = current->next;
	}

	kout << "Heap full" << endl;
	dump_free_memory();
	cpu.die();
	return NULL;
}


void LinkedListAllocator::free(void *ptr) {
	if (heap_start > ptr || heap_end <= ptr) return;

	lock.waitForAcquire();

	char* prevDbg = dbgString;
    dbgString = "Free";

	heap_block* current = (heap_block*)((uint8_t*)ptr - sizeof(heap_block));
	current->is_free = true;

	heap_block* first_free = current;
	/*while (first_free->prev != NULL && first_free->prev->is_free) {
		first_free = first_free->prev;
	}*/

	if (current->prev != NULL && current->prev->is_free) first_free = current->prev;

	heap_block* last_free = current;
	/*while (last_free->next != NULL && last_free->next->is_free) {
		last_free = last_free->next;
	}*/
	if (current->next != NULL && current->next->is_free) last_free = current->next;

	if (last_free == list_end) list_end = first_free;

	uint64_t total_size = (((uint8_t*)last_free) + sizeof(heap_block) + last_free->size) - (((uint8_t*)first_free) + sizeof(heap_block));
	first_free->size = total_size;
	if (last_free->next != NULL) last_free->next->prev = first_free;
	first_free->next = last_free->next;

	dbgString = prevDbg;
	lock.release();
}

bool LinkedListAllocator::is_available() {
	return !lock.isLocked();
}
