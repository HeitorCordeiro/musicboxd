
#include "Musica.h"
#include "Artista.h"
#include "Avaliacao.h"
#include <iostream>

// Implementação dos métodos da classe Musica

// Construtor: inicializa todos os atributos da música
Musica::Musica(std::string titulo, std::string genero, float duracao, Artista* artista, std::string album, int ano, std::string capa, std::vector<std::string> plataformas)
    : titulo(titulo), genero(genero), duracao(duracao), album(album), anoLancamento(ano), capa(capa), plataformas(plataformas), artista(artista) {}

// Retorna o título da música
std::string Musica::getTitulo() const {
    return titulo;
}

// Retorna o ponteiro para o artista
Artista* Musica::getArtista() const {
    return artista;
}

// Adiciona uma avaliação à lista de avaliações
void Musica::adicionarAvaliacao(Avaliacao* avaliacao) {
    avaliacoes.push_back(avaliacao);
}

// Calcula a média das notas das avaliações
float Musica::calcularMedia() const {
    if (avaliacoes.empty()) return 0.0f;
    float soma = 0.0f;
    for (const auto& avaliacao : avaliacoes) {
        soma += avaliacao->getNota();
    }
    return soma / avaliacoes.size();
}

// Exibe todas as informações da música
void Musica::mostrarInfo() const {
    std::cout << "\nInformações da Música:\n";
    std::cout << "Título: " << titulo << "\n";
    std::cout << "Artista: " << (artista ? artista->getNome() : "<desconhecido>") << "\n";
    std::cout << "Gênero: " << genero << "\n";
    std::cout << "Duração: " << duracao << " segundos\n";
    std::cout << "Álbum: " << album << "\n";
    std::cout << "Ano de Lançamento: " << anoLancamento << "\n";
    std::cout << "Plataformas disponíveis:";
    for (const auto& plataforma : plataformas) {
        std::cout << " " << plataforma;
    }
    std::cout << "\nNota média: " << calcularMedia() << "\n";
}
