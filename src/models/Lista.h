#ifndef LISTA_H
#define LISTA_H

#include <string>
#include <vector>
using namespace std;

class Musica;

class Lista {
private:
    string nome;
    vector<Musica*> musicas;
public:
    Lista(string nome);
    void adicionarMusica(Musica* musica);
    void mostrarLista();

};

#endif
