classDiagram
direction LR

class Sistema {
    - usuarios : Usuario[]
    - musicas : Musica[]
    - artistas : Artista[]
    + Sistema()
    + cadastrarUsuario(nome: string, email: string) : void
    + cadastrarArtista(nome: string, genero: string) : void
    + encontrarArtistaPorNome(nome: string) : Artista
    + cadastrarMusica(titulo: string, genero: string, duracao: float, album: string, ano: int, capa: string, plataformas: string[], artistaNome: string) : void
    + listarMusicas() : void
    + getMusicas() : Musica[]
    + editarMusicaTitulo(tituloAtual: string, novoTitulo: string) : void
    + removerMusica(titulo: string) : void
    + removerArtista(nome: string) : void
    + editarArtistaNome(nomeAtual: string, novoNome: string) : void
    + salvarSistema(caminho: string) : void
    + carregarSistema(caminho: string) : void
    + encontrarMusicaPorTitulo(titulo: string) : Musica
    + encontrarUsuarioPorEmail(email: string) : Usuario
    + adicionarAvaliacao(autor: Usuario, tituloMusica: string, nota: int, comentario: string, destaque: bool) : void
    + getAvaliacoesOcultas() : Avaliacao[]
    + suspenderUsuario(email: string, motivo: string) : void
    + setStatusUsuario(email: string, status: string) : void
    + ensureAdminExists() : void
}

class Usuario {
    - nome : string
    - email : string
    - admin : bool
    - status : string
    - avisos : string[]
    - seguindo : Usuario[]
    - seguidores : Usuario[]
    - avaliacoes : Avaliacao[]
    - listas : Lista[]
    - senhaHash : size_t
    + Usuario(nome: string, email: string, admin: bool)
    + setSenha(senha: string) : void
    + verificarSenha(senha: string) : bool
    + toStorageLine() : string
    + {static} carregarUsuarios(caminho: string) : Usuario[]
    + {static} salvarUsuarios(caminho: string, usuarios: Usuario[]) : void
    + getNome() : string
    + getEmail() : string
    + getStatus() : string
    + suspender(motivo: string) : void
    + adicionarAviso(msg: string) : void
    + seguir(outro: Usuario) : void
    + pararDeSeguir(outroUsuario: Usuario) : void
    + listarSeguindo() : void
    + listarSeguidores() : void
    + adicionarAvaliacao(a: Avaliacao) : void
    + removerAvaliacao(a: Avaliacao) : void
    + getAvaliacoes() : Avaliacao[]
    + criarLista(nome: string) : void
    + curtirAvaliacao(a: Avaliacao) : void
    + mostrarPerfil() : void
    + isAdmin() : bool
    + setAdmin(v: bool) : void
    + setStatus(s: string) : void
    + hasPassword() : bool
}

class Lista {
    - nome : string
    - musicas : Musica[]
    + Lista(nome: string)
    + adicionarMusica(m: Musica) : void
    + mostrarLista() : void
}

class Artista {
    - nome : string
    - genero : string
    - musicas : Musica[]
    + Artista(nome: string, genero: string)
    + getNome() : string
    + getGenero() : string
    + setNome(novoNome: string) : void
    + adicionarMusica(m: Musica) : void
    + removerMusica(musica: Musica) : void
    + listarMusicas() : void
}

class Musica {
    - titulo : string
    - genero : string
    - duracao : float
    - album : string
    - anoLancamento : int
    - capa : string
    - plataformas : string[]
    - artista : Artista
    - avaliacoes : Avaliacao[]
    + Musica(titulo: string, genero: string, duracao: float, artista: Artista, album: string, ano: int, capa: string, plataformas: string[])
    + getTitulo() : string
    + getArtista() : Artista
    + setTitulo(novoTitulo: string) : void
    + adicionarAvaliacao(a: Avaliacao) : void
    + removerAvaliacao(a: Avaliacao) : void
    + calcularMedia() : float
    + mostrarInfo() : void
    + getAvaliacoes() : Avaliacao[]
}

class Avaliacao {
    # autor : Usuario
    # musica : Musica
    # nota : int
    # comentario : string
    # oculto : bool
    # dataCriacao : time_t
    + Avaliacao(autor: Usuario, musica: Musica, nota: int, comentario: string)
    + getNota() : int
    + getComentario() : string
    + getAutor() : Usuario
    + getMusica() : Musica
    + isOculto() : bool
    + ocultar() : void
    + revelar() : void
    + mostrarAvaliacao() : void
    + mostrarAvaliacaoAdmin() : void
    + setComentario(c: string) : void
    + setNota(n: int) : void
    + ~Avaliacao() virtual
}

class AvaliacaoComDestaque {
    - curtidasPor : string[]
    + AvaliacaoComDestaque(autor: Usuario, musica: Musica, nota: int, comentario: string, curtidasPor: string[])
    + curtir(usuario: Usuario) : void
    + descurtir(usuario: Usuario) : void
    + jaCurtiu(email: string) : bool
    + getCurtidas() : int
    + getCurtidasPor() : string[]
    + mostrarAvaliacao() : void
    + mostrarAvaliacaoAdmin() : void
}

%% Relações e heranças

Sistema --> Usuario : gerencia
Sistema --> Musica : gerencia
Sistema --> Artista : gerencia

Usuario --> Lista : cria
Usuario --> Avaliacao : faz
Usuario --> Usuario : segue/é seguido
Artista --> Musica : tem
Musica --> Avaliacao : recebe
Avaliacao <|-- AvaliacaoComDestaque : herança

Lista --> Musica : contém