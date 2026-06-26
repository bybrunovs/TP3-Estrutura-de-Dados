/**
 * @file Mercado.h
 * @author Bruno Vieira
 * @brief Classe central que controla o sistema de um mercado.
 * @version 0.2
 * @date 2025-06-23
 */

#pragma once

#include <string>

#include "Vector.h"
#include "ArvoreAVL.h"
#include "Usuario.h"
#include "Produto.h"
#include "Compra.h"
#include "Reposicao.h"

enum class PesquisavelUsuario
{
    id = 0,
    nome = 1,
    idade = 2,
    cidade = 3,
    estado = 4,
    nacionalidade = 5
};

enum class PesquisavelProduto
{
    id = 0,
    nome = 1,
    categoria = 2,
    marca = 3,
    condicao = 4
};

enum class PesquisavelCompra
{
    id = 0,
    timestamp = 1,
    idUsuario = 2,
    idProduto = 3,
};

enum class PesquisavelReposicao
{
    id = 0,
    timestamp = 1,
    idProduto = 2
};

/**
 * @brief Classe central que controla o sistema do MercadoDCC.
 * * Responsável por gerenciar os cadastros, realizar validações de estoque
 * e processar consultas multiatributo através de índices invertidos.
 */
class Mercado
{
private:
    // Vetores dinâmicos para armazenar as entidades sequencialmente.

    TADS::Vector<Usuario> _usuarios;
    TADS::Vector<Produto> _produtos;
    TADS::Vector<Compra> _compras;
    TADS::Vector<Reposicao> _reposicoes;

    // TODO: Adicionar estruturas os Índices Invertidos.
    TADS::Vector<TADS::ArvoreAVL<unsigned>> PesquisaveisUsuario;
    TADS::Vector<TADS::ArvoreAVL<unsigned>> PesquisaveisProduto;
    TADS::Vector<TADS::ArvoreAVL<unsigned>> PesquisaveisCompra;
    TADS::Vector<TADS::ArvoreAVL<unsigned>> PesquisaveisReposicao;

public:
    /**
     * @brief Construtor padrão do Mercado.
     */
    Mercado();

    /**
     * @brief Destrutor padrão do Mercado.
     */
    ~Mercado() = default;

    /**
     * @brief Cadastra um novo usuário no sistema (Comando U).
     * * O ID é gerado automaticamente. O usuário é inserido nos índices invertidos
     * de nome, idade, cidade, estado e nacionalidade.
     * * @param nome Nome do usuário (sem espaços).
     * @param idade Idade do usuário.
     * @param cidade Cidade do usuário.
     * @param estado Estado do usuário.
     * @param nacionalidade Nacionalidade do usuário.
     */
    void cadastrarUsuario(const std::string &nome, unsigned idade, const std::string &cidade, const std::string &estado, const std::string &nacionalidade);

    /**
     * @brief Cadastra um novo produto no sistema (Comando P).
     * * O ID é gerado automaticamente. O produto é inserido nos índices invertidos
     * de nome, categoria, marca e condição.
     * * @param nome Nome do produto.
     * @param preco Preço do produto.
     * @param qtd_inicial Quantidade inicial em estoque.
     * @param categoria Categoria do produto.
     * @param marca Marca do produto.
     * @param condicao Condição do produto (Novo, Usado, etc).
     */
    void cadastrarProduto(const std::string &nome, double preco, unsigned qtd_inicial, const std::string &categoria, const std::string &marca, const std::string &condicao);

    /**
     * @brief Registra uma reposição de estoque (Comando R).
     * * Incrementa o estoque dos produtos correspondentes e registra a operação.
     * * @param timestamp Instante lógico da reposição.
     * @param id_produtos Vetor de identificadores dos produtos repostos.
     * @param qtd_produtos Vetor de quantidades a serem adicionadas.
     */
    void registrarReposicao(unsigned timestamp, const TADS::Vector<unsigned> &id_produtos, const TADS::Vector<unsigned> &qtd_produtos);

    /**
     * @brief Registra uma compra (Comando C).
     * * Valida se há estoque suficiente para todos os produtos[cite: 143].
     * Se houver, decrementa o estoque, salva a compra e gera a saída.
     * Se não houver, a compra é inválida e gera a saída de erro apropriada[cite: 147].
     * * @param timestamp Instante lógico da compra.
     * @param id_usuario Identificador do usuário que realizou a compra.
     * @param id_produtos Vetor de identificadores dos produtos comprados.
     * @param qtd_produtos Vetor de quantidades demandadas.
     */
    void registrarCompra(unsigned timestamp, unsigned id_usuario, const TADS::Vector<unsigned> &id_produtos, const TADS::Vector<unsigned> &qtd_produtos);

    /**
     * @brief Consulta usuários que satisfazem simultaneamente os atributos informados (Comando LU).
     * @param atributos Vetor contendo os nomes dos atributos (ex: "nome", "cidade").
     * @param valores Vetor contendo os valores buscados (ex: "Ana", "BeloHorizonte").
     */
    void consultarUsuarios(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores);

    /**
     * @brief Consulta produtos que satisfazem simultaneamente os atributos informados (Comando LP).
     * @param atributos Vetor contendo os nomes dos atributos (ex: "categoria").
     * @param valores Vetor contendo os valores buscados (ex: "Informatica").
     */
    void consultarProdutos(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores);

    /**
     * @brief Consulta compras que satisfazem simultaneamente os atributos informados (Comando LC).
     * @param atributos Vetor contendo os nomes dos atributos (ex: "id_usuario").
     * @param valores Vetor contendo os valores buscados (ex: "0").
     */
    void consultarCompras(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores);

    /**
     * @brief Consulta reposições que satisfazem simultaneamente os atributos informados (Comando LR).
     * @param atributos Vetor contendo os nomes dos atributos (ex: "id_produto").
     * @param valores Vetor contendo os valores buscados (ex: "3").
     */
    void consultarReposicoes(const TADS::Vector<std::string> &atributos, const TADS::Vector<std::string> &valores);
};