#include "Usuario.h"

Usuario::Usuario() : _id(0), _idade(0), _nome("") {}
Usuario::Usuario(unsigned id, std::string nome, unsigned idade, std::string cidade, std::string estado, std::string nacionalidade): _id(id), _nome(nome), _idade(idade), _cidade(cidade), _estado(estado), _nacionalidade(nacionalidade) {}

std::string Usuario::getNome() const { return _nome; }
unsigned Usuario::getId() const { return _id; }
unsigned Usuario::getIdade() const { return _idade; }