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
    unsigned _id;
    unsigned _timestamp;
    TADS::Vector<unsigned> _id_produtos;
    TADS::Vector<unsigned> _qtd_produtos;
    unsigned _num_produtos;

public:
    /**
     * @brief Construtor padrão que cria uma reposição vazia.
     */
    Reposicao();

    /**
     * @brief Construtor que inicializa todos os dados da reposição de estoque.
     */
    Reposicao(unsigned id, unsigned timestamp,
              const TADS::Vector<unsigned> &id_produtos,
              const TADS::Vector<unsigned> &qtd_produtos,
              unsigned num_produtos);
    /**
     * @brief Destrutor padrão.
     */
    ~Reposicao() = default;

    // Getters
    unsigned getId() const;
    unsigned getTimestamp() const;
    const TADS::Vector<unsigned> &getIdProdutos() const;
    const TADS::Vector<unsigned> &getQtdProdutos() const;
    unsigned getNumProdutos() const;
};