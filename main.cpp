#include "analizador.cpp"
#include "comandos.h"

int main(){
    Analizador analisis;
    inicializarListaMount();
    string entradacmd = "";
    while(entradacmd != "exit"){
        cout << "*----------------------------------------------------------*" << endl;
        cout << "*                    [MIA] PROYECTO1                       *" << endl;
        cout << "*               JUSTIN AGUIRRE - 202004734                 *" << endl;
        cout << "*----------------------------------------------------------*" << endl;
        getline(cin,entradacmd);
        analisis.analizar(entradacmd);
    }
    analisis.analizar("exit");
    return 0;
}

// g++ main.cpp -o main.exe