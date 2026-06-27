namespace TADS
{
    template <typename T>
    ListaOrdenada<T>::ListaOrdenada() : _dados() {}

    template <typename T>
    ListaOrdenada<T>::ListaOrdenada(unsigned capacidade) : _dados(capacidade) {}

    // Inserção

    template <typename T>
    void ListaOrdenada<T>::inserir(const T &elemento)
    {
        // Adiciona no fim para garantir espaço no buffer
        _dados.push_back(elemento);

        // Desloca para a esquerda até encontrar a posição correta
        unsigned i = _dados.tamanho() - 1;
        while (i > 0 && _dados[i] < _dados[i - 1])
        {
            T temp = _dados[i - 1];
            _dados[i - 1] = _dados[i];
            _dados[i] = temp;
            i--;
        }
    }

    // Remoção

    template <typename T>
    void ListaOrdenada<T>::deletar(const T &elemento)
    {
        int indice = buscaBinaria(elemento);
        if (indice == -1)
            throw std::out_of_range("Elemento não encontrado na lista");
        _dados.deleteElemento(static_cast<unsigned>(indice));
    }

    template <typename T>
    int ListaOrdenada<T>::buscaBinaria(const T &elemento) const
    {
        if (_dados.tamanho() == 0)
            return -1;

        unsigned esq = 0;
        unsigned dir = _dados.tamanho() - 1;

        while (esq <= dir)
        {
            unsigned meio = esq + (dir - esq) / 2;

            if (_dados[meio] == elemento)
                return static_cast<int>(meio);
            else if (_dados[meio] < elemento)
                esq = meio + 1;
            else
            {
                if (meio == 0)
                    break; // evita underflow em unsigned
                dir = meio - 1;
            }
        }
        return -1;
    }

    template <typename T>
    bool ListaOrdenada<T>::contains(const T &elemento) const
    {
        return buscaBinaria(elemento) != -1;
    }

    template <typename T>
    int ListaOrdenada<T>::getIndice(const T &elemento) const
    {
        return buscaBinaria(elemento);
    }

    template <typename T>
    const T &ListaOrdenada<T>::operator[](unsigned indice) const
    {
        return _dados[indice];
    }

    template <typename T>
    unsigned ListaOrdenada<T>::tamanho() const
    {
        return _dados.tamanho();
    }

    template <typename T>
    void ListaOrdenada<T>::limpar()
    {
        _dados.limpar();
    }

} // namespace TADS
