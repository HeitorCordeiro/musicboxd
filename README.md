# musicboxd
Sistema orientado a objetos para catalogar músicas e avaliações (inspirado no Letterboxd)
 
## Como compilar (modo sugerido pelo usuário)

Os comandos abaixo compilam o projeto usando um diretório de build (modo recomendado):

```bash
mkdir -p build
cd build
cmake ..
make -j4
./musicboxd
```

O fluxo faz o seguinte:
- `mkdir -p build` cria a pasta de build (se já existir, não faz nada).
- `cd build` entra no diretório de build.
- `cmake ..` gera os Makefiles (ou arquivos do gerador escolhido) a partir do `CMakeLists.txt` na raiz.
- `make -j4` compila o projeto em paralelo (4 jobs). Ajuste `-j4` conforme núcleos disponíveis.
- `./musicboxd` executa o binário gerado.

Se preferir compilar direto com `g++` (teste rápido), execute da raiz:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp src/models/*.cpp -o musicboxd
./musicboxd
```

Observações:
- O arquivo `.gitignore` inclui `build/` e binários para não comitar artefatos gerados.
- Durante desenvolvimento você verá avisos (`-Wall -Wextra -Wpedantic`) por causa de métodos stub; isso é esperado.

## Boas práticas de commits e branches

Sugestões simples e práticas para trabalho em equipe e versionamento:

- Branching
	- `main` deve conter código estável e pronto para integrar/entregar.
	- Crie branches de feature a partir de `main` com nomes claros: `feature/nova-lista`, `feature/avaliacao-destaque`.
	- Para correções rápidas, use `hotfix/*` ou `fix/*` e faça merge em `main` quando pronto.

- Commits
	- Faça commits pequenos e atômicos (cada commit resolve um único objetivo).
	- Use mensagens de commit claras e no imperativo, por exemplo: `Adicionar CMakeLists e .gitignore` ou `Renomear parametros para nomes descritivos`.
	- Evite commitar arquivos binários e diretórios de build (já tratados pelo `.gitignore`).
