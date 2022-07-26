
#ifndef REC_PROJETO2_FUNCOES_H
#define REC_PROJETO2_FUNCOES_H

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <list>
#include <locale>
#include <unistd.h>

using namespace std;

struct tipo_mapa {
    string dest;
    int quantidade;
};

struct tipo_grafo {
    string cidade;
    int distancia;
};

//Função responsável por fazer a leitura do arquivo.
void leitura_arquivo(unordered_map<string,vector<tipo_mapa>>& rota,list<string>& city);

//Função responsável por fazer o cálculo da menor rota entre as cidades informadas.
void calculo(unordered_map<string, vector<tipo_mapa>>& rota, list<string>& city,string& final,
              unordered_map<string, tipo_grafo>& grafo_distancia);

void mostra_cidades(const list<tipo_mapa> & cidades);

void mostra_distancia(unordered_map<string, vector<tipo_mapa>> rota, list<string> city);

void menu(void);
//Função responsável por chamar todas as outras funções e assim mostrar o menu inicial ao usuário.
int main();

#endif //REC_PROJETO2_FUNCOES_H
