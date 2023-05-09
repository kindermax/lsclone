#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

bool is_hidden(char* value) {
    return strncmp(value, ".", 1) == 0;
}

// Simplified version of filemodestring from https://github.com/coreutils/gnulib/blob/master/lib/filemode.c
void filemode(struct stat st, char* str) {
    str[0] = S_ISDIR(st.st_mode) ? 'd' : '-';
    str[1] = st.st_mode & S_IRUSR ? 'r' : '-';
    str[2] = '-';
    str[3] = '-';
    str[4] = '-';
    str[5] = '-';
    str[6] = '-';
    str[7] = '-';
    str[8] = '-';
    str[9] = '-';
    str[10] = '-';
    str[11] = '\0';
}

// TODO: support colors
// TODO: use getopt
// TODO: specify path from args

// Simpler implementation of coreutils ls.
// See: https://github.com/coreutils/coreutils/blob/master/src/ls.c
int main(int argc, char* argv[]) {
    // --help and --version
    bool is_long = false;
    bool hidden = false;
    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            is_long = true;
        }
    }

    char* path = ".";
    DIR* dir = opendir(path);

    if (dir == NULL) {
        printf("Can not open dir: %s\n", path);
        return -1;
    }

    while (dir) {
        struct dirent *entry = readdir(dir);
        if (!entry) {
            break;
        }

        // TODO: extract skip logic into separate function
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (is_hidden(entry->d_name) && !hidden) {
            continue;
        }

        if (is_long) {
            struct stat file_stat;
            stat(entry->d_name, &file_stat);

            char modebuf[12];
            filemode(file_stat, modebuf);

            // mode user group size date name
            printf(
                "%s %d %d %lld %ld %s\n",
               modebuf,
               file_stat.st_uid,
               file_stat.st_gid,
               file_stat.st_size,
               file_stat.st_mtimespec.tv_sec,
               entry->d_name
            );
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
    return 0;
}
