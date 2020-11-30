## Pré-requisitos

Para utilizar esse analisador léxico/sintático é preciso a ferramenta Flex(Lex ou similares), o Bison(YACC ou similares), além de algum compilador da linguagem C(GCC ou similares).

## Gerando o compilador

Caso possua o Makefile, é só executar o comando “make” estando dentro do diretório dos arquivos, caso não tenha, execute dentro do diretório os comandos:

```SHELL
$ bison -d -v -g syntactic.y
$ flex lexicon.l
$ gcc -Wall -ansi -pedantic syntactic.tab.c -o cn.exe
$ rm -f *.yy.c *.yy.h *.tab.c *.tab.h *.output *.dot
```

## Usando o compilador

Após a compilação será gerado o executavel "cn.exe", para usa-lo é só executá-lo passando como primeiro parâmetro o nome do arquivo a ser compilado, o resultado da compilação para a linguagem C será impresso na saída padrão(console) ,ou caso seja passado como segundo parâmetro o caminho onde será salvo o resultado. Caso algum erro ocorra, será exibido a linha e a coluna onde se encontra o erro. Exemplo de uso:

```SHELL
$ ./cn.exe tests/test1.cn
ou
$ ./cn.exe tests/test1.cn test1.c
$ gcc -Wall -ansi -pedantic test1.c -o test1.exe
$ ./test1.exe
```
