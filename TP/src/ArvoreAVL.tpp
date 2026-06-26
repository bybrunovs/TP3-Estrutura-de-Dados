namespace TADS
{
    template <typename T>
    ArvoreAVL<T>::ArvoreAVL()
    {
        this->_raiz = nullptr;
    }

    template <typename T>
    ArvoreAVL<T>::~ArvoreAVL()
    {
        limpar();
    }

    template <typename T>
    void ArvoreAVL<T>::inserir(const T &dado)
    {
        insereRecursivo(this->_raiz, dado);
    }

    template <typename T>
    void ArvoreAVL<T>::insereRecursivo(No<T> *&no, const T &dado)
    {
        if (no == nullptr)
        {
            no = new No<T>(dado);
            return;
        }

        if (dado < no->_dado)
        {
            insereRecursivo(no->esq, dado);
        }
        else if (dado > no->_dado)
        {
            insereRecursivo(no->dir, dado);
        }
        else
        {
            return; // Se o dado for repetido, encerra a função
        }

        no->novaAltura();

        int balanciamento = no->getBalanceamento();

        // Rotação

        // Caso 1: Esquerda-Esquerda (Pesa pra Esquerda e inseriu na Esquerda)
        if (balanciamento < -1 && dado < no->esq->_dado)
        {
            rotacionarDir(no);
        }
        // Caso 2: Direita-Direita (Pesa pra Direita e inseriu na Direita)
        else if (balanciamento > 1 && dado > no->dir->_dado)
        {
            rotacionarEsq(no);
        }
        // Caso 3: Esquerda-Direita (Pesa pra Esquerda, mas inseriu na Direita do filho Esq)
        else if (balanciamento < -1 && dado > no->esq->_dado)
        {
            rotacionarEsq(no->esq);
            rotacionarDir(no);
        }
        // Caso 4: Direita-Esquerda (Pesa pra Direita, mas inseriu na Esquerda do filho Dir)
        else if (balanciamento > 1 && dado < no->dir->_dado)
        {
            rotacionarDir(no->dir);
            rotacionarEsq(no);
        }
    }

    template <typename T>
    void ArvoreAVL<T>::deletar(const T &dado)
    {
        deletarRecursivo(this->_raiz, dado);
    }

    template <typename T>
    void ArvoreAVL<T>::deletarRecursivo(No<T> *&no, const T &dado)
    {
        if (no == nullptr)
        {
            return; // O dado não existe na árvore
        }

        if (dado < no->_dado)
        {
            deletarRecursivo(no->esq, dado);
        }
        else if (dado > no->_dado)
        {
            deletarRecursivo(no->dir, dado);
        }
        else
        {

            // nó com 0 ou 1 filho
            if ((no->esq == nullptr) || (no->dir == nullptr))
            {
                No<T> *temp = no->esq ? no->esq : no->dir;

                // Sem filhos (Folha)
                if (temp == nullptr)
                {
                    temp = no;
                    no = nullptr;
                    delete temp;
                }
                else // Com um filho
                {
                    No<T> *tempDelete = no;
                    no = temp;         // O filho sobe para a posição do pai
                    delete tempDelete; // Apaga o pai
                }
            }
            else
            {
                // nó com 2 filhos
                // Pega o menor elemento da sub-árvore da direita (sucessor)
                No<T> *temp = no->dir;
                while (temp->esq != nullptr)
                {
                    temp = temp->esq;
                }

                no->_dado = temp->_dado;

                deletarRecursivo(no->dir, temp->_dado);
            }
        }

        // Se a árvore tinha só 1 nó e foi deletado
        if (no == nullptr)
            return;

        no->novaAltura();

        int balanciamento = no->getBalanceamento();

        // Rotações de Remoção

        // Caso 1: Esquerda-Esquerda (Pesou pra esquerda, e o filho esquerdo está neutro ou pesando pra esquerda)
        if (balanciamento < -1 && no->esq->getBalanceamento() <= 0)
        {
            rotacionarDir(no);
        }
        // Caso 3: Esquerda-Direita (Pesou pra esquerda, mas o filho esquerdo está pesando pra direita)
        else if (balanciamento < -1 && no->esq->getBalanceamento() > 0)
        {
            rotacionarEsq(no->esq);
            rotacionarDir(no);
        }
        // Caso 2: Direita-Direita (Pesou pra direita, e o filho direito está neutro ou pesando pra direita)
        else if (balanciamento > 1 && no->dir->getBalanceamento() >= 0)
        {
            rotacionarEsq(no);
        }
        // Caso 4: Direita-Esquerda (Pesou pra direita, mas o filho direito está pesando pra esquerda)
        else if (balanciamento > 1 && no->dir->getBalanceamento() < 0)
        {
            rotacionarDir(no->dir);
            rotacionarEsq(no);
        }
    }

    template <typename T>
    void ArvoreAVL<T>::rotacionarDir(No<T> *&no)
    {
        No<T> *novaRaiz = no->esq;
        no->esq = novaRaiz->dir;
        novaRaiz->dir = no;

        no->novaAltura();
        novaRaiz->novaAltura();

        no = novaRaiz;
    }

    template <typename T>
    void ArvoreAVL<T>::rotacionarEsq(No<T> *&no)
    {
        No<T> *novaRaiz = no->dir;
        no->dir = novaRaiz->esq;
        novaRaiz->esq = no;

        no->novaAltura();
        novaRaiz->novaAltura();

        no = novaRaiz;
    }

    template <typename T>
    int ArvoreAVL<T>::getAltura()
    {
        return (this->_raiz != nullptr) ? this->_raiz->altura : 0;
    }

    template <typename T>
    void ArvoreAVL<T>::limpar()
    {
        limparRecursivo(this->_raiz);
        this->_raiz = nullptr; // reseta a árvore
    }

    template <typename T>
    void ArvoreAVL<T>::limparRecursivo(No<T> *no)
    {
        // Pós-Ordem: apaga esquerda, depois direita, depois o nó.
        if (no != nullptr)
        {
            limparRecursivo(no->esq);
            limparRecursivo(no->dir);
            delete no;
        }
    }

} // namespace TADS