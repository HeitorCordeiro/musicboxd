#include "Artista.h"
#include "Musica.h"
#include <iostream>

using namespace std;
// A implementação fornece acesso a
// metadados do artista e operações para gerenciar a lista de músicas
// associadas.

Artista::Artista(string nome, string genero)
    : nome(nome), genero(genero) {

}

string Artista::getNome(){
    return nome;
}


string Artista::getGenero(){
    return genero;
}

void Artista::setNome(const string& novoNome) {
    this->nome = novoNome;
}

void Artista::adicionarMusica(Musica* musica) {
    musicas.push_back(musica);
}

void Artista::removerMusica(Musica* musica){
    // Valida o ponteiro de música antes de tentar remover.
    if (!musica){
        return;
    }
    for (auto it = musicas.begin(); it != musicas.end(); ++it) {
        if (*it == musica) {
            musicas.erase(it);
            return;
        }
    }
}

void Artista::listarMusicas() {
    cout << "Musicas de " << nome << ":" << endl;
    for (size_t i = 0; i < musicas.size(); ++i) {
        cout << " - " << (musicas[i] ? musicas[i]->getTitulo() : string("<desconhecida>")) << endl;
    }
}
