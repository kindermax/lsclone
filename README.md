# lsclone

Simple ls(1) clone written in C for learning purposes.

## Build

### Gcc

```bash
gcc lsclone.c -o build/lsclone

./build/lsclone # try with -l
```

### Cmake

In Clion it just works.

```bash
cmake -B build
cmake --build build

./build/lsclone
```

