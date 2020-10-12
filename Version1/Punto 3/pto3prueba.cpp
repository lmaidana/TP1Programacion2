#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include<stdbool.h>

struct usuario{
    char nombre[30];
    char email[30];
    char contrasenia[30];
};

usuario pidoDatos(usuario datoIngresado);
FILE *abrir(const char *archivo, const char *modo);
bool validarContrasenia(char cadena[]);
bool validarEmail(char cadena[]);
bool validarNombre(char cadena[]);
int validarDatos(usuario datoIngresado);
int buscoExistente(FILE*acepta2,usuario datoIngresado,usuario unregistro, int pos);
void insertoOrdenado(FILE*acepta2,usuario datoIngresado,usuario unregistro,int pos);

using namespace std;

int main(){
    FILE*acepta2=abrir("users.dat","rb+");
    FILE*aux=abrir("auxiliar.dat","rb+");
    if(acepta2==NULL&&aux==NULL){
                printf("Error al abrir archivo");
            exit(EXIT_FAILURE);
        }
    int pos=0;
    int vale=0;
    bool ordenado=false;
    usuario datoIngresado;
    usuario unregistro;
    datoIngresado=pidoDatos(datoIngresado);
    vale=validarDatos(datoIngresado);
    while(vale != 0){
        pos=buscoExistente(acepta2,datoIngresado,unregistro,pos);
        if(pos!=0){
            insertoOrdenado(acepta2,datoIngresado,unregistro,pos);
            vale=0;
        }
    }
    fclose(acepta2);
    fclose(aux);
    return 0;
}
FILE *abrir(const char *archivo, const char *modo) {
    FILE *file = fopen(archivo, modo);
    if (file == NULL){
        printf("Error al abrir el archivo!");
        exit(EXIT_FAILURE);
    }
    return file;
}
usuario pidoDatos(usuario datoIngresado){  //  Pido datos por consola
        printf("Ingrese nombre de usuario:\n");
        gets(datoIngresado.nombre);
        printf("Ingrese email:\n");
        gets(datoIngresado.email);
        printf("Ingrese contrasenia:\n");
        gets(datoIngresado.contrasenia);
    return datoIngresado;
}
bool validarContrasenia(char cadena[]){
    int i=0;
    while(cadena[i]!='\0'){
        if(isalnum(cadena[i])){
        i++;
        }
        else
        {
        return false;
        }
        }if(i<8){
        return false;
        }
        return true;
}
bool validarEmail(char cadena[]){
    if(strchr(cadena,'@') == NULL && strchr(cadena,'.')==NULL){
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
bool validarNombre(char cadena[]){
    if (strlen(cadena)>15){
        return false;
    }else{
    return true;
    }
}
int validarDatos(usuario datoIngresado){
    int i=1;
    while(i<3){
        if(validarNombre(datoIngresado.nombre)&&validarEmail(datoIngresado.email)&&validarContrasenia(datoIngresado.contrasenia)){
            return 1;
        }else{
            printf("\n---------------------------------------\nDatos incorrectos! Vuelva a ingresar:\n---------------------------------------\n");
            i++;
            pidoDatos(datoIngresado);
        }
    }
    printf("\n---------------------------------------\nLimite de intentos, pruebe mas tarde\n---------------------------------------\n");
    return 0;
}
int buscoExistente(FILE*acepta2,usuario datoIngresado,usuario unregistro, int pos){
    int comparo;
    fread(&unregistro,sizeof(usuario),1,acepta2);
    while(!feof(acepta2)){
        fread(&unregistro,sizeof(usuario),0,acepta2);
        comparo=(strcmp(datoIngresado.email,unregistro.email)<0);
        while(comparo<=0){
            fseek(acepta2,0,SEEK_CUR);
            pos=(ftell(acepta2)/(sizeof(usuario)));
                if(strcmp(unregistro.nombre,datoIngresado.nombre)==0){
                     printf("\n-----------------------------\nNombre de usuario existente\n-----------------------------\n");
                     return 0;
                }if(strcmp(unregistro.email,datoIngresado.email)==0){
                    printf("\n-----------------------------\nCorreo existente\n-----------------------------\n");
                    return 0;
                }
        fseek(acepta2,sizeof(usuario)*pos,SEEK_SET);
        fread(&unregistro,sizeof(usuario),1,acepta2);
        pos=(ftell(acepta2)/(sizeof(usuario)));
        comparo=(strcmp(datoIngresado.email,unregistro.email)<0);
        }
        return pos;
    }
}
void insertoOrdenado(FILE*acepta2,usuario datoIngresado,usuario unregistro, int pos){
    pos=pos-1;
    fseek(acepta2,0,SEEK_END);
    int cantRegistros=(ftell(acepta2)/sizeof(usuario));
    usuario vecAux[(cantRegistros-pos)];
    int i=0;
    int j=pos;

    while (i<(cantRegistros-pos)){
        fseek(acepta2,j *sizeof(usuario),SEEK_SET);
        fread(&unregistro,sizeof(usuario),1,acepta2);
        memcpy(&vecAux[i],&unregistro,sizeof(usuario));
        i++;
        j++;
    }

    fseek(acepta2,pos *sizeof(usuario),SEEK_SET);
    fwrite(&datoIngresado,sizeof(usuario),1,acepta2);
    fseek(acepta2,(pos+1)*sizeof(usuario),SEEK_SET);
    for (int j=0; j<i; j++){
        fwrite(&vecAux[j],sizeof(usuario),1,acepta2);
    }
    printf("\n---------------------------------------\nUsuario insertado correctamente !\n---------------------------------------\n");
    return;
}
