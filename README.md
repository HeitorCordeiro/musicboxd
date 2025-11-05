# musicboxd
Sistema orientado a objetos em C++ para catalogar músicas e avaliações, inspirado no Letterboxd.

## Visão Geral do Projeto

O objetivo deste projeto é fornecer uma aplicação de terminal que permite:

- Gerenciar usuários (comuns e administradores) e suas informações, incluindo um sistema de login com senha.
- Permitir que usuários sigam e deixem de seguir outros usuários.
- Cadastrar e manipular artistas e músicas.
- Permitir que usuários criem avaliações (com nota de 0-5 e comentário) para as músicas.
- Implementar um sistema de "avaliações com destaque" que podem ser curtidas por outros usuários.
- Oferecer menus distintos para Administradores (com poderes de moderação, como ocultar avaliações ou suspender usuários) e Usuários Comuns.
- Persistir todos os dados (usuários, músicas, artistas, avaliações) em arquivos locais (`users.db` e `system.json`).

## Tecnologias e Conceitos Utilizados

- Linguagem: C++ (utilizando o padrão C++17)
- Programação Orientada a Objetos (POO): classes como `Sistema`, `Usuario`, `Musica`, `Artista` e `Avaliacao`.
- Polimorfismo: `AvaliacaoComDestaque` sobrescreve métodos virtuais de `Avaliacao` (ex.: `mostrarAvaliacao`).
- Herança: `AvaliacaoComDestaque` herda publicamente de `Avaliacao` para adicionar curtidas.
- Contêineres dinâmicos: uso de `std::vector` para listas de usuários, músicas, avaliações, etc.
- Estrutura CRUD: operações de Create, Read, Update e Delete para as entidades principais.
- Persistência de Dados: serialização manual para JSON simples (`system.json`) e formato texto delimitado (`users.db`).

## Estrutura do Projeto

```
musicboxd/
├── build/            # Arquivos compilados (ignorado pelo .gitignore)
├── src/              # Implementação do código-fonte (.cpp)
│   ├── models/       # Classes de domínio (Sistema, Usuario, Musica, etc.)
│   └── main.cpp      # Arquivo principal (interface de terminal)
├── docs/             # Documentação (ex: uml.md)
├── CMakeLists.txt    # Arquivo de build do CMake
├── system.json       # BD (JSON) de músicas, artistas, avaliações
├── users.db          # BD (texto) de usuários, senhas e seguidores
└── README.md         # Documentação do projeto
```

## Como Executar o Projeto

Os comandos abaixo compilam o projeto usando um diretório de build (modo recomendado):

```bash
mkdir -p build
cd build
cmake ..
make -j4
./musicboxd
```

Explicação rápida:

- `mkdir -p build` cria a pasta de build (se já existir, não faz nada).
- `cd build` entra no diretório de build.
- `cmake ..` gera os Makefiles a partir do `CMakeLists.txt` na raiz.
- `make -j4` compila o projeto em paralelo (ajuste `-j4` conforme núcleos disponíveis).
- `./musicboxd` executa o binário gerado.

Se preferir compilar diretamente com `g++` (teste rápido), execute a partir da raiz do projeto:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp src/models/*.cpp -o musicboxd -Isrc/
./musicboxd
```

## Funcionalidades Principais

1. Cadastro de Usuários

- Administrador: Gerencia usuários (suspender, ativar, tornar/remover admin), gerencia músicas e artistas (editar, excluir) e modera avaliações (visualizar e revelar avaliações ocultas). Um usuário admin padrão (`adm@adm.com`) é criado na primeira execução.
- Usuário Comum: Pode se registrar, fazer login, gerenciar seu perfil, seguir/parar de seguir outros usuários, criar e remover suas próprias avaliações.

2. Gerenciamento de Músicas e Artistas

- Artista: Entidade com nome e gênero; armazena uma lista de músicas associadas.
- Musica: Entidade com título, álbum, ano, etc. Deve ser associada a um `Artista` no cadastro. Armazena uma lista de avaliações recebidas.

3. Controle de Avaliações

- `Avaliacao`: Classe base que armazena autor, música, nota (0-5) e comentário. Possui um status `oculto` para moderação.
- `AvaliacaoComDestaque`: Classe especializada (herda de `Avaliacao`) que permite aos usuários curtir e descurtir. O menu principal permite ao usuário logado interagir com essas avaliações.

4. Sistema Social

- Usuários podem seguir outros usuários através do email.
- O perfil do usuário exibe listas de "Seguindo" e "Seguidores".
- Os dados de seguidores são persistidos no arquivo `users.db`.

## Fluxo de Execução (resumo — `main.cpp`)

1. Inicialização: `main()` instancia `Sistema` e chama `carregarSistema()` para carregar `system.json` e `users.db`.
2. Verificação de Admin: `ensureAdminExists()` garante que `adm@adm.com` exista e tenha senha definida.
3. Menu Principal: loop `while (running)` exibe "Login" (1) e "Registrar" (2).
4. Registro: coleta nome, email e senha (com confirmação). Se o email não existir, cria o usuário e salva.
5. Login: coleta email e senha (leitura oculta no terminal) e valida as credenciais.
6. Sessão do Usuário: em caso de login bem-sucedido, entra em um loop de sessão com opções (perfil, listar músicas, cadastrar, avaliar, gerenciar avaliações, etc.).
7. Interface de Administrador: se `ativo->isAdmin()` for verdadeiro, exibe opções extras de moderação e gerenciamento.

## Detalhes de Implementação (classes principais)

- `Sistema`: classe central (façade) que armazena vetores de `Usuario*`, `Musica*` e `Artista*`. Coordena operações de CRUD e a persistência em `system.json` e `users.db`.
- `Usuario`: armazena dados do usuário e senha (usando `std::hash` para um hash simples). Gerencia relações sociais (`seguindo`, `seguidores`) e o vetor de `Avaliacao*` criadas pelo usuário.
- `Musica`: armazena metadados e ponteiro para `Artista`; mantém `vector<Avaliacao*>` para as avaliações recebidas e fornece `calcularMedia()`.
- `Artista`: armazena dados do artista e um vetor de `Musica*` associadas.
- `Avaliacao` e `AvaliacaoComDestaque`: modelam avaliações; a classe base trata nota, comentário e moderação; a derivada adiciona `curtidasPor` (vector<string>) para gerenciar curtidas.

