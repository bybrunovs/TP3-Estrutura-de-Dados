#!/bin/bash

EXEC="TP/bin/tp2.out"
PASTA_TESTE="testes"

TOTAL=0
PASSOU=0

for i in {1..4}
do
    IN="$PASTA_TESTE/case_$i.in"
    EXPECTED="$PASTA_TESTE/case_$i.out"
    OUTPUT="$PASTA_TESTE/my_case_$i.out"

    # Executa o programa
    "$EXEC" < "$IN" > "$OUTPUT"

    # Compara saída
    if diff -q "$OUTPUT" "$EXPECTED" > /dev/null
    then
        echo "Teste $i: PASSOU"
        PASSOU=$((PASSOU+1))
    else
        echo "Teste $i: FALHOU"
        echo "Diferença:"
        diff "$OUTPUT" "$EXPECTED"
    fi

    TOTAL=$((TOTAL+1))
done

echo "-------------------------"
echo "Resultado: $PASSOU / $TOTAL passaram"