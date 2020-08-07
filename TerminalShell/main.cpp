#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


char *trim(char *string);
void ls(char *cwd);
void execute_normal(char **argv);
void execute_redirect(char **argv, char *location);

int main() {

    while(1) {
        //buffer variables and size
        char *buffer;
        size_t bufsize = 1024;
        size_t characters;

        //allocate space for buffer
        buffer = (char *)malloc(bufsize * sizeof(char));

        //error handling
        if(buffer == NULL) {
            perror("Unable to allocate buffer");
            exit(1);
        }

        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        //ask the user for input
        printf("\x1b[32muser\x1b[0m:\x1b[31m~%s$\x1b[0m ", cwd);

        //get characters from user input
        characters = getline(&buffer, &bufsize, stdin);

        //go through line and split by space
        //store each toke in an array
        int i = 0;
        char *split = trim(buffer);
        split = strtok(buffer, " ");
        char *args[bufsize];

        while(split != NULL){

            args[i++] = split;
            split = strtok(NULL, " ");

            if(split == NULL) {

                args[i++] = NULL;

            }

        }

        if(strcmp(args[0], "exit") == 0) {

            free(buffer);
            break;

        } else if (strcmp(args[0], "echo") == 0) {

            //print out each token of the array
            for (int j = 1; j < i-1; j++){

                printf("%s ", args[j]);

            }

            printf("\n");
            i = 0;
        } else if(strcmp(args[0], "mkdir") == 0) {

            int err;
            err =  mkdir(args[1], 0770);
            if(err == -1) {

                printf("%s\n", strerror(errno));

            }

        } else if(strcmp(args[0], "rmdir") == 0) {

            int err;
            err = rmdir(args[1]);
            if(err == -1) {

                printf("%s\n", strerror(errno));

            }

        } else if(strcmp(args[0], "cd") == 0) {

            int err;
            if(strcmp(args[1], "0") == 0) {

                err = chdir("/home/ibrahim");

            } else {

                err = chdir(args[1]);

            }
            if(err == -1) {

                printf("%s\n", strerror(errno));

            }

        } else if(strcmp(args[0], "ls") == 0) {

            ls(cwd);

        } else {

            if(strcmp(args[1], ">") == 0) {

                strcat(cwd, "/");
                strcat(cwd, args[2]);

                execute_redirect(&args[0], cwd);

            } else {

                execute_normal(&args[0]);

            }

            free(buffer);
        }
    }

}

char *trim(char *string) {

    char *end;

    //trims leading spaces
    while(isspace((unsigned char)*string)) {

        *string++;

    }

    if(*string == 0) {

        return string;

    }

    //trime end of string
    end = string + strlen(string) - 1;

    while(end > string && isspace((unsigned char)*end)) {

        end--;

    }

    *(end+1) = 0;

    return string;
}

void ls(char *cwd) {

    DIR *dip;
    struct dirent *dit;
    int i = 0;

    if((dip = opendir(cwd)) == NULL) {

        if(errno == ENOTDIR) {

            printf("Not Directory\n");

        }
        if(errno==EACCES) {

            printf("Permission Denied\n");

        }
        if(errno==ENOENT) {

            printf("File not found\n");

        }
        exit(0);

    }

    while((dit = readdir(dip)) != NULL) {

        i++;
        char name = dit->d_name[0];
        if(dit->d_type==DT_DIR) {

            if(i%5 == 0) {
                printf("\n");
            }

            printf("\x1b[34m%s\x1b[0m   ", dit->d_name);

        } else {

            if(i%5==0) {
                printf("\n");
            }

            printf("%s   ", dit->d_name);

        }

    }

    if(closedir(dip) == -1) {

        perror("closedir");
        exit(0);

    }

    printf("\n");

}

void execute_normal(char **argv) {

    pid_t pid;
    int status;

    pid= fork();
    if(pid < 0) {

        printf("ERROR fork failed\n");
        exit(1);

    } else if( pid == 0) {

        if(execvp(*argv, argv) < 0) {

            perror("execvp");
            exit(1);

        }

    } else {

        while(wait(&status) != pid) {}

    }

}

void execute_redirect(char **argv, char *location) {

    pid_t pid;
    int status;
    int defout;
    int fd;

    pid = fork();
    if(pid < 0) {

        printf("ERROR fork failed\n");
        exit(1);

    } else if(pid == 0) {

        defout = dup(1);
        fd=open(location, O_RDWR|O_CREAT, 0644);
        dup2(fd, 1);

        if(execvp(*argv, argv) < 0) {

            perror("execvp");
            exit(1);

        }

        close(fd);

    } else {

        while(wait(&status) != pid) {}

    }

}
