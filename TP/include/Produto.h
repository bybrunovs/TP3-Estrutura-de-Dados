/**
 * @file Produto.h
 * @author Bruno Vieira
 * @brief Representa um produto no mercado.
 * @version 0.1
 * @date 2025-06-23
 */

#pragma once

#include <string>

/**
 * @brief Representa um produto cadastrado no mercado.
 */
class Produto
{
private:
    unsigned _id; ///< Identificador do produto
    std::string _nome;
    double _preco;
    unsigned _qtd;
    std::string _categoria;
    std::string _marca;
    std::string _condicao;

public:
    /**
     * @brief Construtor padrão que cria um produto vazio.
     */
    Produto();

    /**
     * @brief Construtor que define os dados do produto.
     */
    Produto(unsigned id, std::string nome, double preco, unsigned qtd, std::string categoria, std::string marca, std::string condicao);

    /**
     * @brief Destrutor padrão.
     */
    ~Produto() = default;

    // Getters
    unsigned getId() const;
    std::string getNome() const;
    double getPreco() const;
    unsigned getQtd() const;
    std::string getCategoria() const;
    std::string getMarca() const;
    std::string getCondicao() const;

    // Setters
    /**
     * @brief Atualiza a quantidade do produto em estoque (usado em compras e reposições).
     * @param qtd Nova quantidade do produto.
     */
    void setQtd(unsigned qtd);
};