/**
 * @file Usuario.h
 * @author Bruno Vieira
 * @brief Representa um utilizador no mercado (MercadoDCC).
 * @version 0.2
 * @date 2025-06-23
 */

#pragma once

#include <string>

/**
 * @brief Representa um utilizador cadastrado na plataforma MercadoDCC.
 */
class Usuario
{
private:
    unsigned _id;
    std::string _nome;
    unsigned _idade;
    std::string _cidade;
    std::string _estado;
    std::string _nacionalidade;

public:
    /**
     * @brief Construtor padrão que cria um utilizador vazio.
     */
    Usuario();

    /**
     * @brief Construtor que inicializa todos os dados do utilizador.
    */
    Usuario(unsigned id, std::string nome, unsigned idade, std::string cidade, std::string estado, std::string nacionalidade);

    /**
     * @brief Destrutor padrão.
     */
    ~Usuario() = default;

    // Getters
    
    unsigned getId() const;
    std::string getNome() const;
    unsigned getIdade() const;
    std::string getCidade() const;
    std::string getEstado() const;
    std::string getNacionalidade() const;
};