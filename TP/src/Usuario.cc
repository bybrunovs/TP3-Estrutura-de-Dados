/**
 * @file Usuario.cpp
 * @author Bruno Vieira
 * @brief Implementação da classe Usuario.
 * @version 0.2
 * @date 2025-06-23
 */

#include "Usuario.h"

Usuario::Usuario()
    : _id(0), _nome(""), _idade(0), _cidade(""), _estado(""), _nacionalidade("") {}

Usuario::Usuario(unsigned id, std::string nome, unsigned idade, std::string cidade, std::string estado, std::string nacionalidade)
    : _id(id), _nome(nome), _idade(idade), _cidade(cidade), _estado(estado), _nacionalidade(nacionalidade) {}

unsigned Usuario::getId() const { return _id; }
std::string Usuario::getNome() const { return _nome; }
unsigned Usuario::getIdade() const { return _idade; }
std::string Usuario::getCidade() const { return _cidade; }
std::string Usuario::getEstado() const { return _estado; }
std::string Usuario::getNacionalidade() const { return _nacionalidade; }
