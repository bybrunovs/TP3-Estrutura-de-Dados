#include "Mercado.h"

// Motor de consulta — Shunting-yard adaptado
//
// Todas as quatro funções de consulta (LU, LP, LC, LR) seguem o mesmo padrão:
//
// 1. Lê tokens da esquerda pra direita.
// 2. Se o token é operador (AND/OR/NOT): descarrega da pilhaOperadores todos os
//    operadores de precedência >= ao atual (exceto NOT descarregando NOT), depois
//    empilha o operador atual.
// 3. Se o token é atributo: lê valor (e valor2 se for intervalo), resolve o
//    filtro atômico e empilha o resultado em pilhaResultados.
// 4. No fim, esvazia pilhaOperadores aplicando cada operador sobre pilhaResultados.
//
// resolverTopo() aplica o operador do topo:
//   NOT → complemento do topo (unário)
//   AND → intersecao dos dois topos
//   OR  → uniao dos dois topos

// Função auxiliar
// Retorna true se o token é um operador booleano
static bool ehOperador(const std::string &t)
{
    return t == "AND" || t == "OR" || t == "NOT";
}

// Função auxiliar
// Verifica se uma string representa um número (inteiro ou decimal)
bool Mercado::eNumero(const std::string &str)
{
    if (str.empty())
        return false;
    bool pontoDecimal = false;
    for (unsigned i = 0; i < str.length(); ++i)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            if (str[i] == '.' && !pontoDecimal)
                pontoDecimal = true;
            else
                return false;
        }
    }
    return true;
}

// Lê o valor e, se aplicável, o segundo valor (consulta por faixa) a partir de pos.
// Avança pos além dos tokens lidos.
void Mercado::lerValores(const TADS::Vector<std::string> &tokens, unsigned &pos, const std::string &atributo, std::string &valor, std::string &valor2)
{
    valor = valor2 = "";

    if (pos < tokens.tamanho() && !ehOperador(tokens[pos]))
    {
        valor = tokens[pos++];
    }

    // Atributos que suportam consulta por faixa
    bool suportaIntervalo = (atributo == "idade" || atributo == "qtd" || atributo == "timestamp" || atributo == "preco");

    if (suportaIntervalo && pos < tokens.tamanho() && !ehOperador(tokens[pos]) && eNumero(tokens[pos]))
    {
        valor2 = tokens[pos++];
    }
}

// Consulta de Usuários (LU)

void Mercado::consultarUsuarios(const TADS::Vector<std::string> &tokens)
{
    // Sem filtros: retorna todos os usuários
    if (tokens.tamanho() == 0)
    {
        TADS::ListaOrdenada<unsigned> todos;
        for (unsigned i = 0; i < _usuarios.tamanho(); i++)
            todos.inserir(i);
        imprimirUsuarios(todos, "LU");
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
            auto v = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(complemento(v, _usuarios.tamanho()));
        }
        else
        {
            auto v2 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            auto v1 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(op == "AND" ? intersecao(v1, v2) : uniao(v1, v2));
        }
    };

    for (unsigned i = 0; i < tokens.tamanho();)
    {
        std::string token = tokens[i];

        if (ehOperador(token))
        {
            // Descarrega operadores de maior ou igual precedência (NOT não descarrega NOT)
            while (pilhaOperadores.tamanho() > 0 &&
                   precedencia(pilhaOperadores[pilhaOperadores.tamanho() - 1]) >= precedencia(token) && !(token == "NOT" && pilhaOperadores[pilhaOperadores.tamanho() - 1] == "NOT"))
                resolverTopo();
            pilhaOperadores.push_back(token);
            i++;
            continue;
        }

        unsigned pos = i + 1;
        std::string valor, valor2;
        lerValores(tokens, pos, token, valor, valor2);

        TADS::ListaOrdenada<unsigned> resultado;
        bool pf = true;

        if (valor2.empty())
        {
            if (token == "id")
                aplicarFiltroInt(PesquisavelInt::usuario_id, std::stoul(valor), resultado, pf);
            else if (token == "idade")
                aplicarFiltroInt(PesquisavelInt::usuario_idade, std::stoul(valor), resultado, pf);
            else if (token == "nome")
                aplicarFiltroString(PesquisavelString::usuario_nome, valor, resultado, pf);
            else if (token == "cidade")
                aplicarFiltroString(PesquisavelString::usuario_cidade, valor, resultado, pf);
            else if (token == "estado")
                aplicarFiltroString(PesquisavelString::usuario_estado, valor, resultado, pf);
            else if (token == "nacionalidade")
                aplicarFiltroString(PesquisavelString::usuario_nacionalidade, valor, resultado, pf);
        }
        else
        {
            if (token == "idade")
                aplicarFiltroIntervaloInt(PesquisavelInt::usuario_idade, std::stoul(valor), std::stoul(valor2), resultado, pf);
        }

        pilhaResultados.push_back(resultado);
        i = pos;
    }

    while (pilhaOperadores.tamanho() > 0)
        resolverTopo();
    imprimirUsuarios(pilhaResultados[0], "LU");
}

// Consulta de Produtos (LP)

void Mercado::consultarProdutos(const TADS::Vector<std::string> &tokens)
{
    if (tokens.tamanho() == 0)
    {
        TADS::ListaOrdenada<unsigned> todos;
        for (unsigned i = 0; i < _produtos.tamanho(); i++)
            todos.inserir(i);
        imprimirProdutos(todos, "LP");
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
            auto v = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(complemento(v, _produtos.tamanho()));
        }
        else
        {
            auto v2 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            auto v1 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(op == "AND" ? intersecao(v1, v2) : uniao(v1, v2));
        }
    };

    for (unsigned i = 0; i < tokens.tamanho();)
    {
        std::string token = tokens[i];

        if (ehOperador(token))
        {
            while (pilhaOperadores.tamanho() > 0 &&
                   precedencia(pilhaOperadores[pilhaOperadores.tamanho() - 1]) >= precedencia(token) &&
                   !(token == "NOT" && pilhaOperadores[pilhaOperadores.tamanho() - 1] == "NOT"))
                resolverTopo();
            pilhaOperadores.push_back(token);
            i++;
            continue;
        }

        unsigned pos = i + 1;
        std::string valor, valor2;
        lerValores(tokens, pos, token, valor, valor2);

        TADS::ListaOrdenada<unsigned> resultado;
        bool pf = true;

        if (valor2.empty())
        {
            if (token == "id")
                aplicarFiltroInt(PesquisavelInt::produto_id, std::stoul(valor), resultado, pf);
            else if (token == "qtd")
                aplicarFiltroInt(PesquisavelInt::produto_qtd, std::stoul(valor), resultado, pf);
            else if (token == "nome")
                aplicarFiltroString(PesquisavelString::produto_nome, valor, resultado, pf);
            else if (token == "categoria")
                aplicarFiltroString(PesquisavelString::produto_categoria, valor, resultado, pf);
            else if (token == "marca")
                aplicarFiltroString(PesquisavelString::produto_marca, valor, resultado, pf);
            else if (token == "condicao")
                aplicarFiltroString(PesquisavelString::produto_condicao, valor, resultado, pf);
        }
        else
        {
            if (token == "qtd")
                aplicarFiltroIntervaloInt(PesquisavelInt::produto_qtd, std::stoul(valor), std::stoul(valor2), resultado, pf);
            else if (token == "preco")
                aplicarFiltroIntervaloDouble(std::stod(valor), std::stod(valor2), resultado, pf);
        }

        pilhaResultados.push_back(resultado);
        i = pos;
    }

    while (pilhaOperadores.tamanho() > 0)
        resolverTopo();
    imprimirProdutos(pilhaResultados[0], "LP");
}

// Consulta de Compras (LC)

void Mercado::consultarCompras(const TADS::Vector<std::string> &tokens)
{
    if (tokens.tamanho() == 0)
    {
        TADS::ListaOrdenada<unsigned> todos;
        for (unsigned i = 0; i < _compras.tamanho(); i++)
            todos.inserir(i);
        imprimirCompras(todos, "LC");
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
            auto v = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(complemento(v, _compras.tamanho()));
        }
        else
        {
            auto v2 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            auto v1 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(op == "AND" ? intersecao(v1, v2) : uniao(v1, v2));
        }
    };

    for (unsigned i = 0; i < tokens.tamanho();)
    {
        std::string token = tokens[i];

        if (ehOperador(token))
        {
            while (pilhaOperadores.tamanho() > 0 &&
                   precedencia(pilhaOperadores[pilhaOperadores.tamanho() - 1]) >= precedencia(token) &&
                   !(token == "NOT" && pilhaOperadores[pilhaOperadores.tamanho() - 1] == "NOT"))
                resolverTopo();
            pilhaOperadores.push_back(token);
            i++;
            continue;
        }

        unsigned pos = i + 1;
        std::string valor, valor2;
        lerValores(tokens, pos, token, valor, valor2);

        TADS::ListaOrdenada<unsigned> resultado;
        bool pf = true;

        if (valor2.empty())
        {
            if (token == "id")
                aplicarFiltroInt(PesquisavelInt::compra_id, std::stoul(valor), resultado, pf);
            else if (token == "timestamp")
                aplicarFiltroInt(PesquisavelInt::compra_timestamp, std::stoul(valor), resultado, pf);
            else if (token == "id_usuario")
                aplicarFiltroInt(PesquisavelInt::compra_idUsuario, std::stoul(valor), resultado, pf);
            else if (token == "id_produto")
                aplicarFiltroInt(PesquisavelInt::compra_idProduto, std::stoul(valor), resultado, pf);
        }
        else
        {
            if (token == "timestamp")
                aplicarFiltroIntervaloInt(PesquisavelInt::compra_timestamp, std::stoul(valor), std::stoul(valor2), resultado, pf);
        }

        pilhaResultados.push_back(resultado);
        i = pos;
    }

    while (pilhaOperadores.tamanho() > 0)
        resolverTopo();
    imprimirCompras(pilhaResultados[0], "LC");
}

// Consulta de Reposições (LR)

void Mercado::consultarReposicoes(const TADS::Vector<std::string> &tokens)
{
    if (tokens.tamanho() == 0)
    {
        TADS::ListaOrdenada<unsigned> todos;
        for (unsigned i = 0; i < _reposicoes.tamanho(); i++)
            todos.inserir(i);
        imprimirReposicoes(todos, "LR");
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
            auto v = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(complemento(v, _reposicoes.tamanho()));
        }
        else
        {
            auto v2 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            auto v1 = pilhaResultados[pilhaResultados.tamanho() - 1];
            pilhaResultados.pop_back();
            pilhaResultados.push_back(op == "AND" ? intersecao(v1, v2) : uniao(v1, v2));
        }
    };

    for (unsigned i = 0; i < tokens.tamanho();)
    {
        std::string token = tokens[i];

        if (ehOperador(token))
        {
            while (pilhaOperadores.tamanho() > 0 &&
                   precedencia(pilhaOperadores[pilhaOperadores.tamanho() - 1]) >= precedencia(token) &&
                   !(token == "NOT" && pilhaOperadores[pilhaOperadores.tamanho() - 1] == "NOT"))
                resolverTopo();
            pilhaOperadores.push_back(token);
            i++;
            continue;
        }

        unsigned pos = i + 1;
        std::string valor, valor2;
        lerValores(tokens, pos, token, valor, valor2);

        TADS::ListaOrdenada<unsigned> resultado;
        bool pf = true;

        if (valor2.empty())
        {
            if (token == "id")
                aplicarFiltroInt(PesquisavelInt::reposicao_id, std::stoul(valor), resultado, pf);
            else if (token == "timestamp")
                aplicarFiltroInt(PesquisavelInt::reposicao_timestamp, std::stoul(valor), resultado, pf);
            else if (token == "id_produto")
                aplicarFiltroInt(PesquisavelInt::reposicao_idProduto, std::stoul(valor), resultado, pf);
        }
        else
        {
            if (token == "timestamp")
                aplicarFiltroIntervaloInt(PesquisavelInt::reposicao_timestamp, std::stoul(valor), std::stoul(valor2), resultado, pf);
        }

        pilhaResultados.push_back(resultado);
        i = pos;
    }

    while (pilhaOperadores.tamanho() > 0)
        resolverTopo();
    imprimirReposicoes(pilhaResultados[0], "LR");
}
