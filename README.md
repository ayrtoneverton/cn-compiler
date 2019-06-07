## Pré-requisitos
Para utilizar esse analisador léxico/sintático é preciso a ferramenta Flex(Lex ou similares), o Bison(YACC ou similares), além de algum compilador da linguagem C(GCC ou similares).

## Compilando o analisador
Caso possua o Makefile, é só executar o comando “make” estando dentro do diretório dos arquivos, caso não tenha, execute dentro do diretório os comandos:
```SHELL
$ flex lexicon.l
$ gcc -g -Wall -ansi -pedantic lex.yy.c -o compiler.exe
$ rm -f *.yy.c *.yy.h *.tab.c *.tab.h *.output *.dot
```

## Usando o analisador
Após a compilação será gerado o executavel “compiler.exe”, para usa-lo é só executá-lo passando como primeiro parâmetro o nome do arquivo a ser analisado, o resultado da análise será impressa na saída padrão(console) com a formação de código reconhecido, caso a estrutura não seja reconhecida a impressão será encerrada e será exibido a linha e a coluna onde se encontra o erro. Exemplo de uso:
```SHELL
$ .\compiler.exe tests.c
```