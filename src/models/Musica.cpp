
#include "Musica.h"
#include "Artista.h"
#include "Avaliacao.h"
#include <iostream>

// Implementação dos métodos da classe Musica

Musica::Musica(string titulo, string genero, float duracao, Artista* artista, string album, int ano, string capa, vector<string> plataformas)
    : titulo(titulo), genero(genero), duracao(duracao), album(album), anoLancamento(ano), capa(capa), plataformas(plataformas), artista(artista) {
    // implementação mínima
}

// Retorna o título da música.
string Musica::getTitulo(){
    return titulo;
}

void Musica::setTitulo(const string& novoTitulo) {
    this->titulo = novoTitulo;
}

// Retorna o ponteiro para o artista associado à música.
Artista* Musica::getArtista(){
    return artista;
}

// Adiciona uma avaliação à música quando o ponteiro for válido.
void Musica::adicionarAvaliacao(Avaliacao* avaliacao){
    if (avaliacao){
        avaliacoes.push_back(avaliacao);
    }
}

void Musica::removerAvaliacao(Avaliacao* avaliacao){
    // Valida o ponteiro antes de remover.
    if (!avaliacao){
        return;
    }
    for (auto it = avaliacoes.begin(); it != avaliacoes.end(); ++it) {
        if (*it == avaliacao) {
            avaliacoes.erase(it);
            return;
        }
    }
}

float Musica::calcularMedia() {
    int soma = 0;
    int count = 0;
    for (auto av : avaliacoes) {
        if (av && !av->isOculto()) {
            soma += av->getNota();
            ++count;
        }
    }
    if (count == 0){
        // Não existem avaliações visíveis; retorna 0.0.
        return 0.0f;
    }
    return static_cast<float>(soma) / static_cast<float>(count);
}

void Musica::mostrarInfo() {
    cout << "Titulo: " << titulo << endl;
    cout << "Artista: " << (artista ? artista->getNome() : string("<desconhecido>")) << endl;
    cout << "Album: " << album << " (" << anoLancamento << ")" << endl;
    cout << "Duracao: " << duracao << endl;
    float media = calcularMedia();
    // Exibe média de avaliações com símbolo de estrela
    cout << "Media: " << media << " ★" << endl;
    cout << "Avaliacoes: " << avaliacoes.size() << endl;
    if (!avaliacoes.empty()) {
        cout << "------------------------------" << endl;
        for (auto av : avaliacoes) {
            if (!av) {
                continue;
            }
            if (av->isOculto()) {
                continue; // skip hidden evaluations in the public listing
            }
            // Mostrar a avaliação (autor, nota, comentário). A função não
            // imprime separador; imprimimos logo após o bloco para garantir
            // que as curtidas (se houver) fiquem associadas à avaliação acima.
            av->mostrarAvaliacao();
            // separador entre avaliações para melhor legibilidade
            cout << "------------------------------" << endl << endl;
        }
        cout << "------------------------------" << endl;
    }
}

const vector<Avaliacao*>& Musica::getAvaliacoes() const {
    return avaliacoes;
}
