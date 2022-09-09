
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
int contadomagico;
int grupoLogeado;
vector <GrupoUser> leidos;
superBloque superLog;
string pathGlobal="";
int inicioG=0;
string dotTree;

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
        //cout<<p.name<<endl;
        if(logeado.Id>0){
        rep(p.name,p.path,p.id,p.ruta);
        inodo InodePrueba;
            FILE* archivo=fopen(pathGlobal.c_str(),"r+b");
            for(int i=0;i<superLog.s_inodes_count;i++){
                fseek(archivo,superLog.s_inode_start+i*sizeof(inodo), SEEK_SET);
                fread(&InodePrueba, sizeof(inodo), 1, archivo);
                //if(InodePrueba.i_gid!=0){
                    //cout<<"numero: "<<i<<"tipo: "<<InodePrueba.i_type<<endl;
                //}
            } 
            fclose(archivo);
        }
    }else if (p.Comando=="mkfile"){
        if(logeado.Id>0){
            mkfile(p.path,p.r,p.s,p.cont);        
            /*inodo InodePrueba;
            FILE* archivo=fopen(pathGlobal.c_str(),"r+b");
            for(int i=0;i<superLog.s_inodes_count;i++){
                fseek(archivo,superLog.s_inode_start+i*sizeof(inodo), SEEK_SET);
                fread(&InodePrueba, sizeof(inodo), 1, archivo);
                if(InodePrueba.i_gid!=0){
                    cout<<"numero: "<<i<<"tipo: "<<InodePrueba.i_type<<endl;
                }
            } 
            fclose(archivo);*/
        }else{
            cout<<"ERROR:  NO HAY NINGUN USUARIO LOGEADO!"<<endl;
        }
    }else if (p.Comando=="chmod"){
        if(logeado.Id>0){
            if(strcmp(logeado.name,"root")==0){
                chmod(p.path,p.ugo,p.r);
            }else{
                cout<<"ERROR: RMGRP SOLO PUEDE SER EJECUTADO POR ROOT!"<<endl;
            }
        }else{
            cout<<"ERROR:  NO HAY NINGUN USUARIO LOGEADO!"<<endl;
        }
    }else if (p.Comando=="remove"){
        if(logeado.Id>0){
            remove(p.path);
        }else{
            cout<<"ERROR:  NO HAY NINGUN USUARIO LOGEADO!"<<endl;
        }
    }else if (p.Comando=="puase"){
        
    }else if (p.Comando=="mkdir"){
        if(logeado.Id>0){
            mkdir(p.path,p.p);
        }else{
            cout<<"ERROR:  NO HAY NINGUN USUARIO LOGEADO!"<<endl;
        }
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
                        cout<<"ERROR: YA EXISTE UNA PARTICION EXTENDIDA. "<<endl;
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


            string nombre="";
            for (int i = 0; i < 4; i++) {
                nombre=particiones[i].part_name;
                if(nombre==name){
                    cout<<"Error: El nombre "<<name<<" ya está ocupado. "<<endl;
                    return;
                }
            }

            
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
                
                inicio = particiones[i].part_start;

                tamanoA = particiones[i].part_s;
                tamanoDiponible = inicio + tamanoA; 


                FILE *f;
                if ((f = fopen(path.c_str(), "r+b")) == NULL) {
                    
                        cout<<"Error: no se ha podido al abrir el disco!\n";
                    
                }else{
                    EBR B_ebr;
                    
                    fseek(f, inicio, SEEK_SET);
                    fread(&B_ebr, sizeof (EBR), 1, f);
                    //cout<<"FIT encontrado: "<<B_ebr.part_fit<<endl;
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
                                if(strcmp(aux.part_name,name.c_str())==0){
                                    cout<<"Error: El nombre "<<name<<" ya está ocupado. "<<endl;
                                    return;
                                }
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
                                    cout<<"ERROR: NO HAY SUFICIENTE ESPACIO PARA CREAR LA PARTICION!\n";
                                    fclose(f);
                                    return;

                                } else {
                                    fwrite(&aux, sizeof (EBR), 1, f);
                                    fseek(f, anterior.part_start, SEEK_SET);
                                    fwrite(&anterior, sizeof (EBR), 1, f);
                                    cout<<"PARTICION LOGICA AÑADIDA CORRECTAMENTE"<<endl;
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
                            //cout<<"aqui va el ebr: "<<inicio<<endl;
                            fseek(f, inicio, SEEK_SET);
                            fwrite(&B_ebr, sizeof (EBR), 1, f);
                            cout<<"PRIMER EBR CREADO"<<endl;
                        }else
                        {
                            cout<<"TAMAÑO INSUFICIENTE PARA PARTICION LOGICA"<<endl;
                        }
                        
                    }
                    fclose(f);
                }
            }else{
                cout<<"ERROR: NO EXISTE UNA PARTICION EXTENDIDA PARA ALOJAR LA PARTICION LOGICA"<<endl;
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
            cout << "ERROR: PARAMETRO UNIT PERMITE: B, K y M" << endl;
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
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[0].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"NUEVO: "<<particiones[0].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==1){//1 0 0 1
                tamanoDisponible = particiones[3].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[0].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"NUEVO: "<<particiones[0].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==0){//1 0 1 0
                tamanoDisponible = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[0].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"NUEVO: "<<particiones[0].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==1){//1 0 1 1
                tamanoDisponible = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[0].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"NUEVO: "<<particiones[0].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0){//1 1 0 0
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[0].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"NUEVO: "<<particiones[0].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==1){//1 1 0 1
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[0].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"NUEVO: "<<particiones[0].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0){//1 1 1 0
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                //cout<<add<<"El tamaño disponible es: "<<tamanoDisponible<<endl;
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[0].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"NUEVO: "<<particiones[0].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1){//1 1 1 1
                tamanoDisponible = particiones[1].part_start - (particiones[0].part_start+particiones[0].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[0].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[0].part_s + tam;
                    cout<<"NUEVO: "<<particiones[0].part_s<<"BYTES"<<endl;

                }
            }
        } else if(i==1){ //Casos en los que se puede añadir a la partición 2
            //cout<<"Añadiendo a la partición 2 del disco"<<endl;
            if(prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0){//X 1 0 0
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[1].part_start+particiones[1].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[1].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[1].part_s + tam;
                    cout<<"NUEVO: "<<particiones[1].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==1){//X 1 0 1
                tamanoDisponible = particiones[3].part_start - (particiones[1].part_start+particiones[1].part_s+1);
                if(tam>tamanoDisponible){
                     cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[1].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[1].part_s + tam;
                    cout<<"NUEVO: "<<particiones[1].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0){//X 1 1 0
                tamanoDisponible = particiones[2].part_start - (particiones[1].part_start+particiones[1].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[1].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[1].part_s + tam;
                    cout<<"NUEVO: "<<particiones[1].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1){//X 1 1 1
                tamanoDisponible = particiones[2].part_start - (particiones[1].part_start+particiones[1].part_s+1);
                if(tam>tamanoDisponible){
                     cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[1].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[1].part_s + tam;
                    cout<<"NUEVO: "<<particiones[1].part_s<<"BYTES"<<endl;

                }
            }
        } else if(i==2){ //Casos en los que se puede añadir a la partición 3
            //cout<<"Añadiendo a la partición 3 del disco"<<endl;
            if(prtActiva[2]==1 && prtActiva[3]==0){//X X 1 0
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[2].part_start+particiones[2].part_s+1);
                if(tam>tamanoDisponible){
                     cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[2].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[2].part_s + tam;
                    cout<<"NUEVO: "<<particiones[2].part_s<<"BYTES"<<endl;

                }
            }else if(prtActiva[2]==1 && prtActiva[3]==1){//X X 1 1
                tamanoDisponible = particiones[3].part_start - (particiones[2].part_start+particiones[2].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[2].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[2].part_s + tam;
                    cout<<"NUEVO: "<<particiones[2].part_s<<"BYTES"<<endl;

                }
            }
        } else if(i==3){ //Casos en los que se puede añadir a la partición 4
            cout<<"Añadiendo a la partición 4 del disco"<<endl;
            if(prtActiva[3]==1){//X X 1 1
                tamanoDisponible = B_mbr.mbr_tamano - (particiones[3].part_start+particiones[3].part_s+1);
                if(tam>tamanoDisponible){
                    cout<<"ERROR: INSUFICIENTE PARA AGREGAR "<<add<<" A LA PARTICION "<<(i+1)<<endl;
                    return;
                } else{
                    cout<<"TAMAÑO ANTERIOR: "<<particiones[3].part_s<<"BYTES"<<endl;
                    particiones[0].part_s = particiones[3].part_s + tam;
                    cout<<"NUEVO: "<<particiones[3].part_s<<"BYTES"<<endl;
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
        cout << "ERROR: NO EXISTE LA PARTICION: "<<name<<endl;
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
            cout << "ELIMINANDO PARTICION: " << i + 1 << ", CON NOMBRE: " << particiones[i].part_name << endl;

            
                cout << "TIPO =FULL, SIZE " << particiones[i].part_s << endl;

                FILE *f;
                if ((f = fopen(path.c_str(), "r+b")) == NULL) {
                    
                        cout << "ERROR: NO SE ABRE EL DISCO!\n";
                    
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
            cout<<"ERROR: PRIMERO DEBE DESMONTAR LA PARTICION"<<endl;
        }

    }else if (encLog == true) {
        cout << "ELIMINANDO PARTICION LOGICA, CON NOMBRE: " << logic.part_name << endl;

        if(logic.part_status=='0') {

                cout << "TIPO =FULL, SIZE " << logic.part_s << endl;

                FILE *f;
                if ((f = fopen(path.c_str(), "r+b")) == NULL) {
                    
                        cout << "ERROR: NO ABRE DISCO!\n";
                    
                } else {
                    fseek(f, logic.part_start, SEEK_SET);
                    
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
            cout<<"ERROR: PRIMERO DEBE DESMONTAR LA PARTICION"<<endl;
        }

    }else{
        cout << "ERPOR: NO SE HA ENCONTRADO LA PARTICION CON NOMBRE : "<<name<<endl;
    }
    
}


prtLogica buscarLogica(string path, string name, partitiond particiones[4], int i) {
    int inicio = particiones[i].part_start;
    int tamano = particiones[i].part_s;
    int tamanoDiponible = inicio + tamano;

    

    FILE *f;
    if ((f = fopen(path.c_str(), "r+b")) == NULL) {
        
            cout << "ERROR: NO ABRE DICO!\n";
        
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




char numeroDeDisco(mnt_lista*lista, string name) {//ESTE METODO NOS DARA EL COMIENZO DEL ID PARA EL DISCO
    mnt_nodo*puntero = lista->cabeza;
    //INICIALMENTE VALUARA SI ES LA PRIMER PARTICION MONTADA
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
        cout << "ACTUALIZANDO PARTICION " << i + 1 << ", NOMBRE: " << particiones[i].part_name << endl;

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
        cout << "ACTUALIZANDO PART LOGICA, CON NOMBRE: " << logic.part_name << endl;
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
        cout << "ERROR: NO SE ENUCNETRA: "<<name<<endl;
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
//ESTE COMANDO ME MATO XD
void Comando::mkfs(string type,string id,string fs){
    //RETORNAMOS LA PARTICION MONTADA
    mnt_nodo particion=retornarNodoMount(id);
    if(particion.mnt_ruta!=""){
        int part_inicio = 0;
        int part_tamano = 0;
        char part_colocacion = ' ';
        int pimerEspacioEBR = 0;

        char path[512];
        char nombre[16];
        char status;
        //REGRESAMOS LA INFO IMPORTANTE
        //SI ES EBR HAY QUE SUMAR SU TAMAÑO PARA SABER EL INICIO DE PARTICION
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
        //DAMOS VALOR POR DEFECTO
        if(fs==" "){
            fs="2fs";
        }
        if(fs=="2fs"){
            if(type=="full"){
                double tamPart = (double) part_tamano;
                double nu;
                
                nu = (tamPart - sizeof(superBloque)) / (4.0 + sizeof(inodo) + 3*64);
                
                
                int n = (int) nu;
                cout<<"part: "<<tamPart<<endl;
                cout<<"super: "<<sizeof(superBloque)<<endl;
                cout<<"inodo: "<<sizeof(inodo)<<endl;
                cout<<n<<endl;
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
//AQUI EMPIEZA LO CHIDO
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
    //LLENAMOS UNA LISRA CON INODOS Y OS LLENAMOS POR DEFECTO
    for (int t = 0; t < n; t++) {
        lista[t].i_uid=0;
        lista[t].i_gid=0;
        strcpy(lista[t].i_atime,"");
        strcpy(lista[t].i_ctime,"");
        strcpy(lista[t].i_mtime,"");
        lista[t].i_type='2';
        for(int i=0;i<16;i++){
            lista[t].i_block[i]=-1;
        }

        
    }
    //EDITAMOS LOS PRIMEROS QUE SIEMPRE SERAN IGUAL POR DEFECTO
    lista[0].i_uid=1;
    lista[0].i_gid=1;
    lista[1].i_uid=1;
    lista[1].i_gid=1;
    lista[0].i_perm=664;
    lista[1].i_perm=700;
    /*for(int i=0;i<16;i++){
        cout<<lista[0].i_block[i]<<endl;;
    }*/
    strcpy(lista[0].i_ctime,fech);
    strcpy(lista[0].i_mtime,fech);
    lista[0].i_type='0';
    lista[0].i_block[0]=0;

    strcpy(lista[1].i_ctime,fech);
    strcpy(lista[1].i_mtime,fech);
    lista[1].i_type='1';
    lista[1].i_block[0]=1;
//ENVIAMOS A ESCRIBIR AL DISCO CREADO
    EscribirNodo(part.mnt_ruta,super.s_inode_start,n,lista);
    

    super.s_firts_ino=super.s_firts_ino+2*sizeof(inodo);

    //REPETIMOS LA CREACION PERO CON BLOQUES
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
    cout<<"SISTEMA EXT2 CREADO CORRECTAMENTE"<<endl;
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
    //AHORA ESCRIBIMOS BITMAPS
    EscribirInodoMap(part.mnt_ruta,bitmapInodo,super.s_bm_inode_start,n);


    inodo root;
    
    times fech;
    fechActual(fech);
    

    for(int i=0;i<16;i++){
        root.i_block[i]=-1;
    }
    
    inodo lista[n];

    for (int t = 0; t < n; t++) {
        lista[t].i_uid=0;
        lista[t].i_gid=0;
        strcpy(lista[t].i_atime,"");
        strcpy(lista[t].i_ctime,"");
        strcpy(lista[t].i_mtime,"");
        lista[t].i_type='2';
        for(int i=0;i<16;i++){
            lista[t].i_block[i]=-1;
        }

        
    }
    
    lista[0].i_uid=1;
    lista[0].i_gid=1;
    lista[1].i_uid=1;
    lista[1].i_gid=1;
    lista[0].i_perm=664;
    lista[1].i_perm=700;
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
    cout<<"SISTEMA EXT3 CREADO CORRECTAMENTE"<<endl;
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
        //cout<<"BLOCK "<<super.s_bm_block_start<<endl;
        //cout<<"INODO "<<super.s_bm_inode_start<<endl;


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
        //cout<<"voy por "<<(inicio+i)<<endl;
        //cout<<"con "<<mapB[i].status<<endl;
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
        //cout<<"voy por "<<(inicio+i)<<endl;
        //cout<<"con "<<mapI[i].status<<endl;
        fseek(f, inicio+i*(sizeof(bmInodo)), SEEK_SET);
        fwrite(&mapI[i], sizeof (bmInodo), 1, f);
    }
    fclose(f);
}
//METODO RE IMPORTANTE CON ESTO SE ARREGLARON BUGS
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
            //cout<<"gid "<<lista[j].i_gid<<endl;
            //fread(&i2,sizeof(i2),1,archivo);
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
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &tstruct);
    strcpy(fecha,buf);
}



void Comando:: LogIn(string usr,string pass,string id){
    mnt_nodo montado=retornarNodoMount(id);
    //cout<<montado.mnt_ruta<<endl;
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
    //cout<<"contenido total: "<<contenido<<endl;
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
                    grupoLogeado=k+1;
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
    bool l=VerificarPermisoLeer(InodeUsers);
    //cout<<"regresar xd: "<<l<<endl;
    if(!l){
        cout<<"ERROR: PERMISOS INSUFICIENTES PARA LA OPERACION"<<endl;
    }
    string contenido;
    fclose(archivo);
    archivo=fopen(pathGlobal.c_str(),"r+b");
    //conseguimos el contenido dentro de los links de bloque del inodo
    for(int i=0;i<16;i++){    
        
        if(InodeUsers.i_block[i]>=0&&i<13){    
            //cout<<"leyendo: "<<i<<" DIRECCION "<<InodeUsers.i_block[i]<<endl;
            bloqueArchivos FileUsers;
            //cout<<"Vinculo: "<<InodeUsers.i_block[i]<<endl;
            int posAct=superLog.s_block_start+InodeUsers.i_block[i]*sizeof(bloqueArchivos);
            fseek(archivo,posAct, SEEK_SET);
            fread(&FileUsers, sizeof(bloqueArchivos), 1, archivo);
            
            contenido+=FileUsers.b_content;
            //cout<<"contenido "<<FileUsers.b_content<<endl;
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
    //cout<<"escribire en "<<posAct<<endl;
    if(!VerificarPermisoEscribir(InodeUsers)){
        cout<<"ERROR: PERMISOS INSUFICIENTES PARA LA OPERACION (ESCRIBIR)"<<endl;
        return;
    }

    InodeUsers.i_size+=nCont.size();
    times fech;
    fechActual(fech);
    strcpy(InodeUsers.i_mtime,fech);

    fseek(archivo,posAct, SEEK_SET);
    fwrite(&InodeUsers,sizeof(InodeUsers),1,archivo);

    fseek(archivo,posAct, SEEK_SET);
    fread(&InodeUsers, sizeof(InodeUsers), 1, archivo);

    bloqueArchivos bloqueA;
    for(int i=0;i<16;i++){
        
        if(i<13){
            cout<<"se supone que tengo:    "<<InodeUsers.i_block[i]<<" en "<<i<<endl;
            if(InodeUsers.i_block[i]==-1){
                cout<<"queriendo escribir:    "<<nCont<<endl;
                InodeUsers.i_block[i]=crearBloque(0,nCont,archivo);
                //cout<<InodeUsers.i_block[i]<<endl;
                cout<<"GRUPO AÑADIDO CON EXITO"<<endl;
                fclose(archivo);
                EscribirInodoUnidad(posAct,InodeUsers);
                return;
            }
            int posActB=InodeUsers.i_block[i]*sizeof(bloqueArchivos)+superLog.s_block_start;
            fseek(archivo,posActB, SEEK_SET);
            fread(&bloqueA, 64, 1, archivo);
            
            string cont=bloqueA.b_content;
            int sizeCont=cont.length();
            //cout<<"tamano disp: "<<64-sizeCont<<endl;;

            if((64-sizeCont)>0){
                cout<<"probando caminos"<<endl;
                if(64-sizeCont>nCont.size()){
                    //cout<<"aqui imprime"<<endl;
                    cont+=nCont;
                    strcpy(bloqueA.b_content,cont.c_str());
                    fseek(archivo,posActB, SEEK_SET);
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
                    fseek(archivo,posActB, SEEK_SET);
                    fwrite(&bloqueA,sizeof(bloqueArchivos),1,archivo);
                    fclose(archivo);
                    //cout<<bloqueA.b_content<<endl;
                    //cout<<"posactual"<<posAct<<endl;
                    //cout<<"posactualLibre"<<superLog.s_first_blo<<endl;
                    archivo=fopen(pathGlobal.c_str(), "r+b");
                    if(i<12){
                        //cout<<"que es resto? "<<resto<<endl;
                        InodeUsers.i_block[i+1]=crearBloque(0,resto,archivo);
                        
                        //cout<<"que contador hay?"<<InodeUsers.i_block[i+1]<<endl;
                        
                        fclose(archivo);
                        EscribirInodoUnidad(posAct,InodeUsers);

                    }else{
                        InodeUsers.i_block[i+1]=crearBloque(1,"",archivo);
                        
                        bloqueApuntadores punt;
                        int posActP=superLog.s_block_start+InodeUsers.i_block[i]*sizeof(bloqueApuntadores);
                        fseek(archivo,posActP, SEEK_SET);
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
            cout<<"probando camino fin"<<endl;

        }if(i==13){
            int posActP=superLog.s_block_start+InodeUsers.i_block[i]*sizeof(bloqueApuntadores);
            if(InodeUsers.i_block[i]==-1){
                InodeUsers.i_block[i]=crearBloque(1,nCont,archivo);
                
                bloqueApuntadores punt;
                
                fseek(archivo,posActP, SEEK_SET);
                fread(&punt, sizeof(bloqueApuntadores), 1, archivo);

                punt.b_pointers[0]=crearBloque(0,nCont,archivo);
                
                fseek(archivo,posActP, SEEK_SET);
                fwrite(&punt,sizeof(bloqueArchivos),1,archivo);
    
                
                fclose(archivo);
                EscribirInodoUnidad(posAct,InodeUsers);

                cout<<"GRUPO AÑADIDO CON EXITO"<<endl;
                
                return;
            }
            bloqueApuntadores punt;
            
            fseek(archivo,posActP, SEEK_SET);
            fread(&punt, sizeof(bloqueApuntadores), 1, archivo);
            for(int j=0;j<16;j++){
                if(punt.b_pointers[i]==-1){
                    punt.b_pointers[i]=crearBloque(0,nCont,archivo);
                    
                    cout<<"GRUPO AÑADIDO CON EXITO"<<endl;
                    fclose(archivo);
                    EscribirBloqueApuntador(pathGlobal,posActP,punt);
                    return;
                }
                int posActA=punt.b_pointers[i]*sizeof(bloqueArchivos)+superLog.s_block_start;
                fseek(archivo,posActA, SEEK_SET);
                fread(&bloqueA, sizeof(bloqueArchivos), 1, archivo);
                string cont=bloqueA.b_content;
                int sizeCont=cont.length();
                //cout<<"tamano disp: "<<64-sizeCont<<endl;;

                if((64-sizeCont)>0){
                    if(64-sizeCont>nCont.size()){
                        cont+=nCont;
                        strcpy(bloqueA.b_content,cont.c_str());
                        fseek(archivo,posActA, SEEK_SET);
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
                        fseek(archivo,posActA, SEEK_SET);
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
                        EscribirBloqueApuntador(pathGlobal,posAct,punt);
                        
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

    char bitmap[superLog.s_blocks_count];
    int contador=0;
    superLog.s_free_blocks_count--;
    fseek(archivo,superLog.s_bm_block_start, SEEK_SET);
    fread(&bitmap,superLog.s_blocks_count,1,archivo);

    for(int i=0;i<superLog.s_blocks_count;i++){
        if(bitmap[i]=='0'){
            contador=i;
            break;
        }
    }
    //cout<<"El nuevo es: "<<superLog.s_block_start+contador*64<<endl;;

    if(tipo==0){
        
        bloqueArchivos bloqueA;
        //cout<<"sigo vivo en verificacion tipo"<<endl;
        strcpy(bloqueA.b_content,escribir.c_str());
        fseek(archivo,superLog.s_block_start+contador*64, SEEK_SET);
        fwrite(&bloqueA,64,1,archivo);
        //cout<<"sigo vivo en verificacion tipo"<<endl;

    }else if(tipo==1){
        bloqueApuntadores bloqueAp;
        for(int i=0;i<16;i++){
            bloqueAp.b_pointers[i]=-1;
        }

        fseek(archivo,superLog.s_block_start+contador*64, SEEK_SET);
        fwrite(&bloqueAp,sizeof(bloqueApuntadores),1,archivo);
    }else if(tipo==2){
        bloqueCarpetas bloqueCar;
        bloqueCar.b_content[0].b_inodo=-1;
        bloqueCar.b_content[1].b_inodo=-1;
        bloqueCar.b_content[2].b_inodo=-1;
        bloqueCar.b_content[3].b_inodo=-1;

        fseek(archivo,superLog.s_block_start+contador*64, SEEK_SET);
        fwrite(&bloqueCar,64,1,archivo);
    }

    superLog.s_first_blo+=64;
    
    fclose(archivo);
    
    //cout<<"estoy vivo antes del super"<<endl;

    EscribirSuper(pathGlobal,superLog,inicioG);
    //cout<<"estoy vivo despues del super"<<endl;
    actualizar_bmBlock(contador);
    //cout<<"actualizo el bitmap"<<endl;

    archivo = fopen(pathGlobal.c_str(), "r+b");
    return contador;
}


int crearInodo(int tipo,string name,int size,FILE* archivo){
    inodo nuevo;
    char bitmap[superLog.s_inodes_count];


    fseek(archivo,superLog.s_bm_inode_start, SEEK_SET);
    fread(&bitmap, superLog.s_inodes_count, 1, archivo);

    int contador=0;
    superLog.s_free_inodes_count--;
    

    for(int i=0;i<superLog.s_inodes_count;i++){
        if(bitmap[i]=='0'){
            contador=i;
            break;
        }
    }
    cout<<"inodo creado:"<<contador<<endl;
    
    inodo lista[superLog.s_inodes_count];    

    fseek(archivo,superLog.s_inode_start, SEEK_SET);
    fread(&lista, superLog.s_inodes_count*sizeof(inodo), 1, archivo);
    fclose(archivo);

    lista[contador].i_gid=grupoLogeado;
    lista[contador].i_uid=logeado.Id;

    lista[contador].i_type=tipo;
    //cout<<"tipo que llega: "<<lista[contador].i_type<<endl;
    lista[contador].i_size=size;
    lista[contador].i_perm=644;
    times fech;
    fechActual(fech);
    
    strcpy(lista[contador].i_mtime,fech);
    strcpy(lista[contador].i_ctime,fech);
    

    EscribirNodo(pathGlobal,superLog.s_inode_start,superLog.s_inodes_count,lista);
    //cout<<"creando en: "<<contador*sizeof(inodo)+superLog.s_inode_start<<endl;
    //EscribirInodoUnidad(contador*sizeof(inodo)+superLog.s_inode_start,nuevo);
    EscribirSuper(pathGlobal,superLog,inicioG);
    actualizar_bmInodo(contador);
    archivo=fopen(pathGlobal.c_str() ,"r+b");
    return contador;

}


void EscribirInodoUnidad(int posAct,inodo InodeUsers){
    FILE* archivo=fopen(pathGlobal.c_str(),"r+b");
    
    fseek(archivo,posAct, SEEK_SET);
    fwrite(&InodeUsers,sizeof(inodo),1,archivo);
    
    fclose(archivo);
}

bool VerificarPermisoLeer(inodo Inodo){
    //cout<<";"<<logeado.name<<";"<<endl;
    if(strcmp(logeado.name,"root")==0||logeado.Id==0){
        return true;
    }
    int permiso;

    if(Inodo.i_uid==logeado.Id){
        permiso=Inodo.i_perm/100;
    }else if(Inodo.i_gid==grupoLogeado){
        permiso=Inodo.i_perm%100;
        permiso=Inodo.i_perm/10;
    }else{
        permiso=Inodo.i_perm%10;
    }
    if(permiso==7||permiso==6||permiso==5||permiso==4)return true;
    return false;
}

bool VerificarPermisoEjecutar(inodo Inodo){
    //cout<<strcmp(logeado.name,"root")<<";"<<endl;
    if(strcmp(logeado.name,"root")==0){
        return true;
    }
    int permiso;

    if(Inodo.i_uid==logeado.Id){
        permiso=Inodo.i_perm/100;
    }else if(Inodo.i_gid==grupoLogeado){
        permiso=Inodo.i_perm%100;
        permiso=Inodo.i_perm/10;
    }else{
        permiso=Inodo.i_perm%10;
    }
    //cout<<permiso<<endl;
    if(permiso==7||permiso==5||permiso==3||permiso==1)return true;
    return false;
}

bool VerificarPermisoEscribir(inodo Inodo){
    if(strcmp(logeado.name,"root")==0||logeado.Id==0){
        return true;
    }
    int permiso;

    if(Inodo.i_uid==logeado.Id){
        permiso=Inodo.i_perm/100;
    }else if(Inodo.i_gid==grupoLogeado){
        permiso=Inodo.i_perm%100;
        permiso=Inodo.i_perm/10;
    }else{
        permiso=Inodo.i_perm%10;
    }
    if(permiso==7||permiso==6||permiso==3||permiso==2)return true;
    return false;
}

int BuscarInodo(string name,int pos,FILE* archivo){
    //contadomagico++;
    
    inodo InodoActual;
    fseek(archivo,pos, SEEK_SET);
    fread(&InodoActual, sizeof(inodo), 1, archivo);
    
    //cout<<"viendo el inodo "<<pos<<endl;
    if(InodoActual.i_type=='0'){
        for(int i=0;i<16;i++){
            //cout<<"PASANDO POR INODO POS: "<<i<<endl;
            if(InodoActual.i_block[i]>-1){
                //cout<<"PASE EL IF:"<<i<<" pos:"<<pos<<endl;
                
                if(i<13){
                    
                    int num=recorrerCarpeta(name,InodoActual.i_block[i]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo);
                    cout<<num<<endl;
                    if(num!=-1){
                        return num;
                    }
                
                }if(i==13){
                    bloqueApuntadores apuntadores;
                    fseek(archivo,InodoActual.i_block[13]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            int num=recorrerCarpeta(name,apuntadores.b_pointers[j]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo);
                            if(num!=-1){
                                return num;
                            }
                        }
                    } 
                }if(i==14){
                    bloqueApuntadores apuntadores,apuntadores2;
                    fseek(archivo,InodoActual.i_block[14]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            fseek(archivo,apuntadores.b_pointers[j]*64+superLog.s_block_start, SEEK_SET);
                            fread(&apuntadores2, 64, 1, archivo);
                            for(int k=0;k<16;k++){
                                if(apuntadores2.b_pointers[k]!=-1){
                                    int num=recorrerCarpeta(name,apuntadores2.b_pointers[k]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo);
                                    if(num!=-1){
                                        return num;
                                    }
                                }
                            }
                        }
                    }
                }if(i==15){
                    bloqueApuntadores apuntadores,apuntadores2,apuntadores3;
                    fseek(archivo,InodoActual.i_block[15]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            fseek(archivo,apuntadores.b_pointers[j]*64+superLog.s_block_start, SEEK_SET);
                            fread(&apuntadores2, 64, 1, archivo);
                            for(int k=0;k<16;k++){
                                if(apuntadores2.b_pointers[k]!=-1){
                                    fseek(archivo,apuntadores2.b_pointers[k]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                                    fread(&apuntadores3, sizeof(bloqueApuntadores), 1, archivo);
                                    for(int l=0;l<16;l++){
                                        if(apuntadores3.b_pointers[l]!=-1){
                                            int num=recorrerCarpeta(name,apuntadores3.b_pointers[l]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo);
                                            if(num!=-1){
                                                return num;
                                            }
                                        }
                                    }
                                }
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
    //cout<<"pos carpeta: "<<pos<<endl;
    for(int i=0;i<4;i++){
        if(bloque.b_content[i].b_inodo>-1){   
            string n=bloque.b_content[i].b_name;
            //cout<<"actual: "<<n<<" - buscado: "<<name<<endl;
            if(strcmp(n.c_str(),name.c_str())==0){
                return bloque.b_content[i].b_inodo;
            }
        }
    }

    for(int i=0;i<4;i++){
        if(bloque.b_content[i].b_inodo>-1){
            inodo actual;
            fseek(archivo,bloque.b_content[i].b_inodo*sizeof(inodo)+superLog.s_inode_start, SEEK_SET);
            fread(&actual, sizeof(inodo), 1, archivo);
            //cout<<"soy: "<<bloque.b_content[i].b_name<<" - tipo: "<<actual.i_type<<endl;
            if(actual.i_type=='0'){
                //cout<<"voy a: "<<bloque.b_content[i].b_inodo<<" - tipo: "<<actual.i_type<<" - soy: "<<bloque.b_content[i].b_name<<endl;
                int num=BuscarInodo(name,bloque.b_content[i].b_inodo*sizeof(inodo)+superLog.s_inode_start,archivo);
                if(num>-1){
                    return num;
                }
            }
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
        
        bitmapI[n]='1';   
        //cout<<"se supone que actualizo "<<bitmapI[n]<<endl;
    }else{
        bitmapI[n]='0';
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


void Comando::rep(string name,string path,string id,string rutai){
        string direct = "";

        string name_p = "";
        string rutaCompleta=path;
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
        //cout<<"terminacion"<<name_p<<endl;
        //cout<<"ruta Completa"<<rutaCompleta<<endl;
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
            //cout<<"aqui va el name: "<<name<<endl;

            if(strcmp(name.c_str(),"mbr")==0){
                MBR mbrRep;
                fseek(archivo, 0, SEEK_SET);
                fread(&mbrRep, sizeof (MBR), 1, archivo);
                fclose(archivo);

                string dot;
                dot+="digraph D{";
                dot+="label=\"REPORTE MBR\"";
                dot+="nodo_mbr[shape=platntext label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n";
                dot+="<TR><TD align=\"left\" color=\"white\" bgcolor=\"darkslateblue\">REPORTE DE MBR</TD><TD color=\"white\" bgcolor=\"darkslateblue\"></TD></TR>\n";
                dot+="<TR><TD color=\"white\">mbr_tamano</TD ><TD color=\"white\">";
                dot+=to_string(mbrRep.mbr_tamano);
                dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">mbr_fecha_creacion</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                dot+=to_string(mbrRep.mbr_fecha_creacion);
                dot+="</TD></TR>\n<TR><TD color=\"white\">mbr_disk_signature</TD><TD color=\"white\">";
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
                        dot+="</TR>\n<TR><TD color=\"white\">part_status</TD><TD color=\"white\">";
                        dot+=particiones[i].part_status;
                        dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">part_type</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                        dot+=particiones[i].part_type;
                        dot+="</TD></TR>\n<TR><TD color=\"white\">part_fit </TD><TD color=\"white\">";
                        dot+=particiones[i].part_fit;
                        dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">part_start</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                        dot+=to_string(particiones[i].part_start);
                        dot+="</TD></TR>\n<TR><TD color=\"white\">part_size </TD><TD color=\"white\">";
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
                                //cout<<"Inicio: "<<inicioE<<endl;

                                fseek(f, inicioE, SEEK_SET);
                                fread(&ebrRep, sizeof (EBR), 1, f);
                                
                                //cout<<"si"<<endl;
                                //cout<<"fit: "<<ebrRep.part_fit<<endl;
                                string B="b",F="f",W="w",fit=to_string(ebrRep.part_fit);
                                if (ebrRep.part_fit=='b' || strcmp( fit.c_str(), F.c_str())==0 || strcmp( fit.c_str(), W.c_str())==0) {
                                    //cout<<"si?"<<endl;
                                    int siguiente=ebrRep.part_next;
                                    while(ebrRep.part_fit=='b' || strcmp( fit.c_str(), F.c_str())==0 || strcmp( fit.c_str(), W.c_str())==0){
                                        dot+="<TR><TD align=\"left\" color=\"white\" bgcolor=\"springgreen1\">PARTICION LOGICA</TD><TD color=\"white\" bgcolor=\"springgreen1\"></TD>";
                                        dot+="</TR>\n<TR><TD color=\"white\">part_status</TD><TD color=\"white\">";
                                        dot+=to_string(ebrRep.part_status);
                                        dot+="</TD></TR>\n<TR><TD color=\"white\" bgcolor=\"springgreen1\">part_next</TD><TD color=\"white\" bgcolor=\"springgreen1\">";
                                        dot+=to_string(ebrRep.part_next);
                                        dot+="</TD></TR>\n<TR><TD color=\"white\">part_fit</TD><TD color=\"white\">";
                                        dot+=ebrRep.part_fit;
                                        dot+="</TD></TR>\n<TR><TD color=\"white\" bgcolor=\"springgreen1\">part_start</TD><TD color=\"white\" bgcolor=\"springgreen1\">";
                                        dot+=to_string(ebrRep.part_start);
                                        dot+="</TD></TR>\n<TR><TD color=\"white\">part_size</TD><TD color=\"white\">";
                                        dot+=to_string(ebrRep.part_s);
                                        dot+="</TD></TR>\n<TR><TD color=\"white\" bgcolor=\"springgreen1\">part_name</TD><TD color=\"white\" bgcolor=\"springgreen1\">";
                                        dot+=ebrRep.part_name;
                                        dot+="</TD></TR>";
                                        fseek(f, siguiente, SEEK_SET);
                                        fread(&ebrRep, sizeof (EBR), 1, f); 
                                        siguiente=ebrRep.part_next;
                                    }
                                }
                                fclose(f);
                            }                                
                        }
                    }
                }
                dot+="</TABLE>>];";
                dot+="}";
                //cout<<dot<<endl;
                escribirDOT(dot,name_p,rutaCompleta);
            }else if(name=="disk"){
                MBR mbrRep;
                fseek(archivo, 0, SEEK_SET);
                fread(&mbrRep, sizeof (MBR), 1, archivo);
                fclose(archivo);

                string dot,dot2;
                dot+="digraph D{";
                dot+="label=\"REPORTE DISK\"";
                dot+="nodo_mbr[shape=platntext label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n";
                partitiond particiones[4];
                particiones[0] = mbrRep.mbr_partition_1;
                particiones[1] = mbrRep.mbr_partition_2;
                particiones[2] = mbrRep.mbr_partition_3;
                particiones[3] = mbrRep.mbr_partition_4;
                
                dot+="<TR><TD rowspan=\"2\" bgcolor=\"lightcoral\">MBR</TD>";
                int ultimaPos,numLogics=0;
                double porcentaje;
                for (int i = 0; i < 4; i++) {

                    if(particiones[i].part_s>0){
                        ultimaPos=particiones[i].part_start+particiones[i].part_s;
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
                                
                                
                                string B="b",F="f",W="w",fit=to_string(ebrRep.part_fit);
                                if (ebrRep.part_fit=='b' || strcmp( fit.c_str(), F.c_str())==0 || strcmp( fit.c_str(), W.c_str())==0) {
                                    dot2+="<TR>";
                                    int siguiente=ebrRep.part_next;
                                    while(ebrRep.part_fit=='b' || strcmp( fit.c_str(), F.c_str())==0 || strcmp( fit.c_str(), W.c_str())==0){
                                        numLogics++;
                                        double prim=(ebrRep.part_s);
                            
                                        double seg=mbrRep.mbr_tamano;
                                            
                                        porcentaje=(prim/seg)*100;
                                        dot2+="<TD bgcolor=\"skyblue1\">LOGICA<BR/>"+to_string(porcentaje)+" %</TD>";                                    
                                        fseek(f, siguiente, SEEK_SET);
                                        fread(&ebrRep, sizeof (EBR), 1, f); 
                                        siguiente=ebrRep.part_next;
                                    }                               
                                    
                                    if(i==3){
                                        if(ebrRep.part_start+ebrRep.part_s<mbrRep.mbr_tamano){
                                            double prim=(mbrRep.mbr_tamano-(ebrRep.part_start+ebrRep.part_s));
                            
                                            double seg=mbrRep.mbr_tamano;
                                            
                                            porcentaje=(prim/seg)*100;
                                            dot2+="<TD bgcolor=\"azure4\">ESPACIO LIBRE<BR/>"+to_string(porcentaje)+" %</TD>";
                                            numLogics++;
                                        }
                                    }else{
                                        if(ebrRep.part_start+ebrRep.part_s<particiones[i+1].part_start){
                                            double prim=(particiones[i+1].part_start-(ebrRep.part_start+ebrRep.part_s));
                            
                                            double seg=mbrRep.mbr_tamano;
                                            
                                            porcentaje=(prim/seg)*100;

                                            
                                            dot2+="<TD bgcolor=\"azure4\">ESPACIO LIBRE<BR/>"+to_string(porcentaje)+" %</TD>";
                                            numLogics++;
                                        }
                                    }
                                    
                                    dot2+="</TR>";
                                }else{
                                    double prim=(particiones[i].part_s);
                            
                                    double seg=mbrRep.mbr_tamano;
                                            
                                    porcentaje=(prim/seg)*100;

                                    dot2+="<TR><TD bgcolor=\"azure4\">ESPACIO LIBRE<BR/>"+to_string(porcentaje)+" %</TD></TR>";
                                } 
                            }       
                            fclose(f);                       
                            if(numLogics==0)numLogics=1;
                            dot+="<TD colspan=\""+to_string(numLogics)+"\" bgcolor=\"skyblue1\">EXTENDIDA</TD>";

                        }else{
                            
                            double prim=particiones[i].part_s;
                            
                            double seg=mbrRep.mbr_tamano;
                            
                            porcentaje=(prim/seg)*100;
                            
                            dot+="<TD rowspan=\"2\" bgcolor= \"mediumspringgreen\">PRIMARIA<BR/>"+to_string(porcentaje)+" %</TD>";
                        }
                    }else{
                        if(i==0){
                            double prim=particiones[i+1].part_start;
                            
                            double seg=mbrRep.mbr_tamano;
                            
                            porcentaje=(prim/seg)*100;
                            dot+="<TD rowspan=\"2\" bgcolor=\"azure4\">ESPACIO LIBRE<BR/>"+to_string(porcentaje)+" %</TD>";
                        }if(i==3){
                            double prim=(mbrRep.mbr_tamano-(particiones[i-1].part_start+particiones[i-1].part_s));
                            
                            double seg=mbrRep.mbr_tamano;
                            
                            porcentaje=(prim/seg)*100;
                            dot+="<TD rowspan=\"2\" bgcolor=\"azure4\">ESPACIO LIBRE<BR/>"+to_string(porcentaje)+" %</TD>";
                        }else{
                            double prim=((particiones[i-1].part_start+particiones[i-1].part_s)-particiones[i+1].part_start);
                            
                            double seg=mbrRep.mbr_tamano;
                            
                            porcentaje=(prim/seg)*100;
                            dot+="<TD rowspan=\"2\" bgcolor=\"azure4\">ESPACIO LIBRE<BR/>"+to_string(porcentaje)+" %</TD>";
                        }
                    }
                }

                if(ultimaPos<mbrRep.mbr_tamano){
                    double prim=(mbrRep.mbr_tamano-ultimaPos);
                            
                    double seg=mbrRep.mbr_tamano;
                            
                    porcentaje=(prim/seg)*100;
                    dot+="<TD rowspan=\"2\" bgcolor=\"azure4\">ESPACIO LIBRE<BR/>"+to_string(porcentaje)+" %</TD>";
                }
                dot+="</TR>";
                dot+=dot2;
                dot+="</TABLE>>];";
                dot+="}";
                //cout<<dot<<endl;
                escribirDOT(dot,name_p,rutaCompleta);
            }else if(name=="inode"){
                superBloque super;
                
                fseek(archivo, inicio, SEEK_SET);
                fread(&super, sizeof (superBloque), 1, archivo);
                
                int nInodos=super.s_inodes_count;
                inodo tablaInodo[nInodos];

                fseek(archivo, super.s_inode_start, SEEK_SET);
                fread(&tablaInodo, nInodos*sizeof(inodo), 1, archivo);

                fclose(archivo);

                string dot="";
                dot+="digraph D{\n";
                dot+="label=\"REPORTE INODE\"\n node[shape=platntext]\n rankdir=LR\n";
                int anterior=-1;
                for(int i=0;i<nInodos;i++){
                    
                    
                    if(tablaInodo[i].i_gid!=0){

                        if(anterior!=-1){
                            dot+="Inodo"+to_string(anterior)+"->Inodo"+to_string(i)+"\n";
                        }
                        //cout<<"guid: "<<tablaInodo[i].i_gid<<endl;

                        dot+="Inodo"+to_string(i)+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                        dot+="<TR><TD bgcolor=\"springgreen1\">INODO</TD><TD bgcolor=\"springgreen1\">"+to_string(i)+"</TD></TR>\n";
                        dot+="<TR><TD>i_uid</TD><TD>"+to_string(tablaInodo[i].i_uid)+"</TD></TR>\n";
                        dot+="<TR><TD>i_gid</TD><TD>"+to_string(tablaInodo[i].i_gid)+"</TD></TR>\n";
                        dot+="<TR><TD>i_size</TD><TD>"+to_string(tablaInodo[i].i_size)+"</TD></TR>\n";
                        //char fecha[80];
                        //strcpy(fecha,tablaInodo[i].i_atime);
                        string fechaA=tablaInodo[i].i_atime;
                        
                        dot+="<TR><TD>i_atime</TD><TD>"+fechaA+"</TD></TR>\n";
                        fechaA=tablaInodo[i].i_mtime;
                        dot+="<TR><TD>i_mtime</TD><TD>"+fechaA+"</TD></TR>\n";
                        fechaA=tablaInodo[i].i_ctime;
                        dot+="<TR><TD>i_ctime</TD><TD>"+fechaA+"</TD></TR>\n";
                        dot+="<TR><TD>i_perm</TD><TD>"+to_string(tablaInodo[i].i_perm)+"</TD></TR>\n";
                        dot+="<TR><TD bgcolor=\"springgreen1\">LINKS UTILIZADOS</TD><TD bgcolor=\"springgreen1\"></TD></TR>\n";
                        for(int j=0;j<16;j++){
                            if(tablaInodo[i].i_block[j]>-1)dot+="<TR><TD>i_block_"+to_string(j)+"</TD><TD>"+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                        }
                        
                        dot+="</TABLE>>];";
                        anterior=i;
                    }
                }
                dot+="}";
                //cout<<dot<<endl;
                escribirDOT(dot,name_p,rutaCompleta);
            }else if(name=="journaling"){
                
            }else if(name=="block"){
                superBloque super;
                
                fseek(archivo, inicio, SEEK_SET);
                fread(&super, sizeof (superBloque), 1, archivo);
                
                int nInodos=super.s_free_inodes_count;
                inodo tablaInodo[nInodos];

                fseek(archivo, super.s_inode_start, SEEK_SET);
                fread(&tablaInodo, nInodos*sizeof(inodo), 1, archivo);

                

                string dot="";
                dot+="digraph D{\n";
                dot+="label=\"REPORTE BLOCK\"\n node[shape=platntext]\n";
                int anterior=0;
                bloqueApuntadores apuntadoresB,apuntadoresB1,apuntadoresB2;
                bloqueArchivos archivoB;
                bloqueCarpetas carpetaB;

                for(int i=0;i<nInodos;i++){
                    if(tablaInodo[i].i_gid!=0){

                        if(anterior!=-1){

                            for(int j=0;j<16;j++){

                                if(tablaInodo[i].i_block[j]>-1){
                                    if(j==13){
                                        fseek(archivo, super.s_block_start+tablaInodo[i].i_block[j]*64, SEEK_SET);
                                        fread(&apuntadoresB, 64, 1, archivo);
                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(tablaInodo[i].i_block[j])+"\n";
                                        dot+="Bloque"+to_string(tablaInodo[i].i_block[j])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                        dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                                        string contenido="";
                                        for(int x=0;x<15;x++){
                                            contenido+=to_string(apuntadoresB.b_pointers[x])+", ";
                                        }
                                        contenido+=to_string(apuntadoresB.b_pointers[15]);
                                        dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                        dot+="</TABLE>>];\n";
                                        anterior=tablaInodo[i].i_block[j];
                                        for(int a=0;a<16;a++){
                                            if(apuntadoresB.b_pointers[a]=!-1){
                                                if(tablaInodo[i].i_type==0){
                                                    fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                    fread(&carpetaB, 64, 1, archivo);
                                                    

                        
                                                    dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a])+"\n";
                                                    dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                    dot+="<TR><TD>BLOQUE CARPETA</TD><TD>"+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                    dot+="<TR><TD>b_name</TD><TD>b_inodo</TD></TR>\n";
                                                    for(int x=0;x<4;x++){
                                                        if(carpetaB.b_content[x].b_inodo>-1){
                                                            string name_B_C=carpetaB.b_content[x].b_name;
                                                            dot+="<TR><TD>"+name_B_C+"</TD><TD>"+to_string(carpetaB.b_content[x].b_inodo)+"</TD></TR>\n";
                                                        }
                                                    }
                                                    dot+="</TABLE>>];\n";
                                                    
                                                    anterior=apuntadoresB.b_pointers[a];
                                                    continue;
                                                }
                                                fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                fread(&archivoB, 64, 1, archivo);
                                                dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a])+"\n";
                                                dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                dot+="<TR><TD>BLOQUE ARCHIVO "+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                string contenido=archivoB.b_content;
                                                dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                dot+="</TABLE>>];\n";
                                                anterior=apuntadoresB.b_pointers[a];
                                            }

                                        }

                                        continue;
                                    }else if(j==14){
                                        fseek(archivo, super.s_block_start+tablaInodo[i].i_block[j]*64, SEEK_SET);
                                        fread(&apuntadoresB1, 64, 1, archivo);
                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(tablaInodo[i].i_block[j])+"\n";
                                        dot+="Bloque"+to_string(tablaInodo[i].i_block[j])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                        dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                                        string contenido="";
                                        for(int x=0;x<15;x++){
                                            contenido+=to_string(apuntadoresB1.b_pointers[x])+", ";
                                        }
                                        contenido+=to_string(apuntadoresB1.b_pointers[15]);
                                        dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                        dot+="</TABLE>>];\n";
                                        anterior=tablaInodo[i].i_block[j];                                                

                                        
                                        for(int b=0;b<16;b++){
                                            if(apuntadoresB1.b_pointers[b]=!-1){
                                                fseek(archivo, super.s_block_start+apuntadoresB1.b_pointers[b]*64, SEEK_SET);
                                                fread(&apuntadoresB, 64, 1, archivo);

                                                dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB1.b_pointers[b])+"\n";
                                                dot+="Bloque"+to_string(apuntadoresB1.b_pointers[b])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(apuntadoresB1.b_pointers[b])+"</TD></TR>\n";
                                                string contenido="";
                                                for(int x=0;x<15;x++){
                                                    contenido+=to_string(apuntadoresB.b_pointers[x])+", ";
                                                }
                                                contenido+=to_string(apuntadoresB.b_pointers[15]);
                                                dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                dot+="</TABLE>>];\n";
                                                anterior=apuntadoresB1.b_pointers[b];

                                                for(int a=0;a<16;a++){
                                                    if(apuntadoresB.b_pointers[a]=!-1){
                                                        if(tablaInodo[i].i_type==0){
                                                            fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                            fread(&carpetaB, 64, 1, archivo);
                                                            
                                                            dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a])+"\n";
                                                            dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                            dot+="<TR><TD>BLOQUE CARPETA</TD><TD>"+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                            dot+="<TR><TD>b_name</TD><TD>b_inodo</TD></TR>\n";
                                                            for(int x=0;x<4;x++){
                                                                if(carpetaB.b_content[x].b_inodo>-1){
                                                                    string name_B_C=carpetaB.b_content[x].b_name;
                                                                    dot+="<TR><TD>"+name_B_C+"</TD><TD>"+to_string(carpetaB.b_content[x].b_inodo)+"</TD></TR>\n";
                                                                }
                                                            }
                                                            dot+="</TABLE>>];\n";
                                                            
                                                            anterior=apuntadoresB.b_pointers[a];
                                                            continue;
                                                        }
                                                        fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                        fread(&archivoB, 64, 1, archivo);
                                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a])+"\n";
                                                        dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                        dot+="<TR><TD>BLOQUE ARCHIVO "+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                        string contenido=archivoB.b_content;
                                                        dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                        dot+="</TABLE>>];\n";
                                                        anterior=apuntadoresB.b_pointers[a];
                                                    }

                                                }
                                                
                                            }
                                        }

                                        continue;
                                    }else if(j==15){
                                        fseek(archivo, super.s_block_start+tablaInodo[i].i_block[j]*64, SEEK_SET);
                                        fread(&apuntadoresB2, 64, 1, archivo);
                                        
                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(tablaInodo[i].i_block[j])+"\n";
                                        dot+="Bloque"+to_string(tablaInodo[i].i_block[j])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                        dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                                        string contenido="";
                                        for(int x=0;x<15;x++){
                                            contenido+=to_string(apuntadoresB2.b_pointers[x])+", ";
                                        }
                                        contenido+=to_string(apuntadoresB2.b_pointers[15]);
                                        dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                        dot+="</TABLE>>];\n";
                                        anterior=tablaInodo[i].i_block[j];                                                

                                        for(int c=0;c<16;c++){
                                            if(apuntadoresB2.b_pointers[c]=!-1){
                                                fseek(archivo, super.s_block_start+apuntadoresB2.b_pointers[c]*64, SEEK_SET);
                                                fread(&apuntadoresB1, 64, 1, archivo);
                                                
                                                dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB2.b_pointers[c])+"\n";
                                                dot+="Bloque"+to_string(apuntadoresB2.b_pointers[c])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(apuntadoresB2.b_pointers[c])+"</TD></TR>\n";
                                                string contenido="";
                                                for(int x=0;x<15;x++){
                                                    contenido+=to_string(apuntadoresB1.b_pointers[x])+", ";
                                                }
                                                contenido+=to_string(apuntadoresB1.b_pointers[15]);
                                                dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                dot+="</TABLE>>];\n";
                                                anterior=apuntadoresB2.b_pointers[c];



                                                for(int b=0;b<16;b++){
                                                    if(apuntadoresB1.b_pointers[b]=!-1){
                                                        fseek(archivo, super.s_block_start+apuntadoresB1.b_pointers[b]*64, SEEK_SET);
                                                        fread(&apuntadoresB, 64, 1, archivo);
                                                        
                                                        dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB1.b_pointers[c])+"\n";
                                                        dot+="Bloque"+to_string(apuntadoresB1.b_pointers[b])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                        dot+="<TR><TD>BLOQUE APUNTADORES "+to_string(apuntadoresB1.b_pointers[c])+"</TD></TR>\n";
                                                        string contenido="";
                                                        for(int x=0;x<15;x++){
                                                            contenido+=to_string(apuntadoresB.b_pointers[x])+", ";
                                                        }
                                                        contenido+=to_string(apuntadoresB.b_pointers[15]);
                                                        dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                        dot+="</TABLE>>];\n";
                                                        anterior=apuntadoresB1.b_pointers[c];
                                                        


                                                        for(int a=0;a<16;a++){
                                                            if(apuntadoresB.b_pointers[a]=!-1){
                                                                if(tablaInodo[i].i_type==0){
                                                                    fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                                    fread(&carpetaB, 64, 1, archivo);
                                                                    
                                                                    dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a])+"\n";
                                                                    dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                                    dot+="<TR><TD>BLOQUE CARPETA</TD><TD>"+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                                    dot+="<TR><TD>b_name</TD><TD>b_inodo</TD></TR>\n";
                                                                    for(int x=0;x<4;x++){
                                                                        if(carpetaB.b_content[x].b_inodo>-1){
                                                                            string name_B_C=carpetaB.b_content[x].b_name;
                                                                            dot+="<TR><TD>"+name_B_C+"</TD><TD>"+to_string(carpetaB.b_content[x].b_inodo)+"</TD></TR>\n";
                                                                        }
                                                                    }
                                                                    dot+="</TABLE>>];\n";
                                                                    
                                                                    anterior=apuntadoresB.b_pointers[a];
                                                                    continue;
                                                                }
                                                                fseek(archivo, super.s_block_start+apuntadoresB.b_pointers[a]*64, SEEK_SET);
                                                                fread(&archivoB, 64, 1, archivo);
                                                                dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(apuntadoresB.b_pointers[a])+"\n";
                                                                dot+="Bloque"+to_string(apuntadoresB.b_pointers[a])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                                                dot+="<TR><TD>BLOQUE ARCHIVO "+to_string(apuntadoresB.b_pointers[a])+"</TD></TR>\n";
                                                                string contenido=archivoB.b_content;
                                                                dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                                                dot+="</TABLE>>];\n";
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
                                    if(tablaInodo[i].i_type=='0'){
                                        fseek(archivo, super.s_block_start+tablaInodo[i].i_block[j]*64, SEEK_SET);
                                        fread(&carpetaB, 64, 1, archivo);
                                        if(anterior>0){
                                            dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(tablaInodo[i].i_block[j])+"\n";
                                        }
                                        dot+="Bloque"+to_string(tablaInodo[i].i_block[j])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                        dot+="<TR><TD>BLOQUE CARPETA</TD><TD>"+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                                        dot+="<TR><TD>b_name</TD><TD>b_inodo</TD></TR>\n";
                                        for(int x=0;x<4;x++){
                                            if(carpetaB.b_content[x].b_inodo>-1){
                                                string name_B_C=carpetaB.b_content[x].b_name;
                                                dot+="<TR><TD>"+name_B_C+"</TD><TD>"+to_string(carpetaB.b_content[x].b_inodo)+"</TD></TR>\n";
                                            }
                                        }
                                        dot+="</TABLE>>];\n";
                                        
                                        anterior=tablaInodo[i].i_block[j];
                                        continue;
                                    }
                                    fseek(archivo, super.s_block_start+tablaInodo[i].i_block[j]*64, SEEK_SET);
                                    fread(&archivoB, 64, 1, archivo);
                                    dot+="Bloque"+to_string(anterior)+"->Bloque"+to_string(tablaInodo[i].i_block[j])+"\n";
                                    dot+="Bloque"+to_string(tablaInodo[i].i_block[j])+"[label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n ";
                                    dot+="<TR><TD>BLOQUE ARCHIVO "+to_string(tablaInodo[i].i_block[j])+"</TD></TR>\n";
                                    string contenido=archivoB.b_content;
                                    dot+="<TR><TD>"+contenido+"</TD></TR>\n";
                                    dot+="</TABLE>>];\n";
                                    anterior=tablaInodo[i].i_block[j];
                                }
                                
                            }
                            
                        }
                        
                    }
                }
                dot+="}";
                //cout<<dot<<endl;
                escribirDOT(dot,name_p,rutaCompleta);
                fclose(archivo);
                //escribirDOT(dot,name_p,rutaCompleta);
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
                    cout << "REPORTE CREADO CON EXITO" << endl;
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
                //cout<<"cantidad "<<inodoCount<<endl;
                //cout<<contenido<<endl;

                archivo = fopen(path.c_str(), "w+");
                if (archivo){
                    fwrite(contenido.c_str(), 1, contenido.size(), archivo);
                    cout << "REPORTE CREADO CON EXITO" << endl;
                }
                fclose(archivo);

            }else if(name=="tree"){
                fseek(archivo, inicio, SEEK_SET);
                fread(&sb, sizeof (superBloque), 1, archivo);
                inodo actual;
                dotTree="digraph g { label=\"REPORTE TREE\"\ngraph [\nrankdir = \"LR\"\n]";
                dotTree+="node [\nfontsize = \"16\"\nshape = \"ellipse\"\n]";
                dotTree+="edge [];\n";

                fseek(archivo, sb.s_inode_start, SEEK_SET);
                fread(&actual, sizeof (inodo), 1, archivo);

                dotTree+="\"node"+to_string(0)+"\" [ shape=\"record\"\n color=\"blue\" label = \"";
                dotTree+="<f20> INODO "+to_string(0)+" |{i_type|"+actual.i_type+"}";
                
                for(int j=0;j<16;j++){
                    if(actual.i_block[j]>-1){
                        dotTree+="| {i_block_"+to_string(j)+"|<f"+to_string(j)+">"+to_string(actual.i_block[j])+"}";
                    }
                }
                dotTree+="| I_perm "+to_string(actual.i_perm)+"\" \n];\n";
                
                for(int j=0;j<15;j++){
                    if(actual.i_block[j]>-1){
                        dotTree+="\"node"+to_string(0)+"\":<f"+to_string(j)+"> -> \"block"+to_string(actual.i_block[j])+"\":<f20>;\n";
                    }
                }
                TreeCascada(sb.s_inode_start,archivo,0);
                dotTree+="}";
                cout<<dotTree<<endl;
                escribirDOT(dotTree,name_p,rutaCompleta);
            }else if(name=="sb"){
                fseek(archivo, inicio, SEEK_SET);
                fread(&sb, sizeof (superBloque), 1, archivo);
                fclose(archivo);

                string dot;

                dot+="digraph D{";
                dot+="label=\"REPORTE SUPER BLOQUE\"";
                dot+="nodo_mbr[shape=platntext label=<<TABLE color=\"black\" cellspacing=\"0\" cellpadding=\"10\">\n";
                dot+="<TR><TD align=\"left\" color=\"white\" bgcolor=\"darkslateblue\">REPORTE SUPER BLOQUE</TD><TD color=\"white\" bgcolor=\"darkslateblue\"></TD></TR>\n";
                dot+="<TR><TD color=\"white\">NOMBRE</TD ><TD color=\"white\">";
                string aux=particion.mnt_particion.part_name;
                dot+=aux;
                dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">sb_inodes_count</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                dot+=to_string(sb.s_inodes_count);
                dot+="</TD></TR>\n<TR><TD color=\"white\">sb_block_count</TD><TD color=\"white\">";
                dot+=to_string(sb.s_blocks_count);

                dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">sb_inodes_free</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                dot+=to_string(sb.s_free_inodes_count);
                dot+="</TD></TR>\n<TR><TD color=\"white\">sb_block_free</TD><TD color=\"white\">";
                dot+=to_string(sb.s_free_blocks_count);

                dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">sb_ultimo_montaje</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                dot+=sb.s_mtime;
                dot+="</TD></TR>\n<TR><TD color=\"white\">sb_numero_montajes</TD><TD color=\"white\">";
                dot+=to_string(sb.s_mnt_count);

                dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">sb_inicio</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                dot+=to_string(sizeof(sb));
                dot+="</TD></TR>\n<TR><TD color=\"white\">sb_inicio_bm_inode</TD><TD color=\"white\">";
                dot+=to_string(sb.s_bm_inode_start);

                dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">sb_nicio_bm_block</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                dot+=to_string(sb.s_bm_block_start);
                dot+="</TD></TR>\n<TR><TD color=\"white\">sb_inicio_inodes</TD><TD color=\"white\">";
                dot+=to_string(sb.s_inode_start);

                dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">sb_inicio_blocks</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                dot+=to_string(sb.s_bm_block_start);
                dot+="</TD></TR>\n<TR><TD color=\"white\">sb_size_inodo</TD><TD color=\"white\">";
                dot+=to_string(sizeof(inodo));

                dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">sb_size_block</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                dot+=to_string(64);
                dot+="</TD></TR>\n<TR><TD color=\"white\">sb_first_free</TD><TD color=\"white\">";
                dot+=to_string(sb.s_firts_ino);

                dot+="</TD></TR>\n<TR><TD bgcolor=\"lightslateblue\" color=\"white\">sb_first_block</TD><TD bgcolor=\"lightslateblue\" color=\"white\">";
                dot+=to_string(sb.s_first_blo);
                dot+="</TD></TR>\n<TR><TD color=\"white\">magic_num</TD><TD color=\"white\">";
                dot+=to_string(sb.s_magic);


                dot+="</TD></TR>\n";
                dot+="</TABLE>>];";
                dot+="}";
                escribirDOT(dot,name_p,rutaCompleta);
            }else if(name=="file"){
                fseek(archivo, inicio, SEEK_SET);
                fread(&sb, sizeof (superBloque), 1, archivo);
                size_t posC;
                string regTemp,delimiter="/";
                inodo InodeUsers;

                while ((posC = rutai.find(delimiter)) != string::npos) {
                    rutai.erase(0, posC + delimiter.length());
                }

                //cout<<"name:"<<rutai<<endl;
                
                int posicionInode=BuscarInodo(rutai,sb.s_inode_start,archivo);
                //cout<<"pos:"<<posicionInode<<endl;
                if(posicionInode<0){
                    cout<<"NO EXISTE PATH ESPECIFICADO"<<endl;
                    return;
                }
                fseek(archivo, posicionInode*sizeof(inodo)+sb.s_inode_start, SEEK_SET);
                fread(&InodeUsers, sizeof (inodo), 1, archivo);

                string contenido=rutai+"\n";
                contenido+=leerArchivo(InodeUsers,archivo);

                //cout<<"INICIO: "<<contenido<<endl;
                archivo = fopen(path.c_str(), "w+");
                if (archivo){
                    fwrite(contenido.c_str(), 1, contenido.size(), archivo);
                    cout << "REPORTE CREADO CON EXITO" << endl;
                }

                fclose(archivo);
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


void Comando::mkfile(string path,string r,string s,string cont){
    
    string regTemp;
    string delimiter="/";
    size_t posC;
    FILE* file=fopen(pathGlobal.c_str(),"r+b");
    inodo InodePrueba;
    
    vector <string> Reg;
    int encontrado,anterior=0,size=0;

    
    inodo Iencontrado;
    bloqueCarpetas carpetaActual;
    bloqueApuntadores apuntadorActual;
    bool creado=false;
    //separamos el contenido dentro de ruta separandolo por /
    while ((posC = path.find(delimiter)) != string::npos) {
        //cout<<"pos encontrada: "<<posC<<endl;
        regTemp=(path.substr(0, posC));
        path.erase(0, posC + delimiter.length());
        
        if(posC==0){
            posC=-1;
            posC = path.find(delimiter);
            //cout<<"pruebas de pos: "<<posC<<endl;
            continue;
        }
        
        encontrado=BuscarInodo(regTemp,superLog.s_inode_start,file);
        
        if(encontrado==-1){
            if(r=="simon"){
                

                fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                fread(&Iencontrado, sizeof (inodo), 1, file);

                
                for(int i=0;i<16;i++){
                    if(i<13){
                        if(Iencontrado.i_block[i]>-1){

                            fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                            fread(&carpetaActual, 64, 1, file);

                            for(int a=0;a<5;a++){
                                if(carpetaActual.b_content[a].b_inodo==-1){
                                    creado=true;
                                    carpetaActual.b_content[a].b_inodo=crearInodo(48,regTemp,0,file);
                                    //actualizar_bmInodo(carpetaActual.b_content[a].b_inodo);
                                    
                                    anterior=carpetaActual.b_content[a].b_inodo;

                                    strcpy(carpetaActual.b_content[a].b_name,regTemp.c_str());
                                    fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                                    fwrite(&carpetaActual, 64, 1, file);
                                    break;
                                }
                            }

                            if(creado)break;
                        }else{
                            Iencontrado.i_block[i]=crearBloque(2,"",file);
                            //actualizar_bmBlock(Iencontrado.i_block[i]);

                            fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                            fread(&carpetaActual, 64, 1, file);

                            creado=true;
                            carpetaActual.b_content[0].b_inodo=crearInodo(48,regTemp,0,file);
                            //actualizar_bmInodo(carpetaActual.b_content[0].b_inodo);

                            strcpy(carpetaActual.b_content[0].b_name,regTemp.c_str());
                            
                            fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                            fwrite(&carpetaActual, 64, 1, file);
                            
                            cout<<""<<anterior<<endl;
                            fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                            fwrite(&Iencontrado, sizeof(inodo), 1, file);

                            anterior=carpetaActual.b_content[0].b_inodo;
                            break;
                        }
                    }if(i==13){
                        if(Iencontrado.i_block[i]>-1){
                            fseek(file,Iencontrado.i_block[i]*sizeof(inodo)+superLog.s_block_start,SEEK_SET);
                            fread(&apuntadorActual, 64, 1, file);
                            for(int j=0;j<16;j++){
                                if(apuntadorActual.b_pointers[j]>-1){
                                    fseek(file,apuntadorActual.b_pointers[j]*64+superLog.s_block_start,SEEK_SET);
                                    fread(&carpetaActual, 64, 1, file);
                                    for(int a=0;a<5;a++){
                                        if(carpetaActual.b_content[a].b_inodo==-1){
                                            creado=true;
                                            carpetaActual.b_content[a].b_inodo=crearInodo(48,regTemp,0,file);
                                            //actualizar_bmInodo(carpetaActual.b_content[a].b_inodo);
                                            anterior=carpetaActual.b_content[a].b_inodo;
                                            strcpy(carpetaActual.b_content[a].b_name,regTemp.c_str());
                                            fseek(file,apuntadorActual.b_pointers[j]*64+superLog.s_block_start,SEEK_SET);
                                            fwrite(&carpetaActual, 64, 1, file);
                                            break;
                                        }
                                    }

                                    if(creado)break;
                                }else{
                                    Iencontrado.i_block[i]=crearBloque(2,"",file);
                                    //actualizar_bmBlock(Iencontrado.i_block[i]);

                                    fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                                    fread(&carpetaActual, 64, 1, file);

                                    creado=true;
                                    carpetaActual.b_content[0].b_inodo=crearInodo(48,regTemp,0,file);
                                    //actualizar_bmInod(carpetaActual.b_content[0].b_inodo);

                                    strcpy(carpetaActual.b_content[0].b_name,regTemp.c_str());
                                    
                                    fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                                    fwrite(&carpetaActual, 64, 1, file);
                                    
                                    fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                                    fwrite(&Iencontrado, sizeof(inodo), 1, file);

                                    anterior=carpetaActual.b_content[0].b_inodo;
                                    break;
                                }
                            }

                        }else{

                            Iencontrado.i_block[i]=crearBloque(1,"",file);
                            //actualizar_bmBlock(Iencontrado.i_block[i]);

                            fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                            fread(&apuntadorActual, 64, 1, file);

                            apuntadorActual.b_pointers[0]=crearBloque(2,"",file);
                            //actualizar_bmBlock(apuntadorActual.b_pointers[0]);
                            


                            fseek(file,apuntadorActual.b_pointers[0]*64+superLog.s_block_start,SEEK_SET);
                            fread(&carpetaActual, 64, 1, file);

                            creado=true;
                            carpetaActual.b_content[0].b_inodo=crearInodo(48,regTemp,0,file);
                            //actualizar_bmInodo(carpetaActual.b_content[0].b_inodo);

                            strcpy(carpetaActual.b_content[0].b_name,regTemp.c_str());
                                    
                            fseek(file,apuntadorActual.b_pointers[0]*64+superLog.s_block_start,SEEK_SET);
                            fwrite(&carpetaActual, 64, 1, file);

                            fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                            fwrite(&apuntadorActual, 64, 1, file);
                                    
                            fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                            fwrite(&Iencontrado, sizeof(inodo), 1, file);

                            anterior=carpetaActual.b_content[0].b_inodo;
                            break;

                        }
                    }
                }
                
            }else{
                cout<<"ERROR: RUTA ESPEFICADA NO ENCONTRADA, PARAMETRO <r> NO ENCONTRADO"<<endl;
                return;
            }
        }else{
            anterior=encontrado;
            fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
            fread(&Iencontrado, sizeof (inodo), 1, file);

            if(!VerificarPermisoEscribir(Iencontrado)){
                cout<<"ERROR: NO POSEE LOS PERMISOS PARA LA OPERACION (ESCRIBIR CARPETA)"<<endl;
                return;
            }
        }
    }

    
    string linea = "",total="";
    fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
    fread(&Iencontrado, sizeof (inodo), 1, file);
    
    if(cont!=" "){
        ifstream archivo(cont);
        // Obtener línea de archivo, y almacenar contenido en "linea"
        while (getline(archivo, linea))
        {
            total+=linea+"\n";
        }

    }else if(s!=" "){
        size=atoi(s.c_str());
        int secuencia=0;
        for(int i=0;i<size;i++){
            total+=to_string(secuencia);
            secuencia++;
            if(secuencia==10)secuencia=0;
        }
    }

    creado=false;
    //cout<<"anterior: "<<anterior<<endl;
    for(int i=0;i<14;i++){
        if(i<13){
            if(Iencontrado.i_block[i]>-1){
                fseek(file,Iencontrado.i_block[i]*sizeof(inodo)+superLog.s_block_start,SEEK_SET);
                fread(&carpetaActual, 64, 1, file);
                for(int a=0;a<5;a++){
                    //cout<<"pos de carpeta: "<<a<<endl;;
                    if(carpetaActual.b_content[a].b_inodo==-1){
                        creado=true;
                        carpetaActual.b_content[a].b_inodo=crearInodo(49,regTemp,0,file);
                        //cout<<"Numero de inodo creado: "<<carpetaActual.b_content[a].b_inodo<<endl;;
                        //actualizar_bmInodo(carpetaActual.b_content[a].b_inodo);
                        anterior=carpetaActual.b_content[a].b_inodo;
                        strcpy(carpetaActual.b_content[a].b_name,path.c_str());
                        //cout<<"nombre: "<<path<<endl;
                        fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                        fwrite(&carpetaActual, 64, 1, file);
                        break;
                    }
                }
                //cout<<"creado?"<<endl;
                if(creado)break;
            }else{
                Iencontrado.i_block[i]=crearBloque(2,"",file);
                //actualizar_bmBlock(Iencontrado.i_block[i]);

                fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                fread(&carpetaActual, 64, 1, file);

                creado=true;
                carpetaActual.b_content[0].b_inodo=crearInodo(49,regTemp,0,file);
                //actualizar_bmInodo(carpetaActual.b_content[0].b_inodo);

                strcpy(carpetaActual.b_content[0].b_name,path.c_str());
                            
                fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                fwrite(&carpetaActual, 64, 1, file);


                fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                fwrite(&Iencontrado, sizeof(inodo), 1, file);
                fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                fread(&Iencontrado, sizeof (inodo), 1, file);
                cout<<"asigno la carpeta?: "<<Iencontrado.i_block[0]<<endl;

                anterior=carpetaActual.b_content[0].b_inodo;
                break;
            }
        }if(i==13){
            if(Iencontrado.i_block[i]>-1){
                fseek(file,Iencontrado.i_block[i]*sizeof(inodo)+superLog.s_block_start,SEEK_SET);
                fread(&apuntadorActual, 64, 1, file);
                for(int j=0;j<16;j++){
                    if(apuntadorActual.b_pointers[j]>-1){
                        fseek(file,apuntadorActual.b_pointers[j]*64+superLog.s_block_start,SEEK_SET);
                        fread(&carpetaActual, 64, 1, file);
                        for(int a=0;a<5;a++){
                            if(carpetaActual.b_content[a].b_inodo==-1){
                                creado=true;
                                carpetaActual.b_content[a].b_inodo=crearInodo(49,regTemp,0,file);
                                //actualizar_bmInodo(carpetaActual.b_content[a].b_inodo);
                                anterior=carpetaActual.b_content[a].b_inodo;
                                strcpy(carpetaActual.b_content[a].b_name,path.c_str());
                                fseek(file,apuntadorActual.b_pointers[j]*64+superLog.s_block_start,SEEK_SET);
                                fwrite(&carpetaActual, 64, 1, file);
                                break;
                            }
                        }

                        if(creado)break;
                    }else{
                        Iencontrado.i_block[i]=crearBloque(2,"",file);
                        //actualizar_bmBlock(Iencontrado.i_block[i]);

                        fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                        fread(&carpetaActual, 64, 1, file);

                        creado=true;
                        carpetaActual.b_content[0].b_inodo=crearInodo(49,regTemp,0,file);
                        //actualizar_bmInodo(carpetaActual.b_content[0].b_inodo);

                        strcpy(carpetaActual.b_content[0].b_name,path.c_str());
                                    
                        fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                        fwrite(&carpetaActual, 64, 1, file);
                                    
                        fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                        fwrite(&Iencontrado, sizeof(inodo), 1, file);

                        anterior=carpetaActual.b_content[0].b_inodo;
                        break;
                    }
                }

            }else{

                Iencontrado.i_block[i]=crearBloque(1,"",file);
                //actualizar_bmBlock(Iencontrado.i_block[i]);

                fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                fread(&apuntadorActual, 64, 1, file);

                apuntadorActual.b_pointers[0]=crearBloque(2,"",file);
                //actualizar_bmBlock(apuntadorActual.b_pointers[0]);
                            


                fseek(file,apuntadorActual.b_pointers[0]*64+superLog.s_block_start,SEEK_SET);
                fread(&carpetaActual, 64, 1, file);

                creado=true;
                carpetaActual.b_content[0].b_inodo=crearInodo(49,regTemp,0,file);
                //actualizar_bmInodo(carpetaActual.b_content[0].b_inodo);

                strcpy(carpetaActual.b_content[0].b_name,path.c_str());
                                    
                fseek(file,apuntadorActual.b_pointers[0]*64+superLog.s_block_start,SEEK_SET);
                fwrite(&carpetaActual, 64, 1, file);

                fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                fwrite(&apuntadorActual, 64, 1, file);
                                
                fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                fwrite(&Iencontrado, sizeof(inodo), 1, file);

                anterior=carpetaActual.b_content[0].b_inodo;
                break;

            }
        }
    }
    fclose(file);
    cout<<"ARCHIVO CREADO CORRECTAMENTE"<<endl;
    
    if(total.size()>0){
        string totalParcial="";
        while(total.size()>64){
            
            totalParcial=(total.substr(0, 63));
            //cout<<"este es el total parcial: "<<totalParcial<<endl;
            //cout<<totalParcial.length()<<endl;
            total.erase(0, 63);
            escribirFile(anterior*sizeof(inodo)+superLog.s_inode_start,Iencontrado,totalParcial);
        }
        //cout<<total<<" escrito en "<<anterior<<endl;
        //cout<<total<<endl;
        escribirFile(anterior*sizeof(inodo)+superLog.s_inode_start,Iencontrado,total);
    }
}

void Comando::chmod(string path,string ugo,string r){
    int p1=atoi(ugo.c_str());
    int permisoUser=p1/100,permisoGroup=(p1%100)/10,permisoOther=p1%10;
    //cout<<permisoUser<<permisoGroup<<permisoOther<<endl;
    if(permisoUser<0||permisoUser>7||permisoGroup<0||permisoGroup>7||permisoOther<0||permisoOther>7){
        cout<<"ERROR: PARAMETRO UGO NO VALIDO DEBE IR PARAMETROS 0-7"<<endl;
        return;
    }
    
    string delimiter="/";
    size_t posC;
    FILE* file=fopen(pathGlobal.c_str(),"r+b");
    
    
    int buscado=0;
    string regTemp="";
    inodo Iencontrado;
    bloqueCarpetas carpetaActual;
    bloqueApuntadores apuntadorActual;
    bool creado=false;
    //separamos el contenido dentro de ruta separandolo por /
    while ((posC = path.find(delimiter)) != string::npos) {
        
        regTemp=(path.substr(0, posC));
        path.erase(0, posC + delimiter.length());
        if(posC==0){
            continue;
        }
        //cout<<regTemp<<endl;
        buscado=BuscarInodo(regTemp,buscado*sizeof(inodo)+superLog.s_inode_start,file);
        if(buscado<0){
            cout<<"ERROR: EL PATH ESPECIFICADO NO EXISTE!"<<endl;
            return;
        }
    }
    //cout<<"nombre archivo: "<<path<<endl;
    //cout<<"empieza buscar en: "<<buscado<<endl;
    buscado=BuscarInodo(path,buscado*sizeof(inodo)+superLog.s_inode_start,file);
    if(buscado<0){
        cout<<"ERROR: EL PATH ESPECIFICADO NO EXISTE!"<<endl;
        return;
    }
    
    fseek(file,buscado*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
    fread(&Iencontrado, sizeof(inodo), 1, file);
    if(strcmp(r.c_str(),"simon")!=0||Iencontrado.i_type=='1'){
        Iencontrado.i_perm=atoi(ugo.c_str());
        fseek(file,buscado*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
        fwrite(&Iencontrado, sizeof(inodo), 1, file);
        fseek(file,buscado*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
        fread(&Iencontrado, sizeof(inodo), 1, file);
        //cout<<"nuevo permiso: "<<Iencontrado.i_perm<<endl;
        cout<<"PERMISO ACTUALIZADO CORRECTAMENTE"<<endl;
        fclose(file);
        return;
    }


    ActualizarPermCascada(buscado*sizeof(inodo)+superLog.s_inode_start,file,atoi(ugo.c_str()));

    fclose(file);
}


void ActualizarPermCascada(int pos,FILE* archivo,int permiso){
    
    inodo InodoActual;
    fseek(archivo,pos, SEEK_SET);
    fread(&InodoActual, sizeof(inodo), 1, archivo);
    times fech;
    fechActual(fech);
    strcpy(InodoActual.i_atime,fech);
    
    fseek(archivo,pos, SEEK_SET);
    fwrite(&InodoActual, sizeof(inodo), 1, archivo);

    fseek(archivo,pos, SEEK_SET);
    fread(&InodoActual, sizeof(inodo), 1, archivo);
    
    if(InodoActual.i_type=='0'){
        for(int i=0;i<16;i++){
            //cout<<"PASANDO POR INODO POS: "<<i<<endl;
            if(InodoActual.i_block[i]>-1){
                //cout<<"PASE EL IF EN "<<i<<endl;
                if(i<13){
                    
                    PermisoCarpeta(InodoActual.i_block[i]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,permiso);
                    
                
                }if(i==13){
                    bloqueApuntadores apuntadores;
                    fseek(archivo,InodoActual.i_block[13]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            PermisoCarpeta(apuntadores.b_pointers[j]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,permiso);
                        }
                    } 
                }if(i==14){
                    bloqueApuntadores apuntadores,apuntadores2;
                    fseek(archivo,InodoActual.i_block[14]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            fseek(archivo,apuntadores.b_pointers[j]*64+superLog.s_block_start, SEEK_SET);
                            fread(&apuntadores2, 64, 1, archivo);
                            for(int k=0;k<16;k++){
                                if(apuntadores2.b_pointers[k]!=-1){
                                    PermisoCarpeta(apuntadores2.b_pointers[k]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,permiso);
                                    
                                }
                            }
                        }
                    }
                }if(i==15){
                    bloqueApuntadores apuntadores,apuntadores2,apuntadores3;
                    fseek(archivo,InodoActual.i_block[15]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            fseek(archivo,apuntadores.b_pointers[j]*64+superLog.s_block_start, SEEK_SET);
                            fread(&apuntadores2, 64, 1, archivo);
                            for(int k=0;k<16;k++){
                                if(apuntadores2.b_pointers[k]!=-1){
                                    fseek(archivo,apuntadores2.b_pointers[k]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                                    fread(&apuntadores3, sizeof(bloqueApuntadores), 1, archivo);
                                    for(int l=0;l<16;l++){
                                        if(apuntadores3.b_pointers[l]!=-1){
                                            PermisoCarpeta(apuntadores3.b_pointers[l]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,permiso);
                                            
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}



 void PermisoCarpeta(int pos,FILE* archivo,int permiso){
    bloqueCarpetas bloque;
    fseek(archivo,pos, SEEK_SET);
    fread(&bloque, sizeof(bloqueCarpetas), 1, archivo);

    for(int i=0;i<4;i++){
        if(bloque.b_content[i].b_inodo>-1){
            inodo actual;
            fseek(archivo,bloque.b_content[i].b_inodo*sizeof(inodo)+superLog.s_inode_start, SEEK_SET);
            fread(&actual, sizeof(inodo), 1, archivo);
            actual.i_perm=permiso;
            fseek(archivo,bloque.b_content[i].b_inodo*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
            fwrite(&actual, sizeof(inodo), 1, archivo);
            cout<<"PERMISO ACTUALIZADO DE "<<bloque.b_content[i].b_name<<endl;
            
            if(actual.i_type=='0'){
                ActualizarPermCascada(bloque.b_content[i].b_inodo*sizeof(inodo)+superLog.s_inode_start,archivo,permiso);                
            }
        }
    }
}

void Comando::remove(string pathBuscar){
    string delimiter="/";
    size_t posC;
    FILE* file=fopen(pathGlobal.c_str(),"r+b");
    
    int buscado=0;
    string regTemp="";
    inodo Iencontrado;
    bloqueCarpetas carpetaActual;
    bloqueApuntadores apuntadorActual;
    bool creado=false;
    //separamos el contenido dentro de ruta separandolo por /
    while ((posC = pathBuscar.find(delimiter)) != string::npos) {
        
        regTemp=(pathBuscar.substr(0, posC));
        pathBuscar.erase(0, posC + delimiter.length());
        if(posC==0){
            continue;
        }
        //cout<<regTemp<<endl;
        buscado=BuscarInodo(regTemp,buscado*sizeof(inodo)+superLog.s_inode_start,file);
        if(buscado<0){
            cout<<"ERROR: EL PATH ESPECIFICADO NO EXISTE!"<<endl;
            return;
        }
    }
    //cout<<"nombre archivo: "<<path<<endl;
    //cout<<"empieza buscar en: "<<buscado<<endl;
    buscado=BuscarInodo(pathBuscar,buscado*sizeof(inodo)+superLog.s_inode_start,file);
    if(buscado<0){
        cout<<"ERROR: EL PATH ESPECIFICADO NO EXISTE!"<<endl;
        return;
    }

    vector <int> listaInode;
    fseek(file,buscado*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
    fread(&Iencontrado, sizeof(inodo), 1, file);
    
    if(Iencontrado.i_type=='1'){
        //Iencontrado.i_perm=atoi(ugo.c_str());
        if(!VerificarPermisoEscribir(Iencontrado)){
            cout<<"ERROR: NO POSEE PERMISOS SUFICIENTES (ESCRITURA) PARA ELIMINAR"<<endl;
            return;
        }

        inodo lista[superLog.s_inodes_count];    

        fseek(file,superLog.s_inode_start, SEEK_SET);
        fread(&lista, superLog.s_inodes_count*sizeof(inodo), 1, file);
        fclose(file);

        lista[buscado].i_gid=grupoLogeado;
        lista[buscado].i_uid=logeado.Id;


        lista[buscado].i_type='2';
        //cout<<"tipo que llega: "<<lista[contador].i_type<<endl;
        lista[buscado].i_size=0;
        
        strcpy(lista[buscado].i_atime,"");
        strcpy(lista[buscado].i_mtime,"");
        strcpy(lista[buscado].i_ctime,"");
        lista[buscado].i_uid=0;
        lista[buscado].i_gid=0;
        
        for(int i=0;i<16;i++){
            if(lista[buscado].i_block[i]>-1){
                actualizar_bmBlock(lista[buscado].i_block[i]);
            }
            lista[buscado].i_block[i]=-1;
        }
        

        EscribirNodo(pathGlobal,superLog.s_inode_start,superLog.s_inodes_count,lista);
        //cout<<"nuevo permiso: "<<Iencontrado.i_perm<<endl;
        cout<<"ARCHIVO ELIMINADO "<<pathBuscar<<" CORRECTAMENTE"<<endl;
        //fclose(file);
        return;
    }


    RemoveCascada(buscado*sizeof(inodo)+superLog.s_inode_start,file,listaInode);
    if(listaInode[listaInode.size()-1]==-1){
        cout<<"ERROR: NO POSEE PERMISOS SUFICIENTES (ESCRITURA) PARA REMOVE"<<endl;
        return;
    }
    inodo lista[superLog.s_inodes_count];    

    fseek(file,superLog.s_inode_start, SEEK_SET);
    fread(&lista, superLog.s_inodes_count*sizeof(inodo), 1, file);
    fclose(file);
    for(int i=0;i<listaInode.size();i++){
        lista[listaInode[i]].i_gid=grupoLogeado;
        lista[listaInode[i]].i_uid=logeado.Id;


        lista[listaInode[i]].i_type='2';
        //cout<<"tipo que llega: "<<lista[contador].i_type<<endl;
        lista[listaInode[i]].i_size=0;
        
        strcpy(lista[listaInode[i]].i_atime,"");
        strcpy(lista[listaInode[i]].i_mtime,"");
        strcpy(lista[listaInode[i]].i_ctime,"");
        lista[listaInode[i]].i_uid=0;
        lista[listaInode[i]].i_gid=0;
        
        for(int j=0;j<16;j++){
            if(lista[listaInode[i]].i_block[j]>-1){
                actualizar_bmBlock(lista[listaInode[i]].i_block[i]);
            }
            lista[listaInode[i]].i_block[i]=-1;
        }
    }
    fclose(file);

}

void RemoveCascada(int pos,FILE* archivo,vector <int> listaInode){
    
    inodo InodoActual;
    fseek(archivo,pos, SEEK_SET);
    fread(&InodoActual, sizeof(inodo), 1, archivo);
    times fech;
    fechActual(fech);
    strcpy(InodoActual.i_atime,fech);
    
    fseek(archivo,pos, SEEK_SET);
    fwrite(&InodoActual, sizeof(inodo), 1, archivo);

    fseek(archivo,pos, SEEK_SET);
    fread(&InodoActual, sizeof(inodo), 1, archivo);
    
    if(InodoActual.i_type=='0'){
        for(int i=0;i<16;i++){
            //cout<<"PASANDO POR INODO POS: "<<i<<endl;
            if(InodoActual.i_block[i]>-1){
                //cout<<"PASE EL IF EN "<<i<<endl;
                if(i<13){
                    
                    RemoveCarpeta(InodoActual.i_block[i]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,listaInode);
                    
                
                }if(i==13){
                    bloqueApuntadores apuntadores;
                    fseek(archivo,InodoActual.i_block[13]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            RemoveCarpeta(apuntadores.b_pointers[j]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,listaInode);
                            if(listaInode[listaInode.size()-1]==-1){
                                cout<<"ERROR: NO POSEE PERMISOS SUFICIENTES (ESCRITURA) PARA REMOVE"<<endl;
                                return;
                            }
                        }
                    } 
                }if(i==14){
                    bloqueApuntadores apuntadores,apuntadores2;
                    fseek(archivo,InodoActual.i_block[14]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            fseek(archivo,apuntadores.b_pointers[j]*64+superLog.s_block_start, SEEK_SET);
                            fread(&apuntadores2, 64, 1, archivo);
                            for(int k=0;k<16;k++){
                                if(apuntadores2.b_pointers[k]!=-1){
                                    RemoveCarpeta(apuntadores2.b_pointers[k]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,listaInode);
                                    if(listaInode[listaInode.size()-1]==-1){
                                        cout<<"ERROR: NO POSEE PERMISOS SUFICIENTES (ESCRITURA) PARA REMOVE"<<endl;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }if(i==15){
                    bloqueApuntadores apuntadores,apuntadores2,apuntadores3;
                    fseek(archivo,InodoActual.i_block[15]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            fseek(archivo,apuntadores.b_pointers[j]*64+superLog.s_block_start, SEEK_SET);
                            fread(&apuntadores2, 64, 1, archivo);
                            for(int k=0;k<16;k++){
                                if(apuntadores2.b_pointers[k]!=-1){
                                    fseek(archivo,apuntadores2.b_pointers[k]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                                    fread(&apuntadores3, sizeof(bloqueApuntadores), 1, archivo);
                                    for(int l=0;l<16;l++){
                                        if(apuntadores3.b_pointers[l]!=-1){
                                            RemoveCarpeta(apuntadores3.b_pointers[l]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,listaInode);
                                            if(listaInode[listaInode.size()-1]==-1){
                                                cout<<"ERROR: NO POSEE PERMISOS SUFICIENTES (ESCRITURA) PARA REMOVE"<<endl;
                                                return;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}



 void RemoveCarpeta(int pos,FILE* archivo,vector <int> listaInode){
    bloqueCarpetas bloque;
    fseek(archivo,pos, SEEK_SET);
    fread(&bloque, sizeof(bloqueCarpetas), 1, archivo);

    for(int i=0;i<4;i++){
        if(bloque.b_content[i].b_inodo>-1){
            inodo actual;
            fseek(archivo,bloque.b_content[i].b_inodo*sizeof(inodo)+superLog.s_inode_start, SEEK_SET);
            fread(&actual, sizeof(inodo), 1, archivo);
            if(!VerificarPermisoEscribir(actual)){
                listaInode.push_back(-1);
                return;
            }
            
            listaInode.push_back(bloque.b_content[i].b_inodo);
            
            if(actual.i_type=='0'){
                RemoveCascada(bloque.b_content[i].b_inodo*sizeof(inodo)+superLog.s_inode_start,archivo,listaInode);                
                if(listaInode[listaInode.size()-1]==-1){
                    cout<<"ERROR: NO POSEE PERMISOS SUFICIENTES (ESCRITURA) PARA REMOVE"<<endl;
                    return;
                }
            }
        }
    }
}

void escribirDOT(string dot,string terminacion,string ruta){
    ofstream myfile;
    myfile.open ("reporte.dot");
    myfile << dot;
    myfile.close();

    char comando[1024];
    strcpy(comando,"dot");
    strcat(comando," \'");
    strcat(comando,"reporte.dot");
    strcat(comando,"\' -T");
    strcat(comando,terminacion.c_str());
    strcat(comando," -o \'");
    strcat(comando,ruta.c_str());
    strcat(comando,"\'");

    system(comando);
    
    cout << "REPORTE CREADO CON EXITO" << endl;

}

void Comando::mkdir(string path,string padres){
    string regTemp;
    string delimiter="/";
    size_t posC;
    FILE* file=fopen(pathGlobal.c_str(),"r+b");
    inodo InodePrueba;
    
    vector <string> Reg;
    int encontrado,anterior=0,size=0;

    
    inodo Iencontrado;
    bloqueCarpetas carpetaActual;
    bloqueApuntadores apuntadorActual;
    bool creado=false;
    //separamos el contenido dentro de ruta separandolo por /
    while ((posC = path.find(delimiter)) != string::npos) {
        //cout<<"pos encontrada: "<<posC<<endl;
        regTemp=(path.substr(0, posC));
        path.erase(0, posC + delimiter.length());
        
        if(posC==0){
            posC=-1;
            posC = path.find(delimiter);
            //cout<<"pruebas de pos: "<<posC<<endl;
            continue;
        }
        
        encontrado=BuscarInodo(regTemp,superLog.s_inode_start,file);
        
        if(encontrado==-1){
            if(padres=="simon"){
                

                fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                fread(&Iencontrado, sizeof (inodo), 1, file);

                
                for(int i=0;i<16;i++){
                    if(i<13){
                        if(Iencontrado.i_block[i]>-1){

                            fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                            fread(&carpetaActual, 64, 1, file);

                            for(int a=0;a<5;a++){
                                if(carpetaActual.b_content[a].b_inodo==-1){
                                    creado=true;
                                    carpetaActual.b_content[a].b_inodo=crearInodo(48,regTemp,0,file);
                                    //actualizar_bmInodo(carpetaActual.b_content[a].b_inodo);
                                    
                                    anterior=carpetaActual.b_content[a].b_inodo;

                                    strcpy(carpetaActual.b_content[a].b_name,regTemp.c_str());
                                    fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                                    fwrite(&carpetaActual, 64, 1, file);
                                    break;
                                }
                            }

                            if(creado)break;
                        }else{
                            Iencontrado.i_block[i]=crearBloque(2,"",file);
                            //actualizar_bmBlock(Iencontrado.i_block[i]);

                            fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                            fread(&carpetaActual, 64, 1, file);

                            creado=true;
                            carpetaActual.b_content[0].b_inodo=crearInodo(48,regTemp,0,file);
                            //actualizar_bmInodo(carpetaActual.b_content[0].b_inodo);

                            strcpy(carpetaActual.b_content[0].b_name,regTemp.c_str());
                            
                            fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                            fwrite(&carpetaActual, 64, 1, file);
                            
                            cout<<""<<anterior<<endl;
                            fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                            fwrite(&Iencontrado, sizeof(inodo), 1, file);

                            anterior=carpetaActual.b_content[0].b_inodo;
                            break;
                        }
                    }if(i==13){
                        if(Iencontrado.i_block[i]>-1){
                            fseek(file,Iencontrado.i_block[i]*sizeof(inodo)+superLog.s_block_start,SEEK_SET);
                            fread(&apuntadorActual, 64, 1, file);
                            for(int j=0;j<16;j++){
                                if(apuntadorActual.b_pointers[j]>-1){
                                    fseek(file,apuntadorActual.b_pointers[j]*64+superLog.s_block_start,SEEK_SET);
                                    fread(&carpetaActual, 64, 1, file);
                                    for(int a=0;a<5;a++){
                                        if(carpetaActual.b_content[a].b_inodo==-1){
                                            creado=true;
                                            carpetaActual.b_content[a].b_inodo=crearInodo(48,regTemp,0,file);
                                            //actualizar_bmInodo(carpetaActual.b_content[a].b_inodo);
                                            anterior=carpetaActual.b_content[a].b_inodo;
                                            strcpy(carpetaActual.b_content[a].b_name,regTemp.c_str());
                                            fseek(file,apuntadorActual.b_pointers[j]*64+superLog.s_block_start,SEEK_SET);
                                            fwrite(&carpetaActual, 64, 1, file);
                                            break;
                                        }
                                    }

                                    if(creado)break;
                                }else{
                                    Iencontrado.i_block[i]=crearBloque(2,"",file);
                                    //actualizar_bmBlock(Iencontrado.i_block[i]);

                                    fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                                    fread(&carpetaActual, 64, 1, file);

                                    creado=true;
                                    carpetaActual.b_content[0].b_inodo=crearInodo(48,regTemp,0,file);
                                    //actualizar_bmInod(carpetaActual.b_content[0].b_inodo);

                                    strcpy(carpetaActual.b_content[0].b_name,regTemp.c_str());
                                    
                                    fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                                    fwrite(&carpetaActual, 64, 1, file);
                                    
                                    fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                                    fwrite(&Iencontrado, sizeof(inodo), 1, file);

                                    anterior=carpetaActual.b_content[0].b_inodo;
                                    break;
                                }
                            }

                        }else{

                            Iencontrado.i_block[i]=crearBloque(1,"",file);
                            //actualizar_bmBlock(Iencontrado.i_block[i]);

                            fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                            fread(&apuntadorActual, 64, 1, file);

                            apuntadorActual.b_pointers[0]=crearBloque(2,"",file);
                            //actualizar_bmBlock(apuntadorActual.b_pointers[0]);
                            


                            fseek(file,apuntadorActual.b_pointers[0]*64+superLog.s_block_start,SEEK_SET);
                            fread(&carpetaActual, 64, 1, file);

                            creado=true;
                            carpetaActual.b_content[0].b_inodo=crearInodo(48,regTemp,0,file);
                            //actualizar_bmInodo(carpetaActual.b_content[0].b_inodo);

                            strcpy(carpetaActual.b_content[0].b_name,regTemp.c_str());
                                    
                            fseek(file,apuntadorActual.b_pointers[0]*64+superLog.s_block_start,SEEK_SET);
                            fwrite(&carpetaActual, 64, 1, file);

                            fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                            fwrite(&apuntadorActual, 64, 1, file);
                                    
                            fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                            fwrite(&Iencontrado, sizeof(inodo), 1, file);

                            anterior=carpetaActual.b_content[0].b_inodo;
                            break;

                        }
                    }
                }
                
            }else{
                cout<<"ERROR: RUTA ESPEFICADA NO ENCONTRADA, PARAMETRO <p> NO ENCONTRADO"<<endl;
                return;
            }
        }else{
            anterior=encontrado;
            fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
            fread(&Iencontrado, sizeof (inodo), 1, file);

            if(!VerificarPermisoEscribir(Iencontrado)){
                cout<<"ERROR: NO POSEE LOS PERMISOS PARA LA OPERACION (ESCRIBIR CARPETA)"<<endl;
                return;
            }
        }
    }

    
    
    fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
    fread(&Iencontrado, sizeof (inodo), 1, file);
    
    

    creado=false;
    //cout<<"anterior: "<<anterior<<endl;
    for(int i=0;i<14;i++){
        if(i<13){
            if(Iencontrado.i_block[i]>-1){
                fseek(file,Iencontrado.i_block[i]*sizeof(inodo)+superLog.s_block_start,SEEK_SET);
                fread(&carpetaActual, 64, 1, file);
                for(int a=0;a<5;a++){
                    //cout<<"pos de carpeta: "<<a<<endl;;
                    if(carpetaActual.b_content[a].b_inodo==-1){
                        creado=true;
                        carpetaActual.b_content[a].b_inodo=crearInodo(48,regTemp,0,file);
                        //cout<<"Numero de inodo creado: "<<carpetaActual.b_content[a].b_inodo<<endl;;
                        //actualizar_bmInodo(carpetaActual.b_content[a].b_inodo);
                        anterior=carpetaActual.b_content[a].b_inodo;
                        strcpy(carpetaActual.b_content[a].b_name,path.c_str());
                        //cout<<"nombre: "<<path<<endl;
                        fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                        fwrite(&carpetaActual, 64, 1, file);
                        break;
                    }
                }
                //cout<<"creado?"<<endl;
                if(creado)break;
            }else{
                Iencontrado.i_block[i]=crearBloque(2,"",file);
                //actualizar_bmBlock(Iencontrado.i_block[i]);

                fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                fread(&carpetaActual, 64, 1, file);

                creado=true;
                carpetaActual.b_content[0].b_inodo=crearInodo(48,regTemp,0,file);
                //actualizar_bmInodo(carpetaActual.b_content[0].b_inodo);

                strcpy(carpetaActual.b_content[0].b_name,path.c_str());
                            
                fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                fwrite(&carpetaActual, 64, 1, file);


                fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                fwrite(&Iencontrado, sizeof(inodo), 1, file);
                fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                fread(&Iencontrado, sizeof (inodo), 1, file);
                cout<<"asigno la carpeta?: "<<Iencontrado.i_block[0]<<endl;

                anterior=carpetaActual.b_content[0].b_inodo;
                break;
            }
        }if(i==13){
            if(Iencontrado.i_block[i]>-1){
                fseek(file,Iencontrado.i_block[i]*sizeof(inodo)+superLog.s_block_start,SEEK_SET);
                fread(&apuntadorActual, 64, 1, file);
                for(int j=0;j<16;j++){
                    if(apuntadorActual.b_pointers[j]>-1){
                        fseek(file,apuntadorActual.b_pointers[j]*64+superLog.s_block_start,SEEK_SET);
                        fread(&carpetaActual, 64, 1, file);
                        for(int a=0;a<5;a++){
                            if(carpetaActual.b_content[a].b_inodo==-1){
                                creado=true;
                                carpetaActual.b_content[a].b_inodo=crearInodo(48,regTemp,0,file);
                                //actualizar_bmInodo(carpetaActual.b_content[a].b_inodo);
                                anterior=carpetaActual.b_content[a].b_inodo;
                                strcpy(carpetaActual.b_content[a].b_name,path.c_str());
                                fseek(file,apuntadorActual.b_pointers[j]*64+superLog.s_block_start,SEEK_SET);
                                fwrite(&carpetaActual, 64, 1, file);
                                break;
                            }
                        }

                        if(creado)break;
                    }else{
                        Iencontrado.i_block[i]=crearBloque(2,"",file);
                        //actualizar_bmBlock(Iencontrado.i_block[i]);

                        fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                        fread(&carpetaActual, 64, 1, file);

                        creado=true;
                        carpetaActual.b_content[0].b_inodo=crearInodo(48,regTemp,0,file);
                        //actualizar_bmInodo(carpetaActual.b_content[0].b_inodo);

                        strcpy(carpetaActual.b_content[0].b_name,path.c_str());
                                    
                        fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                        fwrite(&carpetaActual, 64, 1, file);
                                    
                        fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                        fwrite(&Iencontrado, sizeof(inodo), 1, file);

                        anterior=carpetaActual.b_content[0].b_inodo;
                        break;
                    }
                }

            }else{

                Iencontrado.i_block[i]=crearBloque(1,"",file);
                //actualizar_bmBlock(Iencontrado.i_block[i]);

                fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                fread(&apuntadorActual, 64, 1, file);

                apuntadorActual.b_pointers[0]=crearBloque(2,"",file);
                //actualizar_bmBlock(apuntadorActual.b_pointers[0]);
                            


                fseek(file,apuntadorActual.b_pointers[0]*64+superLog.s_block_start,SEEK_SET);
                fread(&carpetaActual, 64, 1, file);

                creado=true;
                carpetaActual.b_content[0].b_inodo=crearInodo(48,regTemp,0,file);
                //actualizar_bmInodo(carpetaActual.b_content[0].b_inodo);

                strcpy(carpetaActual.b_content[0].b_name,path.c_str());
                                    
                fseek(file,apuntadorActual.b_pointers[0]*64+superLog.s_block_start,SEEK_SET);
                fwrite(&carpetaActual, 64, 1, file);

                fseek(file,Iencontrado.i_block[i]*64+superLog.s_block_start,SEEK_SET);
                fwrite(&apuntadorActual, 64, 1, file);
                                
                fseek(file,anterior*sizeof(inodo)+superLog.s_inode_start,SEEK_SET);
                fwrite(&Iencontrado, sizeof(inodo), 1, file);

                anterior=carpetaActual.b_content[0].b_inodo;
                break;

            }
        }
    }
    fclose(file);
    cout<<"ARCHIVO CREADO CORRECTAMENTE"<<endl;
}



string TreeCascada(int pos,FILE* archivo,int num){
    string dot;
    inodo InodoActual;
    
    fseek(archivo,pos, SEEK_SET);
    fread(&InodoActual, sizeof(inodo), 1, archivo);
    times fech;
    fechActual(fech);
    strcpy(InodoActual.i_atime,fech);
    
    fseek(archivo,pos, SEEK_SET);
    fwrite(&InodoActual, sizeof(inodo), 1, archivo);

    fseek(archivo,pos, SEEK_SET);
    fread(&InodoActual, sizeof(inodo), 1, archivo);
    
    if(InodoActual.i_type=='0'){
        for(int i=0;i<16;i++){
            //cout<<"PASANDO POR INODO POS: "<<i<<endl;
            if(InodoActual.i_block[i]>-1){
                //cout<<"PASE EL IF EN "<<i<<endl;
                if(i<13){
                    
                    cout<<"inodo "<<num<<" ->   bloque "<<InodoActual.i_block[i]<<endl;
                    dotTree+="\"node"+to_string(num)+"\":<f"+to_string(i)+"> -> \"block"+to_string(InodoActual.i_block[i])+"\":<f20>\n";
                    TreeCarpeta(InodoActual.i_block[i]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,InodoActual.i_block[i]);
                    
                
                }if(i==13){
                    bloqueApuntadores apuntadores;
                    fseek(archivo,InodoActual.i_block[13]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]!=-1){
                            //dot+=TreeCarpeta(apuntadores.b_pointers[j]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo);
                        }
                    } 
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]>-1){
                            TreeCarpeta(apuntadores.b_pointers[j]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,apuntadores.b_pointers[j]);
                        }
                    } 
                }if(i==14){
                    bloqueApuntadores apuntadores,apuntadores2;
                    fseek(archivo,InodoActual.i_block[14]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]>-1){
                            fseek(archivo,apuntadores.b_pointers[j]*64+superLog.s_block_start, SEEK_SET);
                            fread(&apuntadores2, 64, 1, archivo);
                            for(int k=0;k<16;k++){
                                if(apuntadores2.b_pointers[k]>-1){
                                    TreeCarpeta(apuntadores2.b_pointers[k]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,apuntadores2.b_pointers[k]);
                                    
                                }
                            }
                        }
                    }
                }if(i==15){
                    bloqueApuntadores apuntadores,apuntadores2,apuntadores3;
                    fseek(archivo,InodoActual.i_block[15]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                    fread(&apuntadores, sizeof(bloqueApuntadores), 1, archivo);
                    for(int j=0;j<16;j++){
                        if(apuntadores.b_pointers[j]>-1){
                            fseek(archivo,apuntadores.b_pointers[j]*64+superLog.s_block_start, SEEK_SET);
                            fread(&apuntadores2, 64, 1, archivo);
                            for(int k=0;k<16;k++){
                                if(apuntadores2.b_pointers[k]>-1){
                                    fseek(archivo,apuntadores2.b_pointers[k]*sizeof(bloqueCarpetas)+superLog.s_block_start, SEEK_SET);
                                    fread(&apuntadores3, sizeof(bloqueApuntadores), 1, archivo);
                                    for(int l=0;l<16;l++){
                                        if(apuntadores3.b_pointers[l]>-1){
                                            TreeCarpeta(apuntadores3.b_pointers[l]*sizeof(bloqueCarpetas)+superLog.s_block_start,archivo,apuntadores3.b_pointers[l]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return dot;
}



 string  TreeCarpeta(int pos,FILE* archivo,int num){
    string dot;
    bloqueCarpetas bloque;
    bloqueArchivos archivoB;
    fseek(archivo,pos, SEEK_SET);
    fread(&bloque, sizeof(bloqueCarpetas), 1, archivo);
    dotTree+="\"block"+to_string(num)+"\" [ shape=\"record\"\n color=\"red\" label = \"";
    dotTree+="<f20> BLOQUE CARPETA "+to_string(num);
    //cout<<"bloque num "<<num<<endl;
    for(int j=0;j<4;j++){
        string n=bloque.b_content[j].b_name;
        if(bloque.b_content[j].b_inodo>-1){
            
           dotTree+="| {"+n+"|<f"+to_string(j)+">"+to_string(bloque.b_content[j].b_inodo)+"}";
        }
    }
    dotTree+="\" \n];\n";

    for(int i=0;i<4;i++){
        if(bloque.b_content[i].b_inodo>-1){
            dotTree+="\"block"+to_string(num)+"\":<f"+to_string(i)+"> -> \"node"+to_string(bloque.b_content[i].b_inodo)+"\":<f20>;\n";
            inodo actual;
            fseek(archivo,bloque.b_content[i].b_inodo*sizeof(inodo)+superLog.s_inode_start, SEEK_SET);
            fread(&actual, sizeof(inodo), 1, archivo);

            dotTree+="\"node"+to_string(bloque.b_content[i].b_inodo)+"\" [ shape=\"record\"\n color=\"blue\" label = \"";
            dotTree+="<f20> inodo"+to_string(bloque.b_content[i].b_inodo)+" |{i_type|"+actual.i_type+"}";
            
            for(int j=0;j<16;j++){
                if(actual.i_block[j]>-1){
                    dotTree+="| {i_block_"+to_string(j)+"|<f"+to_string(j)+">"+to_string(actual.i_block[j])+"}";
                }
            }
            dotTree+="| I_perm "+to_string(actual.i_perm)+"\" \n];\n";
            
            for(int j=0;j<15;j++){
                if(actual.i_block[j]>-1){
                    dotTree+="\"node"+to_string(bloque.b_content[i].b_inodo)+"\":<f"+to_string(j)+"> -> \"block"+to_string(actual.i_block[j])+"\":<f20>;\n";
                }
            }
            
            if(actual.i_type=='0'){
                cout<<"carpeta"<<endl;
                cout<<"bloque "<<num<<" ->   inodo "<<bloque.b_content[i].b_inodo<<endl;
                TreeCascada(bloque.b_content[i].b_inodo*sizeof(inodo)+superLog.s_inode_start,archivo,bloque.b_content[i].b_inodo);                
            }else if (actual.i_type=='1'){
                for(int i=0;i<16;i++){
                    if(actual.i_block[i]>-1){
                        cout<<"archivo"<<endl;
                        cout<<"inodo "<<bloque.b_content[i].b_inodo<<" ->   bloque "<<actual.i_block[i]<<endl;
                        fseek(archivo,actual.i_block[i] *64+superLog.s_block_start, SEEK_SET);
                        fread(&archivoB, 64, 1, archivo);
                        dotTree+="\"block"+to_string(actual.i_block[i])+"\" [label = \"";
                        dotTree+="<f20> BLOQUE ARCHIVO "+to_string(actual.i_block[i])+"";
                        string n=archivoB.b_content;
                        n = replace_txt(n, "\n", "\\n");
                        dotTree+="|"+n+"\"";
                        dotTree+=" shape=\"record\"\n color=\"yellow\"];\n";
                    }
                }
            }
        }
    }
    return dot;
}
