
#include "analizador.h"

vector<string> Analizador::split_txt(string text){ // Split para separar tipo de comando y parametros
    stringstream text_to_split(text);
    string segment;
    vector<string> splited;

    while(std::getline(text_to_split, segment, ' ')){
       splited.push_back(segment);
    }
    return splited;
}

string Analizador::replace_txt(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

void Analizador::analizar(string entrada){
    string entradacmd = entrada;
    // Transformacion de toda la entrada a minusculas
    transform(entradacmd.begin(), entradacmd.end(), entradacmd.begin(), ::tolower);
    vector<string> cmdentrada = split_txt(entradacmd);
    vector<string> parametros;
    string comando = "";
    for(int i = 0; i< cmdentrada.size(); i++){
        if(i == 0){
            comando = cmdentrada.at(i);
        }else{
            parametros.push_back(cmdentrada.at(i));
        }
    }
    // Identificacion de comando, parametros y ejecucion
    identificarParametros(comando, parametros);
}

void Analizador::identificarParametros(string comando, vector<string> parametros){
    string param = "";
    if(comando == "mkdisk"){
        cmd.param.Comando = "mkdisk";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-s->") == 0){
                param = replace_txt(param, "-s->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.s = param;
            }else if(param.find("-f->") == 0){
                param = replace_txt(param, "-f->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.f = param;
            }else if(param.find("-u->") == 0){
                param = replace_txt(param, "-u->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.u = param;
            }else if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            } else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        if(cmd.param.s!=" " && cmd.param.path!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
        
    } else if(comando == "rmdisk"){
        cmd.param.Comando = "rmdisk";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            } else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        
        if(cmd.param.path!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "fdisk"){
        cmd.param.Comando = "fdisk";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-s->") == 0){
                param = replace_txt(param, "-s->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.s = param;
            }else if(param.find("-t->") == 0){
                param = replace_txt(param, "-t->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.t = param;
            }else if(param.find("-u->") == 0){
                param = replace_txt(param, "-u->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.u = param;
            }else if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }else if(param.find("-f->") == 0){
                param = replace_txt(param, "-f->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.f = param;
            }else if(param.find("-delete->") == 0){
                param = replace_txt(param, "-delete->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.delete_ = param;
            } else if(param.find("-name->") == 0){
                param = replace_txt(param, "-name->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.name = param;
            } else if(param.find("-add->") == 0){
                param = replace_txt(param, "-add->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.add_ = param;
            }  else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.s!=" " && cmd.param.path!=" "&& cmd.param.name!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }


    }  else if(comando == "mount"){
        cmd.param.Comando = "mount";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }else if(param.find("-name->") == 0){
                param = replace_txt(param, "-name->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.name = param;
            }  else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "&& cmd.param.name!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "unmount"){
        cmd.param.Comando = "unmount";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-id->") == 0){
                param = replace_txt(param, "-id->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.id = param;
            }  else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.id!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "mkfs"){
        cmd.param.Comando = "mkfs";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-id->") == 0){
                param = replace_txt(param, "-id->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.id = param;
            } else if(param.find("-type->") == 0){
                param = replace_txt(param, "-type->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.type = param;
            } else if(param.find("-fs->") == 0){
                param = replace_txt(param, "-fs->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.fs = param;
            }   else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.id!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "login"){
        cmd.param.Comando = "login";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-id->") == 0){
                param = replace_txt(param, "-id->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.id = param;
            } else if(param.find("-usr->") == 0){
                param = replace_txt(param, "-usr->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.usr = param;
            } else if(param.find("-pass->") == 0){
                param = replace_txt(param, "-pass->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.pass = param;
            }   else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.id!=" "&&cmd.param.usr!=" "&&cmd.param.pass!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "logout"){
        cmd.param.Comando = "logout";

        if(cmd.param.id!=" "&&cmd.param.usr!=" "&&cmd.param.pass!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "mkgrp"){
        cmd.param.Comando = "mkgrp";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-name->") == 0){
                param = replace_txt(param, "-name->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.name = param;
            }   else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.name!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "rmgrp"){
        cmd.param.Comando = "rmgrp";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-name->") == 0){
                param = replace_txt(param, "-name->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.name = param;
            }   else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.name!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "mkusr"){
        cmd.param.Comando = "mkusr";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-usr->") == 0){
                param = replace_txt(param, "-usr->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.usr = param;
            } else if(param.find("-pass->") == 0){
                param = replace_txt(param, "-pass->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.pass = param;
            } else if(param.find("-grp->") == 0){
                param = replace_txt(param, "-grp->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.grp = param;
            }   else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.usr!=" "&&cmd.param.pass!=" "&&cmd.param.grp!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "rmusr"){
        cmd.param.Comando = "rmusr";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-usr->") == 0){
                param = replace_txt(param, "-usr->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.usr = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.usr!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "chmod"){
        cmd.param.Comando = "chmod";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }  else if(param.find("-ugo->") == 0){
                param = replace_txt(param, "-ugo->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.ugo = param;
            }  else if(param.find("-r->") == 0){
                param = replace_txt(param, "-r->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.r = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "&&cmd.param.ugo!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    }else if(comando == "mkfile"){
        cmd.param.Comando = "mkfile";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }  else if(param.find("-s->") == 0){
                param = replace_txt(param, "-s->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.s = param;
            }  else if(param.find("-r->") == 0){
                param = replace_txt(param, "-r->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.r = param;
            } else if(param.find("-cont->") == 0){
                param = replace_txt(param, "-cont->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.cont = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "cat"){
        cmd.param.Comando = "cat";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-file->") == 0){
                param = replace_txt(param, "-file->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.fileN = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "remove"||comando == "rem"){
        cmd.param.Comando = "remove";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "edit"){
        cmd.param.Comando = "edit";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }else if(param.find("-cont->") == 0){
                param = replace_txt(param, "-cont->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.cont = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "&&cmd.param.cont!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "rename"){
        cmd.param.Comando = "rename";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }else if(param.find("-name->") == 0){
                param = replace_txt(param, "-name->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.name = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "&&cmd.param.name!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "mkdir"){
        cmd.param.Comando = "mkdir";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }else if(param.find("-p->") == 0){
                param = replace_txt(param, "-p->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.p = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "copy"){
        cmd.param.Comando = "copy";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }else if(param.find("-destino->") == 0){
                param = replace_txt(param, "-destino->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.destino = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "&&cmd.param.destino!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "move"){
        cmd.param.Comando = "move";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }else if(param.find("-destino->") == 0){
                param = replace_txt(param, "-destino->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.destino = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "&&cmd.param.destino!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "find"){
        cmd.param.Comando = "find";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }else if(param.find("-destino->") == 0){
                param = replace_txt(param, "-destino->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.destino = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "&&cmd.param.name!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "chown"){
        cmd.param.Comando = "chown";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }else if(param.find("-r->") == 0){
                param = replace_txt(param, "-r->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.r = param;
            }else if(param.find("-usr->") == 0){
                param = replace_txt(param, "-usr->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.usr = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "&&cmd.param.usr!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "chgrp"){
        cmd.param.Comando = "chgrp";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-grp->") == 0){
                param = replace_txt(param, "-grp->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.grp = param;
            }else if(param.find("-usr->") == 0){
                param = replace_txt(param, "-usr->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.usr = param;
            }    else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.grp!=" "&&cmd.param.usr!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
    } else if(comando == "pause"){
       
            cmd.identificacionCMD(cmd.param);
       
    } 
}