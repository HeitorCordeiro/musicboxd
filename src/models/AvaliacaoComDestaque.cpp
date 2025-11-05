#include "AvaliacaoComDestaque.h"
#include "Usuario.h"
#include <iostream>
#include <algorithm>

using namespace std;
// Implementa AvaliacaoComDestaque, que estende Avaliacao com suporte a
// curtidas (likes) e comportamento adicional para avaliações em destaque.

AvaliacaoComDestaque::AvaliacaoComDestaque(Usuario* autor, Musica* musica, int nota, string comentario, const std::vector<string>& curtidasPor)
    : Avaliacao(autor, musica, nota, comentario), curtidasPor(curtidasPor) {

}

void AvaliacaoComDestaque::curtir(Usuario* usuario){
    // Verifica se o ponteiro do usuário é válido antes de processar a curtida.
    if (!usuario){
        return;
    }
    string em = usuario->getEmail();
    // previne curtida duplicada
    if (jaCurtiu(em)) {
        cout << "Você já curtiu essa avaliação." << endl;
        return;
    }
    curtidasPor.push_back(em);
}

void AvaliacaoComDestaque::descurtir(Usuario* usuario){
    // Valida o ponteiro do usuário antes de tentar remover a curtida.
    if (!usuario)
    {
        return;
    }
    string em = usuario->getEmail();
    auto it = std::find(curtidasPor.begin(), curtidasPor.end(), em);
    if (it != curtidasPor.end()) {
        curtidasPor.erase(it);
    } else {
        cout << "Você ainda não curtiu essa avaliação." << endl;
    }
}

int AvaliacaoComDestaque::getCurtidas(){
    return static_cast<int>(curtidasPor.size());
}

bool AvaliacaoComDestaque::jaCurtiu(const string& email) const{
    // Retorna verdadeiro se o email estiver presente na lista de curtidas.
    return std::find(curtidasPor.begin(), curtidasPor.end(), email) != curtidasPor.end();
}

const std::vector<string>& AvaliacaoComDestaque::getCurtidasPor() const{
    return curtidasPor;
}

// Exibe a avaliação completa, com curtidas e informações herdadas
void AvaliacaoComDestaque::mostrarAvaliacao() {
    if (isOculto()) {
        cout << "[Avaliação oculta]" << endl;
        return;
    }
    // Mostrar autor/nota/comentário (base)
    Avaliacao::mostrarAvaliacao();
    // Mostra número de curtidas com símbolo de coração para melhor visibilidade
    cout << "♥ " << getCurtidas() << " curtida(s)" << endl;
}

void AvaliacaoComDestaque::mostrarAvaliacaoAdmin() {
    // Mostra a avaliação independente de estar oculta (para moderadores)
    // Reaproveita a implementação base para autor/nota/comentário
    cout << "[MOD] ";
    Avaliacao::mostrarAvaliacaoAdmin();
    cout << "♥ " << getCurtidas() << " curtida(s)" << endl;
}
