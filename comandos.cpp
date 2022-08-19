
#include "comandos.h"
#include <iostream>
#include <string.h>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <unistd.h>

void Comando::identificacionCMD(Parametros p)
{
    if (p.Comando == "mkdisk")
    { // Se identifica el tipo de comando
        if (p.s != " " && p.path != " ")
        { // Se validan los parametros para el comando

            crearArchivo(p.s, p.path, p.f, p.u);
        }
        else
        {
            cout << "Error creando Disco: Parametros obligatorios no definidos." << endl;
        }
    }
    else if (p.Comando == "exec")
    { // Se identifica el tipo de comando
    }
    else if (p.Comando == "rep")
    { // Se identifica el tipo de comando
    }
}

void Comando::crearArchivo(string tam, string path, string ajuste, string dim)
{
    int size_file = 0, tamano = atoi(tam.c_str());

    char dimen = dim.at(0);

    if (dimen == 'k' || dimen == 'K')
    {
        size_file = tamano;
    }
    else if (dimen == 'm' || dimen == 'M' || dim == " ")
    {
        size_file = tamano * 1024;
    }
    else
    {
        cout << "Error creando Disco: Dimensional no reconocible." << endl;
    }

    if (tamano > 0)
    {
        // Preparacion Bloque

        string direct = "";

        string name_p = "";

        int start = 0;
        int end = path.find("/");
        string del = "/";
        while (end != -1)
        {
            cout << path.substr(start, end - start) << endl;
            direct += path.substr(start, end - start);
            direct += "/";
            start = end + del.size();
            end = path.find("/", start);
        }
        name_p = path.substr(start, end - start);

        string ncomando = name_p.substr(0, name_p.find("."));
        char solo_nombre[64];
        strcpy(solo_nombre, name_p.c_str());
        for (char c : ncomando)
        {
            if (!isalnum(c) && !isalpha(c) && c != '_')
            {
                cout << "Error: El nombre debe solo puede contener numeros, letras y '_'." << endl;
                return;
            }
        }
        int pos = name_p.find(".");
        name_p.erase(0, 1 + pos);

        transform(name_p.begin(), name_p.end(), name_p.begin(), ::tolower);

        if (name_p != "dsk")
        {
            cout << "Error: El nombre debe contener la extensión \".dsk\" al final" << endl;
            return;
        }

        char bloque[1024];
        for (int i = 0; i < 1024; i++)
        {
            bloque[i] = '\0';
        }

        // Escritura de Bloque en Archivo
        int limite = 0;

        FILE *archivo;
        direct += solo_nombre;
        direct+= ".dsk";

        
        string crear = "sudo -S mkdir -p \'" + direct + "\'";
        system(crear.c_str());
        // cout<<aux<<endl;
        string compermiso = "sudo -S chmod -R 777 \'" + direct + "\'";
        system(compermiso.c_str());

        if ((archivo = fopen(direct.c_str(), "w")) == NULL)
        {
            printf("[ERROR]error al crear el disco!\n");
        }
        else
        {
            srand(time(NULL));
            MBR p;

            p.mbr_fecha_creacion = time(0);
            p.mbr_tamano = size_file;
            p.mbr_dsk_signature = (rand() % 99);

            p.mbr_partition_1.part_status = '0';
            p.mbr_partition_2.part_status = '0';
            p.mbr_partition_3.part_status = '0';
            p.mbr_partition_4.part_status = '0';

            p.mbr_partition_1.part_type = '\0';
            p.mbr_partition_2.part_type = '\0';
            p.mbr_partition_3.part_type = '\0';
            p.mbr_partition_4.part_type = '\0';

            p.mbr_partition_1.part_fit = '\0';
            p.mbr_partition_2.part_fit = '\0';
            p.mbr_partition_3.part_fit = '\0';
            p.mbr_partition_4.part_fit = '\0';

            p.mbr_partition_1.part_start = 0;
            p.mbr_partition_2.part_start = 0;
            p.mbr_partition_3.part_start = 0;
            p.mbr_partition_4.part_start = 0;

            p.mbr_partition_1.part_s = 0;
            p.mbr_partition_2.part_s = 0;
            p.mbr_partition_3.part_s = 0;
            p.mbr_partition_4.part_s = 0;

            strcpy(p.mbr_partition_1.part_name, "");
            strcpy(p.mbr_partition_2.part_name, "");
            strcpy(p.mbr_partition_3.part_name, "");
            strcpy(p.mbr_partition_4.part_name, "");

            // escribir el mbr
            fwrite(&p, sizeof(MBR), 1, archivo);

            // llenando los espacios en blanco
            char vacio = '\0';
            int i = 0;

            for (i = sizeof(MBR); i < size_file; i++)
            {
                fwrite(&vacio, 1, 1, archivo);
            }

            fclose(archivo);

            
            cout << "\tDISCO CREADO CORRECTAMENTE..." << endl;
        }
    }
    else
    {
        cout << "Error creando Disco: tamaño invalido a ejecución." << endl;
    }
}
