namespace TADS
{
    template <typename T>
    Vector<T>::Vector() : _capacidade(0), _tamanho(0)
    {
        // Inicia sem alocar memória, o primeiro push_back criará a capacidade.
        this->_dados = nullptr;
    }

    template <typename T>
    Vector<T>::Vector(unsigned capacidade) : _dados(new T[capacidade]), _capacidade(capacidade), _tamanho(0)
    {
        // Aloca o buffer inicial com a capacidade especificada.
    }

    template <typename T>
    Vector<T>::Vector(unsigned capacidade, const T &valorPadrao) : _capacidade(capacidade), _tamanho(capacidade)
    {
        // Aloca o buffer inicial com a capacidade especificada.
        this->_dados = new T[capacidade];
        // Inicializa os elementos com o valor padrão.
        for (unsigned i = 0; i < capacidade; i++)
        {
            this->_dados[i] = valorPadrao;
        }
    }

    template <typename T>
    Vector<T>::Vector(const Vector &outro) : _capacidade(outro._capacidade), _tamanho(outro._tamanho)
    {
        // copia os dados do vetor original para um novo buffer.
        this->_dados = new T[outro._capacidade];
        for (unsigned i = 0; i < outro.tamanho(); i++)
        {
            this->_dados[i] = outro._dados[i];
        }
    }

    template <typename T>
    Vector<T>::Vector(Vector &&outro) noexcept : _dados(outro._dados), _capacidade(outro._capacidade), _tamanho(outro._tamanho)
    {
        outro._dados = nullptr;
        outro._tamanho = 0;
        outro._capacidade = 0;
    }

    template <typename T>
    void Vector<T>::push_back(const T &elemento)
    {
        if (this->tamanho() + 1 <= this->_capacidade)
        {
            this->_dados[this->_tamanho] = elemento;
            this->_tamanho++;
            return;
        }

        unsigned novaCapacidade = (this->_capacidade == 0) ? 1 : this->_capacidade * 2;
        T *_novosDados = new T[novaCapacidade];
        for (unsigned i = 0; i < this->_tamanho; i++)
        {
            _novosDados[i] = this->_dados[i];
        }
        delete[] this->_dados;
        this->_dados = _novosDados;
        this->_capacidade = novaCapacidade;
        this->_dados[this->_tamanho] = elemento;
        this->_tamanho++;
    }

    template <typename T>
    void Vector<T>::pop_back()
    {
        this->_tamanho--;
    }

    template <typename T>
    T &Vector<T>::getElemento(unsigned indice)
    {
        if (indice < _tamanho)
        {
            return this->_dados[indice];
        }
        else
            throw std::out_of_range("Índice fora dos limites");
    }

    template <typename T>
    const T &Vector<T>::getElemento(unsigned indice) const
    {
        if (indice < _tamanho)
        {
            return this->_dados[indice];
        }
        else
            throw std::out_of_range("Índice fora dos limites");
    }

    template <typename T>
    void Vector<T>::setElemento(unsigned indice, const T &valor)
    {
        if (indice < _tamanho)
        {
            this->_dados[indice] = valor;
        }
        else
            throw std::out_of_range("Índice fora dos limites");
    }

    template <typename T>
    void Vector<T>::deleteElemento(unsigned indice)
    {
        if (indice < _tamanho)
        {
            // Desloca os elementos subsequentes para preencher o espaço vazio.
            for (unsigned i = indice; i < this->_tamanho - 1; i++)
            {
                this->_dados[i] = this->_dados[i + 1];
            }
            this->_tamanho--;
        }
        else
            throw std::out_of_range("Índice fora dos limites");
    }

    template <typename T>
    bool Vector<T>::contains(const T &elemento) const
    {
        for (unsigned i = 0; i < this->_tamanho; i++)
        {
            if (this->_dados[i] == elemento)
            {
                return true;
            }
        }
        return false;
    }

    template <typename T>
    int Vector<T>::getIndice(const T &elemento) const
    {
        for (unsigned i = 0; i < this->_tamanho; i++)
        {
            if (this->_dados[i] == elemento)
            {
                return i;
            }
        }
        return -1; // Retorna -1 se o elemento não for encontrado.
    }

    template <typename T>
    T &Vector<T>::operator[](unsigned indice)
    {
        if (indice < _tamanho)
        {
            return this->_dados[indice];
        }
        throw std::out_of_range("Índice fora dos limites");
    }

    template <typename T>
    const T &Vector<T>::operator[](unsigned indice) const
    {
        if (indice < _tamanho)
        {
            return this->_dados[indice];
        }
        throw std::out_of_range("Índice fora dos limites");
    }

    template <typename T>
    Vector<T> &Vector<T>::operator=(const Vector<T> &outro)
    {
        if (this != &outro)
        {
            delete[] _dados;
            _capacidade = outro._capacidade;
            _tamanho = outro._tamanho;
            if (_capacidade > 0)
            {
                _dados = new T[_capacidade];
                for (unsigned i = 0; i < _tamanho; i++)
                {
                    _dados[i] = outro._dados[i];
                }
            }
            else
            {
                _dados = nullptr;
            }
        }
        return *this;
    }

    template <typename T>
    Vector<T> &Vector<T>::operator=(Vector<T> &&outro) noexcept
    {
        if (this != &outro)
        {
            delete[] _dados;
            _dados = outro._dados;
            _capacidade = outro._capacidade;
            _tamanho = outro._tamanho;

            outro._dados = nullptr;
            outro._tamanho = 0;
            outro._capacidade = 0;
        }
        return *this;
    }

    template <typename T>
    void Vector<T>::limpar()
    {
        if (this->_dados != nullptr)
            delete[] this->_dados;
        this->_dados = nullptr;
        this->_tamanho = 0;
        this->_capacidade = 0;
    }

    template <typename T>
    unsigned Vector<T>::tamanho() const
    {
        return this->_tamanho;
    }

    template <typename T>
    unsigned Vector<T>::capacidade() const
    {
        return this->_capacidade;
    }

    template <typename T>
    Vector<T> &&move(Vector<T> &outro) noexcept
    {
        return static_cast<TADS::Vector<T> &&>(outro);
    }

    template <typename T>
    Vector<T>::~Vector()
    {
        // Libera a memória dinâmica usada pelo vetor.
        if (this->_dados != nullptr)
        {
            delete[] this->_dados;
        }
    }

} // namespace TADS