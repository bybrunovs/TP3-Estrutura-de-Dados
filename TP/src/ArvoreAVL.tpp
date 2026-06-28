namespace TADS
{
    template <typename TipoChave, typename TipoDado>
    ArvoreAVL<TipoChave, TipoDado>::ArvoreAVL()
    {
        this->_raiz = nullptr;
    }

    template <typename TipoChave, typename TipoDado>
    ArvoreAVL<TipoChave, TipoDado>::ArvoreAVL(const ArvoreAVL &outra)
    {
        this->_raiz = copiarRecursivo(outra._raiz);
    }

    template <typename TipoChave, typename TipoDado>
    ArvoreAVL<TipoChave, TipoDado> &ArvoreAVL<TipoChave, TipoDado>::operator=(const ArvoreAVL &outra)
    {
        if (this != &outra)
        {
            limpar();
            this->_raiz = copiarRecursivo(outra._raiz);
        }
        return *this;
    }

    template <typename TipoChave, typename TipoDado>
    ArvoreAVL<TipoChave, TipoDado>::~ArvoreAVL()
    {
        limpar();
    }

    // Inserção
    template <typename TipoChave, typename TipoDado>
    void ArvoreAVL<TipoChave, TipoDado>::inserir(const TipoChave &chave, const TipoDado &dado)
    {
        insereRecursivo(this->_raiz, chave, dado);
    }

    template <typename TipoChave, typename TipoDado>
    void ArvoreAVL<TipoChave, TipoDado>::insereRecursivo(No<TipoChave, TipoDado> *&no, const TipoChave &chave, const TipoDado &dado)
    {
        if (no == nullptr)
        {
            no = new No<TipoChave, TipoDado>(chave, dado);
            return;
        }

        if (chave < no->_chave)
        {
            insereRecursivo(no->esq, chave, dado);
        }
        else if (chave > no->_chave)
        {
            insereRecursivo(no->dir, chave, dado);
        }
        else
        {
            no->_dado = dado;
            return;
        }

        no->novaAltura();

        int bal = no->getBalanceamento();

        // Caso 1: Esquerda-Esquerda
        if (bal < -1 && chave < no->esq->_chave)
        {
            rotacionarDir(no);
        }
        // Caso 2: Direita-Direita
        else if (bal > 1 && chave > no->dir->_chave)
        {
            rotacionarEsq(no);
        }
        // Caso 3: Esquerda-Direita
        else if (bal < -1 && chave > no->esq->_chave)
        {
            rotacionarEsq(no->esq);
            rotacionarDir(no);
        }
        // Caso 4: Direita-Esquerda
        else if (bal > 1 && chave < no->dir->_chave)
        {
            rotacionarDir(no->dir);
            rotacionarEsq(no);
        }
    }

    // Remoção

    template <typename TipoChave, typename TipoDado>
    void ArvoreAVL<TipoChave, TipoDado>::deletar(const TipoChave &chave)
    {
        deletarRecursivo(this->_raiz, chave);
    }

    template <typename TipoChave, typename TipoDado>
    void ArvoreAVL<TipoChave, TipoDado>::deletarRecursivo(No<TipoChave, TipoDado> *&no, const TipoChave &chave)
    {
        if (no == nullptr)
            return;

        if (chave < no->_chave)
        {
            deletarRecursivo(no->esq, chave);
        }
        else if (chave > no->_chave)
        {
            deletarRecursivo(no->dir, chave);
        }
        else
        {
            // Nó com 0 ou 1 filho
            if (no->esq == nullptr || no->dir == nullptr)
            {
                No<TipoChave, TipoDado> *filho = no->esq ? no->esq : no->dir;

                if (filho == nullptr) // Folha
                {
                    delete no;
                    no = nullptr;
                }
                else // Um filho
                {
                    No<TipoChave, TipoDado> *tempDelete = no;
                    no = filho;
                    delete tempDelete;
                }
            }
            else
            {
                // Nó com 2 filhos: substitui pelo sucessor (menor da subárvore direita)
                No<TipoChave, TipoDado> *sucessor = no->dir;
                while (sucessor->esq != nullptr)
                    sucessor = sucessor->esq;

                no->_chave = sucessor->_chave;
                no->_dado = sucessor->_dado;

                deletarRecursivo(no->dir, sucessor->_chave);
            }
        }

        if (no == nullptr)
            return;

        no->novaAltura();

        int bal = no->getBalanceamento();

        // Caso 1: Esquerda-Esquerda
        if (bal < -1 && no->esq->getBalanceamento() <= 0)
        {
            rotacionarDir(no);
        }
        // Caso 3: Esquerda-Direita
        else if (bal < -1 && no->esq->getBalanceamento() > 0)
        {
            rotacionarEsq(no->esq);
            rotacionarDir(no);
        }
        // Caso 2: Direita-Direita
        else if (bal > 1 && no->dir->getBalanceamento() >= 0)
        {
            rotacionarEsq(no);
        }
        // Caso 4: Direita-Esquerda
        else if (bal > 1 && no->dir->getBalanceamento() < 0)
        {
            rotacionarDir(no->dir);
            rotacionarEsq(no);
        }
    }

    // Busca

    template <typename TipoChave, typename TipoDado>
    TipoDado *ArvoreAVL<TipoChave, TipoDado>::buscar(const TipoChave &chave)
    {
        No<TipoChave, TipoDado> *resultado = buscarRecursivo(this->_raiz, chave);
        return resultado ? &resultado->_dado : nullptr;
    }

    template <typename TipoChave, typename TipoDado>
    No<TipoChave, TipoDado> *ArvoreAVL<TipoChave, TipoDado>::buscarRecursivo(No<TipoChave, TipoDado> *no, const TipoChave &chave) const
    {
        if (no == nullptr || no->_chave == chave)
            return no;

        if (chave < no->_chave)
            return buscarRecursivo(no->esq, chave);
        else
            return buscarRecursivo(no->dir, chave);
    }

    // Rotações

    template <typename TipoChave, typename TipoDado>
    void ArvoreAVL<TipoChave, TipoDado>::rotacionarDir(No<TipoChave, TipoDado> *&no)
    {
        No<TipoChave, TipoDado> *novaRaiz = no->esq;
        no->esq = novaRaiz->dir;
        novaRaiz->dir = no;

        no->novaAltura();
        novaRaiz->novaAltura();

        no = novaRaiz;
    }

    template <typename TipoChave, typename TipoDado>
    void ArvoreAVL<TipoChave, TipoDado>::rotacionarEsq(No<TipoChave, TipoDado> *&no)
    {
        No<TipoChave, TipoDado> *novaRaiz = no->dir;
        no->dir = novaRaiz->esq;
        novaRaiz->esq = no;

        no->novaAltura();
        novaRaiz->novaAltura();

        no = novaRaiz;
    }

    // Utilitários

    template <typename TipoChave, typename TipoDado>
    int ArvoreAVL<TipoChave, TipoDado>::getAltura()
    {
        return (this->_raiz != nullptr) ? this->_raiz->_altura : 0;
    }

    template <typename TipoChave, typename TipoDado>
    void ArvoreAVL<TipoChave, TipoDado>::limpar()
    {
        limparRecursivo(this->_raiz);
        this->_raiz = nullptr;
    }

    template <typename TipoChave, typename TipoDado>
    No<TipoChave, TipoDado> *ArvoreAVL<TipoChave, TipoDado>::copiarRecursivo(const No<TipoChave, TipoDado> *no) const
    {
        if (no == nullptr)
            return nullptr;

        No<TipoChave, TipoDado> *copia = new No<TipoChave, TipoDado>(no->_chave, no->_dado);
        copia->_altura = no->_altura;
        copia->esq = copiarRecursivo(no->esq);
        copia->dir = copiarRecursivo(no->dir);
        return copia;
    }

    template <typename TipoChave, typename TipoDado>
    void ArvoreAVL<TipoChave, TipoDado>::limparRecursivo(No<TipoChave, TipoDado> *no)
    {
        if (no != nullptr)
        {
            limparRecursivo(no->esq);
            limparRecursivo(no->dir);
            delete no;
        }
    }

    template <typename TipoChave, typename TipoDado>
    TADS::ListaOrdenada<TipoDado> *ArvoreAVL<TipoChave, TipoDado>::buscarIntervalo(const TipoChave &min, const TipoChave &max) const
    {
        TADS::ListaOrdenada<TipoDado> *resultado = new TADS::ListaOrdenada<TipoDado>();
        intervaloRecursivo(this->_raiz, min, max, *resultado);
        return resultado;
    }

    template <typename TipoChave, typename TipoDado>
    void ArvoreAVL<TipoChave, TipoDado>::intervaloRecursivo(No<TipoChave, TipoDado> *no, const TipoChave &min, const TipoChave &max, TADS::ListaOrdenada<TipoDado> &resultado) const
    {
        if (no == nullptr)
            return;

        // Se a chave atual é maior que min, pode haver resultados na subárvore esquerda
        if (min < no->_chave)
            intervaloRecursivo(no->esq, min, max, resultado);

        // Visita o nó atual se estiver dentro do intervalo
        if (min <= no->_chave && no->_chave <= max)
            resultado.inserir(no->_dado);

        // Se a chave atual é menor que max, pode haver resultados na subárvore direita
        if (no->_chave < max)
            intervaloRecursivo(no->dir, min, max, resultado);
    }
} // namespace TADS
