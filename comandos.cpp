
#include "comandos.h"
#include "accionesMBR.cpp"

#include <iostream>
#include <string.h>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <unistd.h>

const string fecha(time_t now) {
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X %d-%m-%Y ", &tstruct);

    return buf;
}




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
        else
        {
            cout << "Error eliminando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if (p.Comando=="unmount"){
        if ( p.id != " ")
        { // Se validan los parametros para el comando
            cUnmount(p.id);
        }
        else
        {
            cout << "Error eliminando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if (p.Comando == "rep"){ // Se identifica el tipo de comando

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
                        if(ajuste==" "){
                           ajuste="w";
                        }
                        if(tam<tamanoA){
                        fseek(f, inicio, SEEK_SET);
                        B_ebr.part_status = '0';
                        B_ebr.part_fit = ajuste.at(0);
                        B_ebr.part_start = inicio;
                        B_ebr.part_s= tam;
                        B_ebr.part_next = -1;
                        strcpy(B_ebr.part_name, name.c_str());
                        fwrite(&B_ebr, sizeof (B_ebr), 1, f);
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
        tam=tam/1024;
    }else if(unit=="K"||unit=="k"){
        
    }else if(unit=="M"||unit=="m"){
        tam = 1024 * tam;
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
            
                    //cout<<"3,1"<<endl;
                    if(pAnterior){
                        pAnterior->siguiente=NULL;
                    }else{
                        puntero=NULL;
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
    mnt_lista* lista = listaDeParticiones;
    mnt_nodo* puntero = lista->cabeza;
    int retorno;

    while (puntero) {
        retorno = strncmp(ids.c_str(), puntero->mnt_id, 16);
        if (retorno == 0)//lo encontró
            return *puntero;
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
    }
    
}

void crear_ext2(mnt_nodo part,int n,int inicio){
    superBloque super= crearSuper(part,n,inicio);
    EscribirSuper(part.mnt_ruta,super,inicio);
    bmBloque bitmapBloques[n*3];
    bmBloque aux;
    
    bitmapBloques[0].status='1';
    aux.status='0';
    for(int i=1;i<n*3;i++){
        bitmapBloques[i]=aux;
    }

    EscribirBloque(part.mnt_ruta,bitmapBloques,super.s_bm_block_start,3*n);

    bmInodo bitmapInodo[n];
    bmInodo auxI;
    bitmapInodo[0].status='1';
    auxI.status='0';
    for(int i=1;i<n;i++){
        bitmapInodo[i]=auxI;
    }
    EscribirInodo(part.mnt_ruta,bitmapInodo,super.s_bm_inode_start,n);


    inodo root;
    root.i_uid=1;
    root.i_gid=1;
    strcpy(root.i_atime,"");

    times fech;
    fechActual(fech);
    strcpy(root.i_ctime,fech);

    for(int i=0;i<16;i++){
        root.i_block[i]=-1;
    }

    root.i_type=0;

}

superBloque crearSuper(mnt_nodo part,int n,int inicio){
    superBloque super;
    super.s_block_s=2;
    super.s_inodes_count=n;
    super.s_blocks_count=3*n;
    super.s_free_blocks_count=3*n;
    super.s_free_inodes_count=n;
    strcpy(super.s_umtime,part.tiempo);
    strcpy(super.s_mtime,part.tiempo);
    super.s_mnt_count=1;
    super.s_magic=0xEF53;
    super.s_inode_s=sizeof(inodo);
    super.s_block_s=sizeof(bloqueCarpetas);

    super.s_firts_ino = inicio + sizeof (superBloque) + 3 * n + n; //esta es la primer posicion y ya se agrego el +1 //primer inodo libre
    super.s_first_blo = super.s_firts_ino + n * sizeof (inodo); 

    super.s_bm_inode_start = inicio + sizeof (superBloque) +1;
    super.s_bm_block_start = super.s_bm_inode_start + n+1;

    super.s_inode_start = inicio + sizeof (superBloque) + 3 * n + n;
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


void EscribirBloque(string path,bmBloque mapB[],int inicio, int n){
    FILE* f;
    if((f=fopen(path.c_str(),"r+b"))==NULL){
        cout<<"ERROR, NO SE PUDO ABRIR EL DISCO"<<endl;
        return;
    }
    for(int i=0;i<n;i++){
    fseek(f, inicio+i*(sizeof(bmBloque)), SEEK_SET);
    fwrite(&mapB[i], sizeof (superBloque), 1,f);
    fclose(f);
    }
}


void EscribirInodo(string path,bmInodo mapI[],int inicio,int n){
    FILE* f;
    if((f=fopen(path.c_str(),"r+b"))==NULL){
        cout<<"ERROR, NO SE PUDO ABRIR EL DISCO"<<endl;
        return;
    }
    for(int i=0;i<n;i++){
    fseek(f, inicio+i*(sizeof(bmInodo)), SEEK_SET);
    fwrite(&mapI[i], sizeof (superBloque), 1, f);
    }
    fclose(f);
}

void fechActual(times fecha) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    strcpy(fecha,buf);
}