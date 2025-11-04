#include "Lista.h"
#include "Musica.h"
#include <iostream>

using namespace std;

Lista::Lista(string nome)
    : nome(nome) {
    
}

void Lista::adicionarMusica(Musica* musica) {
    if (musica != nullptr) {
        musicas.push_back(musica);
    } else {
        cout << "Não é possível adicionar uma música nula à lista." << endl;
    }
}

void Lista::mostrarLista() {
    cout << "Lista de Músicas: " << nome << endl;

    if (musicas.empty()) {
        cout << "Nenhuma música cadastrada." << endl;
        return;
    }

    for (size_t i = 0; i < musicas.size(); i++) {
        if (musicas[i] != nullptr) {
            cout << "  " << (i + 1) << ". " << musicas[i]->getTitulo();

            if (musicas[i]->getArtista() != nullptr) {
                cout << " - " << musicas[i]->getArtista()->getNome();
            }
            cout << endl;
        } else {
            cout << "  " << (i + 1) << ". <música nula>" << endl;
        }
    }
}
