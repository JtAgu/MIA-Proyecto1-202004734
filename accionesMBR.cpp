
#include "accionesMBR.h"
#include <iostream>
#include <string.h>
#include <algorithm>
#include <fstream>

#include "comandos.h"

using namespace std;

MBR leerMBR(string ruta){
    MBR retorno;
    FILE *f;

    if ((f = fopen(ruta.c_str(), "r+b")) == NULL) {
        
            cout<<"Error: no se ha podido al abrir el disco!\n";
        
    } else {
        fseek(f, 0, SEEK_SET);
        fread(&retorno, sizeof (MBR), 1, f);
        fclose(f);
    }
    return retorno;
}

void actualizarMBR(MBR nuevo, string ruta) {
    
    bool principal=true;
    FILE *f;
    if ((f = fopen(ruta.c_str(), "r+b")) == NULL) {
        principal= false;
    } else {
        fseek(f, 0, SEEK_SET);
        fwrite(&nuevo, sizeof (MBR), 1, f);
        fclose(f);
    }

    if(principal==true){
        cout<<"\t...................Se ha actualizado la partición................"<<endl;
    }else{
        cout<<"Error: no se ha podido al abrir el disco!\n";
    }
}