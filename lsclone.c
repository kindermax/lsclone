#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>

bool is_hidden(char* value) {
    return strncmp(value, ".", 1) == 0;
}

// Simplified version of filemodestring from https://github.com/coreutils/gnulib/blob/master/lib/filemode.c
void filemode(struct stat st, char* str) {
    str[0] = S_ISDIR(st.st_mode) ? 'd' : '-';
    str[1] = st.st_mode & S_IRUSR ? 'r' : '-';
    str[2] = st.st_mode & S_IWUSR ? 'w' : '-';
    str[3] = st.st_mode & S_IXUSR ? 'x' : '-';
    str[4] = st.st_mode & S_IRGRP ? 'r' : '-';
    str[5] = st.st_mode & S_IWGRP ? 'w' : '-';
    str[6] = st.st_mode & S_IXGRP ? 'x' : '-';
    str[7] = st.st_mode & S_IROTH ? 'r' : '-';
    str[8] = st.st_mode & S_IWOTH ? 'w' : '-';
    str[9] = st.st_mode & S_IXOTH ? 'x' : '-';
    str[10] = ' ';
    str[11] = '\0';
}

void format_time(struct timespec t, char *buf, int buf_len) {
    strftime(buf, buf_len, "%D %T", gmtime(&t.tv_sec));
}

// TODO: support colors
// TODO: use getopt
// TODO: specify path from args

// Simpler implementation of coreutils ls.
// See: https://github.com/coreutils/coreutils/blob/master/src/ls.c
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");
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
            // TODO: can implement when will collect all files in array
            int size_width = 4; //get_size_width();

            struct passwd *pwd = getpwuid(file_stat.st_uid);
            struct group *grp = getgrgid(file_stat.st_gid);

            char time_buf[100];
            format_time(file_stat.st_mtimespec, time_buf, sizeof time_buf);

            // mode user group size date name
            printf(
                "%s %s %s  %*lld %s %s\n",
               modebuf,
               pwd->pw_name,
               grp->gr_name,
               size_width,
               file_stat.st_size,
               time_buf,
               entry->d_name
            );
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
    return 0;
}
