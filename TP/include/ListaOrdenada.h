/**
 * @file ListaOrdenada.h
 * @author Bruno Vieira
 * @brief TAD de lista ordenada com inserção por insertion sort e busca binária.
 * @version 0.1
 * @date 2025-06-26
 */

#pragma once

#include <stdexcept>
#include "Vector.h"

namespace TADS
{
    /**
     * @brief Lista que mantém seus elementos sempre ordenados.
     *
     * Internamente usa um Vector. A inserção usa insertion sort para posicionar
     * o novo elemento no lugar correto. A busca usa busca binária (O(log n)).

     */
    template <typename T>
    class ListaOrdenada
    {
    private:
        Vector<T> _dados;

        /**
         * @brief Retorna o índice onde o elemento deveria estar (busca binária).
         * @param elemento Elemento a ser buscado.
         * @return Índice do elemento, ou -1 se não encontrado.
         */
        int buscaBinaria(const T &elemento) const;

    public:
        /**
         * @brief Cria uma lista ordenada vazia.
         */
        ListaOrdenada();

        /**
         * @brief Cria uma lista ordenada com capacidade inicial.
         * @param capacidade Capacidade inicial do buffer interno.
         */
        ListaOrdenada(unsigned capacidade);

        ~ListaOrdenada() = default;

        /**
         * @brief Insere um elemento mantendo a ordenação (insertion sort).
         * @param elemento Elemento a ser inserido.
         */
        void inserir(const T &elemento);

        /**
         * @brief Remove um elemento da lista.
         * @param elemento Elemento a ser removido.
         * @throws std::out_of_range se o elemento não existir.
         */
        void deletar(const T &elemento);

        /**
         * @brief Verifica se a lista contém o elemento (busca binária).
         * @param elemento Elemento a ser procurado.
         * @return true se encontrado, false caso contrário.
         */
        bool contains(const T &elemento) const;

        /**
         * @brief Retorna o índice do elemento na lista (busca binária).
         * @param elemento Elemento a ser procurado.
         * @return Índice do elemento, ou -1 se não encontrado.
         */
        int getIndice(const T &elemento) const;

        /**
         * @brief Acesso por índice (somente leitura para não quebrar a ordenação).
         * @param indice Índice do elemento.
         * @return Referência constante ao elemento.
         * @throws std::out_of_range se o índice estiver fora dos limites.
         */
        const T &operator[](unsigned indice) const;

        /**
         * @brief Retorna o número de elementos na lista.
         */
        unsigned tamanho() const;

        /**
         * @brief Limpa todos os elementos da lista.
         */
        void limpar();
    };

} // namespace TADS

#include "../src/ListaOrdenada.tpp"


namespace TADS
{
    /**
     * @brief Retorna uma nova ListaOrdenada com os elementos presentes em ambas as listas.
     *
     * Usa dois ponteiros percorrendo as listas em O(n + m), aproveitando a ordenação.
     *
     * @param a Primeira lista ordenada.
     * @param b Segunda lista ordenada.
     * @return ListaOrdenada<T> com os elementos comuns.
     */
    template <typename T>
    ListaOrdenada<T> intersecao(const ListaOrdenada<T> &a, const ListaOrdenada<T> &b)
    {
        ListaOrdenada<T> resultado;
        unsigned i = 0, j = 0;

        while (i < a.tamanho() && j < b.tamanho())
        {
            if (a[i] == b[j])
            {
                resultado.inserir(a[i]);
                i++;
                j++;
            }
            else if (a[i] < b[j])
                i++;
            else
                j++;
        }

        return resultado;
    }
}