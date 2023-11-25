#include <iostream>
#include <vector>

#include "heap/heap.h"

using namespace std;

int main()
{
	vector<char> heap(1000);
	heap::HeapManager heap_manager(&heap[0], heap.size());

	void* ptr = heap_manager.GetStartPointer();

	void* ptr1 = heap_manager.Alloc(0x40);
	void* ptr2 = heap_manager.Alloc(0x40);
	void* ptr3 = heap_manager.Alloc(0x40);
	void* ptr4 = heap_manager.Alloc(0x40);
	void* ptr5 = heap_manager.Alloc(0x40);

	heap_manager.Free(ptr2);
	heap_manager.Free(ptr4);
	heap_manager.Free(ptr3);

	void* ptr6 = heap_manager.Alloc(0x90);

}