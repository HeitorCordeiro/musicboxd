#ifndef AVALIACAO_COM_DESTAQUE_H
#define AVALIACAO_COM_DESTAQUE_H

#include "Avaliacao.h"
#include <vector>
using namespace std;

// Classe derivada que representa uma avaliação com destaque, permitindo curtidas
class AvaliacaoComDestaque : public Avaliacao {
private:
    std::vector<string> curtidasPor;
public:
    AvaliacaoComDestaque(Usuario* autor, Musica* musica, int nota, string comentario, const std::vector<string>& curtidasPor = std::vector<string>());
    void curtir(Usuario* usuario);
    void descurtir(Usuario* usuario);
    int getCurtidas();
    const std::vector<string>& getCurtidasPor() const;
    bool jaCurtiu(const string& email) const;
    void mostrarAvaliacaoAdmin() override;
    void mostrarAvaliacao() override;
};

#endif // AVALIACAO_COM_DESTAQUE_H
