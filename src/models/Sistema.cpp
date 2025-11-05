
#include "Sistema.h"
#include "Artista.h"
#include "Musica.h"
#include "Avaliacao.h"
#include "AvaliacaoComDestaque.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cctype>

// Implementação dos métodos da classe Sistema

// Implementação central do domínio: a classe Sistema coordena usuários,
// músicas, artistas e avaliações. Operações de busca e modificação de
// estado ficam aqui; funções que lidam com I/O e persistência são colocadas
// mais abaixo para separar responsabilidades.

// Declaração adiantada (definida mais abaixo)
static string normalize(const string &s);

Sistema::Sistema() {

}

void Sistema::cadastrarUsuario(string nome, string email) {
    usuarios.push_back(new Usuario(nome, email));
}

void Sistema::cadastrarArtista(string nome, string genero) {
    // evitar duplicatas por nome (normalize: trim + case-insensitive)
    string nNome = normalize(nome);
    for (auto a : artistas) {
        if (a && normalize(a->getNome()) == nNome) {
            cout << "Artista já existe: " << nome << endl;
            return;
        }
    }
    artistas.push_back(new Artista(nome, genero));
}

Artista* Sistema::encontrarArtistaPorNome(const string& nome) {
    string nNome = normalize(nome);
    for (auto a : artistas) {
        if (a && normalize(a->getNome()) == nNome) return a;
    }
    return nullptr;
}

void Sistema::cadastrarMusica(string titulo, string genero, float duracao, string album, int ano, string capa, vector<string> plataformas, string artistaNome) {
    // requer artista (não permitir música sem artista)
    if (artistaNome.empty()) {
        cout << "Erro: é necessário informar um artista para cadastrar a música." << endl;
        return;
    }
    // evitar duplicatas de título
    if (encontrarMusicaPorTitulo(titulo)) {
        cout << "Já existe uma música com o título: " << titulo << endl;
        return;
    }
    // procurar (ou criar) artista associado
    Artista* art = encontrarArtistaPorNome(artistaNome);
    if (!art) {
        art = new Artista(artistaNome, "desconhecido");
        artistas.push_back(art);
    }
    Musica* m = new Musica(titulo, genero, duracao, art, album, ano, capa, plataformas);
    musicas.push_back(m);
    if (art) art->adicionarMusica(m);
}

void Sistema::listarMusicas() {
    cout << "Lista de musicas (" << musicas.size() << "):" << endl;
    for (auto m : musicas) {
        if (m) {
            m->mostrarInfo();
        }
    }
}

Musica* Sistema::encontrarMusicaPorTitulo(const string& titulo) {
    string nTitulo = normalize(titulo);
    for (auto m : musicas) {
        if (m && normalize(m->getTitulo()) == nTitulo) {
            return m;
        }
    }
    return nullptr;
}

Usuario* Sistema::encontrarUsuarioPorEmail(const string& email) {
    for (auto u : usuarios) {
        if (u && u->getEmail() == email) {
            return u;
        }
    }
    return nullptr;
}

void Sistema::adicionarAvaliacao(Usuario* autor, const string& tituloMusica, int nota, const string& comentario, bool destaque) {
    Musica* m = encontrarMusicaPorTitulo(tituloMusica);
    if (!m) {
        cout << "Musica '" << tituloMusica << "' nao encontrada." << endl;
        return;
    }
    Avaliacao* av = nullptr;
    if (destaque) {
        av = new AvaliacaoComDestaque(autor, m, nota, comentario, std::vector<string>());
    } else {
        av = new Avaliacao(autor, m, nota, comentario);
    }
    if (autor) {
        autor->adicionarAvaliacao(av);
    } else {
        m->adicionarAvaliacao(av);
    }
}

std::vector<Avaliacao*> Sistema::getAvaliacoesOcultas() {
    std::vector<Avaliacao*> out;
    for (auto m : musicas) {
        if (!m) continue;
        for (auto av : m->getAvaliacoes()) {
            if (!av) continue;
            if (av->isOculto()) out.push_back(av);
        }
    }
    return out;
}

void Sistema::suspenderUsuario(const string& email, const string& motivo) {
    Usuario* u = encontrarUsuarioPorEmail(email);
    if (!u) {
        cout << "Usuario não encontrado: " << email << endl;
        return;
    }
    u->suspender(motivo);
    cout << "Usuario " << email << " suspenso." << endl;
}

void Sistema::setStatusUsuario(const string& email, const string& status) {
    Usuario* u = encontrarUsuarioPorEmail(email);
    if (!u) {
        cout << "Usuario não encontrado: " << email << endl;
        return;
    }
    u->setStatus(status);
    cout << "Status de " << email << " definido para: " << status << endl;
}

void Sistema::ensureAdminExists() {
    // cria um admin padrão se não existir
    Usuario* adm = encontrarUsuarioPorEmail("adm@adm.com");
    if (!adm) {
        cadastrarUsuario(string("Administrator"), string("adm@adm.com"));
        adm = encontrarUsuarioPorEmail("adm@adm.com");
        if (adm) {
            adm->setAdmin(true);
            // Definir senha padrão "adm" conforme solicitado. Isso evita o fluxo
            // interativo de criação de senha na primeira execução.
            adm->setSenha("adm");
            cout << "Usuario admin criado: adm@adm.com com senha padrão 'adm'" << endl;
            // salvarSistema será chamado externamente quando necessário
        }
    }
}

const std::vector<Musica*>& Sistema::getMusicas() const {
    return musicas;
}

void Sistema::editarMusicaTitulo(const string& tituloAtual, const string& novoTitulo) {
    Musica* m = encontrarMusicaPorTitulo(tituloAtual);
    if (!m) {
        cout << "Musica não encontrada: " << tituloAtual << endl;
        return;
    }
    m->setTitulo(novoTitulo);
}

void Sistema::removerMusica(const string& titulo) {
    Musica* m = encontrarMusicaPorTitulo(titulo);
    if (!m) {
        cout << "Musica não encontrada: " << titulo << endl;
        return;
    }
    // primeiro, remover e deletar avaliações associadas
    auto avals = m->getAvaliacoes();
    // copiar ponteiros para evitar modificar o vetor enquanto iteramos
    vector<Avaliacao*> copia(avals.begin(), avals.end());
    for (auto av : copia) {
        if (!av) continue;
        Usuario* autor = av->getAutor();
        if (autor) {
            // remover via usuario - isso também remove da música e deleta a avaliação
            autor->removerAvaliacao(av);
        } else {
            // sem autor, remover diretamente da música e deletar
            m->removerAvaliacao(av);
            delete av;
        }
    }
    // remover da lista do artista
    Artista* art = m->getArtista();
    if (art) {
        art->removerMusica(m);
    }
    // remover da lista do sistema
    for (auto it = musicas.begin(); it != musicas.end(); ++it) {
        if (*it == m) {
            musicas.erase(it);
            break;
        }
    }
    // finalmente deletar o objeto Musica
    delete m;
}

void Sistema::removerArtista(const string& nome) {
    // procurar artista
    Artista* alvo = nullptr;
    string nNome = normalize(nome);
    for (auto a : artistas) {
        if (a && normalize(a->getNome()) == nNome) { alvo = a; break; }
    }
    if (!alvo) {
        cout << "Artista não encontrado: " << nome << endl;
        return;
    }
    // remover todas as musicas do artista
    // copiar lista para evitar mutação durante iteração
    // iterar sobre musicas do sistema e remover as que pertencem ao artista
    vector<string> titulosParaRemover;
    for (auto m : musicas) {
        if (m && m->getArtista() == alvo) {
            titulosParaRemover.push_back(m->getTitulo());
        }
    }
    for (auto &t : titulosParaRemover) {
        removerMusica(t);
    }
    // remover artista da lista do sistema
    for (auto it = artistas.begin(); it != artistas.end(); ++it) {
        if (*it == alvo) {
            artistas.erase(it);
            break;
        }
    }
    delete alvo;
}

void Sistema::editarArtistaNome(const string& nomeAtual, const string& novoNome) {
    // Valida parâmetros de entrada; se estiverem vazios, cancela a operação.
    if (nomeAtual.empty() || novoNome.empty()) {
        return;
    }
    // procurar artista pelo nome normalizado
    string nAtual = normalize(nomeAtual);
    for (auto a : artistas) {
        if (a && normalize(a->getNome()) == nAtual) {
            a->setNome(novoNome);
            return;
        }
    }
    cout << "Artista não encontrado: " << nomeAtual << endl;
}

// Escape simples para JSON
static string esc(const string& s) {
    string out;
    for (char c : s) {
        if (c == '"') {
            out += "\\\"";
        } else if (c == '\\') {
            out += "\\\\";
        } else if (c == '\n') {
            out += "\\n";
        } else {
            out += c;
        }
    }
    return out;
}

// Normaliza uma string para comparação: trim + ASCII lowercase
static string normalize(const string &s) {
    size_t start = 0;
    size_t end = s.size();
    while (start < end && isspace((unsigned char)s[start])) ++start;
    while (end > start && isspace((unsigned char)s[end-1])) --end;
    string out;
    out.reserve(end - start);
    for (size_t i = start; i < end; ++i) {
        unsigned char c = (unsigned char)s[i];
        out.push_back((char)std::tolower(c));
    }
    return out;
}

// Observação: as rotinas de persistência (salvar/carregar) são colocadas
// mais abaixo no arquivo para melhor separação entre lógica de domínio e
// operações de I/O. A implementação de salvarSistema foi movida para o final
// deste arquivo.

static string extractArray(const string& s, const string& key) {
    size_t p = s.find("\"" + key + "\"");
    if (p == string::npos) return string();
    size_t b = s.find('[', p);
    if (b == string::npos) return string();
    size_t level = 1;
    size_t i = b + 1;
    for (; i < s.size(); ++i) {
        if (s[i] == '[') ++level;
        else if (s[i] == ']') {
            --level;
            if (level == 0) break;
        }
    }
    if (i >= s.size()) return string();
    return s.substr(b + 1, i - (b + 1));
}

static vector<string> splitObjects(const string& arrContent) {
    vector<string> out;
    size_t i = 0;
    while (i < arrContent.size()) {
        while (i < arrContent.size() && isspace((unsigned char)arrContent[i])) ++i;
        if (i >= arrContent.size()) break;
        if (arrContent[i] != '{') {
            ++i;
            continue;
        }
        size_t level = 1;
        size_t j = i + 1;
        for (; j < arrContent.size(); ++j) {
            if (arrContent[j] == '{') ++level;
            else if (arrContent[j] == '}') {
                --level;
                if (level == 0) break;
            }
        }
        if (j < arrContent.size()) {
            out.push_back(arrContent.substr(i, j - i + 1));
            i = j + 1;
        } else {
            break;
        }
    }
    return out;
}

static string extractStringField(const string& obj, const string& key) {
    string pattern = "\"" + key + "\"";
    size_t p = obj.find(pattern);
    if (p == string::npos) return string();
    size_t q = obj.find(':', p);
    if (q == string::npos) return string();
    size_t firstQuote = obj.find('"', q);
    if (firstQuote == string::npos) return string();
    size_t secondQuote = obj.find('"', firstQuote + 1);
    if (secondQuote == string::npos) return string();
    return obj.substr(firstQuote + 1, secondQuote - (firstQuote + 1));
}

static int extractIntField(const string& obj, const string& key) {
    string pattern = "\"" + key + "\"";
    size_t p = obj.find(pattern);
    if (p == string::npos) return 0;
    size_t q = obj.find(':', p);
    if (q == string::npos) return 0;
    size_t start = q + 1;
    while (start < obj.size() && isspace((unsigned char)obj[start])) ++start;
    size_t end = start;
    while (end < obj.size() && (isdigit((unsigned char)obj[end]) || obj[end]=='-' )) ++end;
    if (end <= start) return 0;
    try {
        return stoi(obj.substr(start, end-start));
    } catch(...) { return 0; }
}

static vector<string> extractStringArrayField(const string& obj, const string& key) {
    vector<string> result;
    string pattern = "\"" + key + "\"";
    size_t p = obj.find(pattern);
    if (p == string::npos) return result;
    size_t b = obj.find('[', p);
    if (b == string::npos) return result;
    size_t i = b + 1;
    while (i < obj.size()) {
        // find next quote
        size_t q = obj.find('"', i);
        if (q == string::npos) break;
        string acc;
        size_t j = q + 1;
        for (; j < obj.size(); ++j) {
            if (obj[j] == '\\' && j + 1 < obj.size()) {
                // skip escape, include next char
                ++j;
                continue;
            }
            if (obj[j] == '"') break;
            acc.push_back(obj[j]);
        }
        if (j >= obj.size()) break;
        result.push_back(acc);
        i = j + 1;
        // check if array ended
        size_t nextBracket = obj.find(']', i);
        size_t nextQuote = obj.find('"', i);
        if (nextBracket != string::npos && (nextQuote == string::npos || nextBracket < nextQuote)) break;
    }
    return result;
}

void Sistema::carregarSistema(const string& caminho) {
    ifstream in(caminho);
    if (!in.is_open()) {
        // nada a carregar
        return;
    }
    stringstream ss;
    ss << in.rdbuf();
    string content = ss.str();
    in.close();
    // carregar usuarios via Usuario helper
    vector<Usuario*> loaded = Usuario::carregarUsuarios("users.db");
    for (auto u : loaded) {
        if (u) usuarios.push_back(u);
    }
    // artists
    string artistsArr = extractArray(content, string("artists"));
    vector<string> artistObjs = splitObjects(artistsArr);
    for (auto &ao : artistObjs) {
        string name = extractStringField(ao, "name");
        if (!name.empty()) {
            cadastrarArtista(name, string("desconhecido"));
        }
    }
    // musics
    string musicsArr = extractArray(content, string("musics"));
    vector<string> musicObjs = splitObjects(musicsArr);
    for (auto &mo : musicObjs) {
        string title = extractStringField(mo, "title");
        string artistName = extractStringField(mo, "artist");
        if (!title.empty()) {
            cadastrarMusica(title, string("desconhecido"), 0.0f, string(""), 0, string(""), vector<string>(), artistName);
        }
    }
    // evaluations
    string evalsArr = extractArray(content, string("evaluations"));
    vector<string> evalObjs = splitObjects(evalsArr);
    for (auto &eo : evalObjs) {
        string music = extractStringField(eo, "music");
        string author = extractStringField(eo, "author");
        int rating = extractIntField(eo, "rating");
        string comment = extractStringField(eo, "comment");
        int hidden = extractIntField(eo, "hidden");
        string type = extractStringField(eo, "type");
        int likes = extractIntField(eo, "likes");
        vector<string> likedBy = extractStringArrayField(eo, string("liked_by"));
        Usuario* ua = encontrarUsuarioPorEmail(author);
        Musica* um = encontrarMusicaPorTitulo(music);
        if (um) {
            if (type == "destaque") {
                AvaliacaoComDestaque* av = new AvaliacaoComDestaque(ua, um, rating, comment, likedBy);
                if (hidden) {
                    av->ocultar();
                }
                if (ua) {
                    ua->adicionarAvaliacao(av);
                } else {
                    um->adicionarAvaliacao(av);
                }
            } else {
                Avaliacao* av = new Avaliacao(ua, um, rating, comment);
                if (hidden) {
                    av->ocultar();
                }
                if (ua) {
                    ua->adicionarAvaliacao(av);
                } else {
                    um->adicionarAvaliacao(av);
                }
            }
        }
    }
}

// Persiste o estado do sistema em um arquivo JSON simples.
// Esta função serializa artistas, músicas e avaliações.
void Sistema::salvarSistema(const string& caminho)
{
    ofstream out(caminho, ios::trunc);
    if (!out.is_open())
    {
        cout << "Erro: nao foi possivel abrir " << caminho << " para escrita." << endl;
        return;
    }
    out << "{" << endl;
    // artistas
    out << "  \"artists\": [" << endl;
    for (size_t i = 0; i < artistas.size(); ++i)
    {
        Artista* a = artistas[i];
        if (!a) continue;
        out << "    { \"name\": \"" << esc(a->getNome()) << "\" }";
        if (i + 1 < artistas.size()) out << ",";
        out << endl;
    }
    out << "  ]," << endl;
    // musics
    out << "  \"musics\": [" << endl;
    for (size_t i = 0; i < musicas.size(); ++i)
    {
        Musica* m = musicas[i];
        if (!m) continue;
        out << "    { \"title\": \"" << esc(m->getTitulo()) << "\", \"artist\": \"" << esc(m->getArtista() ? m->getArtista()->getNome() : string("")) << "\" }";
        if (i + 1 < musicas.size()) out << ",";
        out << endl;
    }
    out << "  ]," << endl;
    // avaliacoes
    out << "  \"evaluations\": [" << endl;
    bool firstEval = true;
    for (auto m : musicas)
    {
        if (!m) continue;
        for (auto av : m->getAvaliacoes())
        {
            if (!av) continue;
            if (!firstEval) out << "," << endl;
            out << "    { \"music\": \"" << esc(m->getTitulo()) << "\", \"author\": \"" << esc(av->getAutor() ? av->getAutor()->getEmail() : string("")) << "\", \"rating\": " << av->getNota() << ", \"comment\": \"" << esc(av->getComentario()) << "\", \"hidden\": " << (av->isOculto() ? 1 : 0);
            AvaliacaoComDestaque* ad = dynamic_cast<AvaliacaoComDestaque*>(av);
            if (ad)
            {
                out << ", \"type\": \"destaque\", \"liked_by\": [";
                const auto &likesVec = ad->getCurtidasPor();
                for (size_t li = 0; li < likesVec.size(); ++li)
                {
                    out << "\"" << esc(likesVec[li]) << "\"";
                    if (li + 1 < likesVec.size()) out << ", ";
                }
                out << "]";
                out << ", \"likes\": " << ad->getCurtidas();
            }
            else
            {
                out << ", \"type\": \"normal\"";
            }
            out << " }";
            firstEval = false;
        }
    }
    out << endl << "  ]" << endl;
    out << "}" << endl;
    out.close();
    // salvar usuarios também (persistência separada)
    Usuario::salvarUsuarios("users.db", usuarios);
}
