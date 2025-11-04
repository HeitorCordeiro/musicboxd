#include "Artista.h"
#include "Musica.h"
#include <iostream>

using namespace std;

Artista::Artista(string nome, string genero)
    : nome(nome), genero(genero) { 
}

string Artista::getNome() {
    return nome;
}

string Artista::getGenero() {
    return genero;
}

void Artista::adicionarMusica(Musica* musica) {
    if (musica != nullptr) {
        musicas.push_back(musica);
    } else {
        cout << "Não é possível adicionar uma música nula ao artista." << endl;
    }
}

void Artista::listarMusicas() {
    cout << "Artista: " << nome << " (" << genero << ")" << endl;

    if (musicas.empty()) {
        cout << "Nenhuma musica cadastrada para este artista." << endl;
        return;
    } 

    for (size_t i = 0; i < musicas.size(); i++) {
        if (musicas[i] != nullptr) {
            cout << "  " << (i + 1) << ". " << musicas[i]->getTitulo() << endl;
        } else {
            cout << "  " << (i + 1) << ". <musica nula>" << endl;
        }
    }
}
