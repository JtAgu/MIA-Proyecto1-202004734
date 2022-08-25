
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
    int i_uid;     //UID del usuario propietario del archivo o carpeta
    int i_gid;      //GID al grupo al que pertenece
    int i_size;     //tamaño del archivo en bytes
    times i_atime;  //ultima fecha que se leyó el inodo sin modificarlo
    times i_ctime;  //fecha en la que se creó el inodo
    times i_mtime;  //ultima fecha en la que se modifico el inodo
    int i_block[15];//arreglo de punteros, los prieros 12 son directos, 13 indirecto, 14 indirecto doble, 15 indirecto triple (-1 si no se usan)
    char i_type;    //1 = archivo, 0 = carpeta
    int i_perm;
}inodo;

typedef struct{
    char b_name[12];
    int b_inodo;    //apuntador hacia el innodo asociado
}content;


typedef struct{
    content b_content[4];
}bloque;





//AQUI VAN ESTRUCTURAS AUXILIARES QUE SERVIRAN PARA ACARREAR DATOS DE FORMA ORDENADA EN EJECUION


typedef struct{
    bool encontrado;
    EBR B_ebr;
} prtLogica;


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

//DECLARACION DE FUNCIONES DE COMANDO
class Comando{
    public:
        Parametros param;
        void identificacionCMD(Parametros p);
        void crearArchivo(string tamano, string path, string ajuste, string dim);
        void eliminarArchivo(string path);
        void fdisk(string dimension,string tamano,string path,string tipo,string ajuste,string del,string name,string add);
        void Cmount(string name,string path);
        
};
void cFdisk_add(string unit, string path, string name, string add);
void cFdisk_del(string path, string delet, string name);

prtLogica buscarLogica(string path, string name, partitiond particiones[4], int i);
partitiond BuscarPrimaria(string ruta, string nombre);
bool retornarN(string nombre, string ruta);
void mntPush(mnt_lista* lista, partitiond particion, EBR logica, string path);
void actualizarStatus(string path, string name, char status);
mnt_nodo* mntCrearNodo(partitiond particion, EBR logica, string ruta);
char numeroDeDisco(mnt_lista*lista, string name);

#endif // COMANDO_H