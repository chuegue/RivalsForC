#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DEFINITIONS:
 * DECLARE_DYNAMIC_ARRAY(TYPE) 
 * static DynamicArray_##TYPE* createDynamicArray_##TYPE()
 * static void append_##TYPE(DynamicArray_##TYPE *arr, TYPE element)
 * static TYPE get_##TYPE(DynamicArray_##TYPE *arr, int index)
 * static void freeDynamicArray_##TYPE(DynamicArray_##TYPE *arr)
 * */


#define DECLARE_DYNAMIC_ARRAY(TYPE) \
typedef struct { \
    TYPE *array; \
    int size; \
    int capacity; \
} DynamicArray_##TYPE; \
\
static DynamicArray_##TYPE* createDynamicArray_##TYPE() { \
    DynamicArray_##TYPE *arr = (DynamicArray_##TYPE*)malloc(sizeof(DynamicArray_##TYPE)); \
    if (arr == NULL) { \
        printf("Memory allocation failed!\n"); \
        exit(1); \
    } \
    arr->array = (TYPE*)calloc(INITIAL_CAPACITY, sizeof(TYPE)); \
    if (arr->array == NULL) { \
        printf("Memory allocation failed!\n"); \
        exit(1); \
    } \
    arr->size = 0; \
    arr->capacity = INITIAL_CAPACITY; \
    return arr; \
} \
\
static void resize_##TYPE(DynamicArray_##TYPE *arr, int new_capacity) { \
    arr->array = (TYPE*)realloc(arr->array, sizeof(TYPE) * new_capacity); \
    if (arr->array == NULL) { \
        printf("Memory allocation failed!\n"); \
        exit(1); \
    } \
    /* Initialize the newly allocated memory to zero */ \
	if(new_capacity > arr->capacity){\
	memset(&arr->array[arr->capacity], 0, (new_capacity - arr->capacity) * sizeof(TYPE));\
	}\
    /*for (int i = arr->capacity; i < new_capacity; i++) { */\
    /*    arr->array[i] = 0; */\
    /*} */\
    arr->capacity = new_capacity; \
} \
\
static void append_##TYPE(DynamicArray_##TYPE *arr, TYPE element) { \
    if (arr->size == arr->capacity) { \
        resize_##TYPE(arr, arr->capacity * 2); \
    } \
    arr->array[arr->size++] = element; \
} \
\
static TYPE get_##TYPE(DynamicArray_##TYPE *arr, int index) { \
    if (index < 0 || index >= arr->size) { \
        printf("Index out of bounds!\n"); \
        exit(1); \
    } \
    return arr->array[index]; \
} \
static void removeAt_##TYPE(DynamicArray_##TYPE *arr, int index) { \
    if (index < 0 || index >= arr->size) { \
        printf("Index out of bounds!\n"); \
        exit(1); \
    } \
\
    /* Shift elements to the left */ \
    for (int i = index; i < arr->size - 1; i++) { \
        arr->array[i] = arr->array[i + 1]; \
    } \
\
    arr->size--; \
\
    /* Optionally shrink the array if it's significantly larger than needed */ \
    if (arr->size < arr->capacity / 2 && arr->capacity > INITIAL_CAPACITY) { \
        resize_##TYPE(arr, arr->capacity / 2); \
    } \
} \
\
static void freeDynamicArray_##TYPE(DynamicArray_##TYPE *arr) { \
    free(arr->array); \
    free(arr); \
}

#define INITIAL_CAPACITY 10
DECLARE_DYNAMIC_ARRAY(int)

#endif /* DYNAMIC_ARRAY_H */


