#!/bin/bash

EXEC="TP/bin/tp3.out"
PASTA_TESTE="testes"

TOTAL=0
PASSOU=0

# 1. Segurança: Verifica se você compilou o código antes de testar
if [ ! -f "$EXEC" ]; then
    echo "Erro: Executável '$EXEC' não encontrado! Compile o código primeiro."
    exit 1
fi

for pasta in "normal" "pt extra 1" "pt extra 2"
do
    echo "========================================="
    echo "Executando testes da pasta: $pasta"
    echo "========================================="
    
    for i in {1..6}
    do
        IN="$PASTA_TESTE/$pasta/$i.in"
        EXPECTED="$PASTA_TESTE/$pasta/$i.out"
        OUTPUT="$PASTA_TESTE/$pasta/my_$i.out"

        TOTAL=$((TOTAL+1))

        # Executa o programa
        "$EXEC" < "$IN" > "$OUTPUT"

        # Compara saída
        if diff -w -B -q "$OUTPUT" "$EXPECTED" > /dev/null
        then
            echo "  [OK] Teste $i PASSOU"
            PASSOU=$((PASSOU+1))
        else
            echo "  [X] Teste $i FALHOU"
            echo "      Diferença ( < Seu | > Esperado ):"
            # O diff normal mostra onde errou. Adicionado -w e -B para ignorar espaços em branco e linhas vazias (opcional)
            diff -w -B "$OUTPUT" "$EXPECTED"
        fi

    done
    echo ""
done

echo "-----------------------------------------"
echo "RESULTADO FINAL: $PASSOU / $TOTAL testes passaram."
echo "-----------------------------------------"