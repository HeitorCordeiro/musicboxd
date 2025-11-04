
#include "Sistema.h"
#include "Artista.h"
#include "Musica.h"
#include <iostream>

// Implementação dos métodos da classe Sistema

// Construtor: inicializa o sistema
Sistema::Sistema() {}

// Destrutor: libera memória das músicas e artistas
Sistema::~Sistema() {
    for (auto m : musicas) delete m;
    for (auto a : artistas) delete a;
}

// Cadastra um novo artista no sistema
void Sistema::cadastrarArtista(std::string nome, std::string genero) {
    Artista* novoArtista = new Artista(nome, genero);
    artistas.push_back(novoArtista);
    std::cout << "Artista cadastrado com sucesso!\n";
}

// Cadastra uma nova música associada a um artista existente
void Sistema::cadastrarMusica(std::string titulo, std::string genero, float duracao, std::string album, int ano, std::string capa, std::vector<std::string> plataformas, std::string artistaNome) {
    Artista* artista = nullptr;
    for (auto a : artistas) {
        if (a->getNome() == artistaNome) {
            artista = a;
            break;
        }
    }
    if (!artista) {
        std::cout << "Erro: Artista não encontrado!\n";
        return;
    }
    Musica* novaMusica = new Musica(titulo, genero, duracao, artista, album, ano, capa, plataformas);
    musicas.push_back(novaMusica);
    std::cout << "Música cadastrada com sucesso!\n";
}

// Lista todas as músicas cadastradas no sistema
void Sistema::listarMusicas() const {
    if (musicas.empty()) {
        std::cout << "Não há músicas cadastradas no sistema.\n";
        return;
    }
    std::cout << "\n=== Lista de Músicas ===\n";
    for (const auto& musica : musicas) {
        musica->mostrarInfo();
        std::cout << "------------------------\n";
    }
}