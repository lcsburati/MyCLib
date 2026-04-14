# MyCLab

[English](README.md) | [Português](README.pt-br.md) | [中文版](README.zh-cn.md)

## Índice
- [Objetivo](#objetivo)
- [Como compilar (CMake)](#como-compilar-cmake)
- [Aviso Importante](#aviso-importante)
- [Funções](#funções)
  - [`string_utils.h`](#string_utilsh)
  - [`string_utils.c`](#string_utilsc)

## Objetivo

**Objetivo:** O projeto tem como propósito entender o comportamento da linguagem C por meio de simulações de algumas bibliotecas, baseadas na minha interpretação de como elas deveriam funcionar, e não necessariamente em como funcionam de fato. A ideia é chegar a resultados semelhantes usando meus conhecimentos atuais, focando em compreender a sintaxe da linguagem, suas estruturas de dados primitivas e derivadas, entre outros aspectos. O objetivo é entender de verdade como funcionam as linguagens de baixo nível, entendendo as abstrações que acontecem por trás dos bastidores. Além disso, quero aprofundar meu conhecimento e exercitar meu raciocínio lógico para aprimorar a forma como resolvo problemas.

## Como compilar (CMake)

Você pode compilar o projeto usando **CMake**:

```bash
mkdir build && cd build
cmake ..
make
./main
```

## Aviso Importante

> Todos os arquivos em `src/` e `include/` foram escritos **manualmente sem assistência de IA**, com **exceção de `ui.h`, `ui.c` e `main.c`**, que foram criados usando agentes de IA para fornecer uma interface visual para demonstrar as funcionalidades do backend.

## Funções

### string_utils.h

> Aqui o objetivo é armazenar os prototypes, estruturas e constantes para deixar a source string_utils.c mais limpa.

* #define OFFSET ('a' - 'A'): constante que vai definir a distancia entre lowercase e UPPERCASE (`32`). Utilizada em my_tolower e my_toupper.

* #define STR_BUFFER 256: define o tamanho maximo de caracteres que uma string pode ter.

* OBS: A escolha de #define ao invés de const foi dada devido a simplicidade das variaveis, uma vez que a tipagem nao vai ser importante no tempo de execução.

* typedef char * string: define um apelido para char*. Poderíamos ter trabalhado diretamente com ponteiros, mas, para facilitar o entendimento de como strings funcionam por baixo dos panos, optei por usar o apelido string para char*.

### string_utils.c

> Este é o par de string_utils.h e será onde a lógica das funcoes protótipas definidas no header serao aplicadas.

<details>
<summary><strong>my_strlen</strong> (clique para expandir)</summary>

Nesta função, temos como parâmetro de entrada um tipo `string` (`char*` — ponteiro para `char`).  
O retorno da função é do tipo `size_t`, escolhido por facilitar a portabilidade em relação ao tamanho de um `int` unsigned, já que estamos lidando com o tamanho de arrays (strings).

Começamos definindo a variável que será o contador do tamanho da string (array), nomeada aqui como `length`, também do tipo `size_t`, pelo mesmo motivo do retorno da função (além do fato de que ela será justamente o valor retornado).

A lógica a partir daí é simples: sabendo que toda string em C termina com o caractere nulo `\0`, podemos percorrer a string até encontrá-lo. Esse caractere é interpretado como `0` ou `false` (já que booleanos não existem nativamente em C), então podemos simplesmente usar `while (*str)` para indicar ao programa que ele deve percorrer o ponteiro iniciado na string de entrada até encontrar o `\0`. A cada iteração do loop, incrementamos `length` em 1, acumulando assim o tamanho da string.

**OBS:** Acrescentamos, junto à condição do `while`, a verificação `length < STR_BUFFER`, para garantir que o input esteja protegido contra strings muito grandes, evitando possíveis _overflows_ ou tempos de espera indesejados. Além disso, é uma boa prática de segurança.

**OBS2:** A função reconhece espaços (` `) como um caractere também. Ou seja, se você passar uma string com espaços, o tamanho retornado será o tamanho total da string, incluindo os espaços. Acho importante ressaltar isso, porque dependendo do contexto algumas funções/abordagens tratam o espaço como separador e acabam “ignorando” esse caractere (por exemplo, na leitura de entrada com `scanf("%s")`).

#### Exemplo de uso

A função `my_strlen` fica na barra lateral, na seção **STRING_UTILS**, conforme a imagem abaixo:

![Localização da função my_strlen na sidebar](./assets/my_strlen_exemple.png)

Após selecionar `my_strlen`, é possível inserir uma string e visualizar o tamanho calculado:

![Exemplo de uso da my_strlen](./assets/my_strlen_exemple2.png)

</details>

<details>
<summary><strong>my_toupper</strong> (clique para expandir)</summary>

Nesta função, temos como parâmetro de entrada uma string (ponteiro de char) nomeada como `str`.

Começamos definindo as variáveis necessárias para o processamento: o `length` (`size_t`) e o `result` atualizado. Para o `result`, alocamos memória na *heap* através da função `malloc`, criando um ponteiro que marca o início de um bloco com o tamanho exato da string de entrada + 1 byte de espaço para o caractere nulo `\0`.

Após a alocação, fazemos uma verificação para garantir que o `malloc` foi realizado com sucesso; caso contrário, a função retorna `NULL` (evitando falhas caso não haja espaço contíguo na memória para a alocação do bloco).

Definimos também um ponteiro auxiliar `string current = result`. Isso nos permite percorrer e preencher o novo bloco de memória sem perder a referência do início do bloco (`result`), que será necessária para o retorno da função.

A lógica de conversão percorre a string de entrada (`str`): verificamos se o caractere atual é uma letra minúscula; caso seja, convertemos para maiúscula subtraindo o valor de `OFFSET` (32) ao valor ASCII do caractere. Caso contrário, apenas copiamos o caractere original para o novo bloco. Aproveitamos para conferir o buffer dentro da condição para garantir a segurança da operação.

Ao final do loop, definimos o último caractere de `current` como `\0`, finalizando a string corretamente. Retornamos `result`, que aponta para o início da nova string agora em **UPPERCASE**.

</details>

<details>
<summary><strong>my_tolower</strong> (clique para expandir)</summary>

Nesta função, seguimos a mesma lógica de gerenciamento de memória da `my_toupper`, utilizando um ponteiro de entrada `str` e alocando um novo bloco na *heap* com `malloc` para armazenar o resultado.

A diferença fundamental reside na lógica de conversão: percorremos a string de entrada verificando se o caractere atual é uma letra **maiúscula**. Caso seja, realizamos a conversão para **minúscula** **acrescentando** o valor de `OFFSET` (32) ao valor ASCII do caractere. 

Assim como nas outras funções, mantemos a segurança verificando o sucesso da alocação de memória e controlando o limite do buffer. Ao final, garantimos o fechamento da string com o caractere nulo `\0` e retornamos o ponteiro `result`, que aponta para o início da nova string convertida para **lowercase**.

</details>

<details>
<summary><strong>my_strcmp</strong> (clique para expandir)</summary>

Os parâmetros de input são as duas strings a serem comparadas, e o retorno é um `int` variante ao resultado da comparação: se forem diferentes e a primeira string for maior, retorna 1; se forem diferentes e a primeira string for menor, retorna -1; e se forem iguais, retorna 0.

Aqui, o objetivo é comparar duas strings e retornar o resultado da comparação.

Começamos definindo duas variáveis que irão indicar o tamanho das duas strings.

A partir disso, fazemos um `if` para verificar qual string é maior. Se for a primeira string, percorremos ela com um `while` (já que estamos trabalhando com ponteiros). Neste `while`, já acrescentamos a verificação do buffer e, dentro dele, comparamos se `*str1 != *str2`. Se for verdadeiro, retornamos 1; caso contrário, percorremos toda a string e, se forem iguais, a condição não é satisfeita e retornamos 0.

A mesma lógica é aplicada em um `else if` caso a segunda string seja maior, retornando -1.

Agora caso elas sejam de mesmo tamanho, utilizamos a str1 para percorrer a string e comparar com a str2. Se forem diferentes, retornamos 1; caso contrário, percorremos toda a string e, se forem iguais, a condição não é satisfeita e retornamos 0.

</details>

<details>
<summary><strong>my_strcmp_percent</strong> (clique para expandir)</summary>

Os parâmetros de input são `str1` e `str2`, similar ao `my_strcmp`. A diferença principal está no algoritmo: para calcular a porcentagem de similaridade, utilizamos a **distância de Levenshtein**, que preenche uma matriz com base no número mínimo de operações necessárias para transformar `str2` em `str1` — sendo essas operações: **inserção**, **substituição** e **remoção** de caracteres.

O algoritmo começa definindo os tamanhos de `str1` e `str2`, e em seguida aloca a `matrix` no heap — tratada como um ponteiro duplo (`int**`) — usando `malloc` tanto nas linhas quanto nas colunas via for loop.

Os dois primeiros loops após a alocação definem as **bordas da matriz**: a borda esquerda (coluna 0) e a borda superior (linha 0) são preenchidas com valores crescentes, representando o custo de partir do zero — ou seja, quantas operações seriam necessárias se uma das strings fosse vazia.

No loop principal (dois aninhados percorrendo linhas e colunas), definimos a variável `cost` via condicional ternário: se os caracteres comparados forem iguais, `cost = 0`; caso contrário, `cost = 1`.

Em seguida, pegamos o **mínimo entre as 3 operações** — porque queremos o caminho de menor custo para chegar ao resultado, já que é possível transformar uma string na outra de formas diferentes, mas com energias diferentes. Cada operação é representada por um dos 3 adjacentes da célula atual:

- **`[i-1][j]` → remoção:** avançamos apenas em uma das direções (str1), o que significa que aquele caractere deve ser ignorado — custo +1.
- **`[i][j-1]` → inserção:** avançamos apenas na direção de str2, ou seja, na string que estamos comparando falta 1 caractere — custo +1.
- **`[i-1][j-1]` → substituição (ou match):** voltamos um em ambas as direções, mantendo a sincronia — custo +1 se os caracteres forem diferentes, ou 0 se forem iguais.

Com o loop completo, a matriz está totalmente preenchida e o valor em `matrix[str1_length][str2_length]` é a distância final acumulada de todas as operações.

A partir disso, usamos um condicional ternário para identificar qual string é maior e usar seu tamanho como denominador no cálculo da porcentagem. Por fim, liberamos o espaço alocado da matriz e retornamos a similaridade como `double`.

</details>