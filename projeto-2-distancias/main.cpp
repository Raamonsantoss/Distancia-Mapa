#include "funcoes.h"

void menu(void) {
    setlocale(LC_ALL,"portuguese");
    unordered_map<string, vector<tipo_mapa>> rota;
    list<string> city; //Cria uma lista chamada city
    list<tipo_mapa> cidades;
    int escolha;
    bool continua = true;
    while(continua){
        cout << " _____________________________________________________ " << endl;
        cout << " |                    GPS JOPERA                      |" << endl;
        cout << " | Vamos calcular a melhor rota para voce, para isso: |" << endl;
        cout << " |                                                    |" << endl;
        cout << " |       Digite [1]: Verificar todas as cidades       |" << endl;
        cout << " |       Digite [2]: Calcular a rota                  |" << endl;
        cout << " |       Digite [0]: Sair do GPS                      |" << endl;
        cout << " |____________________________________________________|" << endl;
        cout << " | Digite sua opcao: ";
        cin >> escolha;
        cin.ignore();
        cout << "|" << endl;

        if(escolha == 1){ //Se o usuário digitar 1 irá mostrar as cidades presentes no arquivo.
            mostra_cidades(cidades);
        }else if(escolha == 2){ //Se o usuário digitar 2 irá solicitar duas cidades para que seja feito o calculo da distancia.
            mostra_distancia(rota, city);
        }else if(escolha == 0){ //Se digitar 0, o programa encerra.
            continua = false;
        }else{ // Se for digitado qualquer opção diferente das acimas, retorna mensagem de invalido.
            cout << " |____________________________________________________|" << endl;
            cout << " |                  Opcao Invalida                    |" << endl;
            cout << " |____________________________________________________|" << endl;
            sleep(2);
        }
    }
}

//Função responsável por fazer a leitura do arquivo.
void leitura_arquivo(unordered_map<string,vector<tipo_mapa>>& rota,list<string>& city) {

    ifstream arq("../distancias.txt"); //Abre o arquivo
    if (! arq.is_open()) { //Se o arquivo não abrir, retornar arquivo inválido
        cerr << "Arquivo ivalido";
    }
    string mapa;
    string cidade;
    string destino_I;
    string num;
    int qtde_I;

    //Através de um getline é feito a leitura do arquivo e a separação do conteúdo em strings.
    while (getline(arq, mapa)) {
        // A cada virgula, o conteudo é separado em uma string diferente, criadas acima.
        istringstream s(mapa);
        getline(s, cidade, ',');
        getline(s, destino_I, ',');
        getline(s, num, ',');
        qtde_I = stoi(num);

        city.push_back(cidade);
        // É pego o que foi separado a cima e armazenado na struct tipo_mapa, que foi nomeada de aux
        tipo_mapa aux;
        aux.dest = destino_I;
        aux.quantidade = qtde_I;

        rota[cidade].push_back(aux);
    }
    // retira as cidades repetidas
    city.unique();
}

//Função responsável por fazer o cálculo da menor rota entre as cidades informadas.
void calculo(unordered_map<string, vector<tipo_mapa>>& rota, list<string>& city,string& final,
              unordered_map<string, tipo_grafo>& grafo_distancia) {

    //Nessa primeira parte é criada uma tabela D, onde:
    // D[nodo] = (infinito, próximo_nodo)
   for (auto it : city) {
        grafo_distancia[it] = tipo_grafo{"", 11111110};
    }
    grafo_distancia[final] = tipo_grafo{"", 0};
    // Vale lembrar que o D[nodoA] = (0, nodoA), sendo nodoA o destino


    //Cria Q, lista contendo todos os nodos (incluindo nodoA)
    list<string> Q = city;

    //Enquanto Q não estivar vazio:
    while (!Q.empty()) {
        // Retira de Q o nodo denominado u, que é o nodo com menor distância até nodoA
        string u;
        int m_distancia = 11111110;
        for (auto it : Q) {
            if (grafo_distancia[it].distancia < m_distancia) {
                u = it;
                m_distancia = grafo_distancia[it].distancia;
            }

            // Para cada nodo vizinho (v) de u:
            for (auto vizinho : rota[u]) {
                string v = vizinho.dest;
                int dist_v = m_distancia + vizinho.quantidade; // Calcular a distancia do novo v: dist_v = dist_u + distancia(u,v)
                if (dist_v < grafo_distancia[v].distancia) { // Se dist_v for menor que a distancia contida em D[v]
                    grafo_distancia[v] = tipo_grafo{u, dist_v}; // Atualizar D[v] = (dist_v, u)
                }
            }
        }
        Q.remove(u); // Remove u de Q até ficar vazio
    }
}

void mostra_distancia(unordered_map<string, vector<tipo_mapa>> rota, list<string> city){
    unordered_map<string, vector<tipo_mapa>> rotas;
    list<string> citys;

    leitura_arquivo(rota, city);

    string final;
    string inicio, escolha;
    bool continua = true;

    while(continua){
        cout << " | Digite o nome da cidade de destino:";
        getline(cin, final); //Faz a leitura do que for digitado pelo usuário como cidade de destino
        if (rota.find(final) == rota.end()) { //Caso a cidade digitada não esteja no arquivo
            cout << " | " << final << " Desconhecido" << endl; // Retorna mensagem de desconhecido
            cout << " | Certifique que o nome da cidade esteja correto e com letras maiusculas"
                 << endl; // E pede ao usuario que verifique se digitou corretamente
        } else {
            unordered_map<string, tipo_grafo> grafo_distancia;

            calculo(rota, city, final, grafo_distancia);

            cout << " | Digite o nome da cidade de origem:";
            getline(cin, inicio); //Faz a leitura do que for digitado pelo usuário como cidade de origem

            auto it = rota.find(inicio);

            if (it == rota.end()) { //Caso a cidade digitada não esteja no arquivo
                cout << " | " << inicio << " Desconhecido" << endl; // Retorna mensagem de desconhecido
                cout << " | Certifique que o nome da cidade esteja correto e com letras maiusculas"
                     << endl; // E pede ao usuario que verifique se digitou corretamente
            } else { //Se as cidades estiverem corretas, mostrará o menor caminho entre elas.
                cout << " |" << endl;
                cout << " | Menor trajeto de " << final << " para " << inicio << ": " << endl;
                cout << " | Distancia total: " << grafo_distancia[inicio].distancia << "km" << endl << " |"
                     << endl;//Mostra ao final a distancia total do caminho.
            }
        }
        bool continua2 = true;

        //Opção para o usuário escolher se deseja voltar ao menu ou escolher outras cidades.
        while(continua2) {
            cout << " | " << endl;
            cout << " |____________________________________________________|" << endl;
            cout << " |   Deseja prosseguir na escolha da cidade? [S/N]    |" << endl << " | R: ";
            cin >> escolha;
            cin.ignore();
            cout << "| "<< endl;
            if (escolha == "S" && "s") {
                continua = true;
                continua2 = false;
            } else if (escolha == "N" && "n") {
                continua = false;
                continua2 = false;
            } else {
                cout << " |____________________________________________________|" << endl;
                cout << " |                  Opcao Invalida                    |" << endl;
                cout << " |____________________________________________________|" << endl;
                sleep(2);
                continua2 = true;
            }
        }
    }
}

void mostra_cidades(const list<tipo_mapa> & cidades){
    list<string> valores; //Cria uma lista chamada valores.
    string caracter, cidade, destino_I, num, qtde_I; //Cria strings para inserir os dados presentes no arquivo.
    ifstream arq("../distancias.txt");
    while (getline(arq, caracter)){ //Faz a separação do conteudo do arquivo delimitado pela virgula, em strings.
        istringstream s(caracter);
        getline(s, cidade, ',');
        getline(s, destino_I, ',');
        getline(s, num, ',');
        qtde_I = stoi(num);

        valores.push_back(cidade);
    }

    valores.sort(); //Ordena a lista
    valores.unique(); //Exclui os repetidos e mostra na tela as cidades presentes no arquivo.

    for(auto & x :valores){
        cout << " | " << x << endl;
    }

    //Opção para o usuário escolher se deseja voltar ao menu ou escolher outras cidades.
    bool continua = true;
    string escolha;
    while(continua){
        cout << " |"<< endl;
        cout << " |____________________________________________________|" << endl;
        cout << " |         Voltar para o Menu Principal? [S/N]        |" << endl << " |R: ";
        cin >> escolha;
        cin.ignore();
        if(escolha == "S"){
            continua = false;
        }else if(escolha == "N"){
            continua = true;
        }else{
            cout << "|____________________________________________________|" << endl;
            cout << " |                  Opcao Invalida                    |" << endl;
            cout << " |____________________________________________________|" << endl;
            sleep(2);
        }
    }
}
