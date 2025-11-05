#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>
#include <vector>
using namespace std;

class Musica;

class Artista {
private:
    string nome;
    string genero;
    vector<Musica*> musicas;
public:
    Artista(string nome, string genero);
    string getNome();
    string getGenero();
    void setNome(const string& novoNome);
    void adicionarMusica(Musica* musica);
    void removerMusica(Musica* musica);
    void listarMusicas();

};

#endif 
