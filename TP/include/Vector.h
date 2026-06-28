/**
 * @file Vector.h
 * @author Bruno Vieira
 * @brief TAD que faz o controle de memoria dinamica de um vetor
 * @version 0.2
 * @date 2025-04-22
 */

#pragma once

#include <stdexcept>
namespace TADS
{
    template <typename T>

    /**
     * @brief Classe de vetor dinâmico simples.
     *
     * Implementa um contêiner que armazena elementos em um buffer
     * alocado dinamicamente e cresce automaticamente quando necessário.
     */
    class Vector
    {
    private:
        T *_dados;

        unsigned _capacidade;
        unsigned _tamanho;

    public:
        /**
         * @brief Cria um vetor vazio sem capacidade inicial.
         */
        Vector();

        /**
         * @brief Cria um vetor com capacidade inicial e valor padrão para os elementos.
         * @param capacidade Quantidade de elementos alocados inicialmente.
         */
        Vector(unsigned capacidade);

        /**
         * @brief Cria um vetor com capacidade inicial e valor padrão para os elementos.
         * @param capacidade Quantidade de elementos alocados inicialmente.
         * @param valorPadrao Valor a ser atribuído a cada elemento alocado.
         */
        Vector(unsigned capacidade, const T &valorPadrao);

        /**
         * @brief Construtor de cópia que duplica os dados do outro vetor.
         * @param outro Vetor a ser copiado.
         */
        Vector(const Vector &outro);

        /**
         * @brief Construtor de movimento que transfere os dados do outro vetor.
         * @param outro Vetor cujos dados serão movidos.
         */
        Vector(Vector &&outro) noexcept;

        /**
         * @brief Insere um elemento no fim do vetor.
         *
         * Se a capacidade não for suficiente, o vetor cresce para o dobro.
         * @param elemento Elemento a ser inserido.
         */
        void push_back(const T &elemento);

        void pop_back();

        /**
         * @brief Retorna uma referência ao elemento no índice especificado.
         * @param indice Índice do elemento.
         * @return Referência ao elemento.
         * @throws std::out_of_range se o índice estiver fora dos limites.
         */
        T &getElemento(unsigned indice);

        /**
         * @brief Retorna uma referência ao elemento no índice especificado.
         * @param indice Índice do elemento.
         * @return Referência ao elemento.
         * @throws std::out_of_range se o índice estiver fora dos limites.
         */
        const T &getElemento(unsigned indice) const;

        /**
         * @brief Substitui o valor de um elemento existente.
         * @param indice Índice do elemento.
         * @param valor Novo valor.
         * @throws std::out_of_range se o índice estiver fora dos limites.
         */
        void setElemento(unsigned indice, const T &valor);

        /**
         * @brief Remove o elemento na posição informada deslocando os seguintes.
         * @param indice Índice do elemento a ser removido.
         * @throws std::out_of_range se o índice estiver fora dos limites.
         */
        void deleteElemento(unsigned indice);

        /**
         * @brief Verifica se o vetor contém um elemento equivalente.
         * @param elemento Elemento a ser procurado.
         * @return true se encontrado, false caso contrário.
         */
        bool contains(const T &elemento) const;

        /**
         * @brief Retorna o índice de um elemento no vetor.
         * @param elemento Elemento a ser procurado.
         * @return Índice do elemento.
         * @return -1 se o elemento não for encontrado.
         */
        int getIndice(const T &elemento) const;

        /**
         * @brief Atribuição profunda de outro vetor.
         * @param outro Vetor a ser copiado.
         * @return Referência a este vetor.
         */
        Vector &operator=(const Vector &outro);

        /**
         * @brief Move os dados de outro vetor para este vetor.
         * @param outro Vetor cujos dados serão movidos.
         * @return Referência a este vetor.
         */
        Vector &operator=(Vector &&outro) noexcept;

        /**
         * @brief Acesso ao elemento por índice.
         * @param indice Índice do elemento.
         * @return Referência ao elemento.
         * @throws std::out_of_range se o índice estiver fora dos limites.
         */
        T &operator[](unsigned indice);

        /**
         * @brief Acesso constante ao elemento por índice.
         * @param indice Índice do elemento.
         * @return Referência constante ao elemento.
         * @throws std::out_of_range se o índice estiver fora dos limites.
         */
        const T &operator[](unsigned indice) const;

        /**
         * @brief Desaloca a memória usada pelo vetor, limpando seus dados e resetando tamanho e capacidade.
         */
        void limpar();
        /**
         * @brief Retorna o número de elementos atualmente armazenados.
         * @return Tamanho do vetor.
         */
        unsigned tamanho() const;

        /**
         * @brief Retorna a capacidade atual do buffer interno.
         * @return Capacidade do vetor.
         */
        unsigned capacidade() const;

        /**
         * @brief Destrói o vetor e libera a memória alocada.
         */
        ~Vector();
    };

} // namespace TADS

#include "../src/Vector.tpp"