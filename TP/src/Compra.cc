/**
 * @file Compra.cpp
 * @author Bruno Vieira
 * @brief Implementação da classe Compra.
 * @version 0.1
 * @date 2025-06-23
 */

#include "Compra.h"

Compra::Compra()
    : _id(0), _timestamp(0), _id_usuario(0), _num_produtos(0) {}

Compra::Compra(unsigned id, unsigned timestamp, unsigned id_usuario, const TADS::Vector<unsigned> &id_produtos, const TADS::Vector<unsigned> &qtd_produtos, unsigned num_produtos)
    : _id(id), _timestamp(timestamp), _id_usuario(id_usuario), _id_produtos(id_produtos), _qtd_produtos(qtd_produtos), _num_produtos(num_produtos) {}

unsigned Compra::getId() const { return _id; }
unsigned Compra::getTimestamp() const { return _timestamp; }
unsigned Compra::getIdUsuario() const { return _id_usuario; }
const TADS::Vector<unsigned> &Compra::getIdProdutos() const { return _id_produtos; }
const TADS::Vector<unsigned> &Compra::getQtdProdutos() const { return _qtd_produtos; }
unsigned Compra::getNumProdutos() const { return _num_produtos; }
