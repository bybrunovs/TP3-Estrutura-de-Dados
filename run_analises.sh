#!/bin/bash

EXEC="TP/bin/tp1.out"
PASTA_ANALISE="AE"

for n in acoes cliente consultas cotacoes
do
    for i in {0..4}
    do
        IN="$PASTA_ANALISE/entradas/${n}${i}.txt"
        OUTPUT="$PASTA_ANALISE/output/out_${n}${i}.txt"
        DATA="$PASTA_ANALISE/data/data${n}${i}.txt"
        # Executa o programa
        echo "Processando: $IN"
        /usr/bin/time -v "$EXEC" < "$IN" > "$OUTPUT" 2> "$DATA"
    done
done
echo "-------------------------"
echo "Análise concluída."