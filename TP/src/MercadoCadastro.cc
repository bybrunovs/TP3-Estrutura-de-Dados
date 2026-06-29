#include "Mercado.h"

// Função auxiliar
// Retorna a precedência de um operador booleano (NOT > AND > OR)
int Mercado::precedencia(const std::string &op)
{
    if (op == "NOT")
        return 3;
    if (op == "AND")
        return 2;
    if (op == "OR")
        return 1;
    return 0;
}

// Construtor — inicializa os vetores de árvores AVL para cada atributo indexável

Mercado::Mercado() : _usuarios(0), _produtos(0), _compras(0), _reposicoes(0)
{
    // Criamos vetores vazios para servirem de modelo (valor padrão)
    TADS::Vector<EntradaIndiceInt> vetorIntPadrao(0);
    this->_indicesInt = TADS::Vector<TADS::Vector<EntradaIndiceInt>>(11, vetorIntPadrao);

    TADS::Vector<EntradaIndiceString> vetorStringPadrao(0);
    this->_indicesString = TADS::Vector<TADS::Vector<EntradaIndiceString>>(8, vetorStringPadrao);
}

int Mercado::buscarBinariaInt(PesquisavelInt idx, unsigned chave) const {
    const auto& vetor = _indicesInt[static_cast<unsigned>(idx)];
    if (vetor.tamanho() == 0) return -1;
    unsigned esq = 0;
    unsigned dir = vetor.tamanho() - 1;
    while (esq <= dir) {
        unsigned meio = esq + (dir - esq) / 2;
        if (vetor[meio].chave == chave) return meio;
        else if (vetor[meio].chave < chave) esq = meio + 1;
        else { if (meio == 0) break; dir = meio - 1; }
    }
    return -1;
}

int Mercado::buscarBinariaString(PesquisavelString idx, const std::string &chave) const
{
    const auto &vetor = _indicesString[static_cast<unsigned>(idx)];
    if (vetor.tamanho() == 0)
        return -1;
    unsigned esq = 0;
    unsigned dir = vetor.tamanho() - 1;
    while (esq <= dir)
    {
        unsigned meio = esq + (dir - esq) / 2;
        if (vetor[meio].chave == chave)
            return meio;
        else if (vetor[meio].chave < chave)
            esq = meio + 1;
        else
        {
            if (meio == 0)
                break;
            dir = meio - 1;
        }
    }
    return -1;
}

// Função para comparar doubles com uma tolerância
bool doubleEquals(double a, double b)
{
    const double epsilon = 1e-9;
    return std::abs(a - b) < epsilon;
}

// busca binaria em vector
void buscaBinaria(TADS::ListaOrdenada<unsigned> &lista, TADS::Vector<Produto> &produtos, TADS::Vector<TADS::ListaOrdenada<unsigned>> &dados, double &elemento)
{

    if (dados.tamanho() == 0)
        return;

    unsigned esq = 0;
    unsigned dir = dados.tamanho() - 1;

    while (esq <= dir)
    {
        unsigned meio = esq + (dir - esq) / 2;

        if (doubleEquals(produtos[dados[meio][0]].getPreco(), elemento))
        {
            lista = dados[meio];
        }
        else if (produtos[dados[meio][0]].getPreco() < elemento)
        {
            esq = meio + 1;
        }
        else
        {
            if (meio == 0)
                break; // evita underflow em unsigned
            dir = meio - 1;
        }
    }
}

int Mercado::buscarBinariaDouble(double chave) const
{
    if (_indicesDouble.tamanho() == 0)
        return -1;
    unsigned esq = 0;
    unsigned dir = _indicesDouble.tamanho() - 1;
    while (esq <= dir)
    {
        unsigned meio = esq + (dir - esq) / 2;
        if (doubleEquals(_indicesDouble[meio].chave, chave))
            return meio;
        else if (_indicesDouble[meio].chave < chave)
            esq = meio + 1;
        else
        {
            if (meio == 0)
                break;
            dir = meio - 1;
        }
    }
    return -1;
}

void Mercado::indexarDouble(double chave, unsigned id)
{
    int pos = buscarBinariaDouble(chave);
    if (pos != -1)
    {
        _indicesDouble[pos].ids->inserir(id); // Usa ponteiro
    }
    else
    {
        EntradaIndiceDouble nova;
        nova.chave = chave;
        nova.ids = new TADS::ListaOrdenada<unsigned>(); // Aloca rapidamente
        nova.ids->inserir(id);
        _indicesDouble.push_back(nova);

        unsigned i = _indicesDouble.tamanho() - 1;
        while (i > 0 && _indicesDouble[i].chave < _indicesDouble[i - 1].chave)
        {
            EntradaIndiceDouble temp = _indicesDouble[i - 1];
            _indicesDouble[i - 1] = _indicesDouble[i];
            _indicesDouble[i] = temp;
            i--;
        }
    }
}

// Se a chave já existe, adiciona o id na lista existente.
// Se não existe, cria uma nova lista com o id e insere na árvore.
void Mercado::indexarInt(PesquisavelInt qual, unsigned chave, unsigned id)
{
    auto &vetor = _indicesInt[static_cast<unsigned>(qual)];
    int pos = buscarBinariaInt(qual, chave);

    if (pos != -1)
    {
        vetor[pos].ids->inserir(id); // Usa ->
    }
    else
    {
        EntradaIndiceInt nova;
        nova.chave = chave;
        nova.ids = new TADS::ListaOrdenada<unsigned>(); // Aloca nova lista
        nova.ids->inserir(id);
        vetor.push_back(nova);

        unsigned i = vetor.tamanho() - 1;
        while (i > 0 && vetor[i].chave < vetor[i - 1].chave)
        {
            EntradaIndiceInt temp = vetor[i - 1];
            vetor[i - 1] = vetor[i];
            vetor[i] = temp;
            i--;
        }
    }
}

void Mercado::indexarString(PesquisavelString qual, const std::string &chave, unsigned id)
{
    auto &vetor = _indicesString[static_cast<unsigned>(qual)];
    int pos = buscarBinariaString(qual, chave);

    if (pos != -1)
    {
        vetor[pos].ids->inserir(id);
    }
    else
    {
        EntradaIndiceString nova;
        nova.chave = chave;
        nova.ids = new TADS::ListaOrdenada<unsigned>();
        nova.ids->inserir(id);
        vetor.push_back(nova);

        unsigned i = vetor.tamanho() - 1;
        while (i > 0 && vetor[i].chave < vetor[i - 1].chave)
        {
            EntradaIndiceString temp = vetor[i - 1];
            vetor[i - 1] = vetor[i];
            vetor[i] = temp;
            i--;
        }
    }
}

// Filtros — aplicam um índice sobre o resultado acumulado

// Se primeiroFiltro=true, inicializa resultado com a lista encontrada.
// Caso contrário, faz a interseção com o resultado já acumulado.
// Se a chave não existe no índice, resultado vira vazio.
void Mercado::aplicarFiltroInt(PesquisavelInt idx, unsigned chave, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro)
{
    int pos = buscarBinariaInt(idx, chave);
    if (pos == -1)
    {
        resultado = TADS::ListaOrdenada<unsigned>();
        primeiroFiltro = false;
    }
    else if (primeiroFiltro)
    {
        resultado = *(_indicesInt[static_cast<unsigned>(idx)][pos].ids);
        primeiroFiltro = false;
    }
    else
    {
        resultado = TADS::intersecao(resultado, *(_indicesInt[static_cast<unsigned>(idx)][pos].ids));
    }
}

void Mercado::aplicarFiltroString(PesquisavelString idx, const std::string &chave, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro)
{
    int pos = buscarBinariaString(idx, chave);
    if (pos == -1)
    {
        resultado = TADS::ListaOrdenada<unsigned>();
        primeiroFiltro = false;
    }
    else if (primeiroFiltro)
    {
        resultado = *(_indicesString[static_cast<unsigned>(idx)][pos].ids);
        primeiroFiltro = false;
    }
    else
    {
        resultado = TADS::intersecao(resultado, *(_indicesString[static_cast<unsigned>(idx)][pos].ids));
    }
}

void Mercado::aplicarFiltroIntervaloInt(PesquisavelInt idx, unsigned min, unsigned max, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro) {
    const auto& vetor = _indicesInt[static_cast<unsigned>(idx)];
    unsigned n = vetor.tamanho();
    unsigned inicio = n, fim = n;
    
    unsigned esq = 0, dir = n - 1;
    while (esq <= dir && n > 0) {
        unsigned meio = esq + (dir - esq) / 2;
        if (vetor[meio].chave >= min) {
            inicio = meio;
            if (meio == 0) break;
            dir = meio - 1;
        } else { esq = meio + 1; }
    }
    
    if (n > 0) {
        esq = 0; dir = n - 1;
        while (esq <= dir) {
            unsigned meio = esq + (dir - esq) / 2;
            if (vetor[meio].chave <= max) {
                fim = meio;
                esq = meio + 1;
            } else {
                if (meio == 0) break;
                dir = meio - 1;
            }
        }
    }

    TADS::ListaOrdenada<unsigned> listaResultado;
    if (inicio != n && fim != n && inicio <= fim) {
        for (unsigned i = inicio; i <= fim; i++) {
            // A MAGIA ACONTECE AQUI: Uniao é O(N) invés de inserir um a um O(N^2)
            listaResultado = TADS::uniao(listaResultado, *(vetor[i].ids));
        }
    }

    if (listaResultado.tamanho() == 0) {
        resultado = TADS::ListaOrdenada<unsigned>();
        primeiroFiltro = false;
        return;
    }
    
    if (primeiroFiltro) {
        resultado = listaResultado;
        primeiroFiltro = false;
    } else {
        resultado = TADS::intersecao(resultado, listaResultado);
    }
}

void Mercado::aplicarFiltroIntervaloDouble(double min, double max, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro) {
    unsigned n = _indicesDouble.tamanho();
    unsigned inicio = n, fim = n;
    const double epsilon = 1e-9; // Tolerância para double
    
    unsigned esq = 0, dir = n - 1;
    while (esq <= dir && n > 0) {
        unsigned meio = esq + (dir - esq) / 2;
        if (_indicesDouble[meio].chave >= min - epsilon) {
            inicio = meio;
            if (meio == 0) break;
            dir = meio - 1;
        } else { esq = meio + 1; }
    }
    
    if (n > 0) {
        esq = 0; dir = n - 1;
        while (esq <= dir) {
            unsigned meio = esq + (dir - esq) / 2;
            if (_indicesDouble[meio].chave <= max + epsilon) {
                fim = meio;
                esq = meio + 1;
            } else {
                if (meio == 0) break;
                dir = meio - 1;
            }
        }
    }

    TADS::ListaOrdenada<unsigned> listaResultado;
    if (inicio != n && fim != n && inicio <= fim) {
        for (unsigned i = inicio; i <= fim; i++) {
            listaResultado = TADS::uniao(listaResultado, *(_indicesDouble[i].ids));
        }
    }

    if (listaResultado.tamanho() == 0) {
        resultado = TADS::ListaOrdenada<unsigned>();
        primeiroFiltro = false;
        return;
    }
    if (primeiroFiltro) {
        resultado = listaResultado;
        primeiroFiltro = false;
    } else {
        resultado = TADS::intersecao(resultado, listaResultado);
    }
}

// Cadastro e registro

void Mercado::cadastrarUsuario(const std::string &nome, unsigned idade, const std::string &cidade, const std::string &estado, const std::string &nacionalidade)
{
    unsigned id = _proxIdUsuario++;
    _usuarios.push_back(Usuario(id, nome, idade, cidade, estado, nacionalidade));

    indexarInt(PesquisavelInt::usuario_id, id, id);
    indexarInt(PesquisavelInt::usuario_idade, idade, id);
    indexarString(PesquisavelString::usuario_nome, nome, id);
    indexarString(PesquisavelString::usuario_cidade, cidade, id);
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
    int pos = buscarBinariaInt(PesquisavelInt::produto_qtd, qtdAntiga);
    if (pos != -1)
    {
        auto &vetor = _indicesInt[static_cast<unsigned>(PesquisavelInt::produto_qtd)];
        if (vetor[pos].ids->contains(idProd))
        {
            vetor[pos].ids->deletar(idProd);
        }
    }
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
