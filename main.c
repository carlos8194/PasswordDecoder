#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void open_directory();
char* read_file(char* file_name);
int find_password(char* word);
void error(const char *s);

int const BUF_SIZE = 256;

int main() {
    open_directory();
    return 0;
}

//Tomado de https://poesiabinaria.net/2011/09/listar-archivos-dentro-de-un-directorio-o-carpeta-en-c/
void open_directory() {
    DIR *dir;
    struct dirent *ent;

    dir = opendir ("/home/aduarte/CLionProjects/PasswordDecoder/lists"); //Abre directorio TODO Hacerlo para cualquier dir

    if (dir == NULL)
        error("lists.");

    bool found = false; //Condicion de parada
    char* password; //Donde se guarda la palabra encontrada

    while (((ent = readdir (dir)) != NULL )&& !found)
    {
        /* Devolverá directorio actual (.) y el anterior (..)*/
        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
        {
            password = read_file(ent->d_name); //Llama a leer archivo actual
            if(strcmp(password,"-1") != 0) { //Si la encuentra se detiene
                found = true;
            }
        }
    }

    if(found) {
        printf("Password found: %s\n", password);
    } else {
        printf("Sorry, password not found.\n");
    }

    closedir (dir);
}

//Tomado de https://gsamaras.wordpress.com/code/read-file-line-by-line-in-c-and-c/
//-1 es que no encontró el password. Caso conrario se devuelve la palabra que hizo match.
char* read_file(char* file_name) {
    //Construye el path
    char* name_dir = "/home/aduarte/CLionProjects/PasswordDecoder/lists/";
    char* path = malloc(strlen(name_dir)+100);
    strcpy(path, name_dir);
    strcat(path, file_name);

    bool found = false; //Condición de parada
    char* password = malloc(sizeof(char)*4);
    strcpy(password, "-1"); //Inicializa el valor

    FILE* fp;
    char buf[BUF_SIZE];

    if ((fp = fopen(path, "r")) == NULL)
    {
        perror(file_name);
    }

    while ((fgets(buf, sizeof(buf), fp) != NULL) && !found) {
        buf[strlen(buf) - 1] = '\0';

        if(find_password(buf) == 1) { //Guarda palabra
            strcpy(password, buf);
            found = true;
        }
    }

    fclose(fp);

    free(path);

    return password;
}

//Compara hash generado con el brindado por el profesor
int find_password(char* word) {
    //Construye comando
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

    while (fgets(buf, BUF_SIZE, fp) != NULL && found == 0) {
        buf[strlen(buf) - 1] = '\0';
        if((strcmp("$1$SALt$.3QJjMGNt0dIcUC06TXYc1", buf) == 0)) {
            found = 1;
        }
    }

    if(pclose(fp))  {
        printf("Command not found or exited with error status\n");
        found = -1;
    }

    free(cmd);

    return found;
}


void error(const char *s)
{
    /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
    perror (s);
    exit(EXIT_FAILURE);
}