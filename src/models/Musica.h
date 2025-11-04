
#ifndef MUSICA_H
#define MUSICA_H

#include <string>
#include <vector>

// Representa uma música no sistema
class Artista;
class Avaliacao;

class Musica {
private:
    std::string titulo; // Título da música
    std::string genero; // Gênero musical
    float duracao; // Duração em segundos
    std::string album; // Nome do álbum
    int anoLancamento; // Ano de lançamento
    std::string capa; // Caminho ou nome da imagem de capa
    std::vector<std::string> plataformas; // Plataformas onde a música está disponível
    Artista* artista; // Ponteiro para o artista
    std::vector<Avaliacao*> avaliacoes; // Avaliações recebidas
public:
    // Construtor da música
    Musica(std::string titulo, std::string genero, float duracao, Artista* artista, std::string album, int ano, std::string capa, std::vector<std::string> plataformas);

    // Retorna o título da música
    std::string getTitulo() const;

    // Retorna o ponteiro para o artista
    Artista* getArtista() const;

    // Adiciona uma avaliação à música
    void adicionarAvaliacao(Avaliacao* avaliacao);

    // Calcula a média das avaliações
    float calcularMedia() const;

    // Exibe as informações da música
    void mostrarInfo() const;
};

#endif // MUSICA_H