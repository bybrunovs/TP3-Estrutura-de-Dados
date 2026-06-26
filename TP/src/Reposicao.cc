/**
 * @file Reposicao.cpp
 * @author Bruno Vieira
 * @brief Implementação da classe Reposicao.
 * @version 0.1
 * @date 2025-06-23
 */

#include "Reposicao.h"

Reposicao::Reposicao()
    : _id(0), _timestamp(0), _num_produtos(0) {}

Reposicao::Reposicao(unsigned id, unsigned timestamp, const TADS::Vector<unsigned> &id_produtos, const TADS::Vector<unsigned> &qtd_produtos, unsigned num_produtos)
 : _id(id), _timestamp(timestamp), _id_produtos(id_produtos), _qtd_produtos(qtd_produtos), _num_produtos(num_produtos) {}

unsigned Reposicao::getId() const { return _id; }
unsigned Reposicao::getTimestamp() const { return _timestamp; }
const TADS::Vector<unsigned> &Reposicao::getIdProdutos() const { return _id_produtos; }
const TADS::Vector<unsigned> &Reposicao::getQtdProdutos() const { return _qtd_produtos; }
unsigned Reposicao::getNumProdutos() const { return _num_produtos; }
