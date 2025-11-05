#include "Lista.h"
#include "Musica.h"
#include <iostream>

using namespace std;

Lista::Lista(string nome)
    : nome(nome) {
    // implementação mínima
}

void Lista::adicionarMusica(Musica* musica) {
    musicas.push_back(musica);
}

void Lista::mostrarLista() {
    cout << "Lista: " << nome << endl;
    for (size_t i = 0; i < musicas.size(); ++i) {
        cout << " - " << (musicas[i] ? musicas[i]->getTitulo() : string("<desconhecida>")) << endl;
    }
}
