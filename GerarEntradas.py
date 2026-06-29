import os
import random

# Estrutura de diretórios para os testes
os.makedirs("analise_experimental/in", exist_ok=True)
os.makedirs("analise_experimental/cadastros/in", exist_ok=True)
os.makedirs("analise_experimental/transacoes/in", exist_ok=True)
os.makedirs("analise_experimental/consultas/in", exist_ok=True)

# Dados fictícios sem espaços (conforme a especificação do TP3)
CIDADES = ["5tjr","5fght","458t","5jt","34r","5r","4re","efg","09iu","gh","pl","oikjh","rtuiyo","y","dfgh","uj","hu","tyu","546","egt","er","ert","erd","ejt","fdg","BeloHorizonte", "SaoPaulo", "Rio", "Curitiba", "Recife"]
ESTADOS = ["5tjr","5fght","458t","5jt","34r","5r","4re","efg","09iu","gh","pl","oikjh","rtuiyo","y","dfgh","uj","hu","tyu","546","egt","er","ert","erd","ejt","fdg","MG", "SP", "RJ", "PR", "PE"]
NACION = ["5tjr","5fght","458t","5jt","34r","5r","4re","efg","09iu","gh","pl","oikjh","rtuiyo","y","dfgh","uj","hu","tyu","546","egt","er","ert","erd","ejt","fdg","Brasileira", "Argentina", "Chilena", "Colombiana"]
CATEGORIAS = ["5tjr","5fght","458t","5jt","34r","5r","4re","efg","09iu","gh","pl","oikjh","rtuiyo","y","dfgh","uj","hu","tyu","546","egt","er","ert","erd","ejt","fdg","Informatica", "Livros", "Moveis", "Eletronicos", "Roupas"]
MARCAS = ["5tjr","5fght","458t","5jt","34r","5r","4re","efg","09iu","gh","pl","oikjh","rtuiyo","y","dfgh","uj","hu","tyu","546","egt","er","ert","erd","ejt","fdg","Logitech", "Dell", "Pearson", "Flexform", "Samsung"]
CONDICOES = ["Novo", "Usado", "Recondicionado"]

def gerar_cenario(nome_arquivo, n_operacoes, perfil):
    """
    Gera um arquivo de entrada balanceado para o TP3 MercadoDCC.
    perfil: 'CAD' (Cadastros), 'TRANS' (Transações - Compras/Reposições), 'CONS' (Consultas)
    """
    with open(nome_arquivo, 'w') as f:
        id_usuario_atual = 0
        id_produto_atual = 0
        timestamp = 1

        # Carga Inicial Básica (Garante que consultas e compras tenham dados)
        carga_inicial = max(10, n_operacoes // 10)
        for _ in range(carga_inicial):
            # Usuário
            nome = f"User{id_usuario_atual}"
            idade = random.randint(18, 60)
            cidade = random.choice(CIDADES)
            estado = random.choice(ESTADOS)
            nac = random.choice(NACION)
            f.write(f"U {nome} {idade} {cidade} {estado} {nac}\n")
            id_usuario_atual += 1

            # Produto
            nome_p = f"Prod{id_produto_atual}"
            preco = round(random.uniform(10.0, 5000.0), 2)
            qtd = random.randint(5, 50)
            cat = random.choice(CATEGORIAS)
            marca = random.choice(MARCAS)
            cond = random.choice(CONDICOES)
            f.write(f"P {nome_p} {preco:.2f} {qtd} {cat} {marca} {cond}\n")
            id_produto_atual += 1

        # Distribuição de probabilidades baseada no perfil escolhido
        if perfil == "NORMAL":
            pesos = {'UP': 0.33, 'CR': 0.33, 'L': 0.33}
        elif perfil == 'CAD':
            pesos = {'UP': 0.80, 'CR': 0.10, 'L': 0.10}
        elif perfil == 'TRANS':
            pesos = {'UP': 0.10, 'CR': 0.80, 'L': 0.10}
        else: # 'CONS'
            pesos = {'UP': 0.10, 'CR': 0.10, 'L': 0.80}

        # Geração das Operações Dinâmicas
        for _ in range(n_operacoes):
            sorteio = random.random()
            
            # --- Bloco de CADASTROS (U, P) ---
            if sorteio < pesos['UP']:
                if random.choice(['U', 'P']) == 'U':
                    f.write(f"U User{id_usuario_atual} {random.randint(18, 60)} {random.choice(CIDADES)} {random.choice(ESTADOS)} {random.choice(NACION)}\n")
                    id_usuario_atual += 1
                else:
                    f.write(f"P Prod{id_produto_atual} {round(random.uniform(10.0, 5000.0), 2):.2f} {random.randint(5, 50)} {random.choice(CATEGORIAS)} {random.choice(MARCAS)} {random.choice(CONDICOES)}\n")
                    id_produto_atual += 1

            # --- Bloco de TRANSAÇÕES (C, R) ---
            elif sorteio < pesos['UP'] + pesos['CR']:
                qtd_itens = random.randint(1, min(5, id_produto_atual)) # Quantos produtos diferentes na transação
                produtos_escolhidos = random.sample(range(id_produto_atual), qtd_itens)
                itens_str = " ".join([f"{p_id} {random.randint(1, 5)}" for p_id in produtos_escolhidos])
                
                if random.choice(['C', 'R']) == 'C' and id_usuario_atual > 0:
                    u_id = random.randint(0, id_usuario_atual - 1)
                    f.write(f"C {timestamp} {u_id} {itens_str}\n")
                else:
                    f.write(f"R {timestamp} {itens_str}\n")
                timestamp += 1

            # --- Bloco de CONSULTAS (LU, LP, LC, LR) ---
            else:
                tipo_consulta = random.choice(['LU', 'LP', 'LC', 'LR'])
                
                if tipo_consulta == 'LU':
                    # Pode usar filtros simples ou faixas (se você implementou o extra)
                    atributo = random.choice(["cidade", "estado", "nacionalidade"])
                    valor = random.choice(CIDADES if atributo == "cidade" else ESTADOS if atributo == "estado" else NACION)
                    f.write(f"LU {atributo} {valor}\n")
                
                elif tipo_consulta == 'LP':
                    atributo = random.choice(["categoria", "marca", "condicao"])
                    valor = random.choice(CATEGORIAS if atributo == "categoria" else MARCAS if atributo == "marca" else CONDICOES)
                    f.write(f"LP {atributo} {valor}\n")
                
                elif tipo_consulta == 'LC' and id_usuario_atual > 0:
                    u_id = random.randint(0, id_usuario_atual - 1)
                    f.write(f"LC id_usuario {u_id}\n")
                
                elif tipo_consulta == 'LR' and id_produto_atual > 0:
                    p_id = random.randint(0, id_produto_atual - 1)
                    f.write(f"LR id_produto {p_id}\n")

# ==============================================================================
# EXECUÇÃO DOS CENÁRIOS DE TESTE
# ==============================================================================
print("Gerando arquivos para a Análise Experimental do TP3...")

quantidades = [100, 500, 1000, 5000]

gerar_cenario(f"analise_experimental/in/ops_50.txt", n_operacoes=50, perfil='NORMAL')
gerar_cenario(f"analise_experimental/in/ops_100.txt", n_operacoes=100, perfil='NORMAL')
gerar_cenario(f"analise_experimental/in/ops_2000.txt", n_operacoes=2000, perfil='NORMAL')
gerar_cenario(f"analise_experimental/in/ops_5000.txt", n_operacoes=5000, perfil='NORMAL')

# for n_ops in quantidades:
#     # 1. Perfil com predominância de Cadastros
#     gerar_cenario(f"analise_experimental/cadastros/in/ops_{n_ops}.txt", n_operacoes=n_ops, perfil='CAD')
    
#     # 2. Perfil com predominância de Transações (Compras e Reposições)
#     gerar_cenario(f"analise_experimental/transacoes/in/ops_{n_ops}.txt", n_operacoes=n_ops, perfil='TRANS')
    
#     # 3. Perfil com predominância de Consultas
#     gerar_cenario(f"analise_experimental/consultas/in/ops_{n_ops}.txt", n_operacoes=n_ops, perfil='CONS')

print("Arquivos gerados com sucesso na pasta 'analise_experimental/'!")