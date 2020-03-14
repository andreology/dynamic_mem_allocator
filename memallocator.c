//Andre Barajas
//Spring 2020
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//1.Define a struct to represent an allocation block
struct Block {
	int block_size;				//the number of bytes in this data section
	struct Block *next_block;	//pointer to the next block
};
//2.Determine the size of a Block value & assign it to a global const variable
const int overhead_size = sizeof(struct Block);
//3.Determine size of a void* generic pointer and save as global const variable
const int void_pointer_size = sizeof(void*);
//4.Create a global pointer block to always point to 1st free block in free list
struct Block *free_head;
//Helper function to walk the block and print contents currently saved in heap
void print_block(struct Block *curr) {
	printf("\nFree head list: (");
	    struct Block *current = curr;
	    while(current != NULL){
	        printf("%d) -> (", current->block_size);
	        current = current->next_block;
	    }
}
	// block is already at data section, just add remaining allocated size
	// round up to account f#include <math.h> for fragmentation
int check_fragmentation(int size, int divisible) {
	if(divisible == 0){return size;}
	int fragmentation = size % divisible;
	if(fragmentation == 0){return size;}
	int frag_needed = size + divisible - fragmentation;
	return frag_needed;
}
//checking next base 2 log after adding one
int check_powers(unsigned int input){
    int value = 1;
    while(value <= input){value = value << 1;}
    return value;
}
//5.Create a function for malloc to init a buffer. Global free_head
//will point to buffer & initialize head with values for block_size & next-block
void init_mem_heap(int size) {
  free_head = (struct Block*) malloc(size);
  free_head-> block_size = size  - overhead_size;
  free_head-> next_block = NULL;
}
//6.Fills an allocation request of size bytes and returns a pointer to the data
void* my_memalloc(int size) {
	//init free head
	struct Block* current = free_head;
	//while current is not null traverse the BLock struct
	for(current; current; current = current-> next_block){
					//checking if value is less than current size of block
					//if so use cant_split_block
	        struct Block* cant_split_block;
	        if(current-> next_block != NULL)
	            if(current-> next_block-> block_size > size) { cant_split_block = current; }
	        // SPLITTING  if block size is big enough
	        if(current-> block_size >= size){
	            if(current-> block_size - overhead_size - size >= void_pointer_size){
						    int new_block_size = (current->block_size - overhead_size - size); // compute size for newest block
						    // generate block requested
						    *current = (struct Block) {.block_size = size, .next_block = NULL};
						    struct Block *block_addy = ++current; // assign address to proper location in memory
						    struct Block *newest_block = (char*) current + check_fragmentation(size, void_pointer_size); // aggregate left over size & round up for frags required
						    newest_block-> next_block = current-> next_block;
						    *newest_block = (struct Block){.block_size = new_block_size, .next_block = NULL}; // assign free head to point to new block location in heap mem
						    free_head = newest_block;

						    return block_addy;
	            }
	            else{  // NOT SPLITTING if block aint that big, then move to the next block
	                if( current-> block_size < size ){
	                    cant_split_block-> next_block = current-> next_block;
	                    current-> block_size = size;
	                    current-> next_block = NULL;
	                    return (char*) current + overhead_size; // data ptr
	                }
	                else{  // NOT SPLITTING if block is that big, assign free head to point to this location in heap mem
	                    free_head = free_head-> next_block;
	                    cant_split_block-> block_size = size;
											//requirement: next block should be null
	                    cant_split_block-> next_block = NULL;
	                    return (char*) cant_split_block + overhead_size;
	                }
	            }
	        }
	    }
	    return NULL;
}
//7. Deallocates a value that was allocated on the data heap
//The pointer will be to the data portion of a block
void my_free(void *data) {
	struct Block *remove_block = (struct Block *) data;
	//decrement block pointers
	--remove_block;
	//assignmet location of free head
	remove_block->next_block = free_head;
	free_head = remove_block;
}
//---------------TRIALS: unit testing--------------------------//
void trial_5(){
    init_mem_heap(2100);
		printf("\n---------------trial 5------------------\n");
		//array need with a hunnid
    int hunnid[100];
    struct Block* hunnid_array = my_memalloc(sizeof(hunnid));
    printf("\naddres of array with hundred = %p", hunnid_array);

    int int_value = 10;
    struct Block* single_int = my_memalloc(int_value);
    printf("\naddress of single int = %p", single_int);
		printf("\nfreeing array with one hundred");
    my_free(hunnid_array);
    printf("\n\nChecking address and value has not changed");
    printf("\naddress of signle int =  %p", single_int);
    printf("\naddress of single int =  %d \n", int_value);
}
void trial_4(){
    init_mem_heap(1100);
		printf("\n---------------trial 4------------------\n");
		//int allocating
    struct Block* first = my_memalloc(10);
    printf("\nfirst int = %p", first);
		//allocating double
    struct Block* sec = my_memalloc(sizeof(char));
    printf("\nsecond with double = %p\n", sec);
}
void trial_3(){
    init_mem_heap(1100);
		printf("\n---------------trial 3------------------\n");
    struct Block* first = my_memalloc(10);
    printf("\nfirst = %p",first);

    struct Block* sec = my_memalloc(10);
    printf("\nsecond = %p", sec);

    struct Block* thir = my_memalloc(10);
    printf("\nthird = %p", thir);
		printf("\nfreeing second\n");
    my_free(sec);
		printf("replacing second with double\n");
    struct Block* sec_double = my_memalloc(sizeof(double));
    printf("\ndouble = %p\n", sec_double);
}
void trial_2(){
    init_mem_heap(1100);
		printf("\n---------------trial 2------------------\n");
    struct Block* first = my_memalloc(sizeof(int));
    printf("\naddress of first: %p", first);

    struct Block* sec = my_memalloc(sizeof(int));
    printf("\naddress of second = %p \n", sec);
		printf("\nsize of overhead plus size of *void %d \n", sizeof(overhead_size) + sizeof(void_pointer_size) );
		printf("\nsize of first %d \n", sizeof(first));
		printf("\nsize of second %d \n", sizeof(sec));

}
void trial_1(){
    init_mem_heap(1100);
		printf("\n---------------trial 1------------------\n");
		printf("allocating an int\n");
    struct Block* first = my_memalloc(sizeof(int));
    printf("Address of first int = %p", first);
		printf("\nfreeing first int\n\n");
  	my_free(first);
		printf("allocating another int");
    struct Block* second = my_memalloc(sizeof(int));
    printf("\nAddress of second int = %p\n", second);
}
void standard_dev_main(){
    int input = 0;
    printf("size of array = ");
    scanf("%d", &input);
		// total size big enough for my_memalloc
    init_mem_heap(input * sizeof(int) * overhead_size);
    int* x = (int*) my_memalloc(input * sizeof(int));
    int* header = (char*)x - sizeof(struct Block);

    // users numbers
    int* current = x;
    for(int i = 0; i < input; i++){
        int temp = 0;
				//request input to place in array for std dev
        printf("Input %d of %d numbers for array = ", 1 + i, input);
        scanf("\n%d",&temp);
        *current = temp;
        current = (char *)current + sizeof(int);
    }
    // compute the array of numbers
    float summation = 0.0;
		float standard_dev = 0.0;
		float mean_value;
    current = x; // reset position to where x is
    for(int index = 0; index < input; index++){
        summation += *current;
        current = (char *)current + sizeof(int);
    }
    mean_value = summation / input;
    current = x; // reset position to where x is
    for(int index = 0; index < input; index++){
        int n = *current;
        current = (char *)current + sizeof(int);
        standard_dev += pow(n - mean_value,2);
    }
    printf("standard dev: %f\n", sqrt(standard_dev / input));
}
//main driver for program
void main() {
 //Project requirements, unit testing for program verification
	trial_1();
	trial_2();
	trial_3();
	trial_4();
	trial_5();
	//standard_dev_main();

  return 0;
}
