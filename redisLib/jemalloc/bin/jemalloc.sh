#!/bin/sh

prefix=/usr/local/jemalloc
exec_prefix=/usr/local/jemalloc
libdir=${exec_prefix}/lib

LD_PRELOAD=${libdir}/libjemalloc.so.2
export LD_PRELOAD
exec "$@"
