/**
 * @file ArvoreAVL.h
 * @author Bruno Vieira
 * @brief TAD que faz o controle dos nos de uma árvore balanceada AVL
 * @version 0.3
 * @date 2025-06-23
 */

#pragma once

#include <stdexcept>
#include "Vector.h"

namespace TADS
{
    inline int maximo(int a, int b) { return (a > b) ? a : b; }

    template <typename TipoChave, typename TipoDado>
    struct No
    {
        TipoChave _chave;
        TipoDado _dado;
        int _altura;
        No<TipoChave, TipoDado> *esq;
        No<TipoChave, TipoDado> *dir;

        No(const TipoChave &chave, const TipoDado &dado)
            : _chave(chave), _dado(dado), _altura(1), esq(nullptr), dir(nullptr) {}

        int getBalanceamento()
        {
            int alturaDir = (dir ? dir->_altura : 0);
            int alturaEsq = (esq ? esq->_altura : 0);
            return alturaDir - alturaEsq;
        }

        void novaAltura()
        {
            int alturaDir = (dir ? dir->_altura : 0);
            int alturaEsq = (esq ? esq->_altura : 0);
            _altura = 1 + maximo(alturaDir, alturaEsq);
        }
    };

    template <typename TipoChave, typename TipoDado>
    class ArvoreAVL
    {
    private:
        No<TipoChave, TipoDado> *_raiz = nullptr;

        void insereRecursivo(No<TipoChave, TipoDado> *&no, const TipoChave &chave, const TipoDado &dado);
        void deletarRecursivo(No<TipoChave, TipoDado> *&no, const TipoChave &chave);
        No<TipoChave, TipoDado> *buscarRecursivo(No<TipoChave, TipoDado> *no, const TipoChave &chave) const;
        No<TipoChave, TipoDado> *copiarRecursivo(const No<TipoChave, TipoDado> *no) const;

        void limparRecursivo(No<TipoChave, TipoDado> *no);

        void rotacionarEsq(No<TipoChave, TipoDado> *&no);
        void rotacionarDir(No<TipoChave, TipoDado> *&no);

        /**
         * @brief Percorre in-order coletando dados cujas chaves estão em [min, max].
         */
        void intervaloRecursivo(No<TipoChave, TipoDado> *no, const TipoChave &min, const TipoChave &max, TADS::ListaOrdenada<TipoDado> &resultado) const;

    public:
        ArvoreAVL();
        ArvoreAVL(const ArvoreAVL &outra);
        ArvoreAVL &operator=(const ArvoreAVL &outra);
        ~ArvoreAVL();

        void inserir(const TipoChave &chave, const TipoDado &dado);
        void deletar(const TipoChave &chave);

        /**
         * @brief Busca um elemento pela chave.
         * @param chave Chave a ser buscada.
         * @return Ponteiro para o dado, ou nullptr se não encontrado.
         */
        TipoDado *buscar(const TipoChave &chave);

        /**
         * @brief Retorna um Vector com os dados de todos os nós cuja chave está em [min, max].
         *
         * Os dados são retornados em ordem crescente de chave (in-order).
         * Complexidade: O(log n + k), onde k é o número de resultados.
         *
         * @param min Limite inferior do intervalo (inclusivo).
         * @param max Limite superior do intervalo (inclusivo).
         * @return Vector<TipoDado> com os dados encontrados.
         */
        TADS::ListaOrdenada<TipoDado> *buscarIntervalo(const TipoChave &min, const TipoChave &max) const;

        int getAltura();
        void limpar();
    };
} // namespace TADS

#include "../src/ArvoreAVL.tpp"