#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

struct usuario
{
  char nombre[30];
  char email[30];
  char contrasenia[30];
};
FILE *abrir(const char *archivo, const char *modo);
int busqueda(FILE *acepta2, usuario elementoAbuscar, usuario unregistro);
usuario ingresoDatos();
void login(FILE *acepta2, usuario elementoAbuscar, usuario unregistro);

using namespace std;

int main()
{
  usuario elementoAbuscar;
  usuario unregistro;
  FILE *acepta2 = abrir("users.dat", "rb");
  login(acepta2, elementoAbuscar, unregistro);
  fclose(acepta2);
  return 0;
}
usuario ingresoDatos()
{ //Tomo datos por consola
  usuario elementoAbuscar;
  printf("Ingrese email:\n");
  gets(elementoAbuscar.email);
  printf("Ingrese contrasenia:\n");
  gets(elementoAbuscar.contrasenia);
  return elementoAbuscar;
}
void login(FILE *acepta2, usuario elementoAbuscar, usuario unregistro)
{
  int buscar = 1;
  int intentos = 0;
  elementoAbuscar = ingresoDatos();
  buscar = busqueda(acepta2, elementoAbuscar, unregistro);
  while (buscar == 1)
  {
    printf("----------------------------------------------\nUsuario/clave incorrectos! ingrese nuevamente:\n----------------------------------------------\n\n");
    intentos++;
    while (intentos <= 2 && buscar == 1)
    {
      elementoAbuscar = ingresoDatos();
      buscar = busqueda(acepta2, elementoAbuscar, unregistro);
      intentos++;
      if (buscar == 1 && intentos <= 2)
      {
        printf("----------------------------------------------\nUsuario/clave incorrectos! ingrese nuevamente:\n----------------------------------------------\n\n");
      }
    }
    if (buscar == 1)
    {
      printf("\n----------------------------------------------\nDatos incorrectos, vuelva a intentar en 1 hora\n----------------------------------------------\n");
      return;
    }
  }
  return;
}
int busqueda(FILE *acepta2, usuario elementoAbuscar, usuario unregistro)
{
  int cantRegistros = 0;
  int centro;
  fseek(acepta2, 0, SEEK_SET);
  int inicio = ftell(acepta2);
  fseek(acepta2, 0, SEEK_END);
  cantRegistros = (ftell(acepta2) / sizeof(usuario)) - 1;
  fseek(acepta2, 0, SEEK_SET);
  while (inicio <= cantRegistros)
  {
    centro = (inicio + cantRegistros) / 2;
    fseek(acepta2, centro * sizeof(usuario), SEEK_SET);
    fread(&unregistro, sizeof(usuario), 1, acepta2);
    if (strcmp(unregistro.email, elementoAbuscar.email) == 0 && strcmp(unregistro.contrasenia, elementoAbuscar.contrasenia) == 0)
    {
      printf("\n----------------------------------------------\nBienvenido!\nSus datos:\n%s\n%s\n%s\n----------------------------------------------", unregistro.nombre, unregistro.email, unregistro.contrasenia);
      return 2;
    }
    else if (strcmp(unregistro.email, elementoAbuscar.email) > 0)
    {
      cantRegistros = centro - 1;
    }
    else
    {
      inicio = centro + 1;
    }
  }
  return 1;
}
FILE *abrir(const char *archivo, const char *modo)
{
  FILE *file = fopen(archivo, modo);
  if (file == NULL)
  {
    printf("Error al abrir el archivo!");
    exit(EXIT_FAILURE);
  }
  return file;
}
