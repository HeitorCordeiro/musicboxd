#include "Usuario.h"
#include "Avaliacao.h"
#include "AvaliacaoComDestaque.h"
#include "Lista.h"
#include "Musica.h"
#include <iostream>

using namespace std;

Usuario::Usuario(string nome, string email, bool admin)
    : nome(nome), email(email), admin(admin), status("ativo") {
    // implementação mínima
}

string Usuario::getNome() {
    return nome;
}

string Usuario::getEmail() {
    return email;
}

string Usuario::getStatus() {
    return status;
}

void Usuario::suspender(string motivo) {
    status = "suspenso";
    adicionarAviso(motivo);
}

void Usuario::adicionarAviso(string msg) {
    avisos.push_back(msg);
}

void Usuario::seguir(Usuario* outroUsuario) {
   
    if (!outroUsuario) return;

    seguindo.push_back(outroUsuario);

    outroUsuario->seguidores.push_back(this);

}

void Usuario::listarSeguindo() {

    cout << "Seguindo (" << seguindo.size() << "):\n";

    for (auto u : seguindo) {
        cout << " - " << (u ? u->getNome() : string("<desconhecido>")) << "\n";
    }
}

void Usuario::listarSeguidores() {

    cout << "Seguidores (" << seguidores.size() << "):\n";

    for (auto u : seguidores) {
        cout << " - " << (u ? u->getNome() : string("<desconhecido>")) << "\n";
    }
}

void Usuario::adicionarAvaliacao(Avaliacao* avaliacao) {
   
    if (!avaliacao) return;

    avaliacoes.push_back(avaliacao);

    Musica* m = avaliacao->getMusica();

    if (m) m->adicionarAvaliacao(avaliacao);

}

void Usuario::criarLista(string nomeLista) {
    listas.push_back(new Lista(nomeLista));
}

void Usuario::curtirAvaliacao(Avaliacao* avaliacao) {
    
    if (!avaliacao) return;

    AvaliacaoComDestaque* destaque = dynamic_cast<AvaliacaoComDestaque*>(avaliacao);

    if (destaque) {
        destaque->curtir();
    }else{
        cout << "Avaliacao sem destaque não pode ser curtida." << endl;
    } 
}

void Usuario::mostrarPerfil() {
    cout << "Usuario: " << nome << " <" << email << ">\n";
    cout << "Status: " << status << "\n";
    cout << "Avisos: " << avisos.size() << "\n";
    for (auto &a : avisos) {
        cout << " - " << a << "\n";
    }
    listarSeguindo();
    listarSeguidores();
}
