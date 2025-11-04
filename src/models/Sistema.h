
#ifndef SISTEMA_H
#define SISTEMA_H

#include <vector>
#include <string>
#include "Musica.h"
#include "Artista.h"

// Classe responsável por gerenciar o sistema musical
class Sistema {
private:
    std::vector<Musica*> musicas; // Lista de músicas cadastradas
    std::vector<Artista*> artistas; // Lista de artistas cadastrados
public:
    // Construtor e destrutor
    Sistema();
    ~Sistema();

    // Cadastra um novo artista
    void cadastrarArtista(std::string nome, std::string genero);

    // Cadastra uma nova música associada a um artista
    void cadastrarMusica(std::string titulo, std::string genero, float duracao, std::string album, int ano, std::string capa, std::vector<std::string> plataformas, std::string artistaNome);

    // Lista todas as músicas cadastradas
    void listarMusicas() const;
};

#endif // SISTEMA_H
