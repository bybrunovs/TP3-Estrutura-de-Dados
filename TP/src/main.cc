#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "Mercado.h"
#include "Vector.h"

// Verifica se uma linha de consulta contém operadores booleanos
static bool temOperadorBooleano(const std::string &linha)
{
    std::stringstream ss(linha);
    std::string token;
    while (ss >> token)
        if (token == "AND" || token == "OR" || token == "NOT")
            return true;
    return false;
}

int main(int argc, char *argv[])
{
    try
    {
        Mercado mercado;
        std::string linha;
        std::string token;

        while (std::getline(std::cin, linha))
        {
            if (linha.empty())
                continue;

            std::stringstream ss(linha);
            ss >> token;

            if (token == "U")
            {
                std::string nome, cidade, estado, nacionalidade;
                unsigned idade;
                ss >> nome >> idade >> cidade >> estado >> nacionalidade;

                mercado.cadastrarUsuario(nome, idade, cidade, estado, nacionalidade);
            }
            else if (token == "P")
            {
                std::string nome, categoria, marca, condicao;
                double preco;
                unsigned qtd_inicial;
                ss >> nome >> preco >> qtd_inicial >> categoria >> marca >> condicao;

                mercado.cadastrarProduto(nome, preco, qtd_inicial, categoria, marca, condicao);
            }
            else if (token == "R")
            {
                unsigned timestamp;
                ss >> timestamp;

                TADS::Vector<unsigned> id_produtos;
                TADS::Vector<unsigned> qtd_produtos;
                unsigned temp_id, temp_qtd;

                while (ss >> temp_id >> temp_qtd)
                {
                    id_produtos.push_back(temp_id);
                    qtd_produtos.push_back(temp_qtd);
                }

                mercado.registrarReposicao(timestamp, id_produtos, qtd_produtos);
            }
            else if (token == "C")
            {
                unsigned timestamp, id_usuario;
                ss >> timestamp >> id_usuario;

                TADS::Vector<unsigned> id_produtos;
                TADS::Vector<unsigned> qtd_produtos;
                unsigned temp_id, temp_qtd;

                while (ss >> temp_id >> temp_qtd)
                {
                    id_produtos.push_back(temp_id);
                    qtd_produtos.push_back(temp_qtd);
                }

                mercado.registrarCompra(timestamp, id_usuario, id_produtos, qtd_produtos);
            }
            else if (token == "LU" || token == "LP" || token == "LC" || token == "LR")
            {

                TADS::Vector<std::string> tokens;
                std::string t;

                if (!temOperadorBooleano(linha))
                {
                    TADS::Vector<std::string> lidos;
                    while (ss >> t)
                        lidos.push_back(t);

                    unsigned k = 0;
                    while (k < lidos.tamanho())
                    {
                        if (k != 0)
                            tokens.push_back("AND");

                        tokens.push_back(lidos[k]); // atributo
                        k++;

                        if (k < lidos.tamanho())
                        {
                            tokens.push_back(lidos[k]); // valor
                            k++;
                        }

                        // verifica se o próximo token é segundo valor de intervalo (numérico)
                        if (k < lidos.tamanho() && mercado.eNumero(lidos[k]))
                        {
                            tokens.push_back(lidos[k]); // max do intervalo
                            k++;
                        }
                    }
                }

                else
                {
                    while (ss >> t)
                    {
                        tokens.push_back(t);
                    }
                }

                if (token == "LU")
                {
                    mercado.consultarUsuarios(tokens);
                }
                else if (token == "LP")
                {
                    mercado.consultarProdutos(tokens);
                }
                else if (token == "LC")
                {
                    mercado.consultarCompras(tokens);
                }
                else if (token == "LR")
                {
                    mercado.consultarReposicoes(tokens);
                }
            }
            else
            {
                throw std::invalid_argument("Comando invalido: " + token);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erro: " << e.what() << '\n';
    }

    return 0;
}