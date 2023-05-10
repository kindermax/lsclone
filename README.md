# lsclone

Simple ls(1) clone written in C for learning purposes.

## Build

### Cmake


```bash
cmake -B build
cmake --build build

./build/lsclone
```

Btw, in Clion it just works.

### Make

```bash
make
./build/lsclone
```

### Gcc

```bash
gcc lsclone.c -o build/lsclone

./build/lsclone # try with -l
```
