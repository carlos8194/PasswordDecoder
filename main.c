#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

void open_directory();
void read_file(char* file_name);
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

    while ((ent = readdir (dir)) != NULL)
    {
        /* Devolverá directorio actual (.) y el anterior (..)*/
        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
        {
            //printf("%s\n", ent->d_name);
            read_file(ent->d_name); //Llama a leer archivo
        }
    }

    closedir (dir);
}

//https://gsamaras.wordpress.com/code/read-file-line-by-line-in-c-and-c/
void read_file(char* file_name) {
    //Construye el path
    char* name_dir = "/home/aduarte/CLionProjects/PasswordDecoder/lists/";
    char* path = malloc(strlen(name_dir)+1+11);
    strcpy(path, name_dir);
    strcat(path, file_name);

    FILE* fp;
    char buf[BUF_SIZE];

    if ((fp = fopen(path, "r")) == NULL)
    {
        perror(file_name);
    }

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        buf[strlen(buf) - 1] = '\0';
        //TODO HACER COMPARACION CON ALGORITMO MD5
    }
    fclose(fp);
}

void error(const char *s)
{
    /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
    perror (s);
    exit(EXIT_FAILURE);
}