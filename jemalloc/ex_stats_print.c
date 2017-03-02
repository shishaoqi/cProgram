/*
* @Author: shishao
* @Date:   2017-03-02 21:40:47
* @Last Modified by:   shishao
* @Last Modified time: 2017-03-02 22:26:46
*/
//gcc ex_stats_print.c -o ex_stats_print -I${JEMALLOC_PATH}/include -L${JEMALLOC_PATH}/lib -Wl,-rpath,${JEMALLOC_PATH}/lib -ljemalloc
///usr/local/jemalloc
//gcc ex_stats_print.c -o ex_stats_print -I/usr/local/jemalloc/include -L/usr/local/jemalloc/lib -Wl,-rpath,/usr/local/jemalloc/lib -ljemalloc
#include <stdlib.h>
#include <jemalloc/jemalloc.h>

void do_something(size_t i){
    // Leak some memory.
    malloc(i * 100);
}

int
main(int argc, char **argv){
    size_t i;

    for (i = 0; i < 1000; i++) {
        do_something(i);
    }

    // Dump allocator statistics to stderr.
    malloc_stats_print(NULL, NULL, NULL);

    return (0);
}