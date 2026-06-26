#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "Mercado.h"
#include "Vector.h"

int main(int argc, char *argv[])
{
    try
    {
        Mercado mercado;
        std::string linha;
        std::string token;

        while (std::getline(std::cin, linha))
        {
            if (linha.empty()) continue;

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

                // Extrai identificadores e quantidades dinamicamente
                while (ss >> temp_id >> temp_qtd) {
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

                // Extrai identificadores e quantidades dinamicamente
                while (ss >> temp_id >> temp_qtd) {
                    id_produtos.push_back(temp_id);
                    qtd_produtos.push_back(temp_qtd);
                }

                mercado.registrarCompra(timestamp, id_usuario, id_produtos, qtd_produtos);
            }
            else if (token == "LU" || token == "LP" || token == "LC" || token == "LR")
            {
                TADS::Vector<std::string> atributos;
                TADS::Vector<std::string> valores;
                std::string temp_attr, temp_val;
                
                // Extrai pares de filtros dinamicamente
                while (ss >> temp_attr >> temp_val) {
                    atributos.push_back(temp_attr);
                    valores.push_back(temp_val);
                }

                if (token == "LU") {
                    mercado.consultarUsuarios(atributos, valores);
                } else if (token == "LP") {
                    mercado.consultarProdutos(atributos, valores);
                } else if (token == "LC") {
                    mercado.consultarCompras(atributos, valores);
                } else if (token == "LR") {
                    mercado.consultarReposicoes(atributos, valores);
                }
            }
            else
            {
                throw std::invalid_argument("Comando inválido: " + token);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erro: " << e.what() << '\n';
    }

    return 0;
}