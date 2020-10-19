#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

struct pedido
{
  char email[30];
  int valor;
  int id;
  int cant;
};
struct usuario
{
  char nombre[30];
  char email[30];
  char password[30];
};
struct resumen
{
  char email[30];
  int cant;
  char nombre[30];
  int total;
};

FILE *abrir(const char *archivo, const char *modo);
resumen busqueda(FILE *archivo, pedido pedidos, usuario usuarios);

int main()
{
  FILE *pedidosbin = abrir("pedidos.dat", "rb");
  FILE *usersbin = abrir("users.dat", "rb");
  FILE *pedidostxt = abrir("pedidos.txt", "wt");
  pedido pedidos;
  usuario usuarios;
  resumen resumenes;
  fprintf(pedidostxt, "------------------------------PEDIDOS------------------------------\n");
  fread(&pedidos, sizeof(pedido), 1, pedidosbin);
  while (!feof(pedidosbin))
  {
    resumenes = busqueda(usersbin, pedidos, usuarios);
    resumenes.cant = 0;
    resumenes.total = 0;
    if (strcmp(resumenes.email, "0") != 0) //acá verifico con el flag de la binaria si existe o no.
    {
      while (strcmp(pedidos.email, resumenes.email) == 0 && !feof(pedidosbin))
      {
        resumenes.cant += pedidos.cant;
        resumenes.total += (pedidos.cant * pedidos.valor);
        fread(&pedidos, sizeof(pedido), 1, pedidosbin);
      }
      fprintf(pedidostxt, "Email: %s\tCant de prod: %d\nNombre: %s\tTotal: %d\n\n\n", resumenes.email, resumenes.cant, resumenes.nombre, resumenes.total); //Imprimo en el txt
      fread(&pedidos, sizeof(pedido), -1, pedidosbin);
    }
    else
    {
      fread(&pedidos, sizeof(pedido), 1, pedidosbin);
    }
  }
  fclose(pedidosbin);
  fclose(usersbin);
  fclose(pedidostxt);
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
resumen busqueda(FILE *archivo, pedido pedidos, usuario usuarios)
{
  char flag[2] = "0";
  resumen resumenes;
  int cantRegistros = 0;
  int centro = 0;
  fseek(archivo, 0, SEEK_SET);
  int inicio = ftell(archivo);
  fseek(archivo, 0, SEEK_END);
  cantRegistros = (ftell(archivo) / (sizeof(usuario)));
  fseek(archivo, 0, SEEK_SET);
  while (inicio <= cantRegistros)
  {
    centro = (inicio + cantRegistros) / 2;
    fseek(archivo, centro * sizeof(usuario), SEEK_SET);
    fread(&usuarios, sizeof(usuario), 1, archivo);
    if (strcmp(usuarios.email, pedidos.email) == 0)
    {
      strcpy(resumenes.email, usuarios.email);
      strcpy(resumenes.nombre, usuarios.nombre);
      return resumenes;
    }
    else if (strcmp(usuarios.email, pedidos.email) > 0)
    {
      cantRegistros = centro - 1;
    }
    else
    {
      inicio = centro + 1;
    }
  }
  printf("El correo %s no coincide !\n", pedidos.email); //Imprimo por consola los que no coinciden
  strcpy(resumenes.email, flag);                         //Este flag es para saber si el usuario existe o no, así en el corte de controlo lo saltea o no.
  return resumenes;
}