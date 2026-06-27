/**
 * @file ArvoreAVL.h
 * @author Bruno Vieira
 * @brief TAD que faz o controle dos nos de uma árvore balanceada AVL
 * @version 0.2
 * @date 2025-06-23
 */

#pragma once

#include <stdexcept>
namespace TADS
{
    inline int maximo(int a, int b) { return (a > b) ? a : b; }

    template <typename TipoChave, typename TipoDado>
    struct No
    {
        TipoChave _chave;
        TipoDado _dado;
        int altura;
        No<TipoChave, TipoDado> *esq;
        No<TipoChave, TipoDado> *dir;

        No(const TipoChave &chave, const TipoDado &dado)
            : _chave(chave), _dado(dado), altura(1), esq(nullptr), dir(nullptr) {}

        int getBalanceamento()
        {
            int alturaDir = (dir ? dir->altura : 0);
            int alturaEsq = (esq ? esq->altura : 0);
            return alturaDir - alturaEsq;
        }

        void novaAltura()
        {
            int alturaDir = (dir ? dir->altura : 0);
            int alturaEsq = (esq ? esq->altura : 0);
            altura = 1 + maximo(alturaDir, alturaEsq);
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

        int getAltura();
        void limpar();
    };
} // namespace TADS

#include "../src/ArvoreAVL.tpp"
