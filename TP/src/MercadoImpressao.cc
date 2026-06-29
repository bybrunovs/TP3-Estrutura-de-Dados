#include "Mercado.h"


// Funções de impressão
// Recebem uma lista ordenada de IDs e imprimem os dados no formato especificado.


// Monta a linha de produtos de uma entidade (usuário/compra/reposição) no formato:
// "produto_1 <id> <qtd> produto_2 <id> <qtd> ..."
static std::string montarLinhaProdutos(const TADS::Vector<unsigned> &qtdPorProduto)
{
    std::ostringstream oss;
    unsigned cont = 1;
    for (unsigned j = 0; j < qtdPorProduto.tamanho(); j++)
        if (qtdPorProduto[j] > 0)
            oss << "produto_" << cont++ << " " << j << " " << qtdPorProduto[j] << " ";
    std::string linha = oss.str();
    if (!linha.empty()) linha.pop_back(); // remove espaço final
    return linha;
}

void Mercado::imprimirUsuarios(const TADS::ListaOrdenada<unsigned> &ids, const std::string &prefixo)
{
    if (ids.tamanho() == 0) { std::cout << prefixo << " VAZIO" << std::endl; return; }

    for (unsigned i = 0; i < ids.tamanho(); i++)
    {
        unsigned idUsuario = ids[i];
        const Usuario &u = _usuarios[idUsuario];
        std::cout << prefixo << " resultado_" << i + 1
                  << " usuario " << idUsuario
                  << " " << u.getNome() << " " << u.getIdade()
                  << " " << u.getCidade() << " " << u.getEstado()
                  << " " << u.getNacionalidade() << std::endl;

        // Busca todas as compras do usuário e soma qtd por produto
// Busca todas as compras do usuário e soma qtd por produto
        int pos = buscarBinariaInt(PesquisavelInt::compra_idUsuario, idUsuario);
        TADS::ListaOrdenada<unsigned> *idCompras = nullptr;
        
        if (pos != -1) {
            idCompras = &_indicesInt[static_cast<unsigned>(PesquisavelInt::compra_idUsuario)][pos].ids;
        }
        
        if (idCompras == nullptr) continue;

        TADS::Vector<unsigned> qtdPorProduto(_produtos.tamanho(), 0);
        for (unsigned j = 0; j < idCompras->tamanho(); j++)
        {
            const Compra &c = _compras[(*idCompras)[j]];
            for (unsigned k = 0; k < c.getIdProdutos().tamanho(); k++)
                qtdPorProduto[c.getIdProdutos()[k]] += c.getQtdProdutos()[k];
        }

        std::string linha = montarLinhaProdutos(qtdPorProduto);
        if (!linha.empty()) std::cout << linha << std::endl;
    }
}

void Mercado::imprimirProdutos(const TADS::ListaOrdenada<unsigned> &ids, const std::string &prefixo)
{
    if (ids.tamanho() == 0) { std::cout << prefixo << " VAZIO" << std::endl; return; }

    for (unsigned i = 0; i < ids.tamanho(); i++)
    {
        unsigned idProduto = ids[i];
        const Produto &p = _produtos[idProduto];
        std::cout << prefixo << " resultado_" << i + 1
                  << " produto " << idProduto
                  << " " << p.getNome()
                  << " " << std::fixed << std::setprecision(2) << p.getPreco()
                  << " " << p.getQtd()
                  << " " << p.getCategoria()
                  << " " << p.getMarca()
                  << " " << p.getCondicao() << std::endl;

        // Busca todas as compras do produto e soma qtd por usuário
// Busca todas as compras do produto e soma qtd por usuário
        int pos = buscarBinariaInt(PesquisavelInt::compra_idProduto, idProduto);
        TADS::ListaOrdenada<unsigned> *idCompras = nullptr;
        
        if (pos != -1) {
            idCompras = &_indicesInt[static_cast<unsigned>(PesquisavelInt::compra_idProduto)][pos].ids;
        }
        
        if (idCompras == nullptr) continue;

        TADS::Vector<unsigned> qtdPorUsuario(_usuarios.tamanho(), 0);
        for (unsigned j = 0; j < idCompras->tamanho(); j++)
        {
            const Compra &c = _compras[(*idCompras)[j]];
            for (unsigned k = 0; k < c.getIdProdutos().tamanho(); k++)
                if (c.getIdProdutos()[k] == idProduto)
                    qtdPorUsuario[c.getIdUsuario()] += c.getQtdProdutos()[k];
        }

        std::ostringstream oss;
        unsigned cont = 1;
        for (unsigned j = 0; j < qtdPorUsuario.tamanho(); j++)
            if (qtdPorUsuario[j] > 0)
                oss << "usuario_" << cont++ << " " << j << " " << qtdPorUsuario[j] << " ";
        std::string linha = oss.str();
        if (!linha.empty()) { linha.pop_back(); std::cout << linha << std::endl; }
    }
}

void Mercado::imprimirCompras(const TADS::ListaOrdenada<unsigned> &ids, const std::string &prefixo)
{
    if (ids.tamanho() == 0) { std::cout << prefixo << " VAZIO" << std::endl; return; }

    for (unsigned i = 0; i < ids.tamanho(); i++)
    {
        const Compra &c = _compras[ids[i]];
        std::cout << prefixo << " resultado_" << i + 1
                  << " compra " << c.getId()
                  << " timestamp " << c.getTimestamp()
                  << " usuario " << c.getIdUsuario() << std::endl;

        // Usa vetor indexado por id de produto para garantir ordem crescente
        TADS::Vector<unsigned> qtdPorProduto(_produtos.tamanho(), 0);
        for (unsigned k = 0; k < c.getIdProdutos().tamanho(); k++)
            qtdPorProduto[c.getIdProdutos()[k]] = c.getQtdProdutos()[k];

        std::string linha = montarLinhaProdutos(qtdPorProduto);
        if (!linha.empty()) std::cout << linha << std::endl;
    }
}

void Mercado::imprimirReposicoes(const TADS::ListaOrdenada<unsigned> &ids, const std::string &prefixo)
{
    if (ids.tamanho() == 0) { std::cout << prefixo << " VAZIO" << std::endl; return; }

    for (unsigned i = 0; i < ids.tamanho(); i++)
    {
        const Reposicao &r = _reposicoes[ids[i]];
        std::cout << prefixo << " resultado_" << i + 1
                  << " reposicao " << r.getId()
                  << " timestamp " << r.getTimestamp() << std::endl;

        TADS::Vector<unsigned> qtdPorProduto(_produtos.tamanho(), 0);
        for (unsigned k = 0; k < r.getIdProdutos().tamanho(); k++)
            qtdPorProduto[r.getIdProdutos()[k]] = r.getQtdProdutos()[k];

        std::string linha = montarLinhaProdutos(qtdPorProduto);
        if (!linha.empty()) std::cout << linha << std::endl;
    }
}
