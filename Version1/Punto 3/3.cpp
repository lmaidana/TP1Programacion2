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
bool validarContrasenia(char cadena[]);
bool validarEmail(char cadena[]);
bool validarNombre(char cadena[]);
bool validarDatos(usuario &datoIngresado);
bool buscoNombre(FILE *acepta2, usuario datoIngresado, usuario unregistro);
bool buscoExistente(FILE *acepta2, usuario datoIngresado, usuario unregistro);
void insertoOrdenado(FILE *acepta2, usuario datoIngresado, usuario unregistro, int pos);
int getPos(FILE *acepta2, usuario datoIngresado, usuario unregistro, int pos);
usuario pidoDatos(usuario datoIngresado);

int main()
{
  usuario unregistro;
  int pos = 0;
  FILE *acepta2 = abrir("users.dat", "rb+");
  if (acepta2 == NULL)
  {
    printf("Error al abrir archivo");
    exit(EXIT_FAILURE);
  }
  usuario datoIngresado;
  datoIngresado = pidoDatos(datoIngresado);
  if (validarDatos(datoIngresado))
  {
    if (buscoNombre(acepta2, datoIngresado, unregistro) && buscoExistente(acepta2, datoIngresado, unregistro))
    {
      pos = getPos(acepta2, datoIngresado, unregistro, pos);
      insertoOrdenado(acepta2, datoIngresado, unregistro, pos);
    }
    else
    {
      printf("\n---------------------------------------No se ingresaron, revisar datos !---------------------------------------\n");
    }
  }
  return 0;
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

usuario pidoDatos(usuario datoIngresado)
{ //  Pido datos por consola
  printf("Ingrese nombre de usuario:\n");
  gets(datoIngresado.nombre);
  printf("Ingrese email:\n");
  gets(datoIngresado.email);
  printf("Ingrese contrasenia:\n");
  gets(datoIngresado.contrasenia);
  return datoIngresado;
}

bool validarContrasenia(char cadena[])
{
  int i = 0;
  while (cadena[i] != '\0')
  {
    if (isalnum(cadena[i]))
    {
      i++;
    }
    else
    {
      return false;
    }
  }
  if (i < 8)
  {
    return false;
  }
  return true;
}

bool validarEmail(char cadena[])
{
  if (strchr(cadena, '@') == NULL && strchr(cadena, '.') == NULL)
  {
    return false;
  }
  else
  {
    int i = 0;
    while (cadena[i] != '@')
    {
      i++;
    }
    if (i >= 15)
      return false;
  }
  return true;
}

bool validarNombre(char cadena[])
{
  if (strlen(cadena) > 15)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool validarDatos(usuario &datoIngresado)
{
  int i = 0;
  while (i < 3)
  {
    if (validarNombre(datoIngresado.nombre) && validarEmail(datoIngresado.email) && validarContrasenia(datoIngresado.contrasenia))
    {
      return true;
    }
    else
    {
      printf("\n---------------------------------------\nDatos incorrectos! Vuelva a ingresar:\n---------------------------------------\n");
      i++;
      if (i < 3)
      {
        datoIngresado = pidoDatos(datoIngresado);
      }
    }
  }
  printf("\n---------------------------------------\nLimite de intentos, pruebe mas tarde\n---------------------------------------\n");
  return false;
}

bool buscoNombre(FILE *archivo, usuario datoIngresado, usuario unregistro)
{
  fseek(archivo, 0, SEEK_SET);
  while (!feof(archivo))
  {
    fread(&unregistro, sizeof(usuario), 1, archivo);
    if (strcmp(datoIngresado.nombre, unregistro.nombre) == 0)
    {
      printf("\n-----------------------------\nNombre existente\n-----------------------------\n");
      return false;
    }
  }
  return true;
}

int getPos(FILE *acepta2, usuario datoIngresado, usuario unregistro, int pos)
{
  int centro = 0;
  fseek(acepta2, 0, SEEK_SET);
  int inicio = (ftell(acepta2));
  fseek(acepta2, 0, SEEK_END);
  int final = (ftell(acepta2) / sizeof(usuario)) - 1;
  fseek(acepta2, 0, SEEK_SET);
  while (inicio <= final)
  {
    centro = (inicio + final) / 2;
    fseek(acepta2, centro * sizeof(usuario), SEEK_SET);
    fread(&unregistro, sizeof(usuario), 1, acepta2);
    if (strcmp(unregistro.email, datoIngresado.email) == 0)
    {
      return 0;
    }
    else if (strcmp(unregistro.email, datoIngresado.email) > 0)
    {
      final = centro - 1;
    }
    else if (strcmp(unregistro.email, datoIngresado.email) < 0)
    {
      inicio = centro + 1;
    }
  }
  if (final == -1)
  {
    return pos = 0;
  }
  pos = (ftell(acepta2) / sizeof(usuario));
  if (pos % 2 == 0)
  {
    pos = (ftell(acepta2) / sizeof(usuario) - 1);
  }
  return pos;
}

void insertoOrdenado(FILE *acepta2, usuario datoIngresado, usuario unregistro, int pos)
{
  usuario aux;
  fseek(acepta2, 0, SEEK_END);
  int cantRegistros = (ftell(acepta2) / sizeof(usuario));
  int i = 0;
  while ((i + pos) <= cantRegistros)
  {
    fseek(acepta2, sizeof(usuario) * (pos + i), SEEK_SET);
    fread(&unregistro, sizeof(usuario), 1, acepta2);
    aux = unregistro;
    fseek(acepta2, sizeof(usuario) * (pos + i), SEEK_SET);
    fwrite(&datoIngresado, sizeof(usuario), 1, acepta2);
    datoIngresado = aux;
    i++;
  }
  return;
}

bool buscoExistente(FILE *acepta2, usuario datoIngresado, usuario unregistro)
{
  int centro = 0;
  fseek(acepta2, 0, SEEK_SET);
  int inicio = (ftell(acepta2));
  fseek(acepta2, 0, SEEK_END);
  int final = (ftell(acepta2) / sizeof(usuario)) - 1;
  fseek(acepta2, 0, SEEK_SET);
  while (inicio <= final)
  {
    centro = (inicio + final) / 2;
    fseek(acepta2, centro * sizeof(usuario), SEEK_SET);
    fread(&unregistro, sizeof(usuario), 1, acepta2);
    if (strcmp(unregistro.email, datoIngresado.email) == 0)
    {
      printf("\n-----------------------------\nCorreo existente\n-----------------------------\n");
      return false;
    }
    else if (strcmp(unregistro.email, datoIngresado.email) > 0)
    {
      final = centro - 1;
    }
    else if (strcmp(unregistro.email, datoIngresado.email) < 0)
    {
      inicio = centro + 1;
    }
  }
  int pos = (ftell(acepta2) / sizeof(usuario));
  return true;
}