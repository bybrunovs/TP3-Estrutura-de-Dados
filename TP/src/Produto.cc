/**
 * @file Produto.cpp
 * @author Bruno Vieira
 * @brief Implementação da classe Produto.
 * @version 0.1
 * @date 2025-06-23
 */

#include "Produto.h"

Produto::Produto()
    : _id(0), _nome(""), _preco(0.0), _qtd(0), _categoria(""), _marca(""), _condicao("") {}

Produto::Produto(unsigned id, std::string nome, double preco, unsigned qtd, std::string categoria, std::string marca, std::string condicao)
    : _id(id), _nome(nome), _preco(preco), _qtd(qtd), _categoria(categoria), _marca(marca), _condicao(condicao) {}

unsigned Produto::getId() const { return _id; }
std::string Produto::getNome() const { return _nome; }
double Produto::getPreco() const { return _preco; }
unsigned Produto::getQtd() const { return _qtd; }
std::string Produto::getCategoria() const { return _categoria; }
std::string Produto::getMarca() const { return _marca; }
std::string Produto::getCondicao() const { return _condicao; }

void Produto::setQtd(unsigned qtd) { _qtd = qtd; }
