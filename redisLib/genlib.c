/*
 * File: genlib.c
 * Version: 1.0
 * Last modified on Sun Jul 24 10:29:46 1994 by eroberts
 * -----------------------------------------------------
 * This file implements the general C library package.  See the
 * interface description in genlib.h for details.
 */

#include <stdio.h>
#include <stddef.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <jemalloc/jemalloc.h>
#include "genlib.h"


/*
 * Constants:
 * ----------
 * ErrorExitStatus -- Status value used in exit call
  */

#define ErrorExitStatus 1
#define PREFIX_SIZE (sizeof(size_t))

/* Section 1 -- Define new "primitive" types */

/*
 * Constant: UNDEFINED
 * -------------------
 * This entry defines the target of the UNDEFINED constant.
 */

char undefined_object[] = "UNDEFINED";

/* Section 2 -- Memory allocation */


/* Memory allocation implementation */

/*#define malloc(size) je_malloc(size)
#define calloc(count,size) je_calloc(count,size)
#define realloc(ptr,size) je_realloc(ptr,size)
#define free(ptr) je_free(ptr)

#define update_zmalloc_stat_add(__n) do { \
    pthread_mutex_lock(&used_memory_mutex); \
    used_memory += (__n); \
    pthread_mutex_unlock(&used_memory_mutex); \
} while(0)

#define update_zmalloc_stat_sub(__n) do { \
    pthread_mutex_lock(&used_memory_mutex); \
    used_memory -= (__n); \
    pthread_mutex_unlock(&used_memory_mutex); \
} while(0)

#define update_zmalloc_stat_alloc(__n) do { \
    size_t _n = (__n); \
    if (_n&(sizeof(long)-1)) _n += sizeof(long)-(_n&(sizeof(long)-1)); \
    if (zmalloc_thread_safe) { \
        update_zmalloc_stat_add(_n); \
    } else { \
        used_memory += _n; \
    } \
} while(0)

#define update_zmalloc_stat_free(__n) do { \
    size_t _n = (__n); \
    if (_n&(sizeof(long)-1)) _n += sizeof(long)-(_n&(sizeof(long)-1)); \
    if (zmalloc_thread_safe) { \
        update_zmalloc_stat_sub(_n); \
    } else { \
        used_memory -= _n; \
    } \
} while(0)

static size_t used_memory = 0;
static int zmalloc_thread_safe = 0;
pthread_mutex_t used_memory_mutex = PTHREAD_MUTEX_INITIALIZER;*/

void *GetBlock(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL) Error("No memory available");
    return ptr;
}

void FreeBlock(void *ptr)
{
    free(ptr);
}


/* Section 3 -- Basic error handling */


void Error(string msg, ...)
{
    va_list args;

    va_start(args, msg);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, msg, args);
    fprintf (stderr, "\n");
    va_end(args);
    exit(ErrorExitStatus);
}
