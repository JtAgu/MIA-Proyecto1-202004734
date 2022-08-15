#include "analizador.cpp"

int main(){
    Analizador analisis;
    
    string entradacmd = "";
    while(entradacmd != "exit"){
        cout << "*----------------------------------------------------------*" << endl;
        cout << "*                      [MIA] HT 2                     *" << endl;
        cout << "*----------------------------------------------------------*" << endl;
        cout << "[MIA]@Ejemplo3:~$ ";
        getline(cin,entradacmd);
        analisis.analizar(entradacmd);
    }
    return 0;
}