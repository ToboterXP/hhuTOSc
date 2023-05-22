#ifndef _inc_List
#define _inc_List

#include "lib/Types.h"

template<typename T> class ListBlock {
protected:
	ListBlock<T>* prev = NULL;
	ListBlock<T>* next = NULL;

	ListBlock(T data) {
		this->data = data;
	}

public:
	ListBlock<T>* GetNext() {return next;}
	ListBlock<T>* GetPrev() {return prev;}
	T data;

template <typename S> friend class List;
};

template <typename T> class List {
private:

	int length = 0;
	T error_value;
public:
	ListBlock<T> * first = NULL;
	ListBlock<T> * last = NULL;

	List(T error_value) {this->error_value = error_value;}

	~List() {
		while (last) {
			pop_last();
		}
	}

	//Adds an object to the end of the list
	void append(T object) {
		if (!first) {
			first = new ListBlock<T>(object);
			last = first;
		} else {
			ListBlock<T>* newBlock = new ListBlock<T>(object);
			last->next = newBlock;
			newBlock->prev = last;

			last = newBlock;
		}

		length++;
	}

	//Adds an object to the start of the list
	void prepend(T object) {
		if (!first) {
			first = new ListBlock<T>(object);
			last = first;
		} else {
			ListBlock<T>* newBlock = new ListBlock<T>(object);
			first->prev = newBlock;
			newBlock->next = first;

			first = newBlock;
		}
		length++;
	}

	//returns the first or last block of the list
	ListBlock<T>* get_first() {return first;}
	ListBlock<T>* get_last() {return last;}

	//removes the first block of the list, and returns its value
	T pop_first() {
		if (!first) return error_value;

		ListBlock<T>* retBlock = first;
		T ret = retBlock->data;

		if (retBlock->next) retBlock->next->prev = NULL;

		first = retBlock->next;
		if  (first == NULL) last = NULL;
		delete retBlock;
		length--;

		return ret;
	}

	//removes the last block of the list, and returns its value
	T pop_last() {
		if (!last) return error_value;

		ListBlock<T>* retBlock = last;
		T ret = retBlock->data;

		if (retBlock->prev) retBlock->prev->next = NULL;

		last = retBlock->prev;
		if  (last == NULL) first = NULL;
		delete retBlock;
		length--;

		return ret;
	}

	void remove(ListBlock<T>* block) {
		if (first == block) first = block->next;
		if (last == block) last = block->prev;

		if (block->prev) block->prev->next = block->next;
		if (block->next) block->next->prev = block->prev;
		delete block;
		length--;
	}

	//returns whether the list is empty
	bool is_empty() {
		return first == NULL;
	}

	int get_length() {
		return length;
	}
};


#endif
