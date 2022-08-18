
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
    string mbr_fecha_creacion = " ";
    int mbr_tamano = 0;
    int mbr_dsk_signature = 0;
    char dsk_fit='\0';
    string mbr_partition_1="";
    string mbr_partition_2="";
    string mbr_partition_3="";
    string mbr_partition_4="";
}MBR;



class Comando{
    public:
        Parametros param;
        void identificacionCMD(Parametros p);
        void crearArchivo(string tamano, string path, string ajuste, string dim);
};

#endif // COMANDO_H