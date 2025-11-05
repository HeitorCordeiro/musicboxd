
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
    Musica(string titulo, string genero, float duracao, Artista* artista, string album, int ano, string capa, vector<string> plataformas);
    string getTitulo();
    Artista* getArtista();
    void setTitulo(const string& novoTitulo);
    void adicionarAvaliacao(Avaliacao* avaliacao);
    void removerAvaliacao(Avaliacao* avaliacao);
    float calcularMedia();
    void mostrarInfo();
    const vector<Avaliacao*>& getAvaliacoes() const;

    // Calcula a média das avaliações
    float calcularMedia() const;

    // Exibe as informações da música
    void mostrarInfo() const;
};

#endif // MUSICA_H