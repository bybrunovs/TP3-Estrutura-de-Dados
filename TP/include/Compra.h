/**
 * @file Compra.h
 * @author Bruno Vieira
 * @brief Representa uma compra no mercado.
 * @version 0.1
 * @date 2025-06-23
 */

#pragma once

#include "Vector.h"
/**
 * @brief Representa uma compra válida realizada por um usuário.
 */
class Compra
{
private:
    unsigned _id; ///< Identificador único da compra
    unsigned _timestamp; ///< Instante lógico em que a compra foi realizada
    unsigned _id_usuario; ///< Identificador do usuário que realizou a compra
    TADS::Vector<unsigned> _id_produtos; ///< Ponteiro para arranjo com os identificadores dos produtos comprados
    TADS::Vector<unsigned> _qtd_produtos; ///< Ponteiro para arranjo com as quantidades removidas do estoque
    unsigned _num_produtos; ///< Número total de produtos distintos contidos nesta compra

public:
    /**
     * @brief Construtor padrão que cria uma compra vazia.
     */
    Compra();

    /**
     * @brief Construtor que inicializa todos os dados da compra.
     * @param id Identificador da compra.
     * @param timestamp Instante lógico da operação.
     * @param id_usuario Identificador do comprador.
     * @param id_produtos Arranjo dinâmico contendo os IDs dos produtos comprados.
     * @param qtd_produtos Arranjo dinâmico contendo as respectivas quantidades.
     * @param num_produtos O tamanho dos arranjos.
     */
    Compra(unsigned id, unsigned timestamp, unsigned id_usuario, unsigned* id_produtos, unsigned* qtd_produtos, unsigned num_produtos);

    /**
     * @brief Destrutor que deve liberar a memória alocada para os arranjos dinâmicos.
     */
    ~Compra();

    // Getters
    unsigned getId() const;
    unsigned getTimestamp() const;
    unsigned getIdUsuario() const;
    const TADS::Vector<unsigned>& getIdProdutos() const;
    const TADS::Vector<unsigned>& getQtdProdutos() const;
    unsigned getNumProdutos() const;
};