
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



class Comando{
    public:
        Parametros param;
        void identificacionCMD(Parametros p);
        void crearArchivo(string tamano, string path, string ajuste, string dim);
        
};

#endif // COMANDO_H