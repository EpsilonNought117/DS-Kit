#ifndef DSKIT_H
#define DSKIT_H

/*******************************************/
/*******  PORTABLE HEADERS NEEDED  *********/
/*******************************************/

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>

/*******************************************/
/*************  MISCELLANEOUS  *************/
/*******************************************/

#define TYPE_CHECK(x, y) sizeof(x = y)

/*******************************************/
/****************  VECTOR  *****************/
/*******************************************/

#define Vector(T)                       \
typedef struct                          \
{                                       \
    T* data;                            \
    size_t capacity;                    \
    size_t length;                      \
}                                       

Vector(void) voidVector;    // User should not define this again!

void vectorInit(voidVector* vec_ptr, size_t init_size, size_t dt_size);   // Pass Non-NULL vec_ptr!!!

#define VECTOR_INIT(vec_ptr, init_size) vectorInit((voidVector*)vec_ptr, init_size, sizeof(*(vec_ptr->data)))

void vectorResize(voidVector* vec_ptr, size_t dt_size);

#define VECTOR_RESIZE(vec_ptr) vectorResize((voidVector*)vec_ptr, sizeof(*(vec_ptr->data)))

void vectorFree(voidVector* vec_ptr, void (*dt_destroy)(void*), size_t dt_size);

#define VECTOR_FREE(vec_ptr, dt_destroy) vectorFree((voidVector*)vec_ptr, (void(*)(void*))dt_destroy, sizeof(*(vec_ptr->data)))

void vectorPushBack(voidVector* vec_ptr, const void* element, size_t dt_size);

#define VECTOR_PUSHBACK(vec_ptr, elem) vectorPushBack((voidVector*)vec_ptr, (const void*)elem, sizeof(*(vec_ptr->data)))

void* vectorPopBack(voidVector* vec_ptr, size_t dt_size);

#define VECTOR_POPBACK(vec_ptr) vectorPopBack((voidVector*)vec_ptr, sizeof(*(vec_ptr->data))) 

void vectorShrinkToFit(voidVector* vec_ptr, size_t dt_size);

#define VECTOR_SHRINKTOFIT(vec_ptr) vectorShrinkToFit((voidVector*)vec_ptr, sizeof(*(vec_ptr->data)))

void vectorInsert(voidVector* vec_ptr, size_t index, const void* element, size_t dt_size);

#define VECTOR_INSERT(vec_ptr, index, element) \
vectorInsert((voidVector*)vec_ptr, (size_t)index, (const void*)element, sizeof(*(vec_ptr->data)))

void* vectorRemove(voidVector* vec_ptr, size_t index, size_t dt_size);

#define VECTOR_REMOVE(vec_ptr, index) vectorRemove((voidVector*)vec_ptr, (size_t)index, sizeof(*(vec_ptr->data)))

int vectorIsEmpty(voidVector* vec_ptr);

#define VECTOR_ISEMPTY(vec_ptr) vectorIsEmpty((voidVector*)vec_ptr)

size_t vectorCapacity(voidVector* vec_ptr);

#define VECTOR_CAPACITY(vec_ptr) vectorCapacity((voidVector*)vec_ptr)

size_t vectorLength(voidVector* vec_ptr);   // returns SIZE

#define VECTOR_LENGTH(vec_ptr) vectorLength((voidVector*)vec_ptr)

void* vectorAtIndex(voidVector* vec_ptr, size_t index, size_t dt_size);

#define VECTOR_BACK(vec_ptr)  vectorAtIndex((voidVector*)vec_ptr, vec_ptr->length - 1, sizeof(*(vec_ptr->data)))
#define VECTOR_FRONT(vec_ptr) vectorAtIndex((voidVector*)vec_ptr, 0, sizeof(*(vec_ptr->data)))
#define VECTOR_ATINDEX(vec_ptr, index) vectorAtIndex((voidVector*)vec_ptr, index, sizeof(*(vec_ptr->data)))

void vectorCopy(voidVector* src, voidVector* dest, void* (*dt_copy)(const void*), size_t dt_size);

#define VECTOR_COPY(src, dest, dt_copy) \
vectorCopy((voidVector*)src, (voidVector*)dest, (void* (*)(const void*)) dt_copy, TYPE_CHECK(*src, *dest))

/*******************************************/
/****************  STACK  ******************/
/*******************************************/

/*******************************************/
/****************  QUEUE  ******************/
/*******************************************/

/*******************************************/
/************  FORWARD LIST  ***************/
/*******************************************/

/*******************************************/
/****************  LIST  *******************/
/*******************************************/

/*******************************************/
/****************  DEQUE  ******************/
/*******************************************/

/*******************************************/
/************  PRIORITY QUEUE  *************/
/*******************************************/

/*******************************************/
/****************  HEAP  *******************/
/*******************************************/

/*******************************************/
/************  HASH MAP  *******************/
/*******************************************/

/*******************************************/
/***********  ORDERED SET  *****************/
/*******************************************/

/*******************************************/
/************  UNORDERED SET  **************/
/*******************************************/

/*******************************************/
/************  MULTISET  *******************/
/*******************************************/

/*******************************************/
/*************  STRING  ********************/
/*******************************************/

#endif /* DSKIT_H */

#ifdef DSKIT_IMPL

/*******************************************/
/********** VECTOR IMPLEMENTATION **********/
/*******************************************/

void vectorInit(voidVector* vec_ptr, size_t init_size, size_t dt_size)
{
    assert(vec_ptr != NULL && init_size && dt_size && init_size < (SIZE_MAX / dt_size));

    vec_ptr->data = malloc(init_size * dt_size);

    if (!vec_ptr->data)
    {
        assert("Memory allocation failed due to insufficient memory capacity.");
        return;
    }
    
    vec_ptr->capacity = init_size;
    vec_ptr->length = 0;
    return;
}

void vectorResize(voidVector* vec_ptr, size_t dt_size)
{
    assert(vec_ptr != NULL && dt_size); // incase user accidentaly calls function with wrong dt_size instead of macros

    size_t new_size = (3 * vec_ptr->capacity) / 2 + 8;

    if (new_size > (SIZE_MAX / dt_size))
    {
        assert("Vector size beyond memory capacity. Resize failed.");
        return;
    }

    void* temp = realloc(vec_ptr->data, dt_size * new_size);

    if (!temp)
    {
        assert("Memory reallocation failed due to insufficient memory capacity.");
        return;
    }

    vec_ptr->data = temp;
    temp = NULL;

    vec_ptr->capacity = new_size;
    return;
}

void vectorFree(voidVector* vec_ptr, void (*dt_destroy)(void*), size_t dt_size)
{
    assert(vec_ptr && dt_destroy && dt_size);   // must pass a valid destructor!

    for (size_t i = 0; i < vec_ptr->length; i++)
    {
        dt_destroy(vec_ptr->data + i * dt_size);
    }

    free(vector->data);

    vec_ptr->data = NULL;

    vec_ptr->length = 0;
    vec_ptr->capacity = 0;
    return;
}

void vectorPushBack(voidVector* vec_ptr, const void* elem, size_t dt_size)
{
    assert(vec_ptr && elem && dt_size);
    TYPE_CHECK(*(vec_ptr->data), elem);

    if (vec_ptr->length == vec_ptr->capacity)
    {
        vectorResize(vec_ptr, dt_size);
    }

    void* temp = vec_ptr->data + (vec_ptr->length) * dt_size;
    
    memcpy(temp, elem, dt_size);
    
    vec_ptr->length++;
    return;
}

void* vectorPopBack(voidVector* vec_ptr, size_t dt_size)
{
    assert(vec_ptr && dt_size);

    void* temp = vec_ptr->data + dt_size * (vec_ptr->length - 1);

    vec_ptr->length--;
    return temp;
}

void vectorShrinkToFit(voidVector* vec_ptr, size_t dt_size)
{
    assert(vec_ptr && dt_size);

    if (vec_ptr->capacity == vec_ptr->length)
    {
        return;
    }

    void* temp = realloc(vec_ptr->data, dt_size * (vec_ptr->length));

    if (!temp)
    {
        assert("Memory reallocation failed due to insufficient memory capacity.");
        return;
    }

    vec_ptr->data = temp;
    vec_ptr->capacity = vec_ptr->length;
    return;
}



#endif