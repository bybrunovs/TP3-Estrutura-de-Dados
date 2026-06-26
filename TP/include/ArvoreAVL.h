template <typename T>
struct No
{
    T _dado = T();
    int altura = 0;
    No<T> *esq = nullptr;
    No<T> *dir = nullptr;
    No<T> *pai = nullptr;

    No(T dado = T())
    {
        this->_dado = dado;
        altura = 1;
    }

    int getBalanceamento()
    {
        int alturaDir = (dir ? dir->altura : 0);
        int alturaEsq = (esq ? esq->altura : 0);
        return alturaDir - alturaEsq;
    }
};

template <typename T>
class ArvoreAVL
{
private:
    No<T> *_raiz = nullptr;

    void InsereRecursivo(No<T> *&no, No<T> *&novoNo);
    void ApagaRecursivo(No<T> *no);
    void rotacionarEsq(No<T> *no);
    void rotacionarDir(No<T> *no);
    int max(int a, int b);

public:
    ArvoreAVL();
    ~ArvoreAVL();
    void inserir(T &dado);
    void caminhar(T dado);
    int getAltura();
    void limpar();
};

template <typename T>
ArvoreAVL<T>::ArvoreAVL()
{
}

template <typename T>
void ArvoreAVL<T>::inserir(T &dado)
{
    No<T> *novoNo = new No(dado);
    InsereRecursivo(this->_raiz, novoNo);
}

template <typename T>
void ArvoreAVL<T>::InsereRecursivo(No<T> *&no, No<T> *&novoNo)
{
    if (no == nullptr)
    {
        no = novoNo;
        return;
    }
    else
    {
        // se o dado for repetido, não insere
        if (novoNo->_dado < no->_dado)
        {
            InsereRecursivo(no->esq, novoNo);
        }
        else if (novoNo->_dado > no->_dado)
        {
            InsereRecursivo(no->dir, novoNo);
        }

        no->altura = max(dir ? dir->altura : 0, esq ? esq->altura : 0);

        // -- Balanciamento
        int balanciamento = no->getBalanceamento();

        // esquerda
        if (balanciamento > 1 && novoNo->_dado < no->esq->_dado)
            rotacionarDir(no);

        // direira
        if (balanciamento < -1 && novoNo->_dado > no->dir->_dado)
            rotacionarEsq(no);

        // esquerda direita
        if (balanciamento > 1 && novoNo->_dado > no->esq->_dado)
        {
            rotacionarEsq(no->esq);
            rotacionarDir(no);
        }

        // direita esquerda
        if (balanciamento < -1 && key < node->right->key)
        {
            rotacionarDir(no->dir);
            rotacionarEsq(no);
        }
    }
}

template <typename T>
int ArvoreAVL<T>::max(int a, int b)
{
    if (a < b)
        return b;
    else
        return a;
}

template <typename T>
void ArvoreAVL<T>::rotacionarEsq(No<T> *no)
{
    no->pai->dir = no->esq;
    no->esq = no->pai;

    if (no->pai->pai->esq == no->pai)
    {
        no->pai->pai->esq = no;
    }
    else
    {
        no->pai->pai->dir = no;
    }

    no->pai = no->pai->pai;

    if (no->pai == nullptr)
    {
        this->_raiz = no;
    }

    no->esq->pai = no;
    no->esq->dir->pai = no->esq;
}

template <typename T>
void ArvoreAVL<T>::rotacionarDir(No<T> *no)
{
    no->pai->esq = no->dir;
    no->dir = no->pai;

    if (no->pai->pai->esq == no->pai)
    {
        no->pai->pai->esq = no;
    }
    else
    {
        no->pai->pai->dir = no;
    }

    no->pai = no->pai->pai;
    no->dir->pai = no;
    no->dir->esq->pai = no->dir;
}

template <typename T>
int ArvoreAVL<T>::getAltura()
{
    return this->_raiz->altura;
}
template <typename T>
ArvoreAVL<T>::~ArvoreAVL()
{
    limpar();
}