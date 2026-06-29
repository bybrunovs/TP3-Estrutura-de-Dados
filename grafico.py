import matplotlib.pyplot as plt

arquivo_trace = 'analise_experimental/trace_mem.txt'
tempos, enderecos = [], []
contador = 0

print("Lendo o arquivo de rastreio...")
with open(arquivo_trace, 'r') as f:
    for linha in f:
        if not linha.strip() or linha.startswith('=='):
            continue
            
        partes = linha.split()
        if len(partes) >= 2 and partes[0] in ['L', 'S', 'M']:
            endereco_hex = partes[1].split(',')[0]
            enderecos.append(int(endereco_hex, 16))
            tempos.append(contador)
            contador += 1
            
        if contador >= 500000:
            print("Limite de 500.000 pontos atingido.")
            break

print("Calculando o corte e filtrando a Pilha...")
enderecos_unicos = sorted(list(set(enderecos)))
maior_salto, limiar_corte = 0, 0

for i in range(len(enderecos_unicos) - 1):
    salto = enderecos_unicos[i+1] - enderecos_unicos[i]
    if salto > maior_salto:
        maior_salto = salto
        limiar_corte = enderecos_unicos[i] + (salto // 2)

t_heap, e_heap = [], []
for t, e in zip(tempos, enderecos):
    if e < limiar_corte:
        t_heap.append(t)
        e_heap.append(e)

print("Gerando o gráfico do Heap...")
plt.figure(figsize=(10, 6))
plt.scatter(t_heap, e_heap, s=0.1, color='blue', alpha=0.5)
plt.title('Heap - Base da Memória (Alocações Dinâmicas)')
plt.xlabel('Ordem de Acesso (Instante de Tempo)')
plt.ylabel('Endereço de Memória')
plt.ticklabel_format(style='plain', axis='y')
plt.tight_layout()
plt.show()