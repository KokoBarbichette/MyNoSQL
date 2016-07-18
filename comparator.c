#include "comparator.h"

int genericCompare(void *first, void* second, unsigned int firstElemSize, unsigned int secondElemSize)
{
    unsigned int i;

    if(firstElemSize != secondElemSize)
    {
        return firstElemSize > secondElemSize ? 1 : -1;
    }

    for(i = 0 ; i < firstElemSize ; i++, first++, second++)
    {
        if(*((unsigned char*) first) != *((unsigned char*) second))
        {
            return *((unsigned char*) first) - *((unsigned char*) second);
        }
    }

    return 0;
}

unsigned int* sortArray(void** array, unsigned int size, unsigned int elemSize)
{
    unsigned int i = 0;
    unsigned int* indexArray = (unsigned int*) malloc(sizeof(unsigned int) * size);
    unsigned char sorted = 0;
    unsigned int tmp = 0;

    for (i = 0 ; i < size ; i++)
    {
        indexArray[i] = i;
    }

    while(!sorted)
    {
        sorted = 1;
        for (i = 0 ; i < size - 1 ; i++)
        {
            if(genericCompare(array[indexArray[i]], array[indexArray[i+1]], elemSize, elemSize) > 0)
            {
                tmp = indexArray[i];
                indexArray[i] = indexArray[i + 1];
                indexArray[i + 1] = tmp;
                sorted = 0;
            }
        }
    }
    return indexArray;
}
