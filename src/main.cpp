#include <locale>
// Interface de terminal: protótipo de login/registro
// Este arquivo implementa a interface de terminal (linha de comando) do MusicBoxd.
#include <iostream>
#include <string>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include "models/Sistema.h"
#include "models/Usuario.h"
#include "models/Musica.h"
#include "models/Avaliacao.h"
#include "models/AvaliacaoComDestaque.h"
#include "models/Artista.h"

using namespace std;

static string readPassword(const string& prompt) {
    cout << prompt;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    string senha;
    getline(cin, senha);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;
    return senha;
}

static void clearScreen() { system("clear"); }

int main() {
    // Ajusta a localidade para suportar símbolos UTF-8 (ex.: ♥, ★) no terminal
    std::setlocale(LC_ALL, "");
    const string usersFile = "users.db";
    const string systemFile = "system.json";
    Sistema sistema;
    sistema.carregarSistema(systemFile);
    // Garantir usuário admin padrão existe (criado apenas se ausente)
    sistema.ensureAdminExists();
    // Se o admin padrão não tiver senha, pedir que o usuário defina uma agora
    Usuario* admUser = sistema.encontrarUsuarioPorEmail("adm@adm.com");
    if (admUser && !admUser->hasPassword()) {
        // Definir senha padrão não interativa conforme solicitado
        admUser->setSenha("adm");
        sistema.salvarSistema(systemFile);
    cout << "Conta admin: adm@adm.com criada/ajustada com senha padrão 'adm'." << endl;
    cout << "Pressione Enter para continuar..." << endl;
    string tmp;
    getline(cin, tmp);
    }

    bool running = true;
    while (running) {
        clearScreen();
        cout << "=== MUSICBOXD - Login / Registrar ===" << endl;
        cout << "1) Login" << endl << "2) Registrar" << endl << "0) Sair" << endl;
        cout << "Escolha: ";
        string escolha;
        if (!getline(cin, escolha)) break;
        // limpar a tela logo apos o usuario fazer uma escolha de opcao
        clearScreen();

        if (escolha == "1") {
            // limpar tela ao entrar no fluxo de login
            clearScreen();
            cout << "== LOGIN ==" << endl;
            cout << "Email: ";
            string email;
            getline(cin, email);
            string senha = readPassword("Senha: ");
            Usuario* ativo = sistema.encontrarUsuarioPorEmail(email);
            if (!ativo) {
                cout << "Usuário não encontrado. Pressione Enter para continuar..." << endl;
                string tmp;
                getline(cin, tmp);
                continue;
            }
            if (!ativo->verificarSenha(senha)) {
                cout << "Senha incorreta. Pressione Enter para continuar..." << endl;
                string tmp;
                getline(cin, tmp);
                continue;
            }
            // sessão do usuário
            bool session = true;
            while (session) {
                clearScreen();
             // Cabeçalho da sessão do usuário
             cout << "Bem-vindo, " << ativo->getNome() << endl;
             // Opções principais da aplicação. Símbolos ajudam a destacar ações.
                 cout << "1) Ver perfil" << endl
                 << "2) Listar músicas" << endl
                 << "3) Cadastrar artista/música" << endl
                 << "4) Fazer avaliação (★ nota, comentário)" << endl
                 << "5) Gerenciar minhas avaliações" << endl;
                // se usuário for admin, apresentar opções de moderação
                if (ativo->isAdmin()) {
                    cout << "6) Admin: Ver avaliações ocultas" << endl
                         << "7) Admin: Gerenciar usuário" << endl;
                }
                cout << "9) Logout" << endl
                 << "0) Sair do programa" << endl;
                cout << "Escolha: ";
                string opt;
                if (!getline(cin, opt)) {
                    session = false;
                    running = false;
                    break;
                }
                if (opt == "1") {
                    // Ver perfil com opções de seguir / parar de seguir
                    bool inProfile = true;
                    while (inProfile) {
                        clearScreen();
                        cout << "== PERFIL ==" << endl;
                        ativo->mostrarPerfil();
                        cout << endl;
                        cout << "1) Seguir usuário por email" << endl;
                        cout << "2) Parar de seguir por email" << endl;
                        cout << "0) Voltar" << endl;
                        cout << "Escolha: ";
                        string popt;
                        if (!getline(cin, popt)) { inProfile = false; break; }
                        if (popt == "0") { inProfile = false; break; }
                        else if (popt == "1") {
                            cout << "Email do usuário para seguir: ";
                            string targetEmail;
                            getline(cin, targetEmail);
                            Usuario* alvo = sistema.encontrarUsuarioPorEmail(targetEmail);
                            if (!alvo) {
                                cout << "Usuário não encontrado." << endl;
                            } else {
                                ativo->seguir(alvo);
                                sistema.salvarSistema(systemFile);
                            }
                            cout << "Pressione Enter..." << endl;
                            string tmp;
                            getline(cin, tmp);
                        } else if (popt == "2") {
                            cout << "Email do usuário para deixar de seguir: ";
                            string targetEmail;
                            getline(cin, targetEmail);
                            Usuario* alvo = sistema.encontrarUsuarioPorEmail(targetEmail);
                            if (!alvo) {
                                cout << "Usuário não encontrado." << endl;
                            } else {
                                ativo->pararDeSeguir(alvo);
                                sistema.salvarSistema(systemFile);
                            }
                            cout << "Pressione Enter..." << endl;
                            string tmp;
                            getline(cin, tmp);
                        }
                    }
                } else if (opt == "2") {
                    // Listar músicas de forma interativa: escolher para ver detalhes
                    bool inList = true;
                    while (inList) {
                        clearScreen();
                        const auto& mus = sistema.getMusicas();
                        if (mus.empty()) {
                            cout << "Nenhuma música cadastrada." << endl;
                            cout << "Pressione Enter para voltar..." << endl;
                            string tmp; getline(cin, tmp);
                            break;
                        }
                        cout << "== LISTA DE MÚSICAS ==" << endl;
                        for (size_t i = 0; i < mus.size(); ++i) {
                            cout << (i+1) << ") " << (mus[i] ? mus[i]->getTitulo() : string("<desconhecida>"))
                                 << " - " << (mus[i] && mus[i]->getArtista() ? mus[i]->getArtista()->getNome() : string("<artista>")) << endl;
                        }
                        cout << "0) Voltar" << endl;
                        cout << "Escolha o número da música para ver detalhes: ";
                        string pick; if (!getline(cin, pick)) { inList = false; break; }
                        int pi = 0; try { pi = stoi(pick); } catch(...) { pi = 0; }
                        if (pi <= 0) { inList = false; break; }
                        if (static_cast<size_t>(pi) <= mus.size()) {
                            Musica* sel = mus[pi-1];
                            clearScreen();
                            if (sel) {
                                // Mostrar resumo da música (sem duplicar avaliações)
                                cout << "Titulo: " << sel->getTitulo() << endl;
                                cout << "Artista: " << (sel->getArtista() ? sel->getArtista()->getNome() : string("<desconhecido>")) << endl;
                                cout << "Album: " << "(info indisponível)" << endl;
                                cout << "Media: " << sel->calcularMedia() << " ★" << endl;
                                cout << "Avaliacoes: " << sel->getAvaliacoes().size() << endl;
                                cout << "\n-- Avaliações --" << endl;
                                const auto &avs = sel->getAvaliacoes();
                                if (avs.empty()) cout << "(Sem avaliações)" << endl;
                                for (size_t ai = 0; ai < avs.size(); ++ai) {
                                    cout << "\n[" << (ai+1) << "]" << endl;
                                    if (avs[ai]) avs[ai]->mostrarAvaliacao();
                                }

                                // Interação: escolher avaliação -> menu (Curtir/Descurtir ou Voltar)
                                if (!avs.empty()) {
                                    cout << "\nEscolha o número da avaliação para ações: ";
                                    string evalPick; if (!getline(cin, evalPick)) { /* ignore */; }
                                    int eidx = 0;
                                    try { eidx = stoi(evalPick); } catch(...) { eidx = 0; }
                                    if (eidx > 0 && static_cast<size_t>(eidx) <= avs.size()) {
                                        Avaliacao* alvoAv = avs[eidx-1];
                                        if (alvoAv) {
                                            AvaliacaoComDestaque* ad = dynamic_cast<AvaliacaoComDestaque*>(alvoAv);
                                            clearScreen();
                                            cout << "Avaliação selecionada:\n" << endl;
                                            if (alvoAv) alvoAv->mostrarAvaliacao();
                                            cout << "\n1) Curtir/Descurtir" << endl;
                                            cout << "0) Voltar" << endl;
                                            cout << "Escolha: ";
                                            string act; if (!getline(cin, act)) act = "0";
                                            if (act == "1") {
                                                // limpar tela imediatamente ao escolher a ação
                                                clearScreen();
                                                if (!ad) {
                                                    cout << "Essa avaliação não é destaque; não é possível curtir." << endl;
                                                } else {
                                                    if (ad->jaCurtiu(ativo->getEmail())) {
                                                        ad->descurtir(ativo);
                                                        cout << "Descurtido." << endl;
                                                    } else {
                                                        ad->curtir(ativo);
                                                        cout << "Curtido." << endl;
                                                    }
                                                    sistema.salvarSistema(systemFile);
                                                    // mostrar avaliação atualizada
                                                    cout << "\nAtualizado:\n";
                                                    ad->mostrarAvaliacao();
                                                }
                                                cout << "\nPressione Enter para voltar à lista..." << endl;
                                                string tmp; getline(cin, tmp);
                                            }
                                        }
                                    }
                                }
                            } else {
                                cout << "Erro: música inválida." << endl;
                            }
                            cout << "\nPressione Enter para voltar à lista..." << endl;
                            string tmp; getline(cin, tmp);
                        }
                    }
                } else if (opt == "3") {
                    // Submenu para cadastrar artista ou música com opção de voltar/cancelar
                    bool inCreateMenu = true;
                    while (inCreateMenu) {
                        clearScreen();
                        cout << "== CADASTRAR ==" << endl;
                        cout << "1) Cadastrar artista" << endl;
                        cout << "2) Cadastrar música" << endl;
                        cout << "0) Voltar" << endl;
                        cout << "Escolha: ";
                        string copt;
                        if (!getline(cin, copt)) { inCreateMenu = false; break; }
                        if (copt == "1") {
                            clearScreen();
                            cout << "Cadastrar artista - Nome (ou vazio para cancelar): ";
                            string anome;
                            getline(cin, anome);
                            if (!anome.empty()) {
                                sistema.cadastrarArtista(anome, string("desconhecido"));
                                sistema.salvarSistema(systemFile);
                                cout << "Artista cadastrado." << endl;
                            } else {
                                cout << "Operação cancelada." << endl;
                            }
                            cout << "Pressione Enter..." << endl;
                            string tmp;
                            getline(cin, tmp);
                        } else if (copt == "2") {
                            clearScreen();
                            cout << "Cadastrar música - Título (ou vazio para cancelar): ";
                            string mtitle;
                            getline(cin, mtitle);
                            if (!mtitle.empty()) {
                                // exigir artista (não permitir música sem artista)
                                cout << "Artista (nome) - obrigatório: ";
                                string aname;
                                getline(cin, aname);
                                if (aname.empty()) {
                                    cout << "Erro: artista é obrigatório. Operação cancelada." << endl;
                                } else {
                                    sistema.cadastrarMusica(mtitle, string("desconhecido"), 0.0f, string(""), 0, string(""), vector<string>(), aname);
                                    sistema.salvarSistema(systemFile);
                                    cout << "Música cadastrada." << endl;
                                }
                            } else {
                                cout << "Operação cancelada." << endl;
                            }
                            cout << "Pressione Enter..." << endl;
                            string tmp;
                            getline(cin, tmp);
                        } else {
                            inCreateMenu = false;
                        }
                    }
                } else if (opt == "4") {
                    clearScreen();
                    cout << "== FAZER AVALIAÇÃO ==" << endl;
                    cout << "0) Voltar" << endl;
                    cout << "Fazer avaliação - Titulo da musica: ";
                    string mtitle;
                    getline(cin, mtitle);
                    // permitir voltar ao menu principal digitando 0
                    if (mtitle == "0") {
                        continue;
                    }
                    // Ler a nota e validar: aceitar apenas inteiros entre 0 e 5.
                    int nota = -1;
                    while (true) {
                        cout << "Nota (0-5): ";
                        string notaStr;
                        if (!getline(cin, notaStr)) { nota = 0; break; }
                        try { nota = stoi(notaStr); } catch(...) { nota = -1; }
                        if (nota >= 0 && nota <= 5) break;
                        cout << "Valor inválido. Digite um número entre 0 e 5." << endl;
                    }
                    cout << "Comentário: ";
                    string comentario;
                    getline(cin, comentario);
                    cout << "Marcar como destaque? (s/n): ";
                    string ds;
                    getline(cin, ds);
                    bool destaque = (ds == "s" || ds == "S");
                    sistema.adicionarAvaliacao(ativo, mtitle, nota, comentario, destaque);
                    sistema.salvarSistema(systemFile);
                    cout << "Avaliação criada. Pressione Enter..." << endl;
                    string tmp;
                    getline(cin, tmp);
                } else if (opt == "5") {
                    // Excluir avaliação do usuário
                    clearScreen();
                    const auto &myAvs = ativo->getAvaliacoes();
                    if (myAvs.empty()) {
                        cout << "Você não tem avaliações para excluir." << endl;
                        cout << "Pressione Enter..." << endl;
                        string tmp;
                        getline(cin, tmp);
                        continue;
                    }
                    cout << "Suas avaliações:" << endl;
                    vector<size_t> idxs;
                    for (size_t i = 0; i < myAvs.size(); ++i) {
                        Avaliacao* a = myAvs[i];
                        if (!a) continue;
                        cout << (i+1) << ")" << endl;
                        a->mostrarAvaliacao();
                        cout << "------------------------------" << endl;
                        idxs.push_back(i);
                    }
                    cout << "0) Voltar" << endl;
                    cout << "Escolha a avaliação para gerenciar (0 voltar): ";
                    string pick;
                    getline(cin, pick);
                    int p = 0;
                    try {
                        p = stoi(pick);
                    } catch(...) {
                        p = 0;
                    }
                    if (p > 0 && static_cast<size_t>(p) <= myAvs.size()) {
                        Avaliacao* alvo = myAvs[p-1];
                        // limpar a tela ao entrar no menu de ação da avaliação
                        clearScreen();
                        // cabeçalho claro para a ação (consistente com outros menus)
                        cout << "== GERENCIAR AVALIAÇÃO ==" << endl;
                        cout << "1) Editar avaliação" << endl;
                        cout << "2) Remover avaliação" << endl;
                        cout << "0) Cancelar" << endl;
                        cout << "Escolha: ";
                        string action; getline(cin, action);
                        if (action == "2") {
                            cout << "Tem certeza que deseja excluir esta avaliação? (s/n): ";
                            string conf; getline(cin, conf);
                            if (conf == "s" || conf == "S") {
                                ativo->removerAvaliacao(alvo);
                                sistema.salvarSistema(systemFile);
                                cout << "Avaliação excluída." << endl;
                            } else {
                                cout << "Operação cancelada." << endl;
                            }
                        } else if (action == "1") {
                            // editar nota
                            int novaNota = -1;
                            while (true) {
                                cout << "Nova nota (0-5) ou deixe em branco para manter: ";
                                string notaStr;
                                if (!getline(cin, notaStr)) { break; }
                                if (notaStr.empty()) { novaNota = alvo->getNota(); break; }
                                try { novaNota = stoi(notaStr); } catch(...) { novaNota = -1; }
                                if (novaNota >= 0 && novaNota <= 5) break;
                                cout << "Valor inválido. Digite um número entre 0 e 5 ou vazio." << endl;
                            }
                            cout << "Novo comentário (ou vazio para manter): ";
                            string novoComentario; getline(cin, novoComentario);
                            if (!novoComentario.empty()) alvo->setComentario(novoComentario);
                            alvo->setNota(novaNota);
                            sistema.salvarSistema(systemFile);
                            cout << "Avaliação atualizada." << endl;
                        } else {
                            cout << "Operação cancelada." << endl;
                        }
                    }
                    cout << "Pressione Enter..." << endl;
                    string tmp;
                    getline(cin, tmp);
                } else if (opt == "9") {
                    session = false;
                } else if (opt == "6" && ativo->isAdmin()) {
                    // Admin: ver avaliações ocultas
                    clearScreen();
                    cout << "== AVALIAÇÕES OCULTAS ==" << endl;
                    auto ocultas = sistema.getAvaliacoesOcultas();
                    if (ocultas.empty()) {
                        cout << "Nenhuma avaliação oculta." << endl;
                        cout << "Pressione Enter..." << endl;
                        string tmp;
                        getline(cin, tmp);
                        continue;
                    }
                    cout << "Avaliações ocultas:" << endl;
                    for (size_t i = 0; i < ocultas.size(); ++i) {
                        cout << (i+1) << ")" << endl;
                        ocultas[i]->mostrarAvaliacaoAdmin();
                        cout << "------------------------------" << endl;
                    }
                    cout << "Escolha uma avaliação para revelar (0 cancelar): ";
                    string pick;
                    getline(cin, pick);
                    int p = 0;
                    try {
                        p = stoi(pick);
                    } catch(...) {
                        p = 0;
                    }
                    if (p > 0 && static_cast<size_t>(p) <= ocultas.size()) {
                        ocultas[p-1]->revelar();
                        sistema.salvarSistema(systemFile);
                        cout << "Avaliação revelada." << endl;
                    }
                    cout << "Pressione Enter..." << endl;
                    string tmp;
                    getline(cin, tmp);
                } else if (opt == "7" && ativo->isAdmin()) {
                    // Admin: gerenciar usuário
                    clearScreen();
                    cout << "Gerenciar usuário - digite o email: ";
                    string targetEmail;
                    getline(cin, targetEmail);
                    Usuario* alvo = sistema.encontrarUsuarioPorEmail(targetEmail);
                    if (!alvo) {
                        cout << "Usuário não encontrado." << endl;
                        cout << "Pressione Enter..." << endl;
                        string tmp;
                        getline(cin, tmp);
                        continue;
                    }
                    cout << "== GERENCIAR USUÁRIO ==" << endl;
                    cout << "1) Suspender usuário" << endl << "2) Ativar usuário" << endl << "3) Tornar/Remover admin" << endl << "0) Cancelar" << endl;
                    cout << "Escolha: ";
                    string choice;
                    getline(cin, choice);
                    if (choice == "1") {
                        cout << "Motivo da suspensão: ";
                        string motivo;
                        getline(cin, motivo);
                        sistema.suspenderUsuario(targetEmail, motivo);
                        sistema.salvarSistema(systemFile);
                    } else if (choice == "2") {
                        sistema.setStatusUsuario(targetEmail, string("ativo"));
                        sistema.salvarSistema(systemFile);
                    } else if (choice == "3") {
                        cout << "Tornar admin? (s/n): ";
                        string a;
                        getline(cin, a);
                        if (a == "s" || a == "S") { 
                            alvo->setAdmin(true); 
                        } else { alvo->setAdmin(false); }
                        sistema.salvarSistema(systemFile);
                    }
                    cout << "Operação concluída. Pressione Enter..." << endl;
                    string tmp;
                    getline(cin, tmp);
                } else if (opt == "8" && ativo->isAdmin()) {
                    // Admin: Gerenciar músicas e artistas
                    clearScreen();
                    bool inManage = true;
                    while (inManage) {
                        clearScreen();
                        cout << "== GERENCIAR MÚSICAS / ARTISTAS ==" << endl;
                        cout << "1) Listar músicas (editar/excluir)" << endl;
                        cout << "2) Listar artistas (editar/excluir)" << endl;
                        cout << "0) Voltar" << endl;
                        cout << "Escolha: ";
                        string mg;
                        if (!getline(cin, mg)) { inManage = false; break; }
                        if (mg == "1") {
                            const auto& mus = sistema.getMusicas();
                            if (mus.empty()) {
                                cout << "Nenhuma música cadastrada." << endl;
                                cout << "Pressione Enter..." << endl;
                                string tmp; getline(cin, tmp);
                                continue;
                            }
                            cout << "Músicas:" << endl;
                            for (size_t i = 0; i < mus.size(); ++i) {
                                cout << (i+1) << ") " << (mus[i] ? mus[i]->getTitulo() : string("<desconhecida>")) << " - " << (mus[i] && mus[i]->getArtista() ? mus[i]->getArtista()->getNome() : string("<artista>")) << endl;
                            }
                            cout << "Escolha o número da música (0 cancelar): ";
                            string pick; getline(cin, pick);
                            int pi = 0; try { pi = stoi(pick); } catch(...) { pi = 0; }
                            if (pi > 0 && static_cast<size_t>(pi) <= mus.size()) {
                                Musica* msel = mus[pi-1];
                                cout << "== AÇÃO: MÚSICA ==" << endl;
                                cout << "1) Editar título" << endl << "2) Excluir música" << endl << "0) Cancelar" << endl;
                                cout << "Escolha: ";
                                string action; getline(cin, action);
                                if (action == "1") {
                                    cout << "Novo título: "; string nt; getline(cin, nt);
                                    if (!nt.empty()) {
                                        sistema.editarMusicaTitulo(msel->getTitulo(), nt);
                                        sistema.salvarSistema(systemFile);
                                        cout << "Título atualizado." << endl;
                                    } else {
                                        cout << "Operação cancelada." << endl;
                                    }
                                    cout << "Pressione Enter..." << endl; string tmp; getline(cin, tmp);
                                } else if (action == "2") {
                                    cout << "Confirma exclusão da música '" << msel->getTitulo() << "'? (s/n): "; string conf; getline(cin, conf);
                                    if (conf == "s" || conf == "S") {
                                        sistema.removerMusica(msel->getTitulo());
                                        sistema.salvarSistema(systemFile);
                                        cout << "Música removida." << endl;
                                    } else {
                                        cout << "Operação cancelada." << endl;
                                    }
                                    cout << "Pressione Enter..." << endl; string tmp; getline(cin, tmp);
                                }
                            }
                        } else if (mg == "2") {
                            // artistas
                            // acessar lista de artistas via leitura indireta: Sistema.h doesn't expose artists; we need to add getter or temporarily list by scanning musics' artists (but there may be duplicates)
                            // Simples abordagem: coletar nomes únicos de artistas a partir do sistema.getMusicas()
                            const auto& musAll = sistema.getMusicas();
                            vector<string> nomes;
                            for (auto m : musAll) {
                                if (!m) continue;
                                Artista* a = m->getArtista();
                                if (!a) continue;
                                bool found = false;
                                for (auto &n : nomes) if (n == a->getNome()) { found = true; break; }
                                if (!found) nomes.push_back(a->getNome());
                            }
                            if (nomes.empty()) {
                                cout << "Nenhum artista disponível." << endl;
                                cout << "Pressione Enter..." << endl; string tmp; getline(cin, tmp); continue;
                            }
                            cout << "Artistas:" << endl;
                            for (size_t i = 0; i < nomes.size(); ++i) cout << (i+1) << ") " << nomes[i] << endl;
                            cout << "Escolha o número do artista (0 cancelar): "; string pick; getline(cin, pick);
                            int pi = 0; try { pi = stoi(pick); } catch(...) { pi = 0; }
                            if (pi > 0 && static_cast<size_t>(pi) <= nomes.size()) {
                                string anome = nomes[pi-1];
                                cout << "== AÇÃO: ARTISTA ==" << endl;
                                cout << "1) Editar nome" << endl << "2) Excluir artista (removerá suas músicas)" << endl << "0) Cancelar" << endl;
                                cout << "Escolha: "; string action; getline(cin, action);
                                if (action == "1") {
                                    cout << "Novo nome do artista: "; string nn; getline(cin, nn);
                                    if (!nn.empty()) {
                                        // localizar artista por nome
                                        // Sistema não expõe artistas publicamente; we'll find a matching Musica->getArtista()
                                        Artista* alvo = nullptr;
                                        for (auto m : musAll) {
                                            if (!m || !m->getArtista()) continue;
                                            if (m->getArtista()->getNome() == anome) { alvo = m->getArtista(); break; }
                                        }
                                        if (alvo) {
                                            alvo->setNome(nn);
                                            sistema.salvarSistema(systemFile);
                                            cout << "Nome do artista atualizado." << endl;
                                        } else {
                                            cout << "Erro: artista não encontrado." << endl;
                                        }
                                    } else {
                                        cout << "Operação cancelada." << endl;
                                    }
                                    cout << "Pressione Enter..." << endl; string tmp; getline(cin, tmp);
                                } else if (action == "2") {
                                    cout << "Confirma exclusão do artista '" << anome << "' e todas as suas músicas? (s/n): "; string conf; getline(cin, conf);
                                    if (conf == "s" || conf == "S") {
                                        sistema.removerArtista(anome);
                                        sistema.salvarSistema(systemFile);
                                        cout << "Artista removido." << endl;
                                    } else {
                                        cout << "Operação cancelada." << endl;
                                    }
                                    cout << "Pressione Enter..." << endl; string tmp; getline(cin, tmp);
                                }
                            }
                        } else {
                            inManage = false;
                        }
                    }
                } else if (opt == "0") {
                    session = false;
                    running = false;
                }
            }
        } else if (escolha == "2") {
            // limpar tela ao entrar no fluxo de registro
            clearScreen();
            cout << "== REGISTRAR ==" << endl;
            cout << "Nome: ";
            string nome;
            getline(cin, nome);
            cout << "Email: ";
            string email;
            getline(cin, email);
            string senha = readPassword("Senha: ");
            string senha2 = readPassword("Confirmar senha: ");
            if (senha != senha2) {
                cout << "Senhas não conferem. Pressione Enter..." << endl;
                string tmp;
                getline(cin, tmp);
                continue;
            }
            // checar existência
            bool existe = (sistema.encontrarUsuarioPorEmail(email) != nullptr);
            if (existe) {
                cout << "Email já registrado. Pressione Enter..." << endl;
                string tmp;
                getline(cin, tmp);
                continue;
            }
            sistema.cadastrarUsuario(nome, email);
            // set password on created user
            Usuario* created = sistema.encontrarUsuarioPorEmail(email);
            if (created) {
                created->setSenha(senha);
            }
            sistema.salvarSistema(systemFile);
            cout << "Registrado com sucesso. Pressione Enter para fazer login..." << endl;
            string tmp;
            getline(cin, tmp);
        } else if (escolha == "0") {
            running = false;
        }
    }

    // salvar antes de sair
    sistema.salvarSistema(systemFile);
    clearScreen();
    cout << "Saindo..." << endl;
    return 0;
}
