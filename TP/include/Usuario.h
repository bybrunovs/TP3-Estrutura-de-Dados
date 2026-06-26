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
    unsigned _id; ///< Identificador único do utilizador, atribuído automaticamente a partir de 0[cite: 21, 22].
    std::string _nome; ///< Nome do utilizador (sem espaços)[cite: 53].
    unsigned _idade; ///< Idade do utilizador (inteiro não negativo)[cite: 54].
    std::string _cidade; ///< Cidade do utilizador (sem espaços)[cite: 54].
    std::string _estado; ///< Estado do utilizador (sem espaços)[cite: 54].
    std::string _nacionalidade; ///< Nacionalidade do utilizador (sem espaços)[cite: 55].

public:
    /**
     * @brief Construtor padrão que cria um utilizador vazio.
     */
    Usuario();

    /**
     * @brief Construtor que inicializa todos os dados do utilizador.
     * @param id Identificador do utilizador.
     * @param nome Nome do utilizador.
     * @param idade Idade do utilizador.
     * @param cidade Cidade do utilizador.
     * @param estado Estado do utilizador.
     * @param nacionalidade Nacionalidade do utilizador.
     */
    Usuario(unsigned id, std::string nome, unsigned idade, std::string cidade, std::string estado, std::string nacionalidade);

    /**
     * @brief Destrutor padrão.
     */
    ~Usuario() = default;

    // Getters
    
    /**
     * @brief Retorna o identificador do utilizador.
     * @return Identificador único do utilizador.
     */
    unsigned getId() const;

    /**
     * @brief Retorna o nome do utilizador.
     * @return String com o nome do utilizador.
     */
    std::string getNome() const;

    /**
     * @brief Retorna a idade do utilizador.
     * @return Idade do utilizador.
     */
    unsigned getIdade() const;

    /**
     * @brief Retorna a cidade do utilizador.
     * @return String com a cidade do utilizador.
     */
    std::string getCidade() const;

    /**
     * @brief Retorna o estado do utilizador.
     * @return String com o estado do utilizador.
     */
    std::string getEstado() const;

    /**
     * @brief Retorna a nacionalidade do utilizador.
     * @return String com a nacionalidade do utilizador.
     */
    std::string getNacionalidade() const;
};