#include "Avaliacao.h"
#include "Usuario.h"
#include "Musica.h"
#include <iostream>

using namespace std;

Avaliacao::Avaliacao(Usuario* autor, Musica* musica, int nota, string comentario)
    : autor(autor), musica(musica), nota(nota), comentario(comentario), oculto(false) {
    dataCriacao = time(nullptr);
    // implementação
}

int Avaliacao::getNota() {
    // implementação 
}

bool Avaliacao::isOculto() {
    // implementação 
}

void Avaliacao::ocultar() {
    // implementação
}

void Avaliacao::mostrarAvaliacao() {
    // implementação
}
