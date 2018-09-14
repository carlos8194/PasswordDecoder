#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void open_directory();
bool read_file(char* file_name);
int find_password(char* word);
void error(const char *s);

int const BUF_SIZE = 256;

int main() {
    open_directory();
    return 0;
}

//https://poesiabinaria.net/2011/09/listar-archivos-dentro-de-un-directorio-o-carpeta-en-c/
void open_directory() {
    DIR *dir;
    struct dirent *ent;

    /*char* path;

    Directorio actual */
    /* char cwd[PATH_MAX];
     if (getcwd(cwd, sizeof(cwd)) != NULL) {
         char* name_dir = "/lists";
         path = malloc(strlen(cwd)+1+6);
         strcpy(path, cwd);
         strcat(path, name_dir);

         //printf("Current working dir: %s\n", path);
     } else {
         perror("getcwd() error");
         exit;
     }*/

    dir = opendir ("/home/aduarte/CLionProjects/PasswordDecoder/lists"); //Abre directorio

    if (dir == NULL)
        error("lists.");

    bool found = false;

    while (((ent = readdir (dir)) != NULL )&& !found)
    {
        /* Devolverá directorio actual (.) y el anterior (..)*/
        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
        {
            found = read_file(ent->d_name); //Llama a leer archivo
        }
    }

    closedir (dir);
}

//https://gsamaras.wordpress.com/code/read-file-line-by-line-in-c-and-c/
bool read_file(char* file_name) {
    //Construye el path
    char* name_dir = "/home/aduarte/CLionProjects/PasswordDecoder/lists/";
    char* path = malloc(strlen(name_dir)+1+11);
    strcpy(path, name_dir);
    strcat(path, file_name);
    bool found = false;

    FILE* fp;
    char buf[BUF_SIZE];

    if ((fp = fopen(path, "r")) == NULL)
    {
        perror(file_name);
    }

    while ((fgets(buf, sizeof(buf), fp) != NULL) && !found) {
        buf[strlen(buf) - 1] = '\0';

        if(find_password(buf) == 0) {
            found = true;
        }
    }

    if(found) {
        printf("Password found: %s\n", buf);
    } else {
        printf("Sorry, password not found.\n");
    }

    fclose(fp);

    return found;
}

int find_password(char* word) {
    int found = 0;
    char *command = "openssl passwd -1 -salt SALt ";
    char* cmd = malloc(strlen(command)+1+50);
    strcpy(cmd, command);
    strcat(cmd, word);

    char buf[BUF_SIZE];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        found = -1;
    }

    while (fgets(buf, BUF_SIZE, fp) != NULL) {
        buf[strlen(buf) - 1] = '\0';
        if((strcmp("$1$SALt$SpP1SODdB0LcF8YV4fy51/", buf) == 0)) {
            printf("OUTPUT: %s\n", buf);
            found = 1;
        }
    }

    if(pclose(fp))  {
        printf("Command not found or exited with error status\n");
        found = -1;
    }

    return found;
}


void error(const char *s)
{
    /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
    perror (s);
    exit(EXIT_FAILURE);
}