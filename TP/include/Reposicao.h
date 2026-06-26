/**
 * @file Reposicao.h
 * @author Bruno Vieira
 * @brief Representa uma reposição de estoque no mercado.
 * @version 0.1
 * @date 2025-06-23
 */

#pragma once

#include "Vector.h"

/**
 * @brief Representa uma operação que adiciona unidades ao estoque de um ou mais produtos.
 */
class Reposicao
{
private:
    unsigned _id;                         ///< Identificador único da reposição
    unsigned _timestamp;                  ///< Instante lógico em que a reposição ocorreu
    TADS::Vector<unsigned> _id_produtos;  ///< Ponteiro para arranjo com os identificadores dos produtos repostos
    TADS::Vector<unsigned> _qtd_produtos; ///< Ponteiro para arranjo com as respectivas quantidades adicionadas
    unsigned _num_produtos;               ///< Número total de produtos distintos contidos nesta reposição

public:
    /**
     * @brief Construtor padrão que cria uma reposição vazia.
     */
    Reposicao();

    /**
     * @brief Construtor que inicializa todos os dados da reposição de estoque.
     * @param id Identificador da reposição.
     * @param timestamp Instante lógico da operação.
     * @param id_produtos Arranjo dinâmico contendo os IDs dos produtos repostos.
     * @param qtd_produtos Arranjo dinâmico contendo as quantidades adicionadas.
     * @param num_produtos O tamanho dos arranjos.
     */
    Reposicao(unsigned id, unsigned timestamp, unsigned *id_produtos, unsigned *qtd_produtos, unsigned num_produtos);

    /**
     * @brief Destrutor que deve liberar a memória alocada para os arranjos dinâmicos.
     */
    ~Reposicao();

    // Getters
    unsigned getId() const;
    unsigned getTimestamp() const;
    const TADS::Vector<unsigned> &getIdProdutos() const;
    const TADS::Vector<unsigned> &getQtdProdutos() const;
    unsigned getNumProdutos() const;
};