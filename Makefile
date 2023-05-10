.PHONY: build

build: lsclone.c
	gcc lsclone.c -o ./build/lsclone -Wall -g -std=gnu11
