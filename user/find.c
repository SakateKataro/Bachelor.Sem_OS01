#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char* path, char* filename) {
    // Copy from ls.c
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
		printf("find: path too long\n");
		close(fd);
		return;
	}

    switch(st.type) {
    case T_FILE:
        if (strcmp(path + strlen(path) - strlen(filename), filename) == 0)
            printf("%s\n", path);
        break;
    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            // Dont recurse into "." and ".."
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            find(buf, filename);
        }
        break;
    }

	close(fd);
}

int main(int argc, char *argv[]) {
    if(argc !=  3){
		fprintf(2, "need 3 argument to run the command\n");
		exit(1);
	}

	find(argv[1], argv[2]);

	exit(0);
}