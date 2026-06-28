#include "Mercado.h"

int precedencia(const std::string &op)
{
    if (op == "NOT")
        return 3;
    if (op == "AND")
        return 2;
    if (op == "OR")
        return 1;
    return 0;
}

bool Mercado::eNumero(const std::string &str)
{
    if (str.empty())
        return false;

    bool pontoDecimal = false;

    for (unsigned i = 0; i < str.length(); ++i)
    {
        // Verifica se o caractere não é um dígito de 0 a 9
        if (str[i] < '0' || str[i] > '9')
        {
            // Se for um ponto e ainda não vimos outro ponto, é válido
            if (str[i] == '.' && !pontoDecimal)
            {
                pontoDecimal = true;
            }
            else
            {
                return false; // Caractere inválido encontrado
            }
        }
    }
    return true;
}

Mercado::Mercado()
    : _usuarios(0), _produtos(0), _compras(0), _reposicoes(0)
{
    TADS::ArvoreAVL<unsigned, TADS::ListaOrdenada<unsigned>> novaArvoreInt;
    TADS::Vector<TADS::ArvoreAVL<unsigned, TADS::ListaOrdenada<unsigned>>> vetorInt(11, novaArvoreInt);
    this->_indicesInt = vetorInt;

    TADS::ArvoreAVL<std::string, TADS::ListaOrdenada<unsigned>> novaArvoreString;
    TADS::Vector<TADS::ArvoreAVL<std::string, TADS::ListaOrdenada<unsigned>>> vetorString(8, novaArvoreString);
    this->_indicesString = vetorString;
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
        primeiroFiltro = false; // filtro foi aplicado, resultado é vazio
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

void Mercado::aplicarFiltroString(PesquisavelString idx, const std::string &chave, TADS::ListaOrdenada<unsigned int> &resultado, bool &primeiroFiltro)
{
    TADS::ListaOrdenada<unsigned> *lista = _indicesString[static_cast<unsigned>(idx)].buscar(chave);
    if (lista == nullptr)
    {
        resultado = TADS::ListaOrdenada<unsigned>();
        primeiroFiltro = false; // filtro foi aplicado, resultado é vazio
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

void Mercado::aplicarFiltroIntervaloInt(PesquisavelInt idx, unsigned min, unsigned max, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro)
{
    TADS::ListaOrdenada<TADS::ListaOrdenada<unsigned>> *listas = _indicesInt[static_cast<unsigned>(idx)].buscarIntervalo(min, max);

    TADS::ListaOrdenada<unsigned> listaResultado;
    for (unsigned i = 0; i < listas->tamanho(); i++)
        for (unsigned j = 0; j < (*listas)[i].tamanho(); j++)
            listaResultado.inserir((*listas)[i][j]);

    delete listas;

    if (listaResultado.tamanho() == 0)
    {
        resultado = TADS::ListaOrdenada<unsigned>();
        primeiroFiltro = false;
        return;
    }
    if (primeiroFiltro)
    {
        resultado = listaResultado;
        primeiroFiltro = false;
    }
    else
        resultado = TADS::intersecao(resultado, listaResultado);
}

void Mercado::aplicarFiltroIntervaloDouble(double min, double max, TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro)
{
    TADS::ListaOrdenada<TADS::ListaOrdenada<unsigned>> *listas = _indicesDouble.buscarIntervalo(min, max);

    TADS::ListaOrdenada<unsigned> listaResultado;
    for (unsigned i = 0; i < listas->tamanho(); i++)
        for (unsigned j = 0; j < (*listas)[i].tamanho(); j++)
            listaResultado.inserir((*listas)[i][j]);

    delete listas;

    if (listaResultado.tamanho() == 0)
    {
        resultado = TADS::ListaOrdenada<unsigned>();
        primeiroFiltro = false;
        return;
    }
    if (primeiroFiltro)
    {
        resultado = listaResultado;
        primeiroFiltro = false;
    }
    else
        resultado = TADS::intersecao(resultado, listaResultado);
}

void Mercado::imprimirUsuarios(const TADS::ListaOrdenada<unsigned> &ids, const std::string &prefixo)
{
    if (ids.tamanho() == 0)
    {
        std::cout << prefixo << " VAZIO" << std::endl;
        return;
    }

    for (unsigned i = 0; i < ids.tamanho(); i++)
    {
        unsigned idUsuario = ids[i];
        const Usuario &u = _usuarios[idUsuario];
        std::cout << prefixo << " resultado_" << i + 1
                  << " usuario " << idUsuario
                  << " " << u.getNome() << " " << u.getIdade()
                  << " " << u.getCidade() << " " << u.getEstado()
                  << " " << u.getNacionalidade() << std::endl;

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

        std::ostringstream oss;
        unsigned cont = 1;
        for (unsigned j = 0; j < qtdPorProduto.tamanho(); j++)
            if (qtdPorProduto[j] > 0)
                oss << "produto_" << cont++ << " " << j << " " << qtdPorProduto[j] << " ";
        std::string linha = oss.str();
        if (!linha.empty())
        {
            linha.pop_back();
            std::cout << linha << std::endl;
        }
    }
}

void Mercado::imprimirProdutos(const TADS::ListaOrdenada<unsigned> &ids, const std::string &prefixo)
{
    if (ids.tamanho() == 0)
    {
        std::cout << prefixo << " VAZIO" << std::endl;
        return;
    }

    for (unsigned i = 0; i < ids.tamanho(); i++)
    {
        unsigned idProduto = ids[i];
        const Produto &p = _produtos[idProduto];
        std::cout << prefixo << " resultado_" << i + 1
                  << " produto " << idProduto
                  << " " << p.getNome()
                  << " " << std::fixed << std::setprecision(2) << p.getPreco()
                  << " " << p.getQtd()
                  << " " << p.getCategoria()
                  << " " << p.getMarca()
                  << " " << p.getCondicao() << std::endl;

        TADS::ListaOrdenada<unsigned> *idCompras =
            _indicesInt[static_cast<unsigned>(PesquisavelInt::compra_idProduto)].buscar(idProduto);
        if (idCompras == nullptr)
            continue;

        TADS::Vector<unsigned> qtdPorUsuario(_usuarios.tamanho(), 0);
        for (unsigned j = 0; j < idCompras->tamanho(); j++)
        {
            const Compra &c = _compras[(*idCompras)[j]];
            for (unsigned k = 0; k < c.getIdProdutos().tamanho(); k++)
                if (c.getIdProdutos()[k] == idProduto)
                    qtdPorUsuario[c.getIdUsuario()] += c.getQtdProdutos()[k];
        }

        std::ostringstream oss;
        unsigned cont = 1;
        for (unsigned j = 0; j < qtdPorUsuario.tamanho(); j++)
            if (qtdPorUsuario[j] > 0)
                oss << "usuario_" << cont++ << " " << j << " " << qtdPorUsuario[j] << " ";
        std::string linha = oss.str();
        if (!linha.empty())
        {
            linha.pop_back();
            std::cout << linha << std::endl;
        }
    }
}

void Mercado::imprimirCompras(const TADS::ListaOrdenada<unsigned> &ids, const std::string &prefixo)
{
    if (ids.tamanho() == 0)
    {
        std::cout << prefixo << " VAZIO" << std::endl;
        return;
    }

    for (unsigned i = 0; i < ids.tamanho(); i++)
    {
        const Compra &c = _compras[ids[i]];
        std::cout << prefixo << " resultado_" << i + 1
                  << " compra " << c.getId()
                  << " timestamp " << c.getTimestamp()
                  << " usuario " << c.getIdUsuario() << std::endl;

        TADS::Vector<unsigned> qtdPorProduto(_produtos.tamanho(), 0);
        for (unsigned k = 0; k < c.getIdProdutos().tamanho(); k++)
            qtdPorProduto[c.getIdProdutos()[k]] = c.getQtdProdutos()[k];

        std::ostringstream oss;
        unsigned cont = 1;
        for (unsigned j = 0; j < qtdPorProduto.tamanho(); j++)
            if (qtdPorProduto[j] > 0)
                oss << "produto_" << cont++ << " " << j << " " << qtdPorProduto[j] << " ";
        std::string linha = oss.str();
        if (!linha.empty())
        {
            linha.pop_back();
            std::cout << linha << std::endl;
        }
    }
}

void Mercado::imprimirReposicoes(const TADS::ListaOrdenada<unsigned> &ids, const std::string &prefixo)
{
    if (ids.tamanho() == 0)
    {
        std::cout << prefixo << " VAZIO" << std::endl;
        return;
    }

    for (unsigned i = 0; i < ids.tamanho(); i++)
    {
        const Reposicao &r = _reposicoes[ids[i]];
        std::cout << prefixo << " resultado_" << i + 1
                  << " reposicao " << r.getId()
                  << " timestamp " << r.getTimestamp() << std::endl;

        TADS::Vector<unsigned> qtdPorProduto(_produtos.tamanho(), 0);
        for (unsigned k = 0; k < r.getIdProdutos().tamanho(); k++)
            qtdPorProduto[r.getIdProdutos()[k]] = r.getQtdProdutos()[k];

        std::ostringstream oss;
        unsigned cont = 1;
        for (unsigned j = 0; j < qtdPorProduto.tamanho(); j++)
            if (qtdPorProduto[j] > 0)
                oss << "produto_" << cont++ << " " << j << " " << qtdPorProduto[j] << " ";
        std::string linha = oss.str();
        if (!linha.empty())
        {
            linha.pop_back();
            std::cout << linha << std::endl;
        }
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
    indexarInt(PesquisavelInt::produto_qtd, qtd_inicial, id);

    // Índices string
    indexarString(PesquisavelString::produto_nome, nome, id);
    indexarString(PesquisavelString::produto_categoria, categoria, id);
    indexarString(PesquisavelString::produto_marca, marca, id);
    indexarString(PesquisavelString::produto_condicao, condicao, id);
    indexarDouble(preco, id);
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

        // atualizar arvore de estoque
        auto lista = _indicesInt[static_cast<unsigned>(PesquisavelInt::produto_qtd)].buscar(_produtos[idProd].getQtd());
        if (lista != nullptr && lista->contains(idProd))
        {
            lista->deletar(idProd);
        }
        this->_produtos[idProd].setQtd(_produtos[idProd].getQtd() + qtd_produtos[i]);

        indexarInt(PesquisavelInt::produto_qtd, _produtos[idProd].getQtd(), idProd); // atualizar arvore de estoque
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

        // atualizar arvore de estoque
        auto lista = _indicesInt[static_cast<unsigned>(PesquisavelInt::produto_qtd)].buscar(_produtos[idProd].getQtd());
        if (lista != nullptr && lista->contains(idProd))
        {
            lista->deletar(idProd);
        }

        // atualizar estoque
        this->_produtos[idProd].setQtd(_produtos[idProd].getQtd() - qtd_produtos[i]);

        indexarInt(PesquisavelInt::produto_qtd, _produtos[idProd].getQtd(), idProd); // atualizar arvore de estoque
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

void Mercado::consultarUsuarios(const TADS::Vector<std::string> &tokens)
{
    if (tokens.tamanho() == 0)
    {
        TADS::ListaOrdenada<unsigned> resultado;
        for (unsigned i = 0; i < _usuarios.tamanho(); i++)
            resultado.inserir(i);
        imprimirUsuarios(resultado, "LU");
        return;
    }

    TADS::Vector<TADS::ListaOrdenada<unsigned>> pilhaResultados;
    TADS::Vector<std::string> pilhaOperadores;

    auto resolverTopo = [&]()
    {
        std::string op = pilhaOperadores[pilhaOperadores.tamanho() - 1];
        pilhaOperadores.pop_back();

        if (op == "NOT")
        {
            TADS::ListaOrdenada<unsigned> v = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(complemento(v, _usuarios.tamanho()));
        }
        else
        {
            TADS::ListaOrdenada<unsigned> v2 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            TADS::ListaOrdenada<unsigned> v1 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();

            if (op == "AND")
                pilhaResultados.push_back(intersecao(v1, v2));
            else if (op == "OR")
                pilhaResultados.push_back(uniao(v1, v2));
        }
    };

    for (unsigned i = 0; i < tokens.tamanho();)
    {
        std::string token = tokens[i];

        if (token == "AND" || token == "OR" || token == "NOT")
        {
            while (pilhaOperadores.tamanho() > 0 &&
                   precedencia(pilhaOperadores[pilhaOperadores.tamanho() - 1]) >= precedencia(token) &&
                   !(token == "NOT" && pilhaOperadores[pilhaOperadores.tamanho() - 1] == "NOT"))
            {
                resolverTopo();
            }
            pilhaOperadores.push_back(token);
            i++;
            continue;
        }

        std::string atributo = token;
        std::string valor = "";
        std::string valor2 = "";
        unsigned pos = i + 1;

        if (pos < tokens.tamanho() && tokens[pos] != "AND" && tokens[pos] != "OR" && tokens[pos] != "NOT")
        {
            valor = tokens[pos];
            pos++;
        }

        if ((atributo == "idade" || atributo == "qtd" || atributo == "timestamp" || atributo == "preco") && pos < tokens.tamanho() && tokens[pos] != "AND" && tokens[pos] != "OR" && tokens[pos] != "NOT" && eNumero(tokens[pos]))
        {
            valor2 = tokens[pos];
            pos++;
        }

        TADS::ListaOrdenada<unsigned> resultado;
        bool primeiroFiltro = true;

        if (valor2 == "")
        {
            if (atributo == "id")
                aplicarFiltroInt(PesquisavelInt::usuario_id, std::stoul(valor), resultado, primeiroFiltro);
            else if (atributo == "idade")
                aplicarFiltroInt(PesquisavelInt::usuario_idade, std::stoul(valor), resultado, primeiroFiltro);
            else if (atributo == "nome")
                aplicarFiltroString(PesquisavelString::usuario_nome, valor, resultado, primeiroFiltro);
            else if (atributo == "cidade")
                aplicarFiltroString(PesquisavelString::usuario_cidade, valor, resultado, primeiroFiltro);
            else if (atributo == "estado")
                aplicarFiltroString(PesquisavelString::usuario_estado, valor, resultado, primeiroFiltro);
            else if (atributo == "nacionalidade")
                aplicarFiltroString(PesquisavelString::usuario_nacionalidade, valor, resultado, primeiroFiltro);
        }
        else
        {
            if (atributo == "idade")
                aplicarFiltroIntervaloInt(PesquisavelInt::usuario_idade, std::stoul(valor), std::stoul(valor2), resultado, primeiroFiltro);
        }

        pilhaResultados.push_back(resultado);
        i = pos;
    }

    // Resolve as operações que sobraram na pilha
    while (pilhaOperadores.tamanho() > 0)
    {
        resolverTopo();
    }

    TADS::ListaOrdenada<unsigned> &resultadoFinal = pilhaResultados[0];

    imprimirUsuarios(resultadoFinal, "LU");
}

void Mercado::consultarProdutos(const TADS::Vector<std::string> &tokens)
{
    if (tokens.tamanho() == 0)
    {
        TADS::ListaOrdenada<unsigned> resultado;
        for (unsigned i = 0; i < _produtos.tamanho(); i++)
            resultado.inserir(i);
        imprimirProdutos(resultado, "LP");
        return;
    }

    TADS::Vector<TADS::ListaOrdenada<unsigned>> pilhaResultados;
    TADS::Vector<std::string> pilhaOperadores;

    auto resolverTopo = [&]()
    {
        std::string op = pilhaOperadores[pilhaOperadores.tamanho() - 1];
        pilhaOperadores.pop_back();

        if (op == "NOT")
        {
            TADS::ListaOrdenada<unsigned> v = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(complemento(v, _produtos.tamanho()));
        }
        else
        {
            TADS::ListaOrdenada<unsigned> v2 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            TADS::ListaOrdenada<unsigned> v1 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();

            if (op == "AND")
                pilhaResultados.push_back(intersecao(v1, v2));
            else if (op == "OR")
                pilhaResultados.push_back(uniao(v1, v2));
        }
    };

    for (unsigned i = 0; i < tokens.tamanho();)
    {
        std::string token = tokens[i];

        if (token == "AND" || token == "OR" || token == "NOT")
        {
            while (pilhaOperadores.tamanho() > 0 &&
                   precedencia(pilhaOperadores[pilhaOperadores.tamanho() - 1]) >= precedencia(token) &&
                   !(token == "NOT" && pilhaOperadores[pilhaOperadores.tamanho() - 1] == "NOT"))
            {
                resolverTopo();
            }
            pilhaOperadores.push_back(token);
            i++;
            continue;
        }

        std::string atributo = token;
        std::string valor = "";
        std::string valor2 = "";
        unsigned pos = i + 1;

        if (pos < tokens.tamanho() && tokens[pos] != "AND" && tokens[pos] != "OR" && tokens[pos] != "NOT")
        {
            valor = tokens[pos];
            pos++;
        }

        if ((atributo == "idade" || atributo == "qtd" || atributo == "timestamp" || atributo == "preco") &&
            pos < tokens.tamanho() &&
            tokens[pos] != "AND" && tokens[pos] != "OR" && tokens[pos] != "NOT" &&
            eNumero(tokens[pos]))
        {
            valor2 = tokens[pos];
            pos++;
        }

        TADS::ListaOrdenada<unsigned> resultado;
        bool primeiroFiltro = true;

        if (valor2 == "")
        {
            if (atributo == "id")
                aplicarFiltroInt(PesquisavelInt::produto_id, std::stoul(valor), resultado, primeiroFiltro);
            else if (atributo == "nome")
                aplicarFiltroString(PesquisavelString::produto_nome, valor, resultado, primeiroFiltro);
            else if (atributo == "categoria")
                aplicarFiltroString(PesquisavelString::produto_categoria, valor, resultado, primeiroFiltro);
            else if (atributo == "marca")
                aplicarFiltroString(PesquisavelString::produto_marca, valor, resultado, primeiroFiltro);
            else if (atributo == "condicao")
                aplicarFiltroString(PesquisavelString::produto_condicao, valor, resultado, primeiroFiltro);
            else if (atributo == "qtd")
                aplicarFiltroInt(PesquisavelInt::produto_qtd, std::stoul(valor), resultado, primeiroFiltro);
        }
        else
        {
            if (atributo == "qtd")
                aplicarFiltroIntervaloInt(PesquisavelInt::produto_qtd, std::stoul(valor), std::stoul(valor2), resultado, primeiroFiltro);
            else if (atributo == "preco")
                aplicarFiltroIntervaloDouble(std::stod(valor), std::stod(valor2), resultado, primeiroFiltro);
        }

        pilhaResultados.push_back(resultado);
        i = pos;
    }

    while (pilhaOperadores.tamanho() > 0)
    {
        resolverTopo();
    }

    TADS::ListaOrdenada<unsigned> &resultadoFinal = pilhaResultados[0];
    imprimirProdutos(resultadoFinal, "LP");
}

void Mercado::consultarCompras(const TADS::Vector<std::string> &tokens)
{
    if (tokens.tamanho() == 0)
    {
        TADS::ListaOrdenada<unsigned> resultado;
        for (unsigned i = 0; i < _compras.tamanho(); i++)
            resultado.inserir(i);
        imprimirCompras(resultado, "LC");
        return;
    }

    TADS::Vector<TADS::ListaOrdenada<unsigned>> pilhaResultados;
    TADS::Vector<std::string> pilhaOperadores;

    auto resolverTopo = [&]()
    {
        std::string op = pilhaOperadores[pilhaOperadores.tamanho() - 1];
        pilhaOperadores.pop_back();

        if (op == "NOT")
        {
            TADS::ListaOrdenada<unsigned> v = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(complemento(v, _compras.tamanho()));
        }
        else
        {
            TADS::ListaOrdenada<unsigned> v2 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            TADS::ListaOrdenada<unsigned> v1 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();

            if (op == "AND")
                pilhaResultados.push_back(intersecao(v1, v2));
            else if (op == "OR")
                pilhaResultados.push_back(uniao(v1, v2));
        }
    };

    for (unsigned i = 0; i < tokens.tamanho();)
    {
        std::string token = tokens[i];

        if (token == "AND" || token == "OR" || token == "NOT")
        {
            while (pilhaOperadores.tamanho() > 0 &&
                   precedencia(pilhaOperadores[pilhaOperadores.tamanho() - 1]) >= precedencia(token) &&
                   !(token == "NOT" && pilhaOperadores[pilhaOperadores.tamanho() - 1] == "NOT"))
            {
                resolverTopo();
            }
            pilhaOperadores.push_back(token);
            i++;
            continue;
        }

        std::string atributo = token;
        std::string valor = "";
        std::string valor2 = "";
        unsigned pos = i + 1;

        if (pos < tokens.tamanho() && tokens[pos] != "AND" && tokens[pos] != "OR" && tokens[pos] != "NOT")
        {
            valor = tokens[pos];
            pos++;
        }

        if ((atributo == "idade" || atributo == "qtd" || atributo == "timestamp" || atributo == "preco") &&
            pos < tokens.tamanho() &&
            tokens[pos] != "AND" && tokens[pos] != "OR" && tokens[pos] != "NOT" &&
            eNumero(tokens[pos]))
        {
            valor2 = tokens[pos];
            pos++;
        }

        TADS::ListaOrdenada<unsigned> resultado;
        bool primeiroFiltro = true;

        if (valor2 == "")
        {
            if (atributo == "id")
                aplicarFiltroInt(PesquisavelInt::compra_id, std::stoul(valor), resultado, primeiroFiltro);
            else if (atributo == "timestamp")
                aplicarFiltroInt(PesquisavelInt::compra_timestamp, std::stoul(valor), resultado, primeiroFiltro);
            else if (atributo == "id_usuario")
                aplicarFiltroInt(PesquisavelInt::compra_idUsuario, std::stoul(valor), resultado, primeiroFiltro);
            else if (atributo == "id_produto")
                aplicarFiltroInt(PesquisavelInt::compra_idProduto, std::stoul(valor), resultado, primeiroFiltro);
        }
        else
        {
            if (atributo == "timestamp")
                aplicarFiltroIntervaloInt(PesquisavelInt::compra_timestamp, std::stoul(valor), std::stoul(valor2), resultado, primeiroFiltro);
        }

        pilhaResultados.push_back(resultado);
        i = pos;
    }

    while (pilhaOperadores.tamanho() > 0)
    {
        resolverTopo();
    }

    TADS::ListaOrdenada<unsigned> &resultadoFinal = pilhaResultados[0];
    imprimirCompras(resultadoFinal, "LC");
}

void Mercado::consultarReposicoes(const TADS::Vector<std::string> &tokens)
{
    if (tokens.tamanho() == 0)
    {
        TADS::ListaOrdenada<unsigned> resultado;
        for (unsigned i = 0; i < _reposicoes.tamanho(); i++)
            resultado.inserir(i);
        imprimirReposicoes(resultado, "LR");
        return;
    }

    TADS::Vector<TADS::ListaOrdenada<unsigned>> pilhaResultados;
    TADS::Vector<std::string> pilhaOperadores;

    auto resolverTopo = [&]()
    {
        std::string op = pilhaOperadores[pilhaOperadores.tamanho() - 1];
        pilhaOperadores.pop_back();

        if (op == "NOT")
        {
            TADS::ListaOrdenada<unsigned> v = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(complemento(v, _reposicoes.tamanho()));
        }
        else
        {
            TADS::ListaOrdenada<unsigned> v2 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            TADS::ListaOrdenada<unsigned> v1 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();

            if (op == "AND")
                pilhaResultados.push_back(intersecao(v1, v2));
            else if (op == "OR")
                pilhaResultados.push_back(uniao(v1, v2));
        }
    };

    for (unsigned i = 0; i < tokens.tamanho();)
    {
        std::string token = tokens[i];

        if (token == "AND" || token == "OR" || token == "NOT")
        {
            while (pilhaOperadores.tamanho() > 0 &&
                   precedencia(pilhaOperadores[pilhaOperadores.tamanho() - 1]) >= precedencia(token) &&
                   !(token == "NOT" && pilhaOperadores[pilhaOperadores.tamanho() - 1] == "NOT"))
            {
                resolverTopo();
            }
            pilhaOperadores.push_back(token);
            i++;
            continue;
        }

        std::string atributo = token;
        std::string valor = "";
        std::string valor2 = "";
        unsigned pos = i + 1;

        if (pos < tokens.tamanho() && tokens[pos] != "AND" && tokens[pos] != "OR" && tokens[pos] != "NOT")
        {
            valor = tokens[pos];
            pos++;
        }

        if ((atributo == "idade" || atributo == "qtd" || atributo == "timestamp" || atributo == "preco") &&
            pos < tokens.tamanho() &&
            tokens[pos] != "AND" && tokens[pos] != "OR" && tokens[pos] != "NOT" &&
            eNumero(tokens[pos]))
        {
            valor2 = tokens[pos];
            pos++;
        }

        TADS::ListaOrdenada<unsigned> resultado;
        bool primeiroFiltro = true;

        if (valor2 == "")
        {
            if (atributo == "id")
                aplicarFiltroInt(PesquisavelInt::reposicao_id, std::stoul(valor), resultado, primeiroFiltro);
            else if (atributo == "timestamp")
                aplicarFiltroInt(PesquisavelInt::reposicao_timestamp, std::stoul(valor), resultado, primeiroFiltro);
            else if (atributo == "id_produto")
                aplicarFiltroInt(PesquisavelInt::reposicao_idProduto, std::stoul(valor), resultado, primeiroFiltro);
        }
        else
        {
            if (atributo == "timestamp")
                aplicarFiltroIntervaloInt(PesquisavelInt::reposicao_timestamp, std::stoul(valor), std::stoul(valor2), resultado, primeiroFiltro);
        }

        pilhaResultados.push_back(resultado);
        i = pos;
    }

    while (pilhaOperadores.tamanho() > 0)
    {
        resolverTopo();
    }

    TADS::ListaOrdenada<unsigned> &resultadoFinal = pilhaResultados[0];
    imprimirReposicoes(resultadoFinal, "LR");
}