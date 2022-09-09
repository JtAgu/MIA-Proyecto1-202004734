
#ifndef COMANDO_H
#define COMANDO_H

#include <fstream>
#include <cstdio>
#include <iostream>
#include "stdlib.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <string.h>
#include <vector>

using namespace std;


typedef char times[50];
static char metodoDeColocacionExtendida;


//ESTRUCTURAS DE USO PRINCIPAL
typedef struct{
    string Comando = " ";
    string s = " ";
    string f = " ";
    string u = " ";
    string t = " ";
    string path = " ";
    string delete_ = " ";
    string name = " ";
    string add_ = " ";
    string id = " ";
    string type = " ";
    string fs = " ";
    string usr = " ";
    string pass = " ";
    string grp = " ";
    string ugo = " ";
    string r = " ";
    string cont = " ";
    string fileN="";
    string p = " ";
    string destino = " ";
    string ruta=" ";
}Parametros;


typedef struct{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_s;
    char part_name[16];
} partitiond;


typedef struct{
    time_t mbr_fecha_creacion;
    int mbr_tamano;
    int mbr_dsk_signature;
    char dsk_fit;
    partitiond mbr_partition_1;
    partitiond mbr_partition_2;
    partitiond mbr_partition_3;
    partitiond mbr_partition_4;
}MBR;


typedef struct{
    
    char part_status;
    char part_fit;
    int part_start;
    int part_s;
    int part_next;
    char part_name[16];
}EBR;


typedef struct{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    
    times s_mtime;
    times s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_s;
    int s_block_s;
    int s_firts_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}superBloque;

typedef struct{
    int i_uid;     //UID DEL USR PROPIETARIO
    int i_gid;      //GID DEL GRUPO DEL PROP
    int i_size;     //TAMAÑO EN BYTES(SOLO EN FILES)
    times i_atime;  //FECHA SIN MODIFICAR
    times i_ctime;  //FECHA CREACION
    times i_mtime;  //ULTIMA FECHA MODIFICACION
    int i_block[15];//ENLACES A BLOQUES 12 DIRECT, 13 INDIRECT, 14 DOBLE INDIRECT, 15 TRIPLE INDIRECT (-1 IF NOT USE)
    char i_type;    //1 = FILE, 0 = DIR
    int i_perm;
}inodo;

typedef struct{
    char b_name[12];
    int b_inodo;    //apuntador hacia el innodo asociado
}content;


typedef struct{
    content b_content[4];
}bloqueCarpetas;


typedef struct{
    char b_content[64];
}bloqueArchivos;


typedef struct{
    int b_pointers[16];
}bloqueApuntadores;




//AQUI VAN ESTRUCTURAS AUXILIARES QUE SERVIRAN PARA ACARREAR DATOS DE FORMA ORDENADA EN EJECUION


typedef struct{
    bool encontrado;
    EBR B_ebr;
} prtLogica;

typedef struct{
    char j_tipo;
    char j_nombre[16];
    char j_contenido;
    times j_fecha;
    char Padre[12];
}journaling;

typedef struct mnt_nodo{
    partitiond mnt_particion;   //aplicable en las primaria
    EBR mnt_ebr;                //esto si es extendida
    times tiempo;              //tiempo en que fue montada la particion
    char mnt_ruta[512];
    char mnt_id[16];
    struct mnt_nodo *siguiente;
}mnt_nodo;

typedef struct mnt_lista{
    mnt_nodo* cabeza;
}mnt_lista;

static mnt_lista* listaDeParticiones;

typedef struct{
    char status;
}bmInodo;

typedef struct{
    char status;
}bmBloque;

//DECLARACION DE FUNCIONES DE COMANDO
class Comando{
    public:
        Parametros param;
        void identificacionCMD(Parametros p);
        void crearArchivo(string tamano, string path, string ajuste, string dim);
        void eliminarArchivo(string path);
        void fdisk(string dimension,string tamano,string path,string tipo,string ajuste,string del,string name,string add);
        void Cmount(string name,string path);
        void cUnmount(string id);
        void mkfs(string type,string id,string fs);
        void LogIn(string usr,string pass,string id);
        void mkgrp(string name);
        void mkusr(string usr,string grp,string pass);
        void chmod(string path,string ugo,string r);
        void mkfile(string path,string r,string s,string cont);
        void remove(string pathBuscar);
        void mkdir(string path,string padres);

        void rep(string name,string path,string id,string ruta);
};

typedef struct{
    int Id=0;
    char Grupo[10];
    char name[10];
    char pass[10];
}UserLog;

typedef struct{
    int Id;
    char name[10];
    vector <UserLog> usuarios;
}GrupoUser;

string replace_txt(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}


void cFdisk_add(string unit, string path, string name, string add);
void cFdisk_del(string path, string delet, string name);

prtLogica buscarLogica(string path, string name, partitiond particiones[4], int i);
partitiond BuscarPrimaria(string ruta, string nombre);
bool retornarN(string nombre, string ruta);
void mntPush(mnt_lista* lista, partitiond particion, EBR logica, string path);
void actualizarStatus(string path, string name, char status);
mnt_nodo* mntCrearNodo(partitiond particion, EBR logica, string ruta);
char numeroDeDisco(mnt_lista*lista, string name);

mnt_nodo retornarNodoMount(string ids);
void crear_ext2(mnt_nodo part,int n,int inicio);
void crear_ext3(mnt_nodo part,int n,int inicio);
superBloque crearSuper(mnt_nodo part,int n,int inicio,int tipo);
void EscribirBloqueMap(string path,bmBloque mapB[],int inicio, int n);
void EscribirInodoMap(string path,bmInodo mapI[],int inicio,int n);
void fechActual(times fecha);
void EscribirBloqueArchivo(string path, int inicio,bloqueArchivos file);
void EscribirBloqueCarpeta(string path, int inicio,bloqueCarpetas folder);
void EscribirNodo(string path, int inicio,int n,inodo lista[]);
void EscribirInodoUnidad(int posAct,inodo InodeUsers);
void EscribirSuper(string path,superBloque super,int inicio);

void BuscarUser(string path,int pos,string usr,string pass);
void OrdenarUsers(int posC,string regTemp);
string leerArchivo(inodo InodeUsers,FILE* archivo);
string leerIndirecto(int pos,bloqueApuntadores apuntadores,FILE* archivo);
string leerDobleIndirecto(int pos,bloqueApuntadores apuntadores,FILE* archivo);
string leerTripleIndirecto(int pos,bloqueApuntadores apuntadores,FILE* archivo);
int crearBloque(int tipo,string escribir,FILE* archivo);
int BuscarInodo(string name,int pos,FILE* archivo);
int recorrerCarpeta(string name,int pos,FILE* archivo);
void escribirFile(int posAct,inodo InodeUsers,string nCont);
int crearInodo(char tipo,string name,int size,FILE* archivo);

void actualizar_bmInodo(int n);
void actualizar_bmBlock(int n);
bool VerificarPermisoLeer(inodo Inodo);
bool VerificarPermisoEjecutar(inodo Inodo);
bool VerificarPermisoEscribir(inodo Inodo);
void ActualizarPermCascada(int pos,FILE* archivo,int permiso);
void PermisoCarpeta(int pos,FILE* archivo,int permiso);

void escribirDOT(string dot,string terminacion,string ruta);
string  TreeCarpeta(int pos,FILE* archivo,int num);
string TreeCascada(int pos,FILE* archivo,int num);

void RemoveCascada(int pos,FILE* archivo,vector <int> listaInode);
void RemoveCarpeta(int pos,FILE* archivo,vector <int> listaInode);
#endif // COMANDO_H


//cnijfilter-e400series-4.10-1-deb

//tar xzvf cnijfilter-e400series-4.10-1-deb.tar.gz