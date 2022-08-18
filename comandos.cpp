
#include "comandos.h"
#include <direct.h>

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
        char bloque[1024];
        for (int i = 0; i < 1024; i++)
        {
            bloque[i] = '\0';
        }

        // Escritura de Bloque en Archivo
        int limite = 0;
        FILE *archivo;
        string direct = "";
        ifstream f(path.c_str());
        if (!f.good())
        {
            int start = 0;
            int end = path.find("/");
            string del = "/";
            while (end != -1)
            {
                cout << path.substr(start, end - start) << endl;
                direct += path.substr(start, end - start);
                if (mkdir(direct.c_str()) == 0)
                {
                    cout << "created directory - " << direct << endl;
                }
                else
                {
                    cout << "create_directory() failed" << endl;
                }
                direct += "/";
                start = end + del.size();
                end = path.find("/", start);
            }
            cout << path.substr(start, end - start);
        }
         archivo = fopen(path.c_str(), "w");
         while (limite != size_file)
        {
           fwrite(&bloque, 1024, 1, archivo);
           limite++;
        }
         fclose(archivo);

        /*MBR nuevo;
        nuevo.mbr_tamano = 5;
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%d/%m/%Y :: %H:%M:%S");
        string str = oss.str();
        strcpy(nuevo.mbr_fecha_creacion, str.c_str());

        srand(time(NULL));

        int num = rand();

        archivo = fopen("Archivo.dk", "rb+");

        num = rand() % 11;
        nuevo.mbr_dsk_signature = num;
        fseek(archivo, 0 * sizeof(MBR), SEEK_SET);
        fwrite(&nuevo, sizeof(nuevo), 1, archivo);
        fseek(archivo, 0 * sizeof(MBR), SEEK_SET);
        fclose(archivo);
        cout << "ARCHIVO BINARIO CREADO" << endl;
        cout << "" << endl;*/
    }
    else
    {
        cout << "Error creando Disco: tamaño invalido a ejecución." << endl;
    }
}
