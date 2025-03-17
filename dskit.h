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
#include <stdbool.h>

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
    bool is_initialized;                \
}                                       

Vector(void) voidVector;    // User should not define this again!

void vectorInit(voidVector* vec_ptr, size_t init_size, size_t dt_size);   // Pass Non-NULL vec_ptr!!!

#define VECTOR_INIT(vec_ptr, init_size) vectorInit((voidVector*)vec_ptr, init_size, sizeof(*(vec_ptr->data)))

void vectorResize(voidVector* vec_ptr, size_t new_size, size_t dt_size);

#define VECTOR_RESIZE(vec_ptr, new_size) vectorResize((voidVector*)vec_ptr, (size_t)new_size, sizeof(*(vec_ptr->data)))

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

bool vectorIsEmpty(voidVector* vec_ptr);

#define VECTOR_ISEMPTY(vec_ptr) vectorIsEmpty((voidVector*)vec_ptr)

size_t vectorCapacity(voidVector* vec_ptr);

#define VECTOR_CAPACITY(vec_ptr) vectorCapacity((voidVector*)vec_ptr)

size_t vectorLength(voidVector* vec_ptr);   // returns SIZE

#define VECTOR_LENGTH(vec_ptr) vectorLength((voidVector*)vec_ptr)

const void* vectorAtIndex(voidVector* vec_ptr, size_t index, size_t dt_size);

#define VECTOR_BACK(vec_ptr)  vectorAtIndex((voidVector*)vec_ptr, vec_ptr->length - 1, sizeof(*(vec_ptr->data)))
#define VECTOR_FRONT(vec_ptr) vectorAtIndex((voidVector*)vec_ptr, 0, sizeof(*(vec_ptr->data)))
#define VECTOR_ATINDEX(vec_ptr, index) vectorAtIndex((voidVector*)vec_ptr, index, sizeof(*(vec_ptr->data)))

void vectorCopy(voidVector* src, voidVector* dest, void (*dt_copy)(const void*, void*), size_t dt_size);

#define VECTOR_COPY(src, dest, dt_copy) \
vectorCopy((voidVector*)src, (voidVector*)dest, (void (*)(const void*, void*))dt_copy, TYPE_CHECK(*src, *dest))

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

#ifdef DSKIT_IMPLEMENTATION

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
    vec_ptr->is_initialized = true;
    return;
}

void vectorResize(voidVector* vec_ptr, size_t new_size, size_t dt_size)
{
    assert(vec_ptr != NULL && dt_size && vec_ptr->is_initialized && vec_ptr->capacity != (SIZE_MAX / dt_size) && new_size); 

    size_t temp_size = 0;

    if (vec_ptr->capacity > SIZE_MAX / (2 * dt_size))
    {
        temp_size = SIZE_MAX / dt_size;
    }
    else
    {
        if (new_size > vec_ptr->capacity * 2)
        {
            temp_size = new_size;
        }
        else
        {
            temp_size = 2 * vec_ptr->capacity;
        }
    }

    void* temp = realloc(vec_ptr->data, dt_size * temp_size);

    if (!temp)
    {
        assert("Memory reallocation failed due to insufficient memory capacity.");
        return;
    }

    vec_ptr->data = temp;
    temp = NULL;

    vec_ptr->capacity = temp_size;
    return;
}

void vectorFree(voidVector* vec_ptr, void (*dt_destroy)(void*), size_t dt_size)
{
    assert(vec_ptr && dt_size && dt_destroy && vec_ptr->is_initialized);   // must pass a valid destructor!

    if (dt_destroy != NULL)
    {
        for (size_t i = 0; i < vec_ptr->length; i++)
        {
            dt_destroy(vec_ptr->data + i * dt_size);
        }
    }

    free(vec_ptr->data);

    vec_ptr->data = NULL;

    vec_ptr->length = 0;
    vec_ptr->capacity = 0;
    return;
}

void vectorPushBack(voidVector* vec_ptr, const void* elem, size_t dt_size)
{
    assert(vec_ptr && elem && dt_size && vec_ptr->is_initialized);
    TYPE_CHECK(*(vec_ptr->data), elem);

    if (vec_ptr->length == vec_ptr->capacity)
    {
        vectorResize(vec_ptr, (vec_ptr->capacity * 3) / 2 + 8, dt_size);
    }

    void* temp = vec_ptr->data + (vec_ptr->length) * dt_size;
    
    memcpy(temp, elem, dt_size);
    
    vec_ptr->length++;
    return;
}

void* vectorPopBack(voidVector* vec_ptr, size_t dt_size)
{
    assert(vec_ptr && dt_size && vec_ptr->is_initialized);

    void* temp = malloc(dt_size);

    if (!temp)
    {
        assert("Memory reallocation failed due to insufficient memory capacity.");
        return;
    }

    memcpy(temp, vec_ptr->data + dt_size * (vec_ptr->length - 1), dt_size);

    vec_ptr->length--;
    return temp;
}

void vectorShrinkToFit(voidVector* vec_ptr, size_t dt_size)
{
    assert(vec_ptr && dt_size && vec_ptr->is_initialized);

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

void vectorInsert(voidVector* vec_ptr, size_t index, const void* element, size_t dt_size)
{
    assert(vec_ptr && vec_ptr->is_initialized && index < (vec_ptr->length) && element && dt_size);
    TYPE_CHECK(*(vec_ptr->data), element);

    if (index == vec_ptr->length - 1)
    {
        vectorPushBack(vec_ptr, element, dt_size);
        return;
    }

    if (vec_ptr->length == vec_ptr->capacity)
    {
        vectorResize(vec_ptr, (vec_ptr->capacity * 3) / 2 + 8, dt_size);
    }

    size_t i = vec_ptr->length - 1;

    while (i > index)
    {
        memcpy(vec_ptr->data + (i + 1) * dt_size, vec_ptr->data + i * dt_size, dt_size);
        i--;
    }

    memcpy(vec_ptr->data + index * dt_size, element, dt_size);
    vec_ptr->length++;
    return;
}

void* vectorRemove(voidVector* vec_ptr, size_t index, size_t dt_size)
{
    assert(vec_ptr && vec_ptr->is_initialized && (index < vec_ptr->length) && dt_size);
    
    if (index == vec_ptr->length - 1)
    {
        vectorPopBack(vec_ptr, dt_size);
        return;
    }

    void* temp_ptr = malloc(dt_size);

    if (!temp_ptr)
    {
        assert("Memory reallocation failed due to insufficient memory capacity.");
        return;
    }

    memcpy(temp_ptr, vec_ptr->data + index * dt_size, dt_size);

    size_t i = index;

    while (i < vec_ptr->length - 1)
    {
        memcpy(vec_ptr->data + i * dt_size, vec_ptr->data + (i + 1) * dt_size, dt_size);
        i++;
    }

    vec_ptr->length--;
    return temp_ptr;
}

bool vectorIsEmpty(voidVector* vec_ptr)
{
    assert(vec_ptr);

    return (vec_ptr->length == 0);
}

size_t vectorCapacity(voidVector* vec_ptr)
{
    assert(vec_ptr);

    return vec_ptr->capacity;
}

size_t vectorLength(voidVector* vec_ptr)
{
    assert(vec_ptr);

    return vec_ptr->length;
}

const void* vectorAtIndex(voidVector* vec_ptr, size_t index, size_t dt_size)
{
    assert(vec_ptr && vec_ptr->is_initialized && index < vec_ptr->length && dt_size);

    return (const void*)(vec_ptr->data + index * dt_size);
}

void vectorCopy(voidVector* src, voidVector* dest, void (*dt_copy)(const void*, void*), size_t dt_size)
{
    assert(src && dest && src->data && dt_copy && dt_size);

    if (dest->is_initialized == false)
    {
        vectorInit(dest, src->capacity, dt_size);
    }

    if (dest->capacity < src->length)
    {
        vectorResize(dest, src->length, dt_size);
    }

    if (dt_copy != NULL)
    {
        for (size_t i = 0; i < src->length; i++)
        {
            dt_copy(src->data + i * dt_size, dest->data + i * dt_size);
        }
    }

    dest->length = src->length;
    return;
}

#endif