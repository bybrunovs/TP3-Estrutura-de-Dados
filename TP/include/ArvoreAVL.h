/**
 * @file ArvoreAVL.h
 * @author Bruno Vieira
 * @brief TAD que faz o controle dos nos de uma árvore balanceada AVL
 * @version 0.1
 * @date 2025-06-23
 */

#pragma once

#include <stdexcept>
namespace TADS
{
    // Função auxiliar inline para calcular o máximo na struct
    inline int maximo(int a, int b) { return (a > b) ? a : b; }

    template <typename T>
    struct No
    {
        T _dado;
        int altura;
        No<T> *esq;
        No<T> *dir;

        No(T dado = T()) : _dado(dado), altura(1), esq(nullptr), dir(nullptr) {}

        int getBalanceamento()
        {
            int alturaDir = (dir ? dir->altura : 0);
            int alturaEsq = (esq ? esq->altura : 0);
            return alturaDir - alturaEsq;
            // Negativo = Pesa pra Esquerda | Positivo = Pesa pra Direita
        }

        void novaAltura()
        {
            int alturaDir = (dir ? dir->altura : 0);
            int alturaEsq = (esq ? esq->altura : 0);
            // Altura é 1 + a maior altura entre os filhos
            altura = 1 + maximo(alturaDir, alturaEsq);
        }
    };

    template <typename T>
    class ArvoreAVL
    {
    private:
        No<T> *_raiz = nullptr;

        void insereRecursivo(No<T> *&no, const T &dado);
        void deletarRecursivo(No<T> *&no, const T &dado);

        void limparRecursivo(No<T> *no);

        void rotacionarEsq(No<T> *&no);
        void rotacionarDir(No<T> *&no);

    public:
        ArvoreAVL();
        ~ArvoreAVL();
        void inserir(const T &dado);
        void deletar(const T &dado);

        int getAltura();
        void limpar();
    };
} // namespace TADS

#include "../src/ArvoreAVL.tpp"