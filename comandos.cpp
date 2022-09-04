
#include "comandos.h"
#include "accionesMBR.cpp"

#include <iostream>
#include <string.h>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <fstream>

const string fecha(time_t now) {
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X %d-%m-%Y ", &tstruct);

    return buf;
}

UserLog logeado;
vector <GrupoUser> leidos;
superBloque superLog;
string pathGlobal="";
int inicioG=0;


void Comando::identificacionCMD(Parametros p)
{
    //SE IDENTIFICA EL TIPO DE COMANDO
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
    }else if (p.Comando=="rmdisk"){
        if ( p.path != " ")
        { // Se validan los parametros para el comando

            eliminarArchivo(p.path);
        }
        else
        {
            cout << "Error eliminando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if (p.Comando=="rmdisk"){
        if ( p.path != " ")
        { // Se validan los parametros para el comando

            eliminarArchivo(p.path);
        }
        else
        {
            cout << "Error eliminando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if (p.Comando=="fdisk"){
        if ( p.path != " "&&p.name!=" ")
        { // Se validan los parametros para el comando
            fdisk(p.u,p.s,p.path,p.t,p.f,p.delete_,p.name,p.add_);
        }
        else
        {
            cout << "Error eliminando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if (p.Comando=="mount"){
        if ( p.path != " "&&p.name!=" ")
        { // Se validan los parametros para el comando
            Cmount(p.name,p.path);
        }
        
    }else if (p.Comando=="unmount"){
        if ( p.id != " ")
        { // Se validan los parametros para el comando
            cUnmount(p.id);
        }
        
    }else if (p.Comando=="mkfs"){
        
            mkfs(p.type,p.id,p.fs);
        
    }else if (p.Comando=="login"){
            if(logeado.Id==0){
                LogIn(p.usr,p.pass,p.id);
            }else{
                cout<<"ERROR: YA HAY UN USUARIO LOGEADO!"<<endl;
            }
        
    }else if (p.Comando=="logout"){
            if(logeado.Id>0){
                cout<<"CERRANDO SESION..."<<endl;
                strcpy(logeado.name,"");
                strcpy(logeado.Grupo,"");
                strcpy(logeado.pass,"");
                logeado.Id=0;

                EscribirSuper(pathGlobal,superLog,inicioG);

                leidos.clear();

            }else{
                cout<<"ERROR:  NO HAY NINGUN USUARIO LOGEADO!"<<endl;
            }
        
    }else if (p.Comando=="mkgrp"){
        if(logeado.Id>0){
            if(strcmp(logeado.name,"root")==0){
                mkgrp(p.name);
            }else{
                cout<<"ERROR: MKGRP SOLO PUEDE SER EJECUTADO POR ROOT!"<<endl;
            }
        }else{
            cout<<"ERROR:  NO HAY NINGUN USUARIO LOGEADO!"<<endl;
        }
    }else if (p.Comando=="rmgrp"){
        if(logeado.Id>0){
            if(strcmp(logeado.name,"root")==0){

            }else{
                cout<<"ERROR: RMGRP SOLO PUEDE SER EJECUTADO POR ROOT!"<<endl;
            }
        }else{
            cout<<"ERROR:  NO HAY NINGUN USUARIO LOGEADO!"<<endl;
        }
    }else if (p.Comando=="mkusr"){
        if(logeado.Id>0){
            if(strcmp(logeado.name,"root")==0){
                mkusr(p.usr,p.grp,p.pass);
            }else{
                cout<<"ERROR: MKUSR SOLO PUEDE SER EJECUTADO POR ROOT!"<<endl;
            }
        }else{
            cout<<"ERROR:  NO HAY NINGUN USUARIO LOGEADO!"<<endl;
        }
    }else if (p.Comando=="rmusr"){
        if(logeado.Id>0){
            if(strcmp(logeado.name,"root")==0){

            }else{
                cout<<"ERROR: RMUSR SOLO PUEDE SER EJECUTADO POR ROOT!"<<endl;
            }
        }else{
            cout<<"ERROR:  NO HAY NINGUN USUARIO LOGEADO!"<<endl;
        }
    }else if (p.Comando == "rep"){ // Se identifica el tipo de comando
        rep(p.name,p.path,p.id,p.ruta);
    }
}

void Comando::crearArchivo(string tam, string path, string ajuste, string dim)
{   

    int size_file = 0, tamano = atoi(tam.c_str());

    char dimen = dim.at(0);
    //VERIFICAMOS DIMENSIONAL DEL TAMAÑO
    if (dimen == 'k' || dimen == 'K')
    {
        //KILOBYTES == 1024 BYTES
        size_file = tamano*1024;
    }
    else if (dimen == 'm' || dimen == 'M' || dim == " ")
    {
        size_file = tamano * 1024 *1024;
    }
    else
    {
        cout << "Error creando Disco: Dimensional no reconocible." << endl;
    }

    if (tamano > 0)
    {
        //SEPARAMOS EL NNOMBRE Y RUTA

        string direct = "";

        string name_p = "";

        int start = 0;
        int end = path.find("/");
        string del = "/";
        while (end != -1)
        {
            //cout << path.substr(start, end - start) << endl;
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
        //VERIFICAMOS EXTENSION DEL DISCO
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
        
        //CREACION DE CARPETAS/DIRECTORIOS
        string crear = "sudo -S mkdir " + direct ;
        system(crear.c_str());
        // cout<<aux<<endl;
        string compermiso = "sudo chmod 777 " + direct ;
        system(compermiso.c_str());
        //CREACION DE DISCO
        if ((archivo = fopen(path.c_str(), "w+b")) == NULL)
        {
            printf("[ERROR]error al crear el disco!\n");
        }
        else
        {   //CONFIGURACION DE SUS CARACTERISTICAS
            srand(time(NULL));
            MBR p;

            p.mbr_fecha_creacion=time(0);
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

            if(ajuste==" "){
                ajuste="w";
            }
            
            p.dsk_fit=ajuste.at(0);

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

void Comando::eliminarArchivo (string path){
        string direct = "";

        string name_p = "";
        //SEPARANDO NOMBRE Y RUTA
        int start = 0;
        int end = path.find("/");
        string del = "/";
        while (end != -1)
        {
            
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
        }else{
            //COMANDO DE ELIMINACION DE ARCHIVO
            remove(path.c_str());
        }
}

void Comando :: fdisk(string dimension,string tamano,string path,string tipo,string ajuste,string del,string name,string add){
    //VERIFICAMOS QUE DELETE Y ADD NO VENGAN EN CONJUNTO
    if(del!=" "&&add!=" "){
        cout << "Error: comando Fdisk no puede tener parametros contradictorios" << endl;
        return;
    }
    //cout<<tamano.size()<<endl;
    if(tamano!=" "){
        //BUSCAMOS EL DISCO
        MBR disco=leerMBR(path);

        int prtActiva[] = {0,0,0,0};

        partitiond particiones[4];
        particiones[0] = disco.mbr_partition_1;
        particiones[1] = disco.mbr_partition_2;
        particiones[2] = disco.mbr_partition_3;
        particiones[3] = disco.mbr_partition_4;
        int tam=atoi(tamano.c_str());
        //CONFIGURAMOS LA DIMENSION DE PARTICION
        if(dimension!=" "){
            if(dimension=="k"){
                tam=tam*1024;
            }else if (dimension=="m"){
                tam=tam*1024*1024;
            }else if (dimension=="b"){
                
            }else{
                cout<<"Error: el parametro -unit solo reconoce: B, K y M. "<<endl;
                return;
            }
        }else{
            tam=tam*1024;
        }
        
        if(tipo!="l"){
            for (int i = 0; i < 4; i++) {
                if(particiones[i].part_s!=0){
                    prtActiva[i]=1;
                }
            }

            string nombre = "";
            
            for (int i = 0; i < 4; i++) {
                nombre=particiones[i].part_name;
                if(nombre==name){
                    cout<<"Error: El nombre "<<name<<" ya está ocupado. "<<endl;
                    return;
                }
            }
            //SI LA NUEVA ES EXTENDIDA VERIFICAMOS QUE AUN NO EXISTA ALGUNA
            if(tipo=="e"){
                for (int i = 0; i < 4; i++) {
                    string tipoA="";
                    tipoA=particiones[i].part_type;
                    if(tipoA=="e"){
                        cout<<"Error: Solo puede haber una partición extendida por disco. "<<endl;
                        return;
                    }
                }
            }

            int tamanoDisponible=0;
            int tDisponible[4]={0,0,0,0};
            int particionSeleccionada=0;
            

            //BUSCAMOS LA POSICION CORRECTA PARA AÑADIR LA PARTICION
            if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==0){ 
                tamanoDisponible = disco.mbr_tamano - (sizeof (disco) + 1);
                
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==1) { 
                tamanoDisponible = particiones[3].part_start - (sizeof (disco) + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==0) { 
                
                tDisponible[0] = particiones[2].part_start - (sizeof (disco) + 1);
                tDisponible[3] = disco.mbr_tamano - (particiones[2].part_start+particiones[2].part_s+1);
                if (tam > tDisponible[0]) {
                    if (tam > tDisponible[3]) {
                        cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                        return;
                    } else {
                        tamanoDisponible=tDisponible[3];
                        particionSeleccionada = 3;
                    }
                } else {
                    tamanoDisponible=tDisponible[0];
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==1) { 
                tamanoDisponible = particiones[2].part_start - (sizeof (disco) + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0) { 
                
                tDisponible[0] = particiones[1].part_start - (sizeof (disco) + 1);
                tDisponible[2] = disco.mbr_tamano - (particiones[1].part_start+particiones[1].part_s+1);
                if (tam > tDisponible[0]) {
                    if (tam > tDisponible[2]) {
                        cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                        return;
                    } else {
                        tamanoDisponible=tDisponible[2];
                        particionSeleccionada = 2;
                    }
                } else {
                    tamanoDisponible=tDisponible[0];
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==1) { 
                
                tDisponible[0] = particiones[1].part_start - (sizeof (disco) + 1);
                tDisponible[2] = particiones[3].part_start - (particiones[1].part_start+particiones[1].part_s+1);
                if (tam > tDisponible[0]) {
                    if (tam > tDisponible[2]) {
                        cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                        return;
                    } else {
                        tamanoDisponible=tDisponible[2];
                        particionSeleccionada = 2;
                    }
                } else {
                    tamanoDisponible=tDisponible[0];
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0) { 
                
                tDisponible[0] = particiones[1].part_start - (sizeof (disco) + 1);
                tDisponible[3] = disco.mbr_tamano - (particiones[2].part_start+particiones[2].part_s+1);
                if (tam > tDisponible[0]) {
                    if (tam > tDisponible[3]) {
                        cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                        return;
                    } else {
                        tamanoDisponible=tDisponible[3];
                        particionSeleccionada = 3;
                    }
                } else {
                    tamanoDisponible=tDisponible[0];
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1) { 
                tamanoDisponible = particiones[1].part_start - (sizeof (disco) + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==0) { 
                tamanoDisponible = disco.mbr_tamano - (particiones[0].part_start+particiones[0].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 1;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==1) { 
                tamanoDisponible = particiones[3].part_start - (particiones[0].part_start+particiones[0].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 1;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==0) { 
                
                tDisponible[1] = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_s + 1);
                tDisponible[3] = disco.mbr_tamano - (particiones[2].part_start+particiones[2].part_s+1);
                if (tam > tDisponible[1]) {
                    if (tam > tDisponible[3]) {
                        cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                        return;
                    } else {
                        tamanoDisponible=tDisponible[3];
                        particionSeleccionada = 3;
                    }
                } else {
                    tamanoDisponible=tDisponible[1];
                    particionSeleccionada = 1;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==1) {
                tamanoDisponible = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 1;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0) {
                tamanoDisponible = disco.mbr_tamano - (particiones[1].part_start+particiones[1].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 2;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0) {
                tamanoDisponible = particiones[3].part_start - (particiones[1].part_start+particiones[1].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 2;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0) {
                tamanoDisponible = disco.mbr_tamano - (particiones[2].part_start+particiones[2].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 3;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1) {
                cout<<"Error: ya se han ocupado las 4 particiones en este disco "<<endl;
                return;
            }

            cout<<"El tamaño disponible en el disco es de "<<tamanoDisponible<<" bytes"<<endl;
            cout<<"Se creará una partición con "<<tam<<" bytes"<<endl;
            cout<<"en la partición No."<<(particionSeleccionada+1)<<" del disco"<<endl;
            

            //CONFIGURACION POR DEFECTO
            if(ajuste==" "){
                ajuste="w";
            }
            
            if(tipo==" "){
                tipo="p";
            }

            particiones[particionSeleccionada].part_status='0';
            particiones[particionSeleccionada].part_type=tipo.at(0);
            particiones[particionSeleccionada].part_fit=ajuste.at(0);
            particiones[particionSeleccionada].part_s=tam;
            strcpy(particiones[particionSeleccionada].part_name,name.c_str());

            switch (particionSeleccionada) {
                case 0:
                    particiones[particionSeleccionada].part_start=(sizeof (disco)+1);
                    break;
                case 1:
                    particiones[particionSeleccionada].part_start=(particiones[0].part_start+particiones[0].part_s+1);
                    break;
                case 2:
                    particiones[particionSeleccionada].part_start=(particiones[1].part_start+particiones[1].part_s+1);
                    break;
                case 3:
                    particiones[particionSeleccionada].part_start=(particiones[2].part_start+particiones[2].part_s+1);
                    break;
            }

            cout<<endl;
            cout<<"Type: "<<tipo<<endl;
            cout<<"Fit: "<<ajuste<<endl;
            cout<<"Start: "<<particiones[particionSeleccionada].part_start<<endl;
            cout<<"Size: "<<tam<<endl;
            cout<<"Name: "<<name<<endl;
            cout<<endl;

            disco.mbr_partition_1=particiones[0];
            disco.mbr_partition_2=particiones[1];
            disco.mbr_partition_3=particiones[2];
            disco.mbr_partition_4=particiones[3];
            //ACTUALIZAMOS EL MBR DEL DISCO
            actualizarMBR(disco,path);
        }else{
            //AQUI ES SI LA PARTICION ES TIPO LOGICA
            MBR retorno = leerMBR(path);

            partitiond particiones[4];
            particiones[0] = retorno.mbr_partition_1;
            particiones[1] = retorno.mbr_partition_2;
            particiones[2] = retorno.mbr_partition_3;
            particiones[3] = retorno.mbr_partition_4;

            string tipo="";
            bool hayExt = false;
            int i;
            //VERIFICAMOS QUE EXISTA UNA PARTICION DE TIPO EXTENDIDA
            for (i = 0; i < 4; i++) {
                tipo="";
                tipo=particiones[i].part_type;
                if(tipo=="e"){
                    hayExt = true;
                    break;
                }
            }

            int inicio = 0;
            int tamanoA = 0;
            int tamanoDiponible = 0;

            if(hayExt==true) {
                i=i+1;
                cout<<"La partición extendida es la "<<i<<"del disco "<<endl;
                i=i-1;
                inicio = particiones[i].part_start;

                tamanoA = particiones[i].part_s;
                tamanoDiponible = inicio + tamanoA; 


                FILE *f;
                if ((f = fopen(path.c_str(), "r+b")) == NULL) {
                    
                        cout<<"Error: no se ha podido al abrir el disco!\n";
                    
                }else{
                    EBR B_ebr;
                    cout<<"Inicio: "<<inicio<<endl;
                    cout<<"Tamaño extendida "<<tamanoA<<endl;
                    
                    fseek(f, inicio, SEEK_SET);
                    fread(&B_ebr, sizeof (EBR), 1, f);
                    cout<<"FIT encontrado: "<<B_ebr.part_fit<<endl;
                    //VERIFICAMOS QUE EXISTE UN EBR ŔIMERO
                    if (B_ebr.part_fit == 'b' || B_ebr.part_fit == 'f' || B_ebr.part_fit == 'w') {
                        EBR anterior;
                        anterior = B_ebr;
                        int siguiente = B_ebr.part_next;
                        EBR aux;

                        while (true){
                            fseek(f, siguiente, SEEK_SET);
                            fread(&aux, sizeof (EBR), 1, f);
                            
                            if (aux.part_fit == 'b' || aux.part_fit == 'f' || aux.part_fit == 'w') { 
                                siguiente = aux.part_next;
                                anterior = aux;
                            } else { 
                                aux.part_start = anterior.part_start + sizeof (EBR) + anterior.part_s + 1;
                                fseek(f, aux.part_start, SEEK_SET);
                                aux.part_status = '0';
                                aux.part_fit = ajuste.at(0);

                                aux.part_s = tam;
                                aux.part_next = -1; 

                                strcpy(aux.part_name, name.c_str());
                                anterior.part_next = aux.part_start;

                                if (aux.part_start + aux.part_s > tamanoDiponible) {
                                    cout<<"Error: el tamaño supera el espacio disponible en la partición extendida!\n";
                                    fclose(f);
                                    return;

                                } else {
                                    fwrite(&aux, sizeof (EBR), 1, f);
                                    fseek(f, anterior.part_start, SEEK_SET);
                                    fwrite(&anterior, sizeof (EBR), 1, f);
                                    cout<<"Se creo la partición logica"<<endl;
                                }
                                break;
                            }
                        }
                    }else {
                        //COMO NO EXISTE CREAMOS Y AÑADIMOS LA PARTICION LOGICA
                        if(ajuste==" "||ajuste==""){
                           ajuste="w";
                        }
                        if(tam<tamanoA){
                            cout<<"iniciando"<<endl;
                        
                            B_ebr.part_status = '0';
                            B_ebr.part_fit = ajuste.at(0);
                            B_ebr.part_start = inicio;
                            B_ebr.part_s= tam;
                            B_ebr.part_next = -1;
                            strcpy(B_ebr.part_name, name.c_str());
                            cout<<"aqui va el ebr: "<<inicio<<endl;
                            fseek(f, inicio, SEEK_SET);
                            fwrite(&B_ebr, sizeof (EBR), 1, f);
                            cout<<"Se creó el primer EBR"<<endl;
                        }else
                        {
                            cout<<"TAMAÑO INSUFICIENTE PARA PARTICION LOGICA"<<endl;
                        }
                        
                    }
                    fclose(f);
                }
            }else{
                cout<<"Error: No se encontró ninguna partición extendida en el disco"<<endl;
                return;
            }
        }
        
    }else if(del!=" "){
        //EN CASO DE CASO ELIMINAR PREGUNTAMOS SI DESEA CONTINUAR
        string confirmacion="";
        cout << "¿ESTA SEGURO DE ELIMINAR ESTA PARTICION? ( Y / N )" << endl;
        getline(cin,confirmacion);
        if(confirmacion=="y"||confirmacion=="Y"){
            cFdisk_del(path,del,name);
        }else{
            cout << "VOLVIENDO PRINCIPAL" << endl;
        }
        
    }else if (add!=" "){
        //CASO DE AÑADIR TAMAÑO A LA PARTICION
        cFdisk_add(dimension,path,name,add);
    }
}


void cFdisk_add(string unit, string path, string name, string add) {
    //Evaluando unit
    if(unit!="") {
        if (unit != "B" && unit != "b" && unit != "K" && unit != "k" && unit != "M" && unit != "m") {
            cout << "Error: El parámetro \"@unit\" solo recibe los valores: B, K y M" << endl;
            return;
        }
    }else{
        unit="K";
    }

    int tam=atoi(add.c_str());
    if(unit=="B"||unit=="b"){
        tam=tam;
    }else if(unit=="K"||unit=="k"){
        tam = 1024 * tam;
    }else if(unit=="M"||unit=="m"){
        tam = 1024 * tam*1024;
    }

    MBR B_mbr = leerMBR(path);
    partitiond particiones[4];
    particiones[0] = B_mbr.mbr_partition_1;
    particiones[1] = B_mbr.mbr_partition_2;
    particiones[2] = B_mbr.mbr_partition_3;
    particiones[3] = B_mbr.mbr_partition_4;

    int i;
    char nombre[16];
    char nombre2[16];

    bool encontrado = false;
    for (i = 0; i < 4; i++) {
        strcpy(nombre, particiones[i].part_name);
        strcpy(nombre2, name.c_str());

        if (strncmp(nombre2, nombre, 16) == 0) {
            encontrado = true;
            break;
        }
    }


    int prtActiva[] = {0,0,0,0};

    for (int j = 0; j < 4; j++) {
        if(particiones[j].part_s!=0){
            prtActiva[j]=1;
        }
    }
    

    int tamanoDisponible = 0;
    if(encontrado==true){
        if(i==0){ //Casos en los que se puede añadir a la partición 1
            //Añadiendo a la partición 1 del disco
            cout<<"Añadiendo a la partición 1 del disco"<<endl;
            if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==0){//1 0 0 0
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_s<<"bytes"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==1){//1 0 0 1
                tamanoDisponible = particiones[3].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_s<<"bytes"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==0){//1 0 1 0
                tamanoDisponible = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_s<<"bytes"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==1){//1 0 1 1
                tamanoDisponible = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_s<<"bytes"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0){//1 1 0 0
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_s<<"bytes"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==1){//1 1 0 1
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_s<<"bytes"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0){//1 1 1 0
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                cout<<add<<"El tamaño disponible es: "<<tamanoDisponible<<endl;
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_s<<"bytes"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1){//1 1 1 1
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[0].part_s<<"bytes"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[0].part_s<<"bytes"<<endl;

                }
            }
        } else if(i==1){ //Casos en los que se puede añadir a la partición 2
            cout<<"Añadiendo a la partición 2 del disco"<<endl;
            if(prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0){//X 1 0 0
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[1].part_start+particiones[1].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[1].part_s<<"bytes"<<endl;
                    particiones[1].part_s = particiones[1].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[1].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==1){//X 1 0 1
                tamanoDisponible = particiones[3].part_start - (particiones[1].part_start+particiones[1].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[1].part_s<<"bytes"<<endl;
                    particiones[1].part_s = particiones[1].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[1].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0){//X 1 1 0
                tamanoDisponible = particiones[2].part_start - (particiones[1].part_start+particiones[1].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[1].part_s<<"bytes"<<endl;
                    particiones[1].part_s = particiones[1].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[1].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1){//X 1 1 1
                tamanoDisponible = particiones[2].part_start - (particiones[1].part_start+particiones[1].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[1].part_s<<"bytes"<<endl;
                    particiones[1].part_s = particiones[1].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[1].part_s<<"bytes"<<endl;

                }
            }
        } else if(i==2){ //Casos en los que se puede añadir a la partición 3
            cout<<"Añadiendo a la partición 3 del disco"<<endl;
            if(prtActiva[2]==1 && prtActiva[3]==0){//X X 1 0
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[2].part_start+particiones[2].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[2].part_s<<"bytes"<<endl;
                    particiones[2].part_s = particiones[2].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[2].part_s<<"bytes"<<endl;

                }
            }else if(prtActiva[2]==1 && prtActiva[3]==1){//X X 1 1
                tamanoDisponible = particiones[3].part_start - (particiones[2].part_start+particiones[2].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[2].part_s<<"bytes"<<endl;
                    particiones[2].part_s = particiones[2].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[2].part_s<<"bytes"<<endl;

                }
            }
        } else if(i==3){ //Casos en los que se puede añadir a la partición 4
            cout<<"Añadiendo a la partición 4 del disco"<<endl;
            if(prtActiva[3]==1){//X X 1 1
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[3].part_start+particiones[3].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"Error: el espacio disponible no es suficiente para añadir "<<add<<" a la partición "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"Tamaño anterior: "<<particiones[3].part_s<<"bytes"<<endl;

                    particiones[3].part_s = particiones[3].part_s + tam;
                    cout<<"Tamaño nuevo: "<<particiones[3].part_s<<"bytes"<<endl;
                }
            }
        }
        if (i == 0) {
            B_mbr.mbr_partition_1 = particiones[i];
        } else if (i == 1){
            B_mbr.mbr_partition_2 = particiones[i];
        } else if (i == 2){
            B_mbr.mbr_partition_3 = particiones[i];
        }else if (i == 3) {
            B_mbr.mbr_partition_4 = particiones[i];
        }

        actualizarMBR(B_mbr,path);
    }else{
        cout << "Error: no se encontró la partición: "<<name<<endl;
    }
}

void cFdisk_del(string path, string delet, string name) {
    
    MBR B_mbr = leerMBR(path);
    partitiond particiones[4];
    particiones[0] = B_mbr.mbr_partition_1;
    particiones[1] = B_mbr.mbr_partition_2;
    particiones[2] = B_mbr.mbr_partition_3;
    particiones[3] = B_mbr.mbr_partition_4;

    int i;
    char nombre[16];
    char nombre2[16];

    bool encontrado = false;
    for (i = 0; i < 4; i++) {
        strcpy(nombre, particiones[i].part_name);
        strcpy(nombre2, name.c_str());

        if (strncmp(nombre2, nombre, 16) == 0) {
            encontrado = true;
            break;
        }
    }

    EBR logic;
    bool encLog = false;

    if(encontrado==false){
        for (int j = 0; j < 4; j++) {
            if (particiones[j].part_type=='e') {
                prtLogica log = buscarLogica(path,name,particiones,j);
                encLog = log.encontrado;
                logic = log.B_ebr;
                break;
            }
        }
    }

    char nombreAnterior[16];
    strcpy(nombreAnterior,logic.part_name);

    


    if (encontrado == true) {
        if(particiones[i].part_status=='0'){
            cout << "Eliminando la Particion " << i + 1 << ", Nombre: " << particiones[i].part_name << endl;

            
                cout << "Tipo =full, size " << particiones[i].part_s << endl;

                FILE *f;
                if ((f = fopen(path.c_str(), "r+b")) == NULL) {
                    
                        cout << "Error: no se ha podido al abrir el disco!\n";
                    
                } else {
                    fseek(f, particiones[i].part_start, SEEK_SET);
                    //llenando los espacios en blanco
                    char vacio = '\0';
                    int i = 0;
                    for (i = 0; i < particiones[i].part_s; i++) {
                        fwrite(&vacio, 1, 1, f);
                    }
                    fclose(f);
                }

                particiones[i].part_s = 0;
                particiones[i].part_start = 0;
                particiones[i].part_fit = ' ';
                strcpy(particiones[i].part_name, "");
                particiones[i].part_status = '0';
                particiones[i].part_type = ' ';

            

            if (i == 0) {
                B_mbr.mbr_partition_1 = particiones[i];
            } else if (i == 1){
                B_mbr.mbr_partition_2 = particiones[i];
            } else if (i == 2){
                B_mbr.mbr_partition_3 = particiones[i];
            }else if (i == 3) {
                B_mbr.mbr_partition_4 = particiones[i];
            }
            actualizarMBR(B_mbr, path);
        }else{
            cout<<"Error: debe desmontar la partición para poder eliminarla"<<endl;
        }

    }else if (encLog == true) {
        cout << "Eliminando la Particion Lógica, Nombre: " << logic.part_name << endl;

        if(logic.part_status=='0') {
            
                cout << "Tipo =full, size " << logic.part_s << endl;

                FILE *f;
                if ((f = fopen(path.c_str(), "r+b")) == NULL) {
                    
                        cout << "Error: no se ha podido al abrir el disco!\n";
                    
                } else {
                    fseek(f, logic.part_start, SEEK_SET);
                    //llenando los espacios en blanco
                    char vacio = '\0';
                    int i = 0;
                    for (i = 0; i < logic.part_s; i++) {
                        fwrite(&vacio, 1, 1, f);
                    }
                    fclose(f);
                }

                logic.part_fit = '\0';
                logic.part_start = 0;
                strcpy(logic.part_name, "");
                logic.part_s = 0;
                logic.part_status = '0';

            
        }else{
            cout<<"Error: debe desmontar la partición para poder eliminarla"<<endl;
        }

    }else{
        cout << "Error: no se encontró la partición: "<<name<<endl;
    }
    
}


prtLogica buscarLogica(string path, string name, partitiond particiones[4], int i) {
    int inicio = particiones[i].part_start;
    int tamano = particiones[i].part_s;
    int tamanoDiponible = inicio + tamano;

    

    FILE *f;
    if ((f = fopen(path.c_str(), "r+b")) == NULL) {
        
            cout << "Error: no se ha podido al abrir el disco!\n";
        
    } else {
        EBR B_ebr;
        prtLogica rtrnLogica;
        rtrnLogica.encontrado = false;

        fseek(f, inicio, SEEK_SET);
        fread(&B_ebr, sizeof(EBR), 1, f);
        if (B_ebr.part_fit == 'b' || B_ebr.part_fit == 'f' || B_ebr.part_fit == 'w') {
            char nombre1[16] = "";
            char nombre2[16] = "";

            strcpy(nombre1, B_ebr.part_name);
            strcpy(nombre2, name.c_str());

            if (strcmp(nombre2, nombre1) == 0) {
                rtrnLogica.B_ebr = B_ebr;
                rtrnLogica.encontrado = true;
                return rtrnLogica;
            }

            EBR anterior;
            anterior = B_ebr;
            int siguiente = B_ebr.part_next;
            EBR aux;

            while (true) {
                fseek(f, siguiente, SEEK_SET);
                fread(&aux, sizeof(EBR), 1, f);

                if (aux.part_fit == 'b' || aux.part_fit == 'f' || aux.part_fit == 'w') { //Hay siguiente

                    strcpy(nombre1, aux.part_name);
                    strcpy(nombre2, name.c_str());
                    if (strncmp(nombre2, nombre1, 16) == 0) {
                        rtrnLogica.B_ebr = aux;
                        rtrnLogica.encontrado = true;
                        return rtrnLogica;
                    }

                    siguiente = aux.part_next;
                    anterior = aux;
                } else {
                    break;
                }
            }
        }
        fclose(f);
        return rtrnLogica;
    }
    prtLogica n;
    return n;
}


EBR devLogica(string path, string name){
    MBR B_mbr = leerMBR(path);

    partitiond particiones[4];
    particiones[0] = B_mbr.mbr_partition_1;
    particiones[1] = B_mbr.mbr_partition_2;
    particiones[2] = B_mbr.mbr_partition_3;
    particiones[3] = B_mbr.mbr_partition_4;

    int i;
    char nombre[16];
    char nombre2[16];



    EBR logic;
    bool encLog = false;

    for (int j = 0; j < 4; j++) {
        if (particiones[j].part_type=='e') {
            metodoDeColocacionExtendida = particiones[j].part_fit;
            prtLogica log = buscarLogica(path,name,particiones,j);
            encLog = log.encontrado;
            logic = log.B_ebr;
            break;
        }
    }

    return logic;
}


void Comando::Cmount(string name,string path){
    //primero buscamos en lista de primarias
    
    partitiond part = BuscarPrimaria(path, name); 
    //cout<<part.part_name<<endl;
    EBR eb;
    eb.part_fit='\0';
    strcpy(eb.part_name,"");
    eb.part_status=0;
    eb.part_s=0;
    eb.part_start=0;

    if (part.part_fit == 'b' || part.part_fit == 'f' || part.part_fit == 'w') {//verificamos si existe
        
        bool existe = retornarN(name,path);
        
        if(existe==false) {
            //push a la lista
            mntPush(listaDeParticiones, part, eb, path); 
            actualizarStatus(path, name, '1');
            cout << "\tPARTICION MONTADA EXITOSAMENTE" << endl;
        }else{
            cout<<"ERROR: "<<part.part_name<<" YA ESTA MONTADA "<<endl;
        }
    } else {
        eb = devLogica(path,name);
        //eb = devolverLogica(ruta, nombre);
        if (eb.part_fit == 'b' || eb.part_fit == 'f' || eb.part_fit == 'w') {
            bool existe = retornarN(name,path);
            if(existe== false){
                mntPush(listaDeParticiones, part, eb, path);
                actualizarStatus(path, name, '1');
                cout << "\tPARTICION MONTADA EXITOSAMENTE" << endl;
            }else{
                cout<<"ERROR: "<<eb.part_name<<" YA ESTA MONTADA "<<endl;
            }

        }else
            cout<<"Error: No se encontró la partición en el disco"<<endl;
    }

}

partitiond BuscarPrimaria(string ruta, string nombre) {
    MBR B_mbr = leerMBR(ruta);
    //buscando la posicion de la partición logica
    partitiond particiones[4];
    particiones[0] = B_mbr.mbr_partition_1;
    particiones[1] = B_mbr.mbr_partition_2;
    particiones[2] = B_mbr.mbr_partition_3;
    particiones[3] = B_mbr.mbr_partition_4;

    int i;
    for (i = 0; i < 4; i++) {
        if (strncmp(particiones[i].part_name, nombre.c_str(), 16) == 0) {
            if (particiones[i].part_type == 'p') {
                return particiones[i];
            }else{
                break;
            }
        }
    }

    partitiond reto;
    reto.part_status='0';
    reto.part_type='\0';
    reto.part_fit='\0';
    reto.part_start = 0;
    reto.part_s = 0;
    strcpy(reto.part_name,"");

    return reto;

}


void mntPush(mnt_lista* lista, partitiond particion, EBR logica, string path) {
    mnt_nodo* nodo = mntCrearNodo(particion, logica, path);
    cout<<nodo->mnt_id<<endl;
    if (lista->cabeza == NULL) {
        //Lista vacia
        lista->cabeza = nodo;
    } else {
        //Lista llena
        mnt_nodo* puntero = lista->cabeza;
        while (puntero->siguiente) {
            puntero = puntero->siguiente;
        }
        puntero->siguiente = nodo;
    }
}

mnt_nodo* mntCrearNodo(partitiond particion, EBR logica, string ruta) {
    mnt_nodo* nodo = (mnt_nodo*) malloc(sizeof (mnt_nodo));
    
    nodo->mnt_particion = particion;
    nodo->mnt_ebr = logica;
    strcpy(nodo->mnt_ruta, ruta.c_str());
    

    
    
    string direct = "";

    string name_p = "";

    int start = 0;
    int end = ruta.find("/");
    string del = "/";
    while (end != -1)
    {
        //cout << path.substr(start, end - start) << endl;
        direct += ruta.substr(start, end - start);
        direct += "/";
        start = end + del.size();
        end = ruta.find("/", start);
    }
    name_p = ruta.substr(start, end - start);
    int pos = name_p.find(".");
    name_p.erase(pos, name_p.size());
    string id="34";
    id+= numeroDeDisco(listaDeParticiones,name_p);
    for(int i=0;i<name_p.size();i++){
        id+=name_p.at(i);
    }
    strcpy(nodo->mnt_id, id.c_str());
    time_t tiem = time(0);
    
    srand(time(NULL));
    strcpy(nodo->tiempo, fecha(time(0)).c_str());
    strcpy(nodo->mnt_ruta, ruta.c_str());

    nodo->siguiente = NULL;
    
    return nodo;
}




char numeroDeDisco(mnt_lista*lista, string name) {
    mnt_nodo*puntero = lista->cabeza;
    char letraTemporal = '1';
    int retorno = 1;
    if (lista->cabeza == NULL) {
        return '1';
    }
    
    while (puntero) {
        string namea="";
        for(int i=3;i<16;i++){
            namea+=to_string(puntero->mnt_id[i]);
        }
        if (name != namea)
            retorno = 0;
        //retorno= strcmp(&letra,&puntero->mnt_id[2]);//retorna 0 si son iguales
        if (retorno != 0) {

            if ((puntero->mnt_id[3]) > letraTemporal)
                letraTemporal = puntero->mnt_id[3]; //se va buscando el mas grande

            //si lo encontro retorna 0
        }
        puntero = puntero->siguiente;
    };

    if (retorno != 0) {
        return '1';
    } else {
        return letraTemporal + 1;
    }

}


void actualizarStatus(string path, string name, char status){
    MBR B_mbr = leerMBR(path);
    partitiond particiones[4];
    particiones[0] = B_mbr.mbr_partition_1;
    particiones[1] = B_mbr.mbr_partition_2;
    particiones[2] = B_mbr.mbr_partition_3;
    particiones[3] = B_mbr.mbr_partition_4;

    int i;
    char nombre[16];
    char nombre2[16];

    bool encontrado = false;
    for (i = 0; i < 4; i++) {
        strcpy(nombre, particiones[i].part_name);
        strcpy(nombre2, name.c_str());

        if (strncmp(nombre2, nombre, 16) == 0) {
            encontrado = true;
            break;
        }
    }

    EBR logic;
    bool encLog = false;

    if(encontrado==false){
        for (int j = 0; j < 4; j++) {
            if (particiones[j].part_type=='e') {
                prtLogica log = buscarLogica(path,name,particiones,j);
                encLog = log.encontrado;
                logic = log.B_ebr;
                break;
            }
        }
    }


    char nombreAnterior[16];
    strcpy(nombreAnterior,logic.part_name);

  


    if (encontrado == true) {
        cout << "Actualizando Status de la partición " << i + 1 << ", Nombre: " << particiones[i].part_name << endl;

        if(particiones[i].part_type=='p'){
            particiones[i].part_status = status;
       }

        if (i == 0) {
            B_mbr.mbr_partition_1 = particiones[i];
        } else if (i == 1){
            B_mbr.mbr_partition_2 = particiones[i];
        } else if (i == 2){
            B_mbr.mbr_partition_3 = particiones[i];
        }else if (i == 3) {
            B_mbr.mbr_partition_4 = particiones[i];
        }
        actualizarMBR(B_mbr, path);
    }else if (encLog == true) {
        cout << "Actualizando la Particion Lógica, Nombre: " << logic.part_name << endl;
        //cout<<"LOGIC "<<logic.part_status<<endl;
        logic.part_status=status;
        //cout<<logic.part_name<<" LOGIC "<<logic.part_status<<endl;
        FILE *f;
        if ((f = fopen(path.c_str(), "r+b")) == NULL) {

        }else{
            fseek(f, logic.part_start, SEEK_SET);
            fwrite(&logic, sizeof (logic), 1, f);
            fclose(f);
        }

    }else{
        cout << "Error: no se encontró la partición: "<<name<<endl;
    }
}


bool retornarN(string nombre, string ruta) {
    mnt_lista* lista = listaDeParticiones;
    mnt_nodo* puntero = lista->cabeza;
    int r1,r2;
    //cout<<puntero<<endl;
    while (puntero) {
        //cout<<ruta<<" "<< puntero->mnt_ruta<<endl;
        r1 = strcmp(ruta.c_str(), puntero->mnt_ruta);
        //cout<<nombre<<" "<<puntero->mnt_particion.part_name<<" "<<puntero->mnt_ebr.part_name<<endl;
        if (puntero->mnt_particion.part_fit == 'b' || puntero->mnt_particion.part_fit == 'f' || puntero->mnt_particion.part_fit == 'w') {
            r2 = strcmp(nombre.c_str(), puntero->mnt_particion.part_name);
        }else{
            r2 = strcmp(nombre.c_str(), puntero->mnt_ebr.part_name);
        }
        //cout<<nombre<<" "<<puntero->mnt_particion.part_name<<" "<<puntero->mnt_ebr.part_name<<endl;
        if ((r1 == 0&&r2 == 0))//lo encontró
            return true;
        puntero = puntero->siguiente;
    }
    return false;
}

void TerminarMontado(){
    mnt_lista* lista = listaDeParticiones;
    mnt_nodo* puntero = lista->cabeza;
    while(puntero){
        actualizarStatus(puntero->mnt_ruta,puntero->mnt_particion.part_name,'0');
        puntero=puntero->siguiente;
    }
}

void inicializarListaMount() {
    listaDeParticiones = (mnt_lista*) malloc(sizeof (mnt_lista)); //inicializando las listas
    listaDeParticiones->cabeza = NULL;
}

void Comando ::cUnmount(string id){
    //cout<<"pre"<<endl;
     mnt_nodo* puntero = listaDeParticiones->cabeza;
    if(puntero){
        char identificador[16];
        strcpy(identificador,id.c_str());
        //cout<<"RECIBO "<<identificador<<endl;
            
        mnt_nodo* pAnterior = NULL;
        bool entro= false;
        while (puntero->siguiente) {
            if(strncmp(puntero->mnt_id,identificador,16)==0){
                if(pAnterior==NULL){
                    cout<<"1"<<endl;
                
                    listaDeParticiones->cabeza = puntero->siguiente;
                    if(puntero->mnt_ebr.part_start!=0){
                        actualizarStatus(puntero->mnt_ruta,puntero->mnt_ebr.part_name,'0');
                    } else{
                        actualizarStatus(puntero->mnt_ruta,puntero->mnt_particion.part_name,'0');
                    }

                    entro=true;
                }else{
                    cout<<"2"<<endl;
                    pAnterior->siguiente=puntero->siguiente;
                    if(puntero->mnt_ebr.part_start!=0){
                        actualizarStatus(puntero->mnt_ruta,puntero->mnt_ebr.part_name,'0');
                    } else{
                        actualizarStatus(puntero->mnt_ruta,puntero->mnt_particion.part_name,'0');
                    }
                    entro=true;
                }
            }
            pAnterior = puntero;
            puntero = puntero->siguiente;
        }
        if(entro==false){
            
            if(strncmp(puntero->mnt_id,identificador,16)==0){
            
                    cout<<"3,1"<<endl;
                    if(pAnterior){
                        pAnterior->siguiente=NULL;
                    }else{
                        listaDeParticiones->cabeza=NULL;
                    }
                    if(puntero->mnt_ebr.part_start!=0){
                        actualizarStatus(puntero->mnt_ruta,puntero->mnt_ebr.part_name,'0');
                    } else{
                        actualizarStatus(puntero->mnt_ruta,puntero->mnt_particion.part_name,'0');
                    }
                    
                
            }else{
                cout<<"* NO HAY REGISTRO DEL ID "<<id<<" *"<<endl;
            }

        }
    }else{
        cout<<"* NO HAY PARTICIONES MONTADAS *"<<endl;
    }
}

mnt_nodo retornarNodoMount(string ids) {
    //cout<<ids<<endl;
    mnt_lista* lista = listaDeParticiones;
    mnt_nodo* puntero = lista->cabeza;
    int retorno;
    //cout<<ids<<endl;
    while (puntero) {
        retorno = strncmp(ids.c_str(), puntero->mnt_id, 16);
        if (retorno == 0){//lo encontró
            //cout<<ids<<endl;
            return *puntero;
        }
        puntero = puntero->siguiente;
    }
    cout<<"ERROR: NO HAY REGISTRO DE ID"<<endl;
    
    mnt_nodo re;
    strcpy(re.mnt_ruta, "");
    return re;
}

void Comando::mkfs(string type,string id,string fs){
    mnt_nodo particion=retornarNodoMount(id);
    if(particion.mnt_ruta!=""){
        int part_inicio = 0;
        int part_tamano = 0;
        char part_colocacion = ' ';
        int pimerEspacioEBR = 0;

        char path[512];
        char nombre[16];
        char status;

        if (particion.mnt_particion.part_type == 'p') {
            part_inicio = particion.mnt_particion.part_start;
            part_tamano = particion.mnt_particion.part_s;
            part_colocacion = particion.mnt_particion.part_type;
            strcpy(path,particion.mnt_ruta);
            strcpy(nombre,particion.mnt_particion.part_name);
            status = particion.mnt_particion.part_status;
            pimerEspacioEBR = 0;
        } else {
            part_inicio = particion.mnt_ebr.part_start;
            part_tamano = particion.mnt_ebr.part_s;
            part_colocacion = metodoDeColocacionExtendida;
            strcpy(path,particion.mnt_ruta);
            strcpy(nombre,particion.mnt_ebr.part_name);
            status = particion.mnt_ebr.part_status;
            pimerEspacioEBR = sizeof (EBR);
        }
        //cout<<particion.mnt_id<<endl;
        if(fs==" "){
            fs="2fs";
        }
        if(fs=="2fs"){
            if(type=="full"){
                double partta = (double) part_tamano;
                double nu;
                nu = (partta - sizeof(superBloque)) / (4.0 + sizeof(inodo) + 3*64);
                
                
                int n = (int) nu;

                crear_ext2(particion,n,part_inicio+pimerEspacioEBR);
            }
        }    
        if(type=="3fs"){
            double TamanoPart = (double) part_tamano;
            double NumInodos;
            NumInodos = (TamanoPart - sizeof(superBloque)) / (4.0 + sizeof(inodo) + 3*64 + sizeof(journaling));        
            
            int nInodo = (int) NumInodos;

            crear_ext3(particion,nInodo,part_inicio+pimerEspacioEBR);
        }
    }
    
}

void crear_ext2(mnt_nodo part,int n,int inicio){
    superBloque super= crearSuper(part,n,inicio,2);
    EscribirSuper(part.mnt_ruta,super,inicio);
    bmBloque bitmapBloques[n*3];
    bmBloque aux;
    //cout<<part.mnt_id<<endl;
    bitmapBloques[0].status='1';
    bitmapBloques[1].status='1';
    aux.status='0';
    for(int i=2;i<n*3;i++){
        bitmapBloques[i]=aux;
    }
    //cout<<part.mnt_id<<endl;
    EscribirBloqueMap(part.mnt_ruta,bitmapBloques,super.s_bm_block_start,3*n);
    //cout<<part.mnt_id<<endl;
    bmInodo bitmapInodo[n];
    bmInodo auxI;
    bitmapInodo[0].status='1';
    bitmapInodo[1].status='1';
    auxI.status='0';
    for(int i=2;i<n;i++){
        bitmapInodo[i]=auxI;
    }

    EscribirInodoMap(part.mnt_ruta,bitmapInodo,super.s_bm_inode_start,n);


    inodo root;
    
    times fech;
    fechActual(fech);
    

    for(int i=0;i<16;i++){
        root.i_block[i]=-1;
    }
    
    inodo lista[n];
    for (int t = 0; t < n; t++) {
        lista[t].i_uid=1;
        lista[t].i_gid=1;
        strcpy(lista[t].i_atime,"");
        strcpy(lista[t].i_ctime,"");
        strcpy(lista[t].i_mtime,"");
        lista[t].i_type='0';
        for(int i=0;i<16;i++){
            lista[t].i_block[i]=-1;
        }
    }
    
    strcpy(lista[0].i_ctime,fech);
    strcpy(lista[0].i_mtime,fech);
    lista[0].i_type='0';
    lista[0].i_block[0]=0;
    lista[0].i_perm=700;

    strcpy(lista[1].i_ctime,fech);
    strcpy(lista[1].i_mtime,fech);
    lista[1].i_type='1';
    lista[1].i_block[0]=1;
    lista[1].i_perm=700;

    EscribirNodo(part.mnt_ruta,super.s_inode_start,n,lista);
    
    super.s_firts_ino=super.s_firts_ino+2*sizeof(inodo);

    bloqueCarpetas carpet;
    content contenido;
    contenido.b_inodo=1;
    strcpy(contenido.b_name,"users.txt");
    carpet.b_content[0]=contenido;
    carpet.b_content[1].b_inodo=-1;
    carpet.b_content[2].b_inodo=-1;
    carpet.b_content[3].b_inodo=-1;
    
    EscribirBloqueCarpeta(part.mnt_ruta,super.s_first_blo,carpet);
    super.s_first_blo=super.s_first_blo+sizeof(bloqueCarpetas);

    bloqueArchivos fUsers;
    strcpy(fUsers.b_content,"1,G,root\n1,U,root,root,123\n");
    EscribirBloqueArchivo(part.mnt_ruta,super.s_first_blo,fUsers);
    super.s_first_blo=super.s_first_blo+sizeof(bloqueArchivos);
    EscribirSuper(part.mnt_ruta,super,inicio);
}

void crear_ext3(mnt_nodo part,int n,int inicio){
    superBloque super= crearSuper(part,n,inicio,3);
    EscribirSuper(part.mnt_ruta,super,inicio);

    journaling auxJ[n]; //CREA LISTA DE JOURN
    int inicio1 =inicio + sizeof (superBloque); //ESTABLECE INICIO
    FILE* archivo=fopen(part.mnt_ruta,"r+b");
    for (int j = 0; j < n; j++) {
        fseek(archivo, inicio1 + j * (sizeof (journaling)), SEEK_SET);
        fwrite(&auxJ[j], sizeof (journaling), 1, archivo);
    }


    bmBloque bitmapBloques[n*3];
    bmBloque aux;
    //cout<<part.mnt_id<<endl;
    bitmapBloques[0].status='1';
    bitmapBloques[1].status='1';
    aux.status='0';
    for(int i=2;i<n*3;i++){
        bitmapBloques[i]=aux;
    }
    //cout<<part.mnt_id<<endl;
    EscribirBloqueMap(part.mnt_ruta,bitmapBloques,super.s_bm_block_start,3*n);
    //cout<<part.mnt_id<<endl;
    bmInodo bitmapInodo[n];
    bmInodo auxI;
    bitmapInodo[0].status='1';
    bitmapInodo[1].status='1';
    auxI.status='0';
    for(int i=2;i<n;i++){
        bitmapInodo[i]=auxI;
    }

    EscribirInodoMap(part.mnt_ruta,bitmapInodo,super.s_bm_inode_start,n);


    inodo root;
    
    times fech;
    fechActual(fech);
    

    for(int i=0;i<16;i++){
        root.i_block[i]=-1;
    }
    
    inodo lista[n];
    for (int t = 0; t < n; t++) {
        lista[t].i_uid=1;
        lista[t].i_gid=1;
        strcpy(lista[t].i_atime,"");
        strcpy(lista[t].i_ctime,"");
        strcpy(lista[t].i_mtime,"");
        lista[t].i_type='0';
        for(int i=0;i<16;i++){
            lista[t].i_block[i]=-1;
        }
        lista[t].i_perm=700;
    }
    
    strcpy(lista[0].i_ctime,fech);
    strcpy(lista[0].i_mtime,fech);
    lista[0].i_type='0';
    lista[0].i_block[0]=0;

    strcpy(lista[1].i_ctime,fech);
    strcpy(lista[1].i_mtime,fech);
    lista[1].i_type='1';
    lista[1].i_block[0]=1;

    EscribirNodo(part.mnt_ruta,super.s_inode_start,n,lista);
    
    super.s_firts_ino=super.s_firts_ino+2*sizeof(inodo);

    bloqueCarpetas carpet;
    content contenido;
    contenido.b_inodo=1;
    strcpy(contenido.b_name,"users.txt");
    carpet.b_content[0]=contenido;
    EscribirBloqueCarpeta(part.mnt_ruta,super.s_first_blo,carpet);
    super.s_first_blo=super.s_first_blo+sizeof(bloqueCarpetas);

    bloqueArchivos fUsers;
    strcpy(fUsers.b_content,"1,G,root\n1,U,root,root,123\n");
    EscribirBloqueArchivo(part.mnt_ruta,super.s_first_blo,fUsers);
    super.s_first_blo=super.s_first_blo+sizeof(bloqueArchivos);
    EscribirSuper(part.mnt_ruta,super,inicio);
}

superBloque crearSuper(mnt_nodo part,int n,int inicio,int tipo){
    superBloque super;
    if(tipo==2){
        super.s_filesystem_type=2;
        super.s_inodes_count=n;
        super.s_blocks_count=3*n;
        super.s_free_blocks_count=3*n-2;
        super.s_free_inodes_count=n-2;
        strcpy(super.s_umtime,part.tiempo);
        strcpy(super.s_mtime,part.tiempo);
        super.s_mnt_count=1;
        super.s_magic=0xEF53;
        super.s_inode_s=sizeof(inodo);
        super.s_block_s=sizeof(bloqueCarpetas);

        super.s_firts_ino = inicio + sizeof (superBloque) + 3 * n + n ; 
        super.s_first_blo = super.s_firts_ino + n * sizeof (inodo); 

        super.s_bm_inode_start = inicio + sizeof (superBloque);
        super.s_bm_block_start = super.s_bm_inode_start + n;
        cout<<"BLOCK "<<super.s_bm_block_start<<endl;
        cout<<"INODO "<<super.s_bm_inode_start<<endl;


        super.s_inode_start = super.s_firts_ino;
        super.s_block_start = super.s_inode_start + n * sizeof (inodo);
        return super;
    }
    super.s_filesystem_type=3;
    super.s_inodes_count=n;
    super.s_blocks_count=3*n;
    super.s_free_blocks_count=3*n-2;
    super.s_free_inodes_count=n-2;
    strcpy(super.s_umtime,part.tiempo);
    strcpy(super.s_mtime,part.tiempo);
    super.s_mnt_count=1;
    super.s_magic=0xEF53;
    super.s_inode_s=sizeof(inodo);
    super.s_block_s=sizeof(bloqueCarpetas);

    super.s_firts_ino = inicio + sizeof (superBloque) + 3 * n + n+ n*sizeof(journaling); 
    super.s_first_blo = super.s_firts_ino + n * sizeof (inodo); 

    super.s_bm_inode_start = inicio + sizeof (superBloque)+sizeof(journaling)*n;
    super.s_bm_block_start = super.s_bm_inode_start + n;

    super.s_inode_start = super.s_firts_ino;
    super.s_block_start = super.s_inode_start + n * sizeof (inodo);

    return super;
}



void EscribirSuper(string path,superBloque super,int inicio){
    FILE* f;
    if((f=fopen(path.c_str(),"r+b"))==NULL){
        cout<<"ERROR, NO SE PUDO ABRIR EL DISCO"<<endl;
        return;
    }
    fseek(f, inicio, SEEK_SET);
    fwrite(&super, sizeof (superBloque), 1, f);
    fclose(f);
}


void EscribirBloqueMap(string path,bmBloque mapB[],int inicio, int n){
    FILE* f;
    if((f=fopen(path.c_str(),"r+b"))==NULL){
        cout<<"ERROR, NO SE PUDO ABRIR EL DISCO"<<endl;
        return;
    }
    //cout<<"inicio "<<inicio<<endl;
    for(int i=0;i<n;i++){
        cout<<"voy por "<<(inicio+i)<<endl;
        cout<<"con "<<mapB[i].status<<endl;
        fseek(f, inicio+i, SEEK_SET);
        fwrite(&mapB[i], sizeof (bmBloque), 1,f);
    }
    fclose(f);
}


void EscribirInodoMap(string path,bmInodo mapI[],int inicio,int n){
    FILE* f;
    if((f=fopen(path.c_str(),"r+b"))==NULL){
        cout<<"ERROR, NO SE PUDO ABRIR EL DISCO"<<endl;
        return;
    }
    for(int i=0;i<n;i++){
        cout<<"voy por "<<(inicio+i)<<endl;
        cout<<"con "<<mapI[i].status<<endl;
        fseek(f, inicio+i*(sizeof(bmInodo)), SEEK_SET);
        fwrite(&mapI[i], sizeof (bmInodo), 1, f);
    }
    fclose(f);
}

void EscribirNodo(string path, int inicio,int n,inodo lista[]){
    FILE* archivo;
    
    if ((archivo = fopen(path.c_str(), "r+b")) == NULL) {
        return;
    } else {
        int j;
        //cout<<inicio<<endl;
        inodo i2;
        for (j = 0; j < n; j++) {
            fseek(archivo, inicio + j * (sizeof (inodo)), SEEK_SET);
            fwrite(&lista[j], sizeof (lista[j]), 1, archivo);
            fread(&i2,sizeof(i2),1,archivo);
            //cout<<"inodo "<<j<<" tipo "<<i2.i_type<<endl;
            //cout<<"inodo "<<j<<" tipo "<<to_string(lista[j].i_type)<<endl;
        }
        fclose(archivo);
    }
}

void EscribirBloqueCarpeta(string path, int inicio,bloqueCarpetas folder){
    FILE* archivo;

     if ((archivo = fopen(path.c_str(), "r+b")) == NULL) {
        return;
    } else {
        
        
        fseek(archivo, inicio , SEEK_SET);
        fwrite(&folder, sizeof (bloqueCarpetas), 1, archivo);
        
        fclose(archivo);
    }
}


void EscribirBloqueArchivo(string path, int inicio,bloqueArchivos file){
    FILE* archivo;

     if ((archivo = fopen(path.c_str(), "r+b")) == NULL) {
        return;
    } else {
            fseek(archivo, inicio , SEEK_SET);
            fwrite(&file, sizeof (bloqueArchivos), 1, archivo);
        }
        fclose(archivo);
}

void EscribirBloqueApuntador(string path, int inicio,bloqueApuntadores file){
    FILE* archivo;

     if ((archivo = fopen(path.c_str(), "r+b")) == NULL) {
        return;
    } else {
            fseek(archivo, inicio , SEEK_SET);
            fwrite(&file, sizeof (bloqueApuntadores), 1, archivo);
        }
        fclose(archivo);
}


void fechActual(times fecha) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    strcpy(fecha,buf);
}



void Comando:: LogIn(string usr,string pass,string id){
    mnt_nodo montado=retornarNodoMount(id);
    cout<<montado.mnt_ruta<<endl;
    if(montado.mnt_ruta!=""){
        int part_inicio = 0;
        char part_colocacion = ' ';
        int primerEspacioEBR = 0;
        string path;
        if (montado.mnt_particion.part_type == 'p') {
            part_inicio = montado.mnt_particion.part_start;
            part_colocacion = montado.mnt_particion.part_type;
            path=montado.mnt_ruta;
            primerEspacioEBR = 0;
        } else {
            part_inicio = montado.mnt_ebr.part_start;
            part_colocacion = metodoDeColocacionExtendida;
            path=montado.mnt_ruta;
            primerEspacioEBR = sizeof (EBR);
        }
        pathGlobal=path;
        inicioG=primerEspacioEBR+part_inicio;
        BuscarUser(path,primerEspacioEBR+part_inicio,usr,pass);
        
    }
}

void BuscarUser(string path,int pos,string usr,string pass){
    FILE* archivo;
    archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, pos, SEEK_SET);
    
    //Buscamos el super bloque para poder acceder a la posicion de los inodos y bloques
    fread(&superLog, sizeof(superBloque), 1, archivo);
    
    //posicion del 2do inodo (el inodo archivo users,txt)
    int posAct=superLog.s_inode_start+sizeof(inodo);
    inodo InodeUsers;

    fseek(archivo,posAct, SEEK_SET);
    fread(&InodeUsers, sizeof(InodeUsers), 1, archivo);



    
    string contenido=leerArchivo(InodeUsers,archivo);
    //cout<<contenido<<endl;
    fclose(archivo);
    

    string regTemp;
    string delimiter="\n";
    size_t posC;
    
    vector <string> Reg;

    //separamos el contenido dentro de userx.txt separandolo por "\n"
    while ((posC = contenido.find(delimiter)) != string::npos) {
        regTemp=(contenido.substr(0, posC));
        contenido.erase(0, posC + delimiter.length());
        //Cada string ocnseguido aplicamos mismo metodo y separamos ṕor "," para obtener cada campo
        //cout<<regTemp<<endl;
        OrdenarUsers(posC,regTemp);
        
    }
    //cout<<"sigo"<<endl;
    //regTemp=(contenido.substr(0, posC));
    //OrdenarUsers(posC,regTemp);
    //cout<<"sigo"<<endl;
    bool encontrado=false;
    for(int k=0;k<leidos.size();k++){
        for(int j=0;j<leidos[k].usuarios.size();j++){
            
            string nameReg=leidos[k].usuarios[j].name,PassReg=leidos[k].usuarios[j].pass;
            if(strcmp(nameReg.c_str(),usr.c_str())==0){
                if(strcmp(PassReg.c_str(),pass.c_str())==0){
                    logeado=leidos[k].usuarios[j];
                    cout<<"BIENVENIDO "<<logeado.name<<endl;
                    encontrado=true;
                    break;
                }
            }
        }
        if(encontrado){
            break;
        }
    }
    if(!encontrado){
        cout<<"ERROR: USUARIO NO ENCONTRADO"<<endl;
    }
    //cout<<leidos.size()<<endl;       
}

void OrdenarUsers(int posC,string regTemp){
    vector <string> Reg;
    string delimiter2=",";
    while ((posC = regTemp.find(delimiter2)) != string::npos) {
        Reg.push_back(regTemp.substr(0, posC));
        regTemp.erase(0, posC + delimiter2.length());
    }
    Reg.push_back(regTemp.substr(0, posC));
        //Valuamos que el user o grupo no haya sido eliminado
        /*for(int i=0;i<Reg.size();i++){
            cout<<Reg[i]<<endl;
        }*/
        //cout<<"sigo"<<endl;
    if(Reg[0]!="0"){
        //Verificamos si es un grupo o usr
        if(Reg[1]=="G"||Reg[1]=="g"){
            GrupoUser g;
            //cout<<"sigo"<<endl;
            g.Id=atoi(Reg[0].c_str());
            strcpy(g.name,Reg[2].c_str());
            //Añadimos a la lista
            leidos.push_back(g);
            
        }else if(Reg[1]=="U"||Reg[1]=="u"){
            UserLog u;
            u.Id=atoi(Reg[0].c_str());
            strcpy(u.Grupo,Reg[2].c_str());
            strcpy(u.name,Reg[3].c_str());
            strcpy(u.pass,Reg[4].c_str());
            GrupoUser auxG;
            int cont=0;
            //Buscamos el grupo al que pertenece
            //cout<<"sigo"<<endl;
            while(cont<leidos.size()){
                auxG=leidos[cont];
                string n1=auxG.name,n2=u.Grupo;
                if(strcmp(n1.c_str(),n2.c_str())==0){
                    //añadimos a la lista dentro de lista de grupos
                    //cout<<"entre en el if"<<endl;
                    auxG.usuarios.push_back(u);
                    //cout<<auxG.usuarios.size()<<endl;;
                    leidos[cont]=auxG;
                    break;
                }
                cont++;
            }
            //cout<<"sigo"<<endl;
        }
    }
    //cout<<"sigo"<<endl;
}

string leerArchivo(inodo InodeUsers,FILE* archivo){
    
    string contenido;
    fclose(archivo);
    archivo=fopen(pathGlobal.c_str(),"r+b");
    //conseguimos el contenido dentro de los links de bloque del inodo
    for(int i=0;i<16;i++){    

        if(InodeUsers.i_block[i]>=0&&i<13){    
            bloqueArchivos FileUsers;
            //cout<<"Dentro de For: "<<InodeUsers.i_type<<endl;
            int posAct=superLog.s_block_start+InodeUsers.i_block[i]*sizeof(bloqueArchivos);
            fseek(archivo,posAct, SEEK_SET);
            fread(&FileUsers, sizeof(bloqueArchivos), 1, archivo);
            
            contenido+=FileUsers.b_content;
            //cout<<"c "<<contenido<<endl;
        }else if(i==13&&InodeUsers.i_block[i]>=0){
            bloqueApuntadores apuntador;
            int posAct=superLog.s_block_start+InodeUsers.i_block[i]*sizeof(bloqueArchivos);
            fseek(archivo,posAct, SEEK_SET);
            fread(&apuntador, 64, 1, archivo);
            contenido+=leerIndirecto(posAct,apuntador,archivo);
        }else if(i==14&&InodeUsers.i_block[i]>=0){
            bloqueApuntadores apuntador;
            int posAct=superLog.s_block_start+InodeUsers.i_block[i]*sizeof(bloqueArchivos);
            fseek(archivo,posAct, SEEK_SET);
            fread(&apuntador, 64, 1, archivo);
            contenido+=leerDobleIndirecto(posAct,apuntador,archivo);
        }else if(i==15&&InodeUsers.i_block[i]>=0){
            bloqueApuntadores apuntador;
            int posAct=superLog.s_block_start+InodeUsers.i_block[i]*sizeof(bloqueArchivos);
            fseek(archivo,posAct, SEEK_SET);
            fread(&apuntador, 64, 1, archivo);
            contenido+=leerTripleIndirecto(posAct,apuntador,archivo);
        }
    }
    return contenido;
}

string leerIndirecto(int pos,bloqueApuntadores apuntadores,FILE* archivo){
    bloqueArchivos file;
    string contenido="";
    for(int i=0;i<16;i++){
        if(apuntadores.b_pointers[i]>=0){
            int posAct=superLog.s_block_start+apuntadores.b_pointers[i]*sizeof(bloqueArchivos);
            fseek(archivo,posAct, SEEK_SET);
            fread(&file, sizeof(bloqueApuntadores), 1, archivo);
            contenido+=file.b_content;
        }
    }
    return contenido;
}

string leerDobleIndirecto(int pos,bloqueApuntadores apuntadores,FILE* archivo){
    bloqueApuntadores apuntadoresD;
    string contenido="";
    for(int i=0;i<16;i++){
        if(apuntadores.b_pointers[i]>=0){
            int posAct=superLog.s_block_start+apuntadores.b_pointers[i]*sizeof(bloqueArchivos);
            fseek(archivo,posAct, SEEK_SET);
            fread(&apuntadoresD, sizeof(bloqueApuntadores), 1, archivo);
            contenido+=leerIndirecto(posAct,apuntadoresD,archivo);
        }
    }
    return contenido;
}

string leerTripleIndirecto(int pos,bloqueApuntadores apuntadores,FILE* archivo){
    bloqueApuntadores apuntadoresT;
    string contenido="";
    for(int i=0;i<16;i++){
        if(apuntadores.b_pointers[i]>=0){
            int posAct=superLog.s_block_start+apuntadores.b_pointers[i]*sizeof(bloqueArchivos);
            fseek(archivo,posAct, SEEK_SET);
            fread(&apuntadoresT, sizeof(bloqueApuntadores), 1, archivo);
            contenido+=leerDobleIndirecto(posAct,apuntadoresT,archivo);
        }
    }
    return contenido;
}

void Comando::mkgrp(string name){
    int posAct=superLog.s_inode_start+sizeof(inodo);
    inodo InodeUsers;   
    for(int i=0;i<leidos.size();i++){
        string nameA=leidos[i].name;
        if(nameA==name){
            cout<<"ERROR: NOMBRE DE GRUPO YA EN USO"<<endl;
            return;
        }
    }
    string nCont="";
    int nMayor=0;
    for(int i=0;i<leidos.size();i++){
        if(nMayor<=leidos[i].Id){
            nMayor=leidos[i].Id+1;
        }
    }
    nCont+=to_string(nMayor);
    nCont+=",G,";
    nCont+=name;
    nCont+="\n";
    GrupoUser nuevo;
    nuevo.Id=nMayor;
    strcpy( nuevo.name,name.c_str());
    
    leidos.push_back(nuevo);


    escribirFile(posAct,InodeUsers,nCont);
}

void escribirFile(int posAct,inodo InodeUsers,string nCont){
    FILE* archivo;
    archivo=fopen(pathGlobal.c_str(), "r+b");
    fseek(archivo,posAct, SEEK_SET);
    fread(&InodeUsers, sizeof(InodeUsers), 1, archivo);

    bloqueArchivos bloqueA;
    for(int i=0;i<16;i++){
        posAct=InodeUsers.i_block[i]*sizeof(bloqueArchivos)+superLog.s_block_start;
        if(i<13){
            if(InodeUsers.i_block[i]==-1){
                InodeUsers.i_block[i]=crearBloque(0,nCont,archivo);
                
                cout<<"GRUPO AÑADIDO CON EXITO"<<endl;
                fclose(archivo);
                EscribirInodoUnidad(superLog.s_inode_start+sizeof(inodo),InodeUsers);
                return;
            }
            
            fseek(archivo,posAct, SEEK_SET);
            fread(&bloqueA, sizeof(bloqueArchivos), 1, archivo);
            
            string cont=bloqueA.b_content;
            if(64-cont.length()>0){
                //cout<<"aqui imprime"<<endl;
                if(64-cont.length()>nCont.size()){
                    //cout<<"aqui imprime"<<endl;
                    cont+=nCont;
                    strcpy(bloqueA.b_content,cont.c_str());
                    fseek(archivo,posAct, SEEK_SET);
                    fwrite(&bloqueA,sizeof(bloqueArchivos),1,archivo);
                    //fseek(archivoLog,posAct, SEEK_SET);
                    //fread(&bloqueA, sizeof(bloqueArchivos), 1, archivoLog);
                    //cout<<bloqueA.b_content<<endl;
                    fclose(archivo);
                    cout<<"ESCRITO CON EXITO"<<endl;
                    return;
                }else{
                    int Cantidad=64-cont.length();
                    string nContPartido="",resto="";
                    for(int a=0;a<nCont.size();a++){
                        if(a<Cantidad){
                            nContPartido+=nCont.at(a);
                        }else{
                            resto+=nCont.at(a);
                        }
                    }
                    cont+=nContPartido;
                    strcpy(bloqueA.b_content,cont.c_str());
                    fseek(archivo,posAct, SEEK_SET);
                    fwrite(&bloqueA,sizeof(bloqueArchivos),1,archivo);
                    fclose(archivo);
                    cout<<bloqueA.b_content<<endl;
                    //cout<<"posactual"<<posAct<<endl;
                    //cout<<"posactualLibre"<<superLog.s_first_blo<<endl;
                    archivo=fopen(pathGlobal.c_str(), "r+b");
                    if(i<12){
                        InodeUsers.i_block[i+1]=crearBloque(0,resto,archivo);
                        
                        //cout<<InodeUsers.i_block[i+1]<<endl;
                        posAct=superLog.s_inode_start+sizeof(inodo);
                        fclose(archivo);
                        EscribirInodoUnidad(posAct,InodeUsers);

                    }else{
                        InodeUsers.i_block[i+1]=crearBloque(1,"",archivo);
                        
                        bloqueApuntadores punt;
                        posAct=superLog.s_block_start+InodeUsers.i_block[i]*sizeof(bloqueApuntadores);
                        fseek(archivo,posAct, SEEK_SET);
                        fread(&punt, sizeof(bloqueApuntadores), 1, archivo);

                        punt.b_pointers[0]=crearBloque(0,resto,archivo);
                        
                        fclose(archivo);
                        EscribirInodoUnidad(posAct,InodeUsers);

                        //fwrite(&bloqueA,sizeof(bloqueArchivos),1,archivoLog);
                    }

                    cout<<"ESCRITO CON EXITO"<<endl;
                    //fclose(archivo);
                    return;
                    
                }
            }

        }if(i==13){
            posAct=superLog.s_block_start+InodeUsers.i_block[i]*sizeof(bloqueApuntadores);
            if(InodeUsers.i_block[i]==-1){
                InodeUsers.i_block[i]=crearBloque(1,nCont,archivo);
                
                bloqueApuntadores punt;
                
                fseek(archivo,posAct, SEEK_SET);
                fread(&punt, sizeof(bloqueApuntadores), 1, archivo);

                punt.b_pointers[0]=crearBloque(0,nCont,archivo);
                
                fseek(archivo,posAct, SEEK_SET);
                fwrite(&punt,sizeof(bloqueArchivos),1,archivo);
    
                posAct=superLog.s_inode_start+sizeof(inodo);
                fclose(archivo);
                EscribirInodoUnidad(posAct,InodeUsers);

                cout<<"GRUPO AÑADIDO CON EXITO"<<endl;
                
                return;
            }
            bloqueApuntadores punt;
            
            fseek(archivo,posAct, SEEK_SET);
            fread(&punt, sizeof(bloqueApuntadores), 1, archivo);
            for(int j=0;j<16;j++){
                if(punt.b_pointers[i]==-1){
                    punt.b_pointers[i]=crearBloque(0,nCont,archivo);
                    
                    cout<<"GRUPO AÑADIDO CON EXITO"<<endl;
                    fclose(archivo);
                    EscribirBloqueApuntador(pathGlobal,superLog.s_inode_start+sizeof(inodo),punt);
                    return;
                }
                posAct=punt.b_pointers[i]*sizeof(bloqueArchivos)+superLog.s_block_start;
                fseek(archivo,posAct, SEEK_SET);
                fread(&bloqueA, sizeof(bloqueArchivos), 1, archivo);
                string cont=bloqueA.b_content;
                if(64-cont.length()>0){
                    if(64-cont.length()>nCont.size()){
                        cont+=nCont;
                        strcpy(bloqueA.b_content,cont.c_str());
                        fseek(archivo,posAct, SEEK_SET);
                        fwrite(&bloqueA,sizeof(bloqueArchivos),1,archivo);
                        cout<<"GRUPO AÑADIDO CON EXITO"<<endl;
                        fclose(archivo);
                        return;
                    }else{
                        int Cantidad=64-cont.length();
                        string nContPartido="",resto="";
                        for(int a=0;a<nCont.size();a++){
                            if(a<Cantidad){
                                nContPartido+=nCont.at(a);
                            }else{
                                resto+=nCont.at(a);
                            }
                        }
                        cont+=nContPartido;
                        strcpy(bloqueA.b_content,cont.c_str());
                        fseek(archivo,posAct, SEEK_SET);
                        fwrite(&bloqueA,sizeof(bloqueArchivos),1,archivo);
                        fclose(archivo);
                        //cout<<bloqueA.b_content<<endl;
                        //cout<<"posactual"<<posAct<<endl;
                        //cout<<"posactualLibre"<<superLog.s_first_blo<<endl;
                        archivo=fopen(pathGlobal.c_str(), "r+b");
                    
                        InodeUsers.i_block[i+1]=crearBloque(0,resto,archivo);
                        
                        //cout<<InodeUsers.i_block[i+1]<<endl;
                        posAct=superLog.s_inode_start+sizeof(inodo);
                        fclose(archivo);
                        EscribirBloqueApuntador(pathGlobal,superLog.s_inode_start+sizeof(inodo),punt);
                        
                    }
                }
            }
        }
        
    }

}

void Comando::mkusr(string usr,string grp,string pass){
    int posAct=superLog.s_inode_start+sizeof(inodo);
    inodo InodeUsers;   
    string nCont="";
    int nMayor=0;
    
    for(int i=0;i<leidos.size();i++){
        string name= leidos[i].name;
        if(name==grp){
            for(int j=0;j<leidos[i].usuarios.size();j++){
                string nameU=leidos[i].usuarios[j].name;
                if(nameU==usr){
                    cout<<"ERRROR: NOMBRE DE USUARIO YA EN USO"<<endl;
                    return;
                }
            }
        }
    }

    for(int i=0;i<leidos.size();i++){
        string name= leidos[i].name;
        if(strcmp(name.c_str(),grp.c_str())==0){
            //cout<<name<<" vs "<<grp<<endl;
            for(int j=0;j<leidos[i].usuarios.size();j++){
                cout<<j<<endl;
                if(nMayor<=leidos[i].usuarios[j].Id){
                    nMayor=leidos[i].usuarios[j].Id+1;
                }
            }
            if(nMayor==0){
                nMayor=1;
            }
            nCont+=to_string(nMayor);
            nCont+=",U,";
            nCont+=grp;
            nCont+=",";
            nCont+=usr;
            nCont+=",";
            nCont+=pass;
            nCont+="\n";
            UserLog nuevo;
            nuevo.Id=nMayor;

            strcpy( nuevo.name,usr.c_str());
            strcpy( nuevo.pass,pass.c_str());
            strcpy( nuevo.Grupo,grp.c_str());
            
            leidos[i].usuarios.push_back(nuevo);
            break;
        }
        
    }
    
    escribirFile(posAct,InodeUsers,nCont);
}

int crearBloque(int tipo,string escribir,FILE* archivo){
    
    if(tipo==0){
        bloqueArchivos bloqueA;
        strcpy(bloqueA.b_content,escribir.c_str());
        fseek(archivo,superLog.s_first_blo, SEEK_SET);
        fwrite(&bloqueA,sizeof(bloqueArchivos),1,archivo);
    }else if(tipo==1){
        bloqueApuntadores bloqueAp;
        bloqueAp.b_pointers[0]=superLog.s_blocks_count-superLog.s_free_blocks_count+1;
        fseek(archivo,superLog.s_first_blo, SEEK_SET);
        fwrite(&bloqueAp,sizeof(bloqueApuntadores),1,archivo);
    }
    superLog.s_first_blo+=64;
    int contador=superLog.s_blocks_count-superLog.s_free_blocks_count;
    superLog.s_free_blocks_count--;
    fclose(archivo);
    
    EscribirSuper(pathGlobal,superLog,inicioG);
    actualizar_bmBlock(contador);
    archivo = fopen(pathGlobal.c_str(), "r+b");
    return contador;
}

void crearInodo(){

}

void EscribirInodoUnidad(int posAct,inodo InodeUsers){
    FILE* archivo=fopen(pathGlobal.c_str(),"r+b");
    
    fseek(archivo,posAct, SEEK_SET);
    fwrite(&InodeUsers,sizeof(inodo),1,archivo);
    fclose(archivo);
}


int BuscarInodo(string name,int pos,FILE* archivo){
    
    inodo InodoActual;
    fseek(archivo,pos, SEEK_SET);
    fread(&InodoActual, sizeof(inodo), 1, archivo);
    if(InodoActual.i_type=='0'){
        for(int i=0;i<16;i++){
            if(InodoActual.i_block[i]!=-1){
                if(i<13){
                    int num=recorrerCarpeta(name,InodoActual.i_block[i]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo);
                    if(num!=-1){
                        return num;
                    }
                
                }if(i==13){
                    bloqueApuntadores apuntadores;
                    fseek(archivo,pos, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            int num=recorrerCarpeta(name,apuntadores.b_pointers[j],archivo);
                            if(num!=-1){
                                return num;
                            }
                        }
                    } 
                }
                
            }
        }
    }
    return -1;
}



int recorrerCarpeta(string name,int pos,FILE* archivo){
    bloqueCarpetas bloque;
    fseek(archivo,pos, SEEK_SET);
    fread(&bloque, sizeof(bloqueCarpetas), 1, archivo);
    for(int i=0;i<4;i++){
        string n=bloque.b_content[i].b_name;
        if(n==name){
            return bloque.b_content[i].b_inodo;
        }
    }
    for(int i=0;i<4;i++){
        int num=BuscarInodo(name,bloque.b_content[i].b_inodo,archivo);
        if(num!=-1){
            return num;
        }
    }

    return -1;
}


void actualizar_bmInodo(int n){
    
    int inodoCount=superLog.s_inodes_count;
    char bitmapI[inodoCount];
    FILE* archivo=fopen(pathGlobal.c_str(),"r+b");
    fseek(archivo, superLog.s_bm_inode_start, SEEK_SET);
    fread(&bitmapI, inodoCount, 1, archivo);

    if(bitmapI[n]=='0'){
        bitmapI[n]=='1';   
    }else{
        bitmapI[n]=='0';
    }
    fseek(archivo, superLog.s_bm_inode_start, SEEK_SET);
    fwrite(&bitmapI, 1, superLog.s_inodes_count, archivo);
    fclose(archivo);
}


void actualizar_bmBlock(int n){
    
    int inodoCount=superLog.s_blocks_count;
    char bitmapI[inodoCount];
    FILE* archivo=fopen(pathGlobal.c_str(),"r+b");
    fseek(archivo, superLog.s_bm_block_start, SEEK_SET);
    fread(&bitmapI, inodoCount, 1, archivo);

    if(bitmapI[n]=='0'){
        bitmapI[n]='1';   
    }else{
        bitmapI[n]='0';
    }

    fseek(archivo, superLog.s_bm_block_start, SEEK_SET);
    fwrite(bitmapI, 1, inodoCount, archivo);
    fclose(archivo);
}


void Comando::rep(string name,string path,string id,string ruta){
        string direct = "";

        string name_p = "";

        int start = 0;
        int end = path.find("/");
        string del = "/";
        while (end != -1)
        {
            //cout << path.substr(start, end - start) << endl;
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
        //VERIFICAMOS EXTENSION DEL DISCO
        

        char bloque[1024];
        for (int i = 0; i < 1024; i++)
        {
            bloque[i] = '\0';
        }

        // Escritura de Bloque en Archivo
        int limite = 0;

        FILE *archivo;
        
        //CREACION DE CARPETAS/DIRECTORIOS
        string crear = "sudo -S mkdir " + direct ;
        system(crear.c_str());
        // cout<<aux<<endl;
        string compermiso = "sudo chmod 777 " + direct ;
        system(compermiso.c_str());
        //CREACION DE DISCO
        if (false)
        {
            printf("ERROR: NO SE PUDO GENERAR EL ARCHIVO\n");
            return;
        }else{
            //fclose(archivo);
            superBloque sb;
            mnt_nodo particion=retornarNodoMount(id);
        if(particion.mnt_ruta!=""){
            int part_inicio = 0;
            int EBRi = 0;
            char pathD[512];
            string name;
            if (particion.mnt_particion.part_type == 'p') {
                part_inicio = particion.mnt_particion.part_start;
                strcpy(pathD,particion.mnt_ruta);
                EBRi = 0;
                
            } else {
                part_inicio = particion.mnt_ebr.part_start;
                strcpy(pathD,particion.mnt_ruta);
                EBRi = sizeof (EBR);
            }
            particion.mnt_particion.part_name;
            int inicio=EBRi+part_inicio;
            archivo=fopen(particion.mnt_ruta,"r+b");
            
            if(strcmp(name.c_str(),"mbr")){
                MBR mbrRep;
                fseek(archivo, 0, SEEK_SET);
                fread(&mbrRep, sizeof (MBR), 1, archivo);
                fclose(archivo);

                string dot;
                dot+="digraph D{";
                dot+="label=\"REPORTE MBR\"";
                dot+="nodo_mbr[shape=platntext label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n";
                dot+="<TR><TD align=\"left\" color=\"white\" bgcolor=\"darkslateblue\">REPORTE DE MBR</TD><TD color=\"white\" bgcolor=\"darkslateblue\"></TD></TR>\n";
                dot+="<TR><TD>mbr_tamano</TD><TD>";
                dot+=to_string(mbrRep.mbr_tamano);
                dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">mbr_fecha_creacion</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                dot+=to_string(mbrRep.mbr_fecha_creacion);
                dot+="</TD></TR>\n<TR><TD>mbr_disk_signature</TD><TD>";
                dot+=to_string(mbrRep.mbr_dsk_signature);
                dot+="</TD></TR>\n";
                
                partitiond particiones[4];
                particiones[0] = mbrRep.mbr_partition_1;
                particiones[1] = mbrRep.mbr_partition_2;
                particiones[2] = mbrRep.mbr_partition_3;
                particiones[3] = mbrRep.mbr_partition_4;

                for (int i = 0; i < 4; i++) {
                    if(particiones[i].part_s!=0){
                        
                        dot+="<TR><TD align=\"left\" color=\"white\" bgcolor=\"darkslateblue\">PARTICION</TD><TD color=\"white\" bgcolor=\"darkslateblue\"></TD>";
                        dot+="</TR>\n<TR><TD>part_status</TD><TD>";
                        dot+=particiones[i].part_status;
                        dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">part_type</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                        dot+=particiones[i].part_type;
                        dot+="</TD></TR>\n<TR><TD>part_fit </TD><TD>";
                        dot+=particiones[i].part_fit;
                        dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">part_start</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                        dot+=to_string(particiones[i].part_start);
                        dot+="</TD></TR>\n<TR><TD>part_size </TD><TD>";
                        dot+=to_string(particiones[i].part_s);
                        dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">part_name</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                        dot+=particiones[i].part_name;
                        dot+="</TD></TR>\n";
                        if(particiones[i].part_type=='e'){
                            int inicioE = particiones[i].part_start;

                            FILE *f;
                            if ((f = fopen(particion.mnt_ruta, "r+b")) == NULL) {
                                
                                    cout<<"Error: no se ha podido al abrir el disco!\n";
                                
                            }else{
                                EBR ebrRep;
                                MBR m;
                                cout<<"Inicio: "<<inicioE<<endl;

                                fseek(f, inicioE, SEEK_SET);
                                fread(&ebrRep, sizeof (EBR), 1, f);
                                
                                cout<<"si"<<endl;
                                cout<<"fit: "<<ebrRep.part_fit<<endl;
                                string B="b",F="f",W="w",fit=to_string(ebrRep.part_fit);
                                if (ebrRep.part_fit=='b' || strcmp( fit.c_str(), F.c_str())==0 || strcmp( fit.c_str(), W.c_str())==0) {
                                    cout<<"si?"<<endl;
                                    int siguiente=ebrRep.part_next;
                                    while(siguiente!=-1){
                                        dot+="<TR><TD align=\"left\" color=\"white\" bgcolor=\"springgreen1\">PARTICION LOGICA</TD><TD color=\"white\" bgcolor=\"springgreen1\"></TD>";
                                        dot+="</TR>\n<TR><TD>part_status</TD><TD>";
                                        dot+=to_string(ebrRep.part_status);
                                        dot+="</TD></TR>\n<TR><TD color=\"white\" bgcolor=\"springgreen1\">part_next</TD><TD color=\"white\" bgcolor=\"springgreen1\">";
                                        dot+=to_string(ebrRep.part_next);
                                        dot+="</TD></TR>\n<TR><TD>part_fit</TD><TD>";
                                        dot+=ebrRep.part_fit;
                                        dot+="</TD></TR>\n<TR><TD color=\"white\" bgcolor=\"springgreen1\">part_start</TD><TD color=\"white\" bgcolor=\"springgreen1\">";
                                        dot+=to_string(ebrRep.part_start);
                                        dot+="</TD></TR>\n<TR><TD>part_size</TD><TD>";
                                        dot+=to_string(ebrRep.part_s);
                                        dot+="</TD></TR>\n<TR><TD color=\"white\" bgcolor=\"springgreen1\">part_name</TD><TD color=\"white\" bgcolor=\"springgreen1\">";
                                        dot+=ebrRep.part_name;
                                        dot+="</TD></TR>";
                                        fseek(f, siguiente, SEEK_SET);
                                        fread(&ebrRep, sizeof (EBR), 1, f); 
                                        siguiente=ebrRep.part_next;
                                    }
                                    dot+="<TR><TD align=\"left\" color=\"white\" bgcolor=\"springgreen1\">PARTICION LOGICA</TD><TD color=\"white\" bgcolor=\"springgreen1\"></TD>";
                                    dot+="</TR>\n<TR><TD>part_status</TD><TD>";
                                    dot+=to_string(ebrRep.part_status);
                                    dot+="</TD></TR>\n<TR><TD color=\"white\" bgcolor=\"springgreen1\">part_next</TD><TD color=\"white\" bgcolor=\"springgreen1\">";
                                    dot+=to_string(ebrRep.part_next);
                                    dot+="</TD></TR>\n<TR><TD>part_fit</TD><TD>";
                                    dot+=ebrRep.part_fit;
                                    dot+="</TD></TR>\n<TR><TD color=\"white\" bgcolor=\"springgreen1\">part_start</TD><TD color=\"white\" bgcolor=\"springgreen1\">";
                                    dot+=to_string(ebrRep.part_start);
                                    dot+="</TD></TR>\n<TR><TD>part_size</TD><TD>";
                                    dot+=to_string(ebrRep.part_s);
                                    dot+="</TD></TR>\n<TR><TD color=\"white\" bgcolor=\"springgreen1\">part_name</TD><TD color=\"white\" bgcolor=\"springgreen1\">";
                                    dot+=ebrRep.part_name;
                                    dot+="</TD></TR>";
                                }
                                fclose(f);
                            }                                
                        }
                    }
                }
                dot+="</TABLE>>];";
                dot+="}";
                cout<<dot<<endl;

            }else if(name=="disk"){

            }else if(name=="inode"){
                superBloque super;
                
                fseek(archivo, sizeof(MBR), SEEK_SET);
                fread(&super, sizeof (superBloque), 1, archivo);
                
                int nInodos=super.s_free_inodes_count;
                inodo tablaInodo[nInodos];

                fseek(archivo, super.s_inode_start, SEEK_SET);
                fread(&tablaInodo, nInodos*sizeof(inodo), 1, archivo);

                fclose(archivo);

                string dot="";
                dot+="digraph D{\n";
                dot+="label=\"REPORTE INODE\"\n node[shape=platntext]\n";
                int anterior=-1;
                for(int i=0;i<nInodos;i++){
                    if(tablaInodo[i].i_gid!=0){
                        if(anterior!=-1)
                            dot+="Inodo"+to_string(anterior)+"->Inodo"+to_string(i);
                        dot+="Inodo"+to_string(i)+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                        dot+="<TR><TD>INODO</TD><TD>"+to_string(i)+"</TD></TR>\n";
                        dot+="<TR><TD>i_uid</TD><TD>"+to_string(tablaInodo[i].i_uid)+"</TD></TR>\n";
                        dot+="<TR><TD>i_gid</TD><TD>"+to_string(tablaInodo[i].i_gid)+"</TD></TR>\n";
                        dot+="<TR><TD>i_size</TD><TD>"+to_string(tablaInodo[i].i_size)+"</TD></TR>\n";
                        char fecha[80];
                        strcpy(fecha,tablaInodo[i].i_atime);
                        string fechaA=fecha;
                        dot+="<TR><TD>i_atime</TD><TD>"+fechaA+"</TD></TR>\n";
                        for(int j=0;j<16;j++){
                            dot+="<TR><TD>i_block_"+to_string(j)+"</TD><TD>"+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                        }
                        dot+="<TR><TD>i_perm</TD><TD>"+to_string(tablaInodo[i].i_perm)+"</TD></TR>\n";
                        dot+="</TABLE>>];";
                        anterior=i;
                    }
                }
                dot+="}";

            }else if(name=="journaling"){
                
            }else if(name=="block"){
                superBloque super;
                
                fseek(archivo, sizeof(MBR), SEEK_SET);
                fread(&super, sizeof (superBloque), 1, archivo);
                
                int nInodos=super.s_free_inodes_count;
                inodo tablaInodo[nInodos];

                fseek(archivo, super.s_inode_start, SEEK_SET);
                fread(&tablaInodo, nInodos*sizeof(inodo), 1, archivo);

                

                string dot="";
                dot+="digraph D{\n";
                dot+="label=\"REPORTE INODE\"\n node[shape=platntext]\n";
                int anterior=-1;
                bloqueApuntadores apuntadoresB,apuntadoresB1,apuntadoresB2;
                bloqueArchivos archivoB;
                bloqueCarpetas carpetaB;

                for(int i=0;i<nInodos;i++){
                    if(tablaInodo[i].i_gid!=0){

                        if(anterior!=-1){

                            for(int j=0;j<16;j++){

                                if(tablaInodo[i].i_block[j]!=-1){
                                    if(j==13){
                                        fseek(archivo, super.s_block_start+tablaInodo[i].i_block[j]*64, SEEK_SET);
                                        fread(&apuntadoresB, 64, 1, archivo);
                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(tablaInodo[i].i_block[j]);
                                        dot+="Bloque"+to_string(tablaInodo[i].i_block[j])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                        dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                                        string contenido="";
                                        for(int x=0;x<15;x++){
                                            contenido+=to_string(apuntadoresB.b_pointers[x])+", ";
                                        }
                                        contenido+=to_string(apuntadoresB.b_pointers[15]);
                                        dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                        dot+="</TABLE>>];";
                                        anterior=tablaInodo[i].i_block[j];
                                        for(int a=0;a<16;a++){
                                            if(apuntadoresB.b_pointers[a]=!-1){
                                                if(tablaInodo[i].i_type==0){
                                                    fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                    fread(&carpetaB, 64, 1, archivo);
                                                    

                        
                                                    dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a]);
                                                    dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                    dot+="<TR><TD>BLOQUE CARPETA</TD><TD>"+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                    dot+="<TR><TD>b_name</TD><TD>b_inodo</TD></TR>\n";
                                                    for(int x=0;x<4;x++){
                                                        if(carpetaB.b_content[x].b_inodo!=-1){
                                                            string name_B_C=carpetaB.b_content[x].b_name;
                                                            dot+="<TR><TD>"+name_B_C+"</TD><TD>"+to_string(carpetaB.b_content[x].b_inodo)+"</TD></TR>\n";
                                                        }
                                                    }
                                                    dot+="</TABLE>>];";
                                                    
                                                    anterior=apuntadoresB.b_pointers[a];
                                                    continue;
                                                }
                                                fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                fread(&archivoB, 64, 1, archivo);
                                                dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a]);
                                                dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                dot+="<TR><TD>BLOQUE ARCHIVO "+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                string contenido=archivoB.b_content;
                                                dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                dot+="</TABLE>>];";
                                                anterior=apuntadoresB.b_pointers[a];
                                            }

                                        }

                                        continue;
                                    }else if(j==14){
                                        fseek(archivo, super.s_block_start+tablaInodo[i].i_block[j]*64, SEEK_SET);
                                        fread(&apuntadoresB1, 64, 1, archivo);
                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(tablaInodo[i].i_block[j]);
                                        dot+="Bloque"+to_string(tablaInodo[i].i_block[j])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                        dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                                        string contenido="";
                                        for(int x=0;x<15;x++){
                                            contenido+=to_string(apuntadoresB1.b_pointers[x])+", ";
                                        }
                                        contenido+=to_string(apuntadoresB1.b_pointers[15]);
                                        dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                        dot+="</TABLE>>];";
                                        anterior=tablaInodo[i].i_block[j];                                                

                                        
                                        for(int b=0;b<16;b++){
                                            if(apuntadoresB1.b_pointers[b]=!-1){
                                                fseek(archivo, super.s_block_start+apuntadoresB1.b_pointers[b]*64, SEEK_SET);
                                                fread(&apuntadoresB, 64, 1, archivo);

                                                dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB1.b_pointers[b]);
                                                dot+="Bloque"+to_string(apuntadoresB1.b_pointers[b])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(apuntadoresB1.b_pointers[b])+"</TD></TR>\n";
                                                string contenido="";
                                                for(int x=0;x<15;x++){
                                                    contenido+=to_string(apuntadoresB.b_pointers[x])+", ";
                                                }
                                                contenido+=to_string(apuntadoresB.b_pointers[15]);
                                                dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                dot+="</TABLE>>];";
                                                anterior=apuntadoresB1.b_pointers[b];

                                                for(int a=0;a<16;a++){
                                                    if(apuntadoresB.b_pointers[a]=!-1){
                                                        if(tablaInodo[i].i_type==0){
                                                            fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                            fread(&carpetaB, 64, 1, archivo);
                                                            
                                                            dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a]);
                                                            dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                            dot+="<TR><TD>BLOQUE CARPETA</TD><TD>"+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                            dot+="<TR><TD>b_name</TD><TD>b_inodo</TD></TR>\n";
                                                            for(int x=0;x<4;x++){
                                                                if(carpetaB.b_content[x].b_inodo!=-1){
                                                                    string name_B_C=carpetaB.b_content[x].b_name;
                                                                    dot+="<TR><TD>"+name_B_C+"</TD><TD>"+to_string(carpetaB.b_content[x].b_inodo)+"</TD></TR>\n";
                                                                }
                                                            }
                                                            dot+="</TABLE>>];";
                                                            
                                                            anterior=apuntadoresB.b_pointers[a];
                                                            continue;
                                                        }
                                                        fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                        fread(&archivoB, 64, 1, archivo);
                                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a]);
                                                        dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                        dot+="<TR><TD>BLOQUE ARCHIVO "+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                        string contenido=archivoB.b_content;
                                                        dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                        dot+="</TABLE>>];";
                                                        anterior=apuntadoresB.b_pointers[a];
                                                    }

                                                }
                                                
                                            }
                                        }

                                        continue;
                                    }else if(j==15){
                                        fseek(archivo, super.s_block_start+tablaInodo[i].i_block[j]*64, SEEK_SET);
                                        fread(&apuntadoresB2, 64, 1, archivo);
                                        
                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(tablaInodo[i].i_block[j]);
                                        dot+="Bloque"+to_string(tablaInodo[i].i_block[j])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                        dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                                        string contenido="";
                                        for(int x=0;x<15;x++){
                                            contenido+=to_string(apuntadoresB2.b_pointers[x])+", ";
                                        }
                                        contenido+=to_string(apuntadoresB2.b_pointers[15]);
                                        dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                        dot+="</TABLE>>];";
                                        anterior=tablaInodo[i].i_block[j];                                                

                                        for(int c=0;c<16;c++){
                                            if(apuntadoresB2.b_pointers[c]=!-1){
                                                fseek(archivo, super.s_block_start+apuntadoresB2.b_pointers[c]*64, SEEK_SET);
                                                fread(&apuntadoresB1, 64, 1, archivo);
                                                
                                                dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB2.b_pointers[c]);
                                                dot+="Bloque"+to_string(apuntadoresB2.b_pointers[c])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(apuntadoresB2.b_pointers[c])+"</TD></TR>\n";
                                                string contenido="";
                                                for(int x=0;x<15;x++){
                                                    contenido+=to_string(apuntadoresB1.b_pointers[x])+", ";
                                                }
                                                contenido+=to_string(apuntadoresB1.b_pointers[15]);
                                                dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                dot+="</TABLE>>];";
                                                anterior=apuntadoresB2.b_pointers[c];



                                                for(int b=0;b<16;b++){
                                                    if(apuntadoresB1.b_pointers[b]=!-1){
                                                        fseek(archivo, super.s_block_start+apuntadoresB1.b_pointers[b]*64, SEEK_SET);
                                                        fread(&apuntadoresB, 64, 1, archivo);
                                                        
                                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB1.b_pointers[c]);
                                                        dot+="Bloque"+to_string(apuntadoresB1.b_pointers[b])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                        dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(apuntadoresB1.b_pointers[c])+"</TD></TR>\n";
                                                        string contenido="";
                                                        for(int x=0;x<15;x++){
                                                            contenido+=to_string(apuntadoresB.b_pointers[x])+", ";
                                                        }
                                                        contenido+=to_string(apuntadoresB.b_pointers[15]);
                                                        dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                        dot+="</TABLE>>];";
                                                        anterior=apuntadoresB1.b_pointers[c];
                                                        


                                                        for(int a=0;a<16;a++){
                                                            if(apuntadoresB.b_pointers[a]=!-1){
                                                                if(tablaInodo[i].i_type==0){
                                                                    fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                                    fread(&carpetaB, 64, 1, archivo);
                                                                    
                                                                    dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a]);
                                                                    dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                                    dot+="<TR><TD>BLOQUE CARPETA</TD><TD>"+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                                    dot+="<TR><TD>b_name</TD><TD>b_inodo</TD></TR>\n";
                                                                    for(int x=0;x<4;x++){
                                                                        if(carpetaB.b_content[x].b_inodo!=-1){
                                                                            string name_B_C=carpetaB.b_content[x].b_name;
                                                                            dot+="<TR><TD>"+name_B_C+"</TD><TD>"+to_string(carpetaB.b_content[x].b_inodo)+"</TD></TR>\n";
                                                                        }
                                                                    }
                                                                    dot+="</TABLE>>];";
                                                                    
                                                                    anterior=apuntadoresB.b_pointers[a];
                                                                    continue;
                                                                }
                                                                fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                                fread(&archivoB, 64, 1, archivo);
                                                                dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a]);
                                                                dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                                dot+="<TR><TD>BLOQUE ARCHIVO "+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                                string contenido=archivoB.b_content;
                                                                dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                                dot+="</TABLE>>];";
                                                                anterior=apuntadoresB.b_pointers[a];
                                                            }

                                                        }
                                                        continue;
                                                    }
                                                }
                                            }
                                        }
                                        continue;
                                    }
                                    if(tablaInodo[i].i_type==0){
                                        fseek(archivo, super.s_block_start+tablaInodo[i].i_block[j]*64, SEEK_SET);
                                        fread(&carpetaB, 64, 1, archivo);
                                        
                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(tablaInodo[i].i_block[j]);
                                        dot+="Bloque"+to_string(tablaInodo[i].i_block[j])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                        dot+="<TR><TD>BLOQUE CARPETA</TD><TD>"+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                                        dot+="<TR><TD>b_name</TD><TD>b_inodo</TD></TR>\n";
                                        for(int x=0;x<4;x++){
                                            if(carpetaB.b_content[x].b_inodo!=-1){
                                                string name_B_C=carpetaB.b_content[x].b_name;
                                                dot+="<TR><TD>"+name_B_C+"</TD><TD>"+to_string(carpetaB.b_content[x].b_inodo)+"</TD></TR>\n";
                                            }
                                        }
                                        dot+="</TABLE>>];";
                                        
                                        anterior=tablaInodo[i].i_block[j];
                                        continue;
                                    }
                                    fseek(archivo, super.s_block_start+tablaInodo[i].i_block[j]*64, SEEK_SET);
                                    fread(&archivoB, 64, 1, archivo);
                                    dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(tablaInodo[i].i_block[j]);
                                    dot+="Bloque"+to_string(tablaInodo[i].i_block[j])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                    dot+="<TR><TD>BLOQUE ARCHIVO "+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                                    string contenido=archivoB.b_content;
                                    dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                    dot+="</TABLE>>];";
                                    anterior=tablaInodo[i].i_block[j];
                                }
                                
                            }
                            
                        }
                        
                    }
                }
                dot+="}";
                fclose(archivo);
            }else if(strcmp(name.c_str(),"bm_inode")==0){
                
                fseek(archivo, inicio, SEEK_SET);
                fread(&sb, sizeof (superBloque), 1, archivo);
                int inodoCount=sb.s_inodes_count;
                char bitmap[inodoCount];
                fseek(archivo, sb.s_bm_inode_start, SEEK_SET);
                fread(&bitmap, inodoCount, 1, archivo);
                fclose(archivo);
                string contenido;
                for(int i=0;i<inodoCount;i++){
                    if(i%20==0){
                        contenido+="\n";    
                    }
                    contenido+=bitmap[i];
                    contenido+="   ";
                }
                //cout<<"INICIO: "<<contenido<<endl;
                archivo = fopen(path.c_str(), "w+");
                if (archivo){
                    fwrite(contenido.c_str(), 1, contenido.size(), archivo);
                    cout << "Done Writing!" << endl;
                }

                fclose(archivo);
                
            }else if(name=="bm_block"){
                fseek(archivo, inicio, SEEK_SET);
                fread(&sb, sizeof (superBloque), 1, archivo);
                int inodoCount=sb.s_blocks_count;
                char bitmapB[inodoCount];
                fseek(archivo, sb.s_bm_block_start, SEEK_SET);
                fread(&bitmapB, inodoCount, 1, archivo);
                fclose(archivo);

                string contenido;
                //cout<<"INICIO: "<<bitmapB[inodoCount]<<endl;
                //cout<<"posicion: "<<sb.s_bm_block_start<<endl;
                for(int i=0;i<inodoCount;i++){
                    
                    if(i%20==0){
                        contenido+="\n";    
                    }
                    contenido+=bitmapB[i];
                    contenido+="   ";
                }
                cout<<"cantidad "<<inodoCount<<endl;
                cout<<contenido<<endl;

                archivo = fopen(path.c_str(), "w+");
                if (archivo){
                    fwrite(contenido.c_str(), 1, contenido.size(), archivo);
                    cout << "Done Writing!" << endl;
                }
                fclose(archivo);
            }else if(name=="tree"){

            }else if(name=="sb"){
                fseek(archivo, inicio, SEEK_SET);
                fread(&sb, sizeof (superBloque), 1, archivo);
                fclose(archivo);

                string dot;
                dot+="digraph D{";
                dot+="label=\"REPORTE SUPER BLOQUE";
                dot+=name;
                dot+="\";";
                dot+="nodo_super_bloque[shape=record label=\"{REPORTE SUPER BLOQUE|";
                dot+="{|";
                dot+="}";
            }else if(name=="file"){

            }else if(name=="ls"){

            }else{
                fclose(archivo);
                cout<<"ERROR: NOMBRE DE REPORTE INVALIDO"<<endl;
                return;
            }
        }else{
            cout<<"ERROR: SIN REGISTROS DE ID INGRESADO"<<endl;
        }
    }
}



