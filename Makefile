SHELL=/bin/bash

mod_distance.so: mod_distance.c
	@gcc -g -O2 -fPIC -shared mod_distance.c -o mod_distance.so

build: mod_distance.so

install: build
	@if [ $$(id -u) == 0 ] ; then \
		cp mod_distance.so /usr/lib/mod_distance.so && \
		ldconfig; \
	fi
