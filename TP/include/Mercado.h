/**
 * @file Mercado.h
 * @author Bruno Vieira
 * @brief Classe central que controla o sistema de um mercado.
 * @version 0.3
 * @date 2025-06-23
 */

#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Vector.h"
#include "ListaOrdenada.h"
#include "ArvoreAVL.h"
#include "Usuario.h"
#include "Produto.h"
#include "Compra.h"
#include "Reposicao.h"

enum class PesquisavelInt
{
    usuario_id = 0,
    usuario_idade = 1,
    produto_id = 2,
    reposicao_id = 3,
    reposicao_timestamp = 4,
    reposicao_idProduto = 5,
    compra_id = 6,
    compra_timestamp = 7,
    compra_idUsuario = 8,
    compra_idProduto = 9
};

enum class PesquisavelString
{
    usuario_nome = 0,
    usuario_cidade = 1,
    usuario_estado = 2,
    usuario_nacionalidade = 3,
    produto_nome = 4,
    produto_categoria = 5,
    produto_marca = 6,
    produto_condicao = 7
};

/**
 * @brief Classe central que controla o sistema do MercadoDCC.
 *
 * Responsável por gerenciar os cadastros, realizar validações de estoque
 * e processar consultas multiatributo através de índices invertidos.
 */
class Mercado
{
private:
    unsigned _proxIdUsuario = 0;
    unsigned _proxIdProduto = 0;
    unsigned _proxIdCompra = 0;
    unsigned _proxIdReposicao = 0;

    // Entidades
    TADS::Vector<Usuario> _usuarios;
    TADS::Vector<Produto> _produtos;
    TADS::Vector<Compra> _compras;
    TADS::Vector<Reposicao> _reposicoes;

    // Índices invertidos
    TADS::Vector<TADS::ArvoreAVL<unsigned, TADS::ListaOrdenada<unsigned>>> _indicesInt;
    TADS::Vector<TADS::ArvoreAVL<std::string, TADS::ListaOrdenada<unsigned>>> _indicesString;

    /**
     * @brief Insere um id em um índice invertido inteiro.
     *
     * Se a chave já existir na árvore, adiciona o id na lista existente.
     * Caso contrário, cria uma nova lista com o id e insere na árvore.
     *
     * @param qual  Qual índice usar (enum PesquisavelInt).
     * @param chave Chave a indexar.
     * @param id    ID da entidade a associar.
     */
    void indexarInt(PesquisavelInt qual, unsigned chave, unsigned id);

    /**
     * @brief Insere um id em um índice invertido string.
     * @param qual  Qual índice usar (enum PesquisavelString).
     * @param chave Chave a indexar.
     * @param id    ID da entidade a associar.
     */
    void indexarString(PesquisavelString qual, const std::string &chave, unsigned id);

    void aplicarFiltroInt(PesquisavelInt idx, unsigned chave,
                          TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro);

    void aplicarFiltroString(PesquisavelString idx, const std::string &chave,
                             TADS::ListaOrdenada<unsigned> &resultado, bool &primeiroFiltro);

public:
    Mercado();
    ~Mercado() = default;

    void cadastrarUsuario(const std::string &nome, unsigned idade, const std::string &cidade, const std::string &estado, const std::string &nacionalidade);
    void cadastrarProduto(const std::string &nome, double preco, unsigned qtd_inicial, const std::string &categoria, const std::string &marca, const std::string &condicao);
    void registrarReposicao(unsigned timestamp, const TADS::Vector<unsigned> &id_produtos, const TADS::Vector<unsigned> &qtd_produtos);
    void registrarCompra(unsigned timestamp, unsigned id_usuario, const TADS::Vector<unsigned> &id_produtos, const TADS::Vector<unsigned> &qtd_produtos);

    void consultarUsuarios(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores);
    void consultarProdutos(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores);
    void consultarCompras(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores);
    void consultarReposicoes(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores);
};
