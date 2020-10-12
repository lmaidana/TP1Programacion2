#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
using namespace std;
struct usuario{
    char nombre[30];
    char email[30];
    char contrasenia[30];
};
FILE *abrir(const char *archivo, const char *modo);
int validarContrasenia(char cadena[]);
bool validarEmail(char cadena[]);
int contarCaracteres(char cadena[]);
void validarDatos(usuario usuarios,FILE*todos,FILE*rechazado,FILE*aceptado);

int main(){
    FILE*todos=abrir("users.txt","r");
    FILE*rechazado=abrir("rejected.txt","w");
    FILE*aceptado=abrir("users.dat","wb");
    usuario usuarios;
    validarDatos(usuarios,todos,rechazado,aceptado);
    fclose(todos);
    fclose(rechazado);
    fclose(aceptado);
return 0;
}
void validarDatos(usuario usuarios,FILE*todos,FILE*rechazado,FILE*aceptado){
    while(fscanf(todos,"%[^;];%[^;];%[^\n]\n",usuarios.nombre,usuarios.email,usuarios.contrasenia) != EOF ){
        int flag=0;
        if (contarCaracteres(usuarios.nombre)>=15){
         fprintf(rechazado,"%s\t%s\t%s\n",usuarios.nombre,usuarios.email,usuarios.contrasenia);
         flag=1;
        }else if (validarEmail(usuarios.email)==0&&flag!=1){
          fprintf(rechazado,"%s\t%s\t%s\n",usuarios.nombre,usuarios.email,usuarios.contrasenia);
          flag=1;
        }else if (validarContrasenia(usuarios.contrasenia)==0&&flag!=1){
            fprintf(rechazado,"%s\t%s\t%s\n",usuarios.nombre,usuarios.email,usuarios.contrasenia);
        }else{
            fwrite(&usuarios,sizeof(usuario),1,aceptado);//Escribo en users.dat
            printf("%s\t%s\t%s\t\n", usuarios.nombre, usuarios.email, usuarios.contrasenia);//Imprimo por consola los aceptados.
        }
    }
    return;
}
int validarContrasenia(char cadena[]){
    int i=0;
    while(cadena[i]!='\0'){
        if(isalnum(cadena[i])){
        i++;
        }
        else
        {
        return 0;
        }
        }if(i<8){
        return 0;
        }
        return 1;
}
bool validarEmail(char cadena[]){
    if(strchr(cadena,'@') == NULL){
        return false;
    }else{
    int i=0;
    while(cadena[i]!= '@'){
        i++;
    }
    if(i>=15)
    return false;
    }
    return true;
}
int contarCaracteres(char cadena[]){
    int i=0;
    while(cadena[i]!= '\0'){
        i++;
    }
    return i;
}
FILE *abrir(const char *archivo, const char *modo) {
    FILE *file = fopen(archivo, modo);
    if (file == NULL){
        printf("Error al abrir el archivo!");
        exit(EXIT_FAILURE);
    }
    return file;
}
