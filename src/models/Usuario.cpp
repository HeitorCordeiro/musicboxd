#include "Usuario.h"
#include "Avaliacao.h"
#include "AvaliacaoComDestaque.h"
#include "Lista.h"
#include "Musica.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <unordered_map>

using namespace std;

// Este arquivo implementa a lógica associada à entidade Usuario.
// As funções aqui gerenciam comportamento do usuário (seguir, listar,
// gerenciar avaliações) e operações de persistência simples.

// Construtor do usuário
Usuario::Usuario(string nome, string email, bool admin)
    : nome(nome), email(email), admin(admin), status("ativo") {
    senhaHash = 0;
}

// Observação: esta classe usa um hash protótipo (std::hash) com um salt fixo.
// Para produção, substitua por bcrypt/argon2 para segurança.

string Usuario::getNome(){
    return nome;
}

string Usuario::getEmail(){
    return email;
}

string Usuario::getStatus(){
    return status;
}

bool Usuario::isAdmin() const{
    return admin;
}

// Define o usuário como administrador (true) ou não (false).
void Usuario::setAdmin(bool v){
    admin = v;
}

// Atualiza o status do usuário.
void Usuario::setStatus(const string& s){
    status = s;
}

bool Usuario::hasPassword() const {
    return senhaHash != 0;
}

void Usuario::suspender(string motivo) {
    status = "suspenso";
    adicionarAviso(motivo);
}

void Usuario::adicionarAviso(string msg) {
    avisos.push_back(msg);
}

void Usuario::seguir(Usuario* outroUsuario) {
    if (!outroUsuario) {
        return;
    }
    // nao permitir seguir a si mesmo
    if (outroUsuario->getEmail() == this->getEmail()) {
        cout << "Você não pode seguir a si mesmo." << endl;
        return;
    }
    // evitar duplicatas
    for (auto u : seguindo) {
        if (u && u->getEmail() == outroUsuario->getEmail()) {
            cout << "Você já segue esse usuário." << endl;
            return;
        }
    }
    seguindo.push_back(outroUsuario);
    outroUsuario->seguidores.push_back(this);
    cout << "Agora você segue: " << outroUsuario->getNome() << " <" << outroUsuario->getEmail() << ">" << endl;
}

void Usuario::pararDeSeguir(Usuario* outroUsuario) {
    // Verifica se o parâmetro é válido; cancela a operação se for nulo.
    if (!outroUsuario) {
        return;
    }
    // remover de seguindo
    for (auto it = seguindo.begin(); it != seguindo.end(); ++it) {
        if (*it == outroUsuario) {
            seguindo.erase(it);
            break;
        }
    }
    // remover de seguidores do outro
    for (auto it = outroUsuario->seguidores.begin(); it != outroUsuario->seguidores.end(); ++it) {
        if (*it == this) {
            outroUsuario->seguidores.erase(it);
            break;
        }
    }
    cout << "Você deixou de seguir: " << outroUsuario->getNome() << " <" << outroUsuario->getEmail() << ">" << endl;
}

void Usuario::listarSeguindo() {
    cout << "Seguindo (" << seguindo.size() << "):" << endl;
    for (auto u : seguindo) {
        cout << " - " << (u ? u->getNome() : string("<desconhecido>")) << endl;
    }
}

void Usuario::listarSeguidores() {
    cout << "Seguidores (" << seguidores.size() << "):" << endl;
    for (auto u : seguidores) {
        cout << " - " << (u ? u->getNome() : string("<desconhecido>")) << endl;
    }
}

void Usuario::adicionarAvaliacao(Avaliacao* avaliacao) {
    if (!avaliacao) {
        return;
    }
    avaliacoes.push_back(avaliacao);
    Musica* m = avaliacao->getMusica();
    if (m) {
        m->adicionarAvaliacao(avaliacao);
    }
}

void Usuario::removerAvaliacao(Avaliacao* avaliacao) {
    // Verifica se o ponteiro de avaliação é válido.
    if (!avaliacao) {
        return;
    }
    // remover da lista do usuário
    for (auto it = avaliacoes.begin(); it != avaliacoes.end(); ++it) {
        if (*it == avaliacao) {
            avaliacoes.erase(it);
            break;
        }
    }
    // remover da música associada
    Musica* m = avaliacao->getMusica();
    if (m) {
        m->removerAvaliacao(avaliacao);
    }
    // liberar memória da avaliação
    delete avaliacao;
}

const std::vector<Avaliacao*>& Usuario::getAvaliacoes() const {
    return avaliacoes;
}

void Usuario::criarLista(string nomeLista) {
    listas.push_back(new Lista(nomeLista));
}

void Usuario::curtirAvaliacao(Avaliacao* avaliacao) {
    if (!avaliacao) {
        return;
    }
    AvaliacaoComDestaque* destaque = dynamic_cast<AvaliacaoComDestaque*>(avaliacao);
    if (destaque) {
        // nao permitir curtir propria avaliacao
        //if (avaliacao->getAutor() && avaliacao->getAutor()->getEmail() == this->getEmail()) {
        //    cout << "Você não pode curtir sua própria avaliação." << endl;
        //    return;
        //}
        // toggle: se já curtiu, descurte; caso contrário, curte
        if (destaque->jaCurtiu(this->getEmail())) {
            destaque->descurtir(this);
            cout << "Avaliação descurtida." << endl;
        } else {
            destaque->curtir(this);
            cout << "Avaliação curtida." << endl;
        }
    } else {
        cout << "Avaliacao sem destaque não pode ser curtida." << endl;
    }
}

void Usuario::mostrarPerfil() {
    cout << "Usuario: " << nome << " <" << email << ">" << endl;
    cout << "Status: " << status << endl;
    cout << "Avisos: " << avisos.size() << endl;
    for (auto &a : avisos) {
        cout << " - " << a << endl;
    }
    listarSeguindo();
    listarSeguidores();
}

// senha e persistencia
void Usuario::setSenha(const string& senha) {
    const string salt = "musicboxd_salt_v1";
    std::hash<string> hasher;
    senhaHash = hasher(senha + salt);
}

bool Usuario::verificarSenha(const string& senha) const {
    const string salt = "musicboxd_salt_v1";
    std::hash<string> hasher;
    return senhaHash == hasher(senha + salt);
}

string Usuario::toStorageLine() const {
    ostringstream oss;
    oss << nome << "|" << email << "|" << senhaHash << "|" << (admin ? 1 : 0) << "|" << status;
    // adicionar lista de emails que este usuario segue (campo opcional, separado por vírgula)
    if (!seguindo.empty()) {
        oss << "|";
        for (size_t i = 0; i < seguindo.size(); ++i) {
            if (seguindo[i]) {
                oss << seguindo[i]->getEmail();
            }
            if (i + 1 < seguindo.size()) oss << ",";
        }
    }
    return oss.str();
}

vector<Usuario*> Usuario::carregarUsuarios(const string& caminho) {
    vector<Usuario*> resultado;
    ifstream in(caminho);
    if (!in.is_open()) return resultado;
    string linha;
    // armazenar temporariamente listas de seguindo como CSV para resolver após criar todos os usuários
    vector<vector<string>> seguindoCsvPerUser;
    while (getline(in, linha)) {
        if (linha.empty()) continue;
        // split by '|'
        vector<string> parts;
        size_t start = 0;
        while (start <= linha.size()) {
            size_t pos = linha.find('|', start);
            if (pos == string::npos) {
                parts.push_back(linha.substr(start));
                break;
            } else {
                parts.push_back(linha.substr(start, pos - start));
                start = pos + 1;
            }
        }
        if (parts.size() < 5) continue; // formato inesperado
        string nome_ = parts[0];
        string email_ = parts[1];
        string hashStr = parts[2];
        string adminStr = parts[3];
        string status_ = parts[4];
        Usuario* u = new Usuario(nome_, email_, adminStr=="1");
        try {
            u->senhaHash = stoull(hashStr);
        } catch(...) {
            u->senhaHash = 0;
        }
        u->status = status_;
        resultado.push_back(u);
        // parsear lista de seguindo, se existir (parte 5)
        vector<string> follows;
        if (parts.size() >= 6) {
            string csv = parts[5];
            size_t s = 0;
            while (s < csv.size()) {
                size_t comma = csv.find(',', s);
                if (comma == string::npos) {
                    string item = csv.substr(s);
                    if (!item.empty()) follows.push_back(item);
                    break;
                } else {
                    string item = csv.substr(s, comma - s);
                    if (!item.empty()) follows.push_back(item);
                    s = comma + 1;
                }
            }
        }
        seguindoCsvPerUser.push_back(follows);
    }
    in.close();

    // resolver emails para ponteiros e preencher seguindo/seguidores
    // construir mapa email -> Usuario*
    unordered_map<string, Usuario*> mapa;
    for (auto u : resultado) {
        if (u) mapa[u->getEmail()] = u;
    }
    for (size_t i = 0; i < resultado.size(); ++i) {
        Usuario* u = resultado[i];
        if (!u) continue;
        const auto &csv = (i < seguindoCsvPerUser.size() ? seguindoCsvPerUser[i] : vector<string>());
        for (const auto &emailSeg : csv) {
            auto it = mapa.find(emailSeg);
            if (it != mapa.end()) {
                Usuario* alvo = it->second;
                // evitar duplicatas
                bool exists = false;
                for (auto s : u->seguindo) { if (s && s->getEmail() == alvo->getEmail()) { exists = true; break; } }
                if (!exists) {
                    u->seguindo.push_back(alvo);
                    alvo->seguidores.push_back(u);
                }
            }
        }
    }
    return resultado;
}

void Usuario::salvarUsuarios(const string& caminho, const vector<Usuario*>& usuarios) {
    ofstream out(caminho, ios::trunc);
    if (!out.is_open()) {
        // Não foi possível abrir o arquivo de usuários; retorna sem carregar nada.
        return;
    }
    for (auto u : usuarios) {
        if (!u) continue;
        // construir CSV de seguindo
        ostringstream oss;
        for (size_t i = 0; i < u->seguindo.size(); ++i) {
            if (u->seguindo[i]) {
                oss << u->seguindo[i]->getEmail();
            }
            if (i + 1 < u->seguindo.size()) oss << ",";
        }
        // escrever linha: nome|email|hash|admin|status|follow1,follow2
        out << u->getNome() << "|" << u->getEmail() << "|" << u->senhaHash << "|" << (u->admin ? 1 : 0) << "|" << u->status;
        string csv = oss.str();
        if (!csv.empty()) out << "|" << csv;
        out << "\n";
    }
    out.close();
}
