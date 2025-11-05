
#ifndef SISTEMA_H
#define SISTEMA_H

#include <vector>
#include <string>
using namespace std;

class Usuario;
class Musica;
class Artista;
class Avaliacao;

// Classe responsável por gerenciar o sistema musical
class Sistema {
private:
    std::vector<Musica*> musicas; // Lista de músicas cadastradas
    std::vector<Artista*> artistas; // Lista de artistas cadastrados
public:
    // Construtor e destrutor
    Sistema();
    void cadastrarUsuario(string nome, string email);
    void cadastrarArtista(string nome, string genero);
    Artista* encontrarArtistaPorNome(const string& nome);
    void cadastrarMusica(string titulo, string genero, float duracao, string album, int ano, string capa, vector<string> plataformas, string artistaNome);
    void listarMusicas();
    const std::vector<Musica*>& getMusicas() const;
    void editarMusicaTitulo(const string& tituloAtual, const string& novoTitulo);
    void removerMusica(const string& titulo);
    void removerArtista(const string& nome);
    void editarArtistaNome(const string& nomeAtual, const string& novoNome);
    void salvarSistema(const string& caminho);
    void carregarSistema(const string& caminho);
    Musica* encontrarMusicaPorTitulo(const string& titulo);
    Usuario* encontrarUsuarioPorEmail(const string& email);
    void adicionarAvaliacao(Usuario* autor, const string& tituloMusica, int nota, const string& comentario, bool destaque);
    std::vector<class Avaliacao*> getAvaliacoesOcultas();
    void suspenderUsuario(const string& email, const string& motivo);
    void setStatusUsuario(const string& email, const string& status);
    void ensureAdminExists();

    // Lista todas as músicas cadastradas
    void listarMusicas() const;
};

#endif // SISTEMA_H
