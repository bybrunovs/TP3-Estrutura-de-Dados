#include "Mercado.h"


// Função auxiliar
// Retorna a precedência de um operador booleano (NOT > AND > OR)
int Mercado::precedencia(const std::string &op)
{
    if (op == "NOT") return 3;
    if (op == "AND") return 2;
    if (op == "OR")  return 1;
    return 0;
}


// Construtor — inicializa os vetores de árvores AVL para cada atributo indexável

Mercado::Mercado()
    : _usuarios(0), _produtos(0), _compras(0), _reposicoes(0)
{
    // 11 índices inteiros (ver enum PesquisavelInt) e 8 índices string (ver enum PesquisavelString)
    TADS::ArvoreAVL<unsigned, TADS::ListaOrdenada<unsigned>> arvoreInt;
    this->_indicesInt = TADS::Vector<TADS::ArvoreAVL<unsigned, TADS::ListaOrdenada<unsigned>>>(11, arvoreInt);

    TADS::ArvoreAVL<std::string, TADS::ListaOrdenada<unsigned>> arvoreString;
    this->_indicesString = TADS::Vector<TADS::ArvoreAVL<std::string, TADS::ListaOrdenada<unsigned>>>(8, arvoreString);
}


// Indexação — inserem um id numa árvore de índice invertido


// Se a chave já existe, adiciona o id na lista existente.
// Se não existe, cria uma nova lista com o id e insere na árvore.
void Mercado::indexarInt(PesquisavelInt qual, unsigned chave, unsigned id)
{
    auto &arvore = _indicesInt[static_cast<unsigned>(qual)];
    TADS::ListaOrdenada<unsigned> *lista = arvore.buscar(chave);
    if (lista == nullptr)
    {
        TADS::ListaOrdenada<unsigned> novaLista;
        novaLista.inserir(id);
        arvore.inserir(chave, novaLista);
    }
    else
        lista->inserir(id);
}

void Mercado::indexarString(PesquisavelString qual, const std::string &chave, unsigned id)
{
    auto &arvore = _indicesString[static_cast<unsigned>(qual)];
    TADS::ListaOrdenada<unsigned> *lista = arvore.buscar(chave);
    if (lista == nullptr)
    {
        TADS::ListaOrdenada<unsigned> novaLista;
        novaLista.inserir(id);
        arvore.inserir(chave, novaLista);
    }
    else
        lista->inserir(id);
}

// Índice de preço (double), separado pois usa uma AVL própria (_indicesDouble)
void Mercado::indexarDouble(unsigned chave, unsigned id)
{
    TADS::ListaOrdenada<unsigned> *lista = _indicesDouble.buscar(chave);
    if (lista == nullptr)
    {
        TADS::ListaOrdenada<unsigned> novaLista;
        novaLista.inserir(id);
        _indicesDouble.inserir(chave, novaLista);
    }
    else
        lista->inserir(id);
}


// Filtros — aplicam um índice sobre o resultado acumulado


// Se primeiroFiltro=true, inicializa resultado com a lista encontrada.
// Caso contrário, faz a interseção com o resultado já acumulado.
// Se a chave não existe no índice, resultado vira vazio.
void Mercado::aplicarFiltroInt(PesquisavelInt idx, unsigned chave, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro)
{
    TADS::ListaOrdenada<unsigned> *lista = _indicesInt[static_cast<unsigned>(idx)].buscar(chave);
    if (lista == nullptr)
    {
        resultado = TADS::ListaOrdenada<unsigned>();
        primeiroFiltro = false;
    }
    else if (primeiroFiltro)
    {
        resultado = *lista;
        primeiroFiltro = false;
    }
    else
        resultado = TADS::intersecao(resultado, *lista);
}

void Mercado::aplicarFiltroString(PesquisavelString idx, const std::string &chave,
                                   TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro)
{
    TADS::ListaOrdenada<unsigned> *lista = _indicesString[static_cast<unsigned>(idx)].buscar(chave);
    if (lista == nullptr)
    {
        resultado = TADS::ListaOrdenada<unsigned>();
        primeiroFiltro = false;
    }
    else if (primeiroFiltro)
    {
        resultado = *lista;
        primeiroFiltro = false;
    }
    else
        resultado = TADS::intersecao(resultado, *lista);
}

// buscarIntervalo retorna ListaOrdenada<ListaOrdenada<unsigned>>* onde cada nó da AVL
// dentro do intervalo contribui com sua lista de ids.
void Mercado::aplicarFiltroIntervaloInt(PesquisavelInt idx, unsigned min, unsigned max, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro)
{
    TADS::ListaOrdenada<TADS::ListaOrdenada<unsigned>> *listas = _indicesInt[static_cast<unsigned>(idx)].buscarIntervalo(min, max);

    TADS::ListaOrdenada<unsigned> listaResultado;
    for (unsigned i = 0; i < listas->tamanho(); i++)
        for (unsigned j = 0; j < (*listas)[i].tamanho(); j++)
            listaResultado.inserir((*listas)[i][j]);
    delete listas;

    if (listaResultado.tamanho() == 0) { resultado = TADS::ListaOrdenada<unsigned>(); primeiroFiltro = false; return; }
    if (primeiroFiltro) { resultado = listaResultado; primeiroFiltro = false; }
    else resultado = TADS::intersecao(resultado, listaResultado);
}

void Mercado::aplicarFiltroIntervaloDouble(double min, double max, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro)
{
    TADS::ListaOrdenada<TADS::ListaOrdenada<unsigned>> *listas = _indicesDouble.buscarIntervalo(min, max);

    TADS::ListaOrdenada<unsigned> listaResultado;
    for (unsigned i = 0; i < listas->tamanho(); i++)
        for (unsigned j = 0; j < (*listas)[i].tamanho(); j++)
            listaResultado.inserir((*listas)[i][j]);
    delete listas;

    if (listaResultado.tamanho() == 0) { resultado = TADS::ListaOrdenada<unsigned>(); primeiroFiltro = false; return; }
    if (primeiroFiltro) { resultado = listaResultado; primeiroFiltro = false; }
    else resultado = TADS::intersecao(resultado, listaResultado);
}


// Cadastro e registro


void Mercado::cadastrarUsuario(const std::string &nome, unsigned idade, const std::string &cidade, const std::string &estado, const std::string &nacionalidade)
{
    unsigned id = _proxIdUsuario++;
    _usuarios.push_back(Usuario(id, nome, idade, cidade, estado, nacionalidade));

    indexarInt(PesquisavelInt::usuario_id, id, id);
    indexarInt(PesquisavelInt::usuario_idade, idade, id);
    indexarString(PesquisavelString::usuario_nome, nome, id);
    indexarString(PesquisavelString::usuario_cidade,cidade, id);
    indexarString(PesquisavelString::usuario_estado, estado, id);
    indexarString(PesquisavelString::usuario_nacionalidade, nacionalidade, id);

    std::cout << "U " << id << std::endl;
}

void Mercado::cadastrarProduto(const std::string &nome, double preco, unsigned qtd_inicial, const std::string &categoria, const std::string &marca, const std::string &condicao)
{
    unsigned id = _proxIdProduto++;
    _produtos.push_back(Produto(id, nome, preco, qtd_inicial, categoria, marca, condicao));

    indexarInt(PesquisavelInt::produto_id, id, id);
    indexarInt(PesquisavelInt::produto_qtd, qtd_inicial, id);
    indexarString(PesquisavelString::produto_nome, nome, id);
    indexarString(PesquisavelString::produto_categoria, categoria, id);
    indexarString(PesquisavelString::produto_marca, marca, id);
    indexarString(PesquisavelString::produto_condicao, condicao, id);
    indexarDouble(preco, id);

    std::cout << "P " << id << std::endl;
}

// Atualiza o índice de qtd após uma mudança de estoque
// remove o produto da lista da qtd antiga e insere na nova
void Mercado::atualizarIndiceQtd(unsigned idProd, unsigned qtdAntiga, unsigned qtdNova)
{
    TADS::ListaOrdenada<unsigned> *lista = _indicesInt[static_cast<unsigned>(PesquisavelInt::produto_qtd)].buscar(qtdAntiga);
    if (lista != nullptr && lista->contains(idProd))
        lista->deletar(idProd);
    indexarInt(PesquisavelInt::produto_qtd, qtdNova, idProd);
}

void Mercado::registrarReposicao(unsigned timestamp, const TADS::Vector<unsigned> &id_produtos, const TADS::Vector<unsigned> &qtd_produtos)
{
    unsigned id = _proxIdReposicao++;
    _reposicoes.push_back(Reposicao(id, timestamp, id_produtos, qtd_produtos, id_produtos.tamanho()));

    for (unsigned i = 0; i < id_produtos.tamanho(); i++)
    {
        unsigned idProd = id_produtos[i];
        unsigned qtdAntiga = _produtos[idProd].getQtd();
        _produtos[idProd].setQtd(qtdAntiga + qtd_produtos[i]);
        atualizarIndiceQtd(idProd, qtdAntiga, _produtos[idProd].getQtd());

        indexarInt(PesquisavelInt::reposicao_idProduto, idProd, id);
    }

    indexarInt(PesquisavelInt::reposicao_id, id, id);
    indexarInt(PesquisavelInt::reposicao_timestamp, timestamp, id);

    std::cout << "R " << id << std::endl;
}

void Mercado::registrarCompra(unsigned timestamp, unsigned id_usuario, const TADS::Vector<unsigned> &id_produtos, const TADS::Vector<unsigned> &qtd_produtos)
{
    // Valida estoque de todos os produtos antes de registrar
    for (unsigned i = 0; i < id_produtos.tamanho(); i++)
    {
        if (_produtos[id_produtos[i]].getQtd() < qtd_produtos[i])
        {
            std::cout << "C INV" << std::endl;
            return;
        }
    }

    unsigned id = _proxIdCompra++;
    _compras.push_back(Compra(id, timestamp, id_usuario, id_produtos, qtd_produtos, id_produtos.tamanho()));

    for (unsigned i = 0; i < id_produtos.tamanho(); i++)
    {
        unsigned idProd = id_produtos[i];
        unsigned qtdAntiga = _produtos[idProd].getQtd();
        _produtos[idProd].setQtd(qtdAntiga - qtd_produtos[i]);
        atualizarIndiceQtd(idProd, qtdAntiga, _produtos[idProd].getQtd());

        indexarInt(PesquisavelInt::compra_idProduto, idProd, id);
    }

    indexarInt(PesquisavelInt::compra_id, id, id);
    indexarInt(PesquisavelInt::compra_timestamp, timestamp, id);
    indexarInt(PesquisavelInt::compra_idUsuario, id_usuario, id);

    std::cout << "C " << id << std::endl;
}
