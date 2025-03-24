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
#include <stdio.h>

/*******************************************/
/*************  MISCELLANEOUS  *************/
/*******************************************/

#define TYPE_CHECK_PTR(x_ptr, y_ptr) sizeof(x_ptr = y_ptr)

#define LOG_N_ABORT(msg) do { \
    fprintf(stderr, "ERROR: %s\nFile: %s\nLine: %d\n", msg, __FILE__, __LINE__); \
    abort(); \
} while (0)

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

Vector(void) voidVector;

#define MAX_BYTES (size_t)(1 << 40) // No more than 1 TB (default)

void vectorInit(voidVector* vec_ptr, size_t init_size, size_t dt_size);
#define VECTOR_INIT(vec_ptr, init_size) vectorInit((voidVector*)vec_ptr, init_size, sizeof(*(vec_ptr->data)))

void vectorResize(voidVector* vec_ptr, size_t new_size, size_t dt_size);
#define VECTOR_RESIZE(vec_ptr, new_size) vectorResize((voidVector*)vec_ptr, (size_t)new_size, sizeof(*(vec_ptr->data)))

void vectorFree(voidVector* vec_ptr, void (*dt_destroy)(void*), size_t dt_size);
#define VECTOR_FREE(vec_ptr, dt_destroy) vectorFree((voidVector*)vec_ptr, (void(*)(void*))dt_destroy, sizeof(*(vec_ptr->data)))

void vectorPushBack(voidVector* vec_ptr, const void* element, size_t dt_size);
#define VECTOR_PUSHBACK(vec_ptr, element) \
( TYPE_CHECK_PTR(vec_ptr->data, element), vectorPushBack((voidVector*)vec_ptr, (const void*)element, sizeof(*(vec_ptr->data))) )

void vectorPopBack(voidVector* vec_ptr, size_t dt_size);
#define VECTOR_POPBACK(vec_ptr) vectorPopBack((voidVector*)vec_ptr, sizeof(*(vec_ptr->data))) 

void vectorShrinkToFit(voidVector* vec_ptr, size_t dt_size);
#define VECTOR_SHRINKTOFIT(vec_ptr) vectorShrinkToFit((voidVector*)vec_ptr, sizeof(*(vec_ptr->data)))

void vectorInsert(voidVector* vec_ptr, size_t index, const void* element, size_t dt_size);
#define VECTOR_INSERT(vec_ptr, index, element) \
( TYPE_CHECK_PTR(vec_ptr->data, element), vectorInsert((voidVector*)vec_ptr, (size_t)index, (const void*)element, sizeof(*(vec_ptr->data))) )

void vectorRemove(voidVector* vec_ptr, size_t index, size_t dt_size);
#define VECTOR_REMOVE(vec_ptr, index) vectorRemove((voidVector*)vec_ptr, (size_t)index, sizeof(*(vec_ptr->data)))

void* vectorAtIndex(voidVector* vec_ptr, size_t index, size_t dt_size);
#define VECTOR_BACK(vec_ptr)  vectorAtIndex((voidVector*)vec_ptr, vec_ptr->length - 1, sizeof(*(vec_ptr->data)))
#define VECTOR_FRONT(vec_ptr) vectorAtIndex((voidVector*)vec_ptr, 0, sizeof(*(vec_ptr->data)))
#define VECTOR_ATINDEX(vec_ptr, index) vectorAtIndex((voidVector*)vec_ptr, index, sizeof(*(vec_ptr->data)))

void vectorAppend(voidVector * dest, voidVector * src, size_t index, size_t dt_size);
#define VECTOR_APPEND(dest, src, dest_index)  \
( TYPE_CHECK_PTR(dest->data, src->data), vectorAppend((voidVector*)dest, (voidVector*)src, (size_t) dest_index, sizeof(*(src->data))) )

/*******************************************/
/****************  STACK  ******************/
/*******************************************/

#define Stack(T) Vector(T)                              

Stack(void) voidStack;

#define STACK_INIT(st_ptr, size)        VECTOR_INIT(st_ptr, size)
#define STACK_RESIZE(st_ptr, new_size)  VECTOR_RESIZE(st_ptr, new_size)
#define STACK_PUSH(st_ptr, elem)        VECTOR_PUSHBACK(st_ptr, elem)
#define STACK_POP(st_ptr)               VECTOR_POPBACK(st_ptr)
#define STACK_PEEK(st_ptr)              VECTOR_BACK(st_ptr)
#define STACK_CLEAR(st_ptr, dt_destroy) VECTOR_FREE(st_ptr, dt_destroy)
#define STACK_SIZE(st_ptr)              (st_ptr->length)

/*******************************************/
/****************  QUEUE  ******************/
/*******************************************/

/*******************************************/
/****************  DEQUE  ******************/
/*******************************************/

/*******************************************/
/************  FORWARD LIST  ***************/
/*******************************************/

/*******************************************/
/****************  LIST  *******************/
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
    assert(vec_ptr != NULL && dt_size && init_size < (MAX_BYTES / dt_size));

    vec_ptr->data = malloc(init_size * dt_size);

    if (init_size && !vec_ptr->data)
    {
        LOG_N_ABORT("Memory allocation failure.");
        return;
    }
    
    vec_ptr->capacity = init_size;
    vec_ptr->length = 0;
    return;
}

void vectorResize(voidVector* vec_ptr, size_t new_size, size_t dt_size)
{
    assert(vec_ptr != NULL && dt_size && new_size); 

    size_t temp_size = 0;

    if (vec_ptr->capacity > MAX_BYTES / (2 * dt_size))
    {
        temp_size = MAX_BYTES / dt_size;
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
        LOG_N_ABORT("Memory allocation failure.");
        return;
    }

    vec_ptr->data = temp;
    temp = NULL;

    vec_ptr->capacity = temp_size;
    return;
}

void vectorFree(voidVector* vec_ptr, void (*dt_destroy)(void*), size_t dt_size)
{
    assert(vec_ptr && dt_size && vec_ptr->data);

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
    assert(vec_ptr && vec_ptr->data && elem && dt_size);

    if (vec_ptr->length == vec_ptr->capacity)
    {
        vectorResize(vec_ptr, 2 * vec_ptr->capacity, dt_size);
    }

    void* temp = vec_ptr->data + (vec_ptr->length) * dt_size;
    
    memcpy(temp, elem, dt_size);
    
    vec_ptr->length++;
    return;
}

void vectorPopBack(voidVector* vec_ptr, size_t dt_size)
{
    assert(vec_ptr && vec_ptr->data && dt_size);

    vec_ptr->length--;
    return;
}

void vectorShrinkToFit(voidVector* vec_ptr, size_t dt_size)
{
    assert(vec_ptr && vec_ptr->data && dt_size);

    if (vec_ptr->capacity == vec_ptr->length)
    {
        return;
    }

    void* temp = realloc(vec_ptr->data, dt_size * (vec_ptr->length));

    if (!temp)
    {
        LOG_N_ABORT("Memory allocation failure.");
        return;
    }

    vec_ptr->data = temp;
    vec_ptr->capacity = vec_ptr->length;
    return;
}

void vectorInsert(voidVector* vec_ptr, size_t index, const void* element, size_t dt_size)
{
    assert(vec_ptr && vec_ptr->data && index < (vec_ptr->length) && element && dt_size);

    if (index == vec_ptr->length - 1)
    {
        vectorPushBack(vec_ptr, element, dt_size);
        return;
    }

    if (vec_ptr->length == vec_ptr->capacity)
    {
        vectorResize(vec_ptr, 2 * vec_ptr->capacity, dt_size);
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

void vectorRemove(voidVector* vec_ptr, size_t index, size_t dt_size)
{
    assert(vec_ptr && vec_ptr->data && (index < vec_ptr->length) && dt_size);
    
    if (index == vec_ptr->length - 1)
    {
        vectorPopBack(vec_ptr, dt_size);
        return;
    }

    size_t i = index;

    while (i < vec_ptr->length - 1)
    {
        memcpy(vec_ptr->data + i * dt_size, vec_ptr->data + (i + 1) * dt_size, dt_size);
        i++;
    }

    vec_ptr->length--;
    return;
}

void* vectorAtIndex(voidVector* vec_ptr, size_t index, size_t dt_size)
{
    assert(vec_ptr && vec_ptr->data && index < vec_ptr->length && dt_size);

    return (void*)(vec_ptr->data + index * dt_size);
}

void vectorAppend(voidVector* dest, voidVector* src, size_t dest_index, size_t dt_size)
{
    assert(src && src->data && dt_size);
    assert(dest && dest->data && dest_index <= dest->length);

    size_t new_total_length = dest->length + src->length;
    if (dest->capacity < new_total_length)
    {
        vectorResize(dest, new_total_length, dt_size);
    }

    size_t tail_length = dest->length - dest_index;
    void* temp = NULL;

    if (tail_length > 0)
    {
        temp = malloc(dt_size * tail_length);
        if (!temp)
        {
            LOG_N_ABORT("Memory allocation failure.");
            return;
        }
        memcpy(temp, (char*)dest->data + dest_index * dt_size, dt_size * tail_length);
    }

    memcpy((char*)dest->data + dest_index * dt_size, src->data, dt_size * src->length);

    if (tail_length > 0)
    {
        memcpy((char*)dest->data + (dest_index + src->length) * dt_size, temp, dt_size * tail_length);
        free(temp);
    }

    dest->length = new_total_length;
    return;
}

#endif