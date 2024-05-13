#include <bitset>
#include <cstdlib>
#include <ctime>
#include <cstdio>

unsigned char arr[0x200000];
std::bitset<0x200000> bits;

#define BITMAP_SIZE 0x8000
uint64_t bitmap[BITMAP_SIZE];

clock_t start, end;
double rwtime;

size_t loops = 0x10000000;

unsigned char null = 0;

double test_array()
{
	start = clock();
	// write
	for (size_t i = 0; i < loops; i++) {
		size_t idx = rand() % 0x200000;
		if (rand() % 2)
			arr[idx] = 1;
	}
	// read
	for (size_t i = 0; i < loops; i++) {
		size_t idx = rand() % 0x200000;
		null = arr[idx];
	}
	end = clock();
	rwtime = (double)(end - start) / CLOCKS_PER_SEC;
	return rwtime;
}

double test_bitset()
{
	start = clock();
	// write 
	for (size_t i = 0; i < loops; i++) {
		size_t idx = rand() % 0x200000;
		unsigned char val = rand() % 2;
		bits[idx] = val;
	}
	// read
	for (size_t i = 0; i < loops; i++) {
		size_t idx = rand() % 0x200000;
		null = bits[idx];
	}
	end = clock();
	rwtime = (double)(end - start) / CLOCKS_PER_SEC;
	return rwtime;
}

double test_bitmap1()
{
	start = clock();
	// write
	for (size_t i = 0; i < loops; i++) {
		size_t idx = rand() % 0x200000;
		if(rand() % 2) {
			bitmap[idx / 64] |= 1 << (idx % 64);
		}
	}
	// read
	for (size_t i = 0; i < loops; i++) {
		size_t idx = rand() % 0x200000;
		null = bitmap[idx / 64 ] & (1 << (idx % 64));
	}
	end = clock();
	rwtime = (double)(end - start) / CLOCKS_PER_SEC;
	return rwtime;
}

double test_bitmap2()
{
	start = clock();
	// write
	for (size_t i = 0; i < loops; i++) {
		size_t idx = rand() % 0x200000;
		if(rand() % 2) {
			bitmap[idx >> 6] |= 1 << (idx & 0x3f);
		}
	}
	// read
	for (size_t i = 0; i < loops; i++) {
		size_t idx = rand() % 0x200000;
		null = bitmap[idx >> 6] & (1 << (idx & 0x3f));
	}
	end = clock();
	rwtime = (double)(end - start) / CLOCKS_PER_SEC;
	return rwtime;
}



int main()
{
	srand(time(NULL));
	double array_time = 0, bitset_time = 0, bitmap1_time = 0, bitmap2_time = 0;
	for (int i = 0; i < 5; i++) {
		array_time += test_array();
		bitset_time += test_bitset();
		bitmap1_time += test_bitmap1();
		bitmap2_time += test_bitmap2();
	}
	printf("Test array access time ==> 5 loops average: %fs\n", array_time / 5);
	printf("Test bitset access time ==> 5 loops average: %fs\n", bitset_time / 5);
	printf("Test bitmap1 access time ==> 5 loops average: %fs\n", bitmap1_time / 5);
	printf("Test bitmap2 access time ==> 5 loops average: %fs\n", bitmap2_time / 5);
	return 0;
}