#include "heap_test.h"

int main(int argc, char const *argv[])
{
    if(argc != 2) return 1;

    test_heap(atoi(argv[1]));

    return 0;
}
