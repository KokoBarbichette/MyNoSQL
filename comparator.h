#ifndef COMPARATOR_H

    #define COMPARATOR_H

    #include <stdlib.h>

    int genericCompare(void *first, void* second, unsigned int firstElemSize, unsigned int secondElemSize);
    unsigned int* sortArray(void** array, unsigned int size, unsigned int elemSize);

#endif // COMPARATOR_H
