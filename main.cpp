#include "analizador.cpp"

int main(){
    Analizador analisis;
    
    string entradacmd = "";
    while(entradacmd != "exit"){
        cout << "*----------------------------------------------------------*" << endl;
        cout << "*                    [MIA] PROYECTO1                       *" << endl;
        cout << "*               JUSTIN AGUIRRE - 202004734                 *" << endl;
        cout << "*----------------------------------------------------------*" << endl;
        getline(cin,entradacmd);
        analisis.analizar(entradacmd);
    }
    return 0;
}

// g++ main.cpp -o main.exe