#!/bin/bash

echo "Teste 1:"
echo ""

./bin/client execute 30 -p 'cat ola.txt | grep \"nano\"'

echo ""
echo "  teste 1 bem sucedido!"
echo ""

echo "Teste 2:"
echo ""

./bin/client execute 140 -p 'cat ola.txt | grep \"palavra\" | wc -l\'

echo ""
echo "  teste 2 bem sucedido!"
echo ""

echo "Teste 3:"
echo ""

./bin/client execute 10 -u 'ls -l\'

echo ""
echo "  teste 3 bem sucedido!"
echo ""

echo "Teste 4:"
echo ""

./bin/client execute 5 -u 'touch teste4.txt'

echo ""
echo "  teste 4 bem sucedido!"
echo ""

echo "Teste 5:"
echo ""

./bin/client execute 10 -u 'echo Dentro do teste4 temos o teste5 >>teste4.txt'

echo ""
echo "  teste 5 bem sucedido!"
echo ""

echo "Teste 6:"
echo ""

./bin/client execute 10 -u 'rm teste4.txt'

echo ""
echo "  teste 6 bem sucedido!"
echo ""

echo "Done!"