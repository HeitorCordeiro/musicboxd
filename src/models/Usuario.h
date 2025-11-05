// Arquivo: Usuario.h
#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <vector>
using namespace std;

class Avaliacao;
class Lista;
class Usuario {
private:
    string nome;
    string email;
    bool admin;
    string status;
    vector<string> avisos;
    vector<Usuario*> seguindo;
    vector<Usuario*> seguidores;
    vector<Avaliacao*> avaliacoes;
    vector<Lista*> listas;
    size_t senhaHash;
public:
    Usuario(string nome, string email, bool admin=false);

    void setSenha(const string& senha);
    bool verificarSenha(const string& senha) const;

    string toStorageLine() const;
    static vector<Usuario*> carregarUsuarios(const string& caminho);
    static void salvarUsuarios(const string& caminho, const vector<Usuario*>& usuarios);
    string getNome();
    string getEmail();
    string getStatus();
    void suspender(string motivo);
    void adicionarAviso(string msg);
    void seguir(Usuario* outroUsuario);
    void pararDeSeguir(Usuario* outroUsuario);
    void listarSeguindo();
    void listarSeguidores();
    void adicionarAvaliacao(Avaliacao* avaliacao);
    void removerAvaliacao(Avaliacao* avaliacao);
    const std::vector<Avaliacao*>& getAvaliacoes() const;
    void criarLista(string nomeLista);
    void curtirAvaliacao(Avaliacao* avaliacao);
    void mostrarPerfil();

    bool isAdmin() const;
    void setAdmin(bool v);
    void setStatus(const string& s);
    bool hasPassword() const;

};

#endif // USUARIO_H
