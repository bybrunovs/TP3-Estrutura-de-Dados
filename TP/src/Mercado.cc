#include "Mercado.h"

Mercado::Mercado()
    : _usuarios(0), _produtos(0), _compras(0), _reposicoes(0)
{
    TADS::ArvoreAVL<unsigned, TADS::ListaOrdenada<unsigned>> novaArvoreInt;
    TADS::Vector<TADS::ArvoreAVL<unsigned, TADS::ListaOrdenada<unsigned>>> vetorInt(10, novaArvoreInt);
    this->_indicesInt = vetorInt;

    TADS::ArvoreAVL<std::string, TADS::ListaOrdenada<unsigned>> novaArvoreString;
    TADS::Vector<TADS::ArvoreAVL<std::string, TADS::ListaOrdenada<unsigned>>> vetorString(8, novaArvoreString);
    this->_indicesString = vetorString;
    //     for (unsigned i = 0; i < 10; i++)
    //     {
    //         TADS::ArvoreAVL<unsigned, TADS::ListaOrdenada<unsigned>> novaArvore;
    //         this->_indicesInt[i] = novaArvore;
    //     }

    //         for (unsigned i = 0; i < 8; i++)
    //     {
    //         TADS::ArvoreAVL<std::string, TADS::ListaOrdenada<unsigned>> novaArvore;
    //         this->_indicesString[i] = novaArvore;
    //     }
}

void Mercado::indexarInt(PesquisavelInt qual, unsigned chave, unsigned id)
{
    auto &arvore = this->_indicesInt[static_cast<unsigned>(qual)];
    TADS::ListaOrdenada<unsigned> *lista = arvore.buscar(chave);
    if (lista == nullptr)
    {
        TADS::ListaOrdenada<unsigned> novaLista;
        novaLista.inserir(id);
        arvore.inserir(chave, novaLista);
    }
    else
    {
        lista->inserir(id);
    }
}

void Mercado::indexarString(PesquisavelString qual, const std::string &chave, unsigned id)
{
    auto &arvore = this->_indicesString[static_cast<unsigned>(qual)];
    TADS::ListaOrdenada<unsigned> *lista = arvore.buscar(chave);
    if (lista == nullptr)
    {
        TADS::ListaOrdenada<unsigned> novaLista;
        novaLista.inserir(id);
        arvore.inserir(chave, novaLista);
    }
    else
    {
        lista->inserir(id);
    }
}

void Mercado::aplicarFiltroInt(PesquisavelInt idx, unsigned chave, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro)
{
    TADS::ListaOrdenada<unsigned> *lista = _indicesInt[static_cast<unsigned>(idx)].buscar(chave);
    if (lista == nullptr)
    {
        resultado = TADS::ListaOrdenada<unsigned>();
    }
    else if (primeiroFiltro)
    {
        resultado = *lista;
        primeiroFiltro = false;
    }
    else
    {
        resultado = TADS::intersecao(resultado, *lista);
    }
}

void Mercado::aplicarFiltroString(PesquisavelString idx, const std::string &chave, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro)
{
    TADS::ListaOrdenada<unsigned> *lista = _indicesString[static_cast<unsigned>(idx)].buscar(chave);
    if (lista == nullptr)
    {
        resultado = TADS::ListaOrdenada<unsigned>();
    }
    else if (primeiroFiltro)
    {
        resultado = *lista;
        primeiroFiltro = false;
    }
    else
    {
        resultado = TADS::intersecao(resultado, *lista);
    }
}

void Mercado::cadastrarUsuario(const std::string &nome, unsigned idade, const std::string &cidade, const std::string &estado, const std::string &nacionalidade)
{
    unsigned id = this->_proxIdUsuario++;
    this->_usuarios.push_back(Usuario(id, nome, idade, cidade, estado, nacionalidade));

    // Índices inteiros
    indexarInt(PesquisavelInt::usuario_id, id, id);
    indexarInt(PesquisavelInt::usuario_idade, idade, id);

    // Índices string
    indexarString(PesquisavelString::usuario_nome, nome, id);
    indexarString(PesquisavelString::usuario_cidade, cidade, id);
    indexarString(PesquisavelString::usuario_estado, estado, id);
    indexarString(PesquisavelString::usuario_nacionalidade, nacionalidade, id);

    std::cout << "U " << id << std::endl;
}

void Mercado::cadastrarProduto(const std::string &nome, double preco, unsigned qtd_inicial, const std::string &categoria, const std::string &marca, const std::string &condicao)
{
    unsigned id = this->_proxIdProduto++;
    this->_produtos.push_back(Produto(id, nome, preco, qtd_inicial, categoria, marca, condicao));

    // Índices inteiros
    indexarInt(PesquisavelInt::produto_id, id, id);

    // Índices string
    indexarString(PesquisavelString::produto_nome, nome, id);
    indexarString(PesquisavelString::produto_categoria, categoria, id);
    indexarString(PesquisavelString::produto_marca, marca, id);
    indexarString(PesquisavelString::produto_condicao, condicao, id);

    std::cout << "P " << id << std::endl;
}

void Mercado::registrarReposicao(unsigned timestamp, const TADS::Vector<unsigned> &id_produtos, const TADS::Vector<unsigned> &qtd_produtos)
{
    unsigned id = this->_proxIdReposicao++;
    this->_reposicoes.push_back(Reposicao(id, timestamp, id_produtos, qtd_produtos, id_produtos.tamanho()));

    // Atualiza estoque
    for (unsigned i = 0; i < id_produtos.tamanho(); i++)
    {
        unsigned idProd = id_produtos[i];
        this->_produtos[idProd].setQtd(_produtos[idProd].getQtd() + qtd_produtos[i]);
    }

    // Índices inteiros
    indexarInt(PesquisavelInt::reposicao_id, id, id);
    indexarInt(PesquisavelInt::reposicao_timestamp, timestamp, id);

    for (unsigned i = 0; i < id_produtos.tamanho(); i++)
    {
        indexarInt(PesquisavelInt::reposicao_idProduto, id_produtos[i], id);
    }

    std::cout << "R " << id << std::endl;
}

void Mercado::registrarCompra(unsigned timestamp, unsigned id_usuario, const TADS::Vector<unsigned> &id_produtos, const TADS::Vector<unsigned> &qtd_produtos)
{
    // Valida estoque antes de registrar
    for (unsigned i = 0; i < id_produtos.tamanho(); i++)
    {
        unsigned idProd = id_produtos[i];
        if (this->_produtos[idProd].getQtd() < qtd_produtos[i])
        {
            // compra inválida, sem estoque
            std::cout << "C INV" << std::endl;
            return;
        }
    }

    unsigned id = this->_proxIdCompra++;
    this->_compras.push_back(Compra(id, timestamp, id_usuario, id_produtos, qtd_produtos, id_produtos.tamanho()));

    // Decrementa estoque
    for (unsigned i = 0; i < id_produtos.tamanho(); i++)
    {
        unsigned idProd = id_produtos[i];
        this->_produtos[idProd].setQtd(_produtos[idProd].getQtd() - qtd_produtos[i]);
    }

    indexarInt(PesquisavelInt::compra_id, id, id);
    indexarInt(PesquisavelInt::compra_timestamp, timestamp, id);
    indexarInt(PesquisavelInt::compra_idUsuario, id_usuario, id);

    for (unsigned i = 0; i < id_produtos.tamanho(); i++)
    {
        indexarInt(PesquisavelInt::compra_idProduto, id_produtos[i], id);
    }

    std::cout << "C " << id << std::endl;
}

// Consultas

void Mercado::consultarUsuarios(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores)
{
    TADS::ListaOrdenada<unsigned> resultado;
    bool primeiroFiltro = true;

    for (unsigned i = 0; i < atributos.tamanho(); i++)
    {
        if (atributos[i] == "id")
            aplicarFiltroInt(PesquisavelInt::usuario_id, std::stoul(valores[i]), resultado, primeiroFiltro);
        else if (atributos[i] == "idade")
            aplicarFiltroInt(PesquisavelInt::usuario_idade, std::stoul(valores[i]), resultado, primeiroFiltro);
        else if (atributos[i] == "nome")
            aplicarFiltroString(PesquisavelString::usuario_nome, valores[i], resultado, primeiroFiltro);
        else if (atributos[i] == "cidade")
            aplicarFiltroString(PesquisavelString::usuario_cidade, valores[i], resultado, primeiroFiltro);
        else if (atributos[i] == "estado")
            aplicarFiltroString(PesquisavelString::usuario_estado, valores[i], resultado, primeiroFiltro);
        else if (atributos[i] == "nacionalidade")
            aplicarFiltroString(PesquisavelString::usuario_nacionalidade, valores[i], resultado, primeiroFiltro);
    }

    if (resultado.tamanho() == 0)
    {
        std::cout << "LU VAZIO" << std::endl;
        return;
    }

    for (unsigned i = 0; i < resultado.tamanho(); i++)
    {
        unsigned idUsuario = resultado[i];
        const Usuario &u = _usuarios[idUsuario];

        std::cout << "LU resultado_" << i + 1
                  << " usuario " << idUsuario
                  << " " << u.getNome()
                  << " " << u.getIdade()
                  << " " << u.getCidade()
                  << " " << u.getEstado()
                  << " " << u.getNacionalidade() << std::endl;

        // Soma quantidade total comprada de cada produto pelo usuário
        TADS::ListaOrdenada<unsigned> *idCompras =
            _indicesInt[static_cast<unsigned>(PesquisavelInt::compra_idUsuario)].buscar(idUsuario);

        if (idCompras == nullptr)
            continue;

        TADS::Vector<unsigned> qtdPorProduto(_produtos.tamanho(), 0);

        for (unsigned j = 0; j < idCompras->tamanho(); j++)
        {
            const Compra &c = _compras[(*idCompras)[j]];
            for (unsigned k = 0; k < c.getIdProdutos().tamanho(); k++)
                qtdPorProduto[c.getIdProdutos()[k]] += c.getQtdProdutos()[k];
        }

        // Verifica se há algum produto para imprimir
        bool temProduto = false;
        for (unsigned j = 0; j < qtdPorProduto.tamanho(); j++)
            if (qtdPorProduto[j] > 0)
            {
                temProduto = true;
                break;
            }

        // continua somente se tiver um produto para imprimir
        if (!temProduto)
            continue;

        std::ostringstream oss; // para poder tirar o ultimo espaço mais fácil
        unsigned contadorProduto = 1;
        for (unsigned j = 0; j < qtdPorProduto.tamanho(); j++)
        {
            if (qtdPorProduto[j] > 0)
                oss << "produto_" << contadorProduto++ << " " << j << " " << qtdPorProduto[j] << " ";
        }

        std::string linha = oss.str();
        if (!linha.empty())
            linha.pop_back(); // remove o último espaço
        std::cout << linha << std::endl;
    }
}

void Mercado::consultarProdutos(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores)
{
    TADS::ListaOrdenada<unsigned> resultado;
    bool primeiroFiltro = true;

    for (unsigned i = 0; i < atributos.tamanho(); i++)
    {
        if (atributos[i] == "id")
            aplicarFiltroInt(PesquisavelInt::produto_id, std::stoul(valores[i]), resultado, primeiroFiltro);
        else if (atributos[i] == "nome")
            aplicarFiltroString(PesquisavelString::produto_nome, valores[i], resultado, primeiroFiltro);
        else if (atributos[i] == "categoria")
            aplicarFiltroString(PesquisavelString::produto_categoria, valores[i], resultado, primeiroFiltro);
        else if (atributos[i] == "marca")
            aplicarFiltroString(PesquisavelString::produto_marca, valores[i], resultado, primeiroFiltro);
        else if (atributos[i] == "condicao")
            aplicarFiltroString(PesquisavelString::produto_condicao, valores[i], resultado, primeiroFiltro);
    }

    if (resultado.tamanho() == 0)
    {
        std::cout << "LP VAZIO" << std::endl;
        return;
    }

    for (unsigned i = 0; i < resultado.tamanho(); i++)
    {
        unsigned idProduto = resultado[i];
        const Produto &p = _produtos[idProduto];

        std::cout << "LP resultado_" << i + 1
                  << " produto " << idProduto
                  << " " << p.getNome()
                  << " " << std::fixed << std::setprecision(2) << p.getPreco()
                  << " " << p.getQtd()
                  << " " << p.getCategoria()
                  << " " << p.getMarca()
                  << " " << p.getCondicao() << std::endl;

        // Soma quantidade total comprada de cada usuário para este produto
        TADS::ListaOrdenada<unsigned> *idCompras =
            _indicesInt[static_cast<unsigned>(PesquisavelInt::compra_idProduto)].buscar(idProduto);

        if (idCompras == nullptr)
            continue;

        TADS::Vector<unsigned> qtdPorUsuario(_usuarios.tamanho(), 0);

        for (unsigned j = 0; j < idCompras->tamanho(); j++)
        {
            const Compra &c = _compras[(*idCompras)[j]];
            for (unsigned k = 0; k < c.getIdProdutos().tamanho(); k++)
            {
                if (c.getIdProdutos()[k] == idProduto)
                    qtdPorUsuario[c.getIdUsuario()] += c.getQtdProdutos()[k];
            }
        }

        bool temUsuario = false;
        for (unsigned j = 0; j < qtdPorUsuario.tamanho(); j++)
            if (qtdPorUsuario[j] > 0)
            {
                temUsuario = true;
                break;
            }

        if (!temUsuario)
            continue;

        std::ostringstream oss; // para poder tirar o ultimo espaço mais fácil

        unsigned contadorUsuario = 1;
        for (unsigned j = 0; j < qtdPorUsuario.tamanho(); j++)
        {
            if (qtdPorUsuario[j] > 0)
                oss << "usuario_" << contadorUsuario++ << " " << j << " " << qtdPorUsuario[j] << " ";
        }
        std::string linha = oss.str();
        if (!linha.empty())
            linha.pop_back(); // remove o último espaço
        std::cout << linha << std::endl;
    }
}

void Mercado::consultarCompras(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores)
{
    TADS::ListaOrdenada<unsigned> resultado;
    bool primeiroFiltro = true;

    for (unsigned i = 0; i < atributos.tamanho(); i++)
    {
        if (atributos[i] == "id")
            aplicarFiltroInt(PesquisavelInt::compra_id, std::stoul(valores[i]), resultado, primeiroFiltro);
        else if (atributos[i] == "timestamp")
            aplicarFiltroInt(PesquisavelInt::compra_timestamp, std::stoul(valores[i]), resultado, primeiroFiltro);
        else if (atributos[i] == "id_usuario")
            aplicarFiltroInt(PesquisavelInt::compra_idUsuario, std::stoul(valores[i]), resultado, primeiroFiltro);
        else if (atributos[i] == "id_produto")
            aplicarFiltroInt(PesquisavelInt::compra_idProduto, std::stoul(valores[i]), resultado, primeiroFiltro);
    }

    if (resultado.tamanho() == 0)
    {
        std::cout << "LC VAZIO" << std::endl;
        return;
    }

    for (unsigned i = 0; i < resultado.tamanho(); i++)
    {
        const Compra &c = _compras[resultado[i]];

        // Linha 1: dados gerais da compra
        std::cout << "LC resultado_" << i + 1
                  << " compra " << c.getId()
                  << " timestamp " << c.getTimestamp()
                  << " usuario " << c.getIdUsuario() << std::endl;

        // Linha 2: produtos da compra em ordem crescente de id
        // os produtos já são armazenados em ordem de inserção, não necessariamente ordenados
        // usamos um vetor auxiliar para garantir ordem crescente de id
        TADS::Vector<unsigned> qtdPorProduto(_produtos.tamanho(), 0);
        for (unsigned k = 0; k < c.getIdProdutos().tamanho(); k++)
            qtdPorProduto[c.getIdProdutos()[k]] = c.getQtdProdutos()[k];

        std::ostringstream oss; // para poder tirar o ultimo espaço mais fácil

        unsigned contadorProduto = 1;
        for (unsigned j = 0; j < qtdPorProduto.tamanho(); j++)
        {
            if (qtdPorProduto[j] > 0)
                oss << "produto_" << contadorProduto++ << " " << j << " " << qtdPorProduto[j] << " ";
        }

        std::string linha = oss.str();
        if (!linha.empty())
            linha.pop_back(); // remove o último espaço
        std::cout << linha << std::endl;
    }
}

void Mercado::consultarReposicoes(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores)
{
    TADS::ListaOrdenada<unsigned> resultado;
    bool primeiroFiltro = true;

    for (unsigned i = 0; i < atributos.tamanho(); i++)
    {
        if (atributos[i] == "id")
            aplicarFiltroInt(PesquisavelInt::reposicao_id, std::stoul(valores[i]), resultado, primeiroFiltro);
        else if (atributos[i] == "timestamp")
            aplicarFiltroInt(PesquisavelInt::reposicao_timestamp, std::stoul(valores[i]), resultado, primeiroFiltro);
        else if (atributos[i] == "id_produto")
            aplicarFiltroInt(PesquisavelInt::reposicao_idProduto, std::stoul(valores[i]), resultado, primeiroFiltro);
    }

    if (resultado.tamanho() == 0)
    {
        std::cout << "LR VAZIO" << std::endl;
        return;
    }

    for (unsigned i = 0; i < resultado.tamanho(); i++)
    {
        const Reposicao &r = _reposicoes[resultado[i]];

        // Linha 1: dados gerais da reposição
        std::cout << "LR resultado_" << i + 1
                  << " reposicao " << r.getId()
                  << " timestamp " << r.getTimestamp() << std::endl;

        // Linha 2: produtos da reposição em ordem crescente de id
        TADS::Vector<unsigned> qtdPorProduto(_produtos.tamanho(), 0);
        for (unsigned k = 0; k < r.getIdProdutos().tamanho(); k++)
            qtdPorProduto[r.getIdProdutos()[k]] = r.getQtdProdutos()[k];

        std::ostringstream oss; // para poder tirar o ultimo espaço mais fácil

        unsigned contadorProduto = 1;
        for (unsigned j = 0; j < qtdPorProduto.tamanho(); j++)
        {
            if (qtdPorProduto[j] > 0)
                oss << "produto_" << contadorProduto++ << " " << j << " " << qtdPorProduto[j] << " ";
        }

        std::string linha = oss.str();
        if (!linha.empty())
            linha.pop_back(); // remove o último espaço
        std::cout << linha << std::endl;
    }
}