#include "Avaliacao.h"
#include "Usuario.h"
#include "Musica.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Construtor: inicializa todos os campos da avaliação, definindo data atual e oculto como falso
Avaliacao::Avaliacao(Usuario* autor, Musica* musica, int nota, string comentario)
    : autor(autor), musica(musica), nota(nota), comentario(comentario), oculto(false) {
    dataCriacao = time(nullptr);
    // Garantir invariante: a nota deve estar entre 0 e 5.
    // Se uma nota inválida for passada por algum código, corrigimos silenciosamente
    // para o valor mais próximo (0 ou 5). Isso protege o modelo independentemente
    // da validação na interface do usuário.
    if (this->nota < 0) this->nota = 0;
    else if (this->nota > 5) this->nota = 5;
}

// Retorna a nota atribuída à música
int Avaliacao::getNota() {
    return nota;
}

string Avaliacao::getComentario() {
    return comentario;
}

// Retorna verdadeiro se a avaliação estiver marcada como oculta
bool Avaliacao::isOculto() {
    return oculto;
}

// Marca a avaliação como oculta, impedindo que seja exibida
void Avaliacao::ocultar() {
    oculto = true;
}

void Avaliacao::revelar() {
    oculto = false;
}

Usuario* Avaliacao::getAutor() {
    return autor;
}

Musica* Avaliacao::getMusica() {
    return musica;
}

// Retorna o ponteiro da música associada à avaliação
Musica* Avaliacao::getMusica() {
    return musica;
}

// Exibe as informações da avaliação no console, incluindo autor, nota e data
void Avaliacao::mostrarAvaliacao() {
    if (oculto) {
        cout << "[Avaliação oculta]" << endl;
        return;
    }
    // Exibe autor e nota com um símbolo de estrela (★) para melhor leitura
    cout << "Avaliacao por: " << (autor ? autor->getNome() : string("<desconhecido>"));
    if (autor) {
        cout << " <" << autor->getEmail() << ">";
    }
    cout << " | Nota: " << nota << " ★" << endl;
    // Exibe comentário se existir
    if (!comentario.empty()) {
        cout << "Comentário: " << comentario << endl;
    }
    // Nota: não imprimimos o separador aqui para que quem chamar a função
    // tenha controle sobre como organizar as avaliações na tela (ex.: imprimir
    // curtidas logo em seguida). O separador será impresso por quem lista as
    // avaliações (Musica::mostrarInfo) para manter blocos consistentes.
}

void Avaliacao::mostrarAvaliacaoAdmin() {
    // Mostra a avaliação completa mesmo se estiver oculta (visão de moderador)
    cout << "Avaliacao por: " << (autor ? autor->getNome() : string("<desconhecido>"));
    if (autor) {
        cout << " <" << autor->getEmail() << ">";
    }
    cout << " | Nota: " << nota << " ★" << endl;
    if (!comentario.empty()) {
        cout << "Comentário: " << comentario << endl;
    }
}

void Avaliacao::setComentario(const string& c) {
    comentario = c;
}

void Avaliacao::setNota(int n) {
    if (n < 0) n = 0;
    else if (n > 5) n = 5;
    nota = n;
}
