#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef PR_SET_CHILD_SUBREAPER
#define PR_SET_CHILD_SUBREAPER 36
#endif

int
main(int argc, char** argv)
{
    if(argc < 2) {
        fprintf(stderr, "usage: fork-wrapper command...\n");
        exit(EXIT_FAILURE);
    }

    if(prctl(PR_SET_CHILD_SUBREAPER, 1) < 0) {
        perror("could not set subreaper process attribute");
        exit(EXIT_FAILURE);
    }

    int rc = fork();

    if(rc < 0) {
        perror("could not fork");
        exit(EXIT_FAILURE);
    }

    if(rc == 0) {
        execvp(argv[1], argv + 1);

        perror("could not exec");
        exit(EXIT_FAILURE);
    }
}
