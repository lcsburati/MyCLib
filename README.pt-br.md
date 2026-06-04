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

**Objetivo:** O projeto tem como propósito entender o comportamento da linguagem C por meio de simulações de algumas bibliotecas, baseadas na minha interpretação. A ideia é chegar a resultados semelhantes usando meus conhecimentos atuais, focando em compreender a sintaxe da linguagem, suas estruturas de dados, entre outros aspectos. O objetivo é entender de verdade como funcionam as linguagens de baixo nível (No caso C), entendendo as abstrações que acontecem por trás dos bastidores. Além disso, quero aprofundar meu conhecimento e exercitar meu raciocínio lógico para aprimorar a forma como resolvo problemas.

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

* typedef char * string: define um apelido para char*. Poderíamos ter trabalhado diretamente com ponteiros, mas, para facilitar o entendimento de como strings funcionam por baixo dos panos, optei por usar o apelido string para char*. (Entendo que nao é uma boa prática em C esconder ponteiros em typedef's, mas o caso foi apenas para garantir o entendimento de que strings sao naturalmente ponteiros).

### string_utils.c

> Este é o par de string_utils.h e será onde a lógica das funcoes protótipas definidas no header serao aplicadas.

<details>
<summary><strong>my_strlen</strong> (clique para expandir)</summary>

Nesta função, temos como parâmetro de entrada um tipo `string` (`char*` — ponteiro para `char`).  
O retorno da função é do tipo `size_t`, escolhido por facilitar a portabilidade em relação ao tamanho de um `int` unsigned, já que estamos lidando com o tamanho de arrays (strings).

Começamos definindo a variável que será o contador do tamanho da string (array), nomeada aqui como `length`, também do tipo `size_t`, pelo mesmo motivo do retorno da função (além do fato de que ela será justamente o valor retornado).

A lógica a partir daí é simples: sabendo que toda string em C termina com o caractere nulo `\0`, podemos percorrer a string até encontrá-lo. Esse caractere é interpretado como `0` ou `false` (já que booleanos não existem nativamente em C), então podemos simplesmente usar `while (*str)` para indicar ao programa que ele deve percorrer o ponteiro iniciado na string de entrada até encontrar o `\0`. A cada iteração do loop, incrementamos `length` em 1, acumulando assim o tamanho da string.

**Verificação de entrada inválida e overflow:** A função verifica se a string é `NULL`; caso seja, retorna `0` imediatamente. Além disso, se durante a iteração o tamanho atingir ou ultrapassar `STR_BUFFER`, a função retorna `STR_BUFFER + 1`. Fiz isso para diferenciar um erro/overflow de uma contagem válida, já que retornar `0` também poderia representar string vazia. O `+ 1` ajuda a lidar com casos de borda e deixa mais claro para as funções que dependem de `my_strlen` que aquele tamanho passou do limite esperado.

**OBS:** A função reconhece espaços (` `) como um caractere também. Ou seja, se você passar uma string com espaços, o tamanho retornado será o tamanho total da string, incluindo os espaços. Acho importante ressaltar isso, porque dependendo do contexto algumas funções/abordagens tratam o espaço como separador e acabam “ignorando” esse caractere (por exemplo, na leitura de entrada com `scanf("%s")`).

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

A lógica de conversão percorre a string de entrada (`str`): verificamos se o caractere atual é uma letra minúscula; caso seja, convertemos para maiúscula subtraindo o valor de `OFFSET` (32) ao valor ASCII do caractere. Caso contrário, apenas copiamos o caractere original para o novo bloco.

Ao final do loop, definimos o último caractere de `current` como `\0`, finalizando a string corretamente. Retornamos `result`, que aponta para o início da nova string agora em **UPPERCASE**.

**Verificação de entrada vazia ou acima do buffer:** A função verifica se o tamanho da string é `0` ou maior que `STR_BUFFER`; caso seja, retorna `NULL` para evitar alocações desnecessárias e também impedir que uma string sinalizada como overflow por `my_strlen` siga para a alocação.

</details>

<details>
<summary><strong>my_tolower</strong> (clique para expandir)</summary>

Nesta função, seguimos a mesma lógica de gerenciamento de memória da `my_toupper`, utilizando um ponteiro de entrada `str` e alocando um novo bloco na *heap* com `malloc` para armazenar o resultado.

A diferença fundamental reside na lógica de conversão: percorremos a string de entrada verificando se o caractere atual é uma letra **maiúscula**. Caso seja, realizamos a conversão para **minúscula** **acrescentando** o valor de `OFFSET` (32) ao valor ASCII do caractere. 

Assim como nas outras funções, mantemos a segurança verificando o sucesso da alocação de memória. Ao final, garantimos o fechamento da string com o caractere nulo `\0` e retornamos o ponteiro `result`, que aponta para o início da nova string convertida para **lowercase**.

**Verificação de entrada vazia ou acima do buffer:** A função verifica se o tamanho da string é `0` ou maior que `STR_BUFFER`; caso seja, retorna `NULL` para evitar alocações desnecessárias e também impedir que uma string sinalizada como overflow por `my_strlen` siga para a alocação.

</details>

<details>
<summary><strong>my_strcmp</strong> (clique para expandir)</summary>

Os parâmetros de input são duas strings que serão comparadas, `str1` e `str2`. A ideia aqui é simular o comportamento da função `strcmp`: se as duas strings forem iguais, o retorno é `0`; se forem diferentes, o retorno vem da diferença entre o primeiro caractere diferente encontrado.

Primeiro validamos se alguma das entradas é nula. Caso `str1` ou `str2` seja `NULL`, a função retorna `1`, indicando que a comparação não é igual.

Depois começa o processamento principal: enquanto nenhuma das duas strings chegou no caractere nulo `\0`, o loop continua válido. Dentro desse loop, comparamos o caractere atual de `str1` com o caractere atual de `str2`. Se eles forem diferentes, a função já retorna a diferença entre esses dois caracteres. Se forem iguais, avançamos os dois ponteiros com `str1++` e `str2++`, indo para o próximo caractere das duas strings.

Quando o loop acaba, fazemos mais um retorno com a diferença entre os caracteres atuais. Isso cobre o caso em que uma string termina antes da outra, por exemplo `"abc"` e `"ab"`: nesse ponto a comparação fica entre `'c'` e `'\0'`.

Foi usado type cast para `unsigned char` nos retornos porque estamos tratando `char` dentro de uma operação com inteiros. Dependendo do compilador, `char` pode ser interpretado como signed ou unsigned, e isso pode dar diferença principalmente com caracteres fora da tabela ASCII básica. Convertendo para `unsigned char`, a comparação fica mais previsível e mais próxima do comportamento esperado de `strcmp`.

**Códigos de retorno:**

- `0`: as strings são iguais.
- Valor negativo: o primeiro caractere diferente de `str1` é menor que o de `str2`.
- Valor positivo: o primeiro caractere diferente de `str1` é maior que o de `str2`.
- `1` na verificação inicial: pelo menos uma das entradas é `NULL`.

</details>

<details>
<summary><strong>my_strcmp_percent</strong> (clique para expandir)</summary>

Os parâmetros de input são `str1` e `str2`, similar ao `my_strcmp`. A diferença principal está no algoritmo: para calcular a porcentagem de similaridade, utilizamos a **distância de Levenshtein**, que preenche uma matriz com base no número mínimo de operações necessárias para transformar `str2` em `str1` — sendo essas operações: **inserção**, **substituição** e **remoção** de caracteres.

O algoritmo começa definindo os tamanhos de `str1` e `str2` usando `my_strlen`. Como `my_strlen` agora retorna `STR_BUFFER + 1` quando detecta overflow, o `my_strcmp_percent` usa essa informação para parar antes de tentar montar a matriz de Levenshtein com um tamanho inválido.

Antes de seguir para o cálculo principal, a função trata alguns retornos especiais: se alguma entrada for `NULL`, retorna `-3`; se alguma string passar de `STR_BUFFER`, retorna `-2`; e se alguma alocação de memória falhar, retorna `-1`.

Depois dessas verificações, a função aloca a `matrix` no heap — tratada como um ponteiro duplo (`int**`) — usando `malloc` tanto nas linhas quanto nas colunas via for loop. Também foi adicionada uma proteção para o caso de falha no meio da alocação das linhas: se alguma coluna não conseguir ser alocada, a função libera tudo que já tinha sido reservado antes de retornar erro, evitando vazamento de memória.

Os dois primeiros loops após a alocação definem as **bordas da matriz**: a borda esquerda (coluna 0) e a borda superior (linha 0) são preenchidas com valores crescentes, representando o custo de partir do zero — ou seja, quantas operações seriam necessárias se uma das strings fosse vazia.

No loop principal (dois aninhados percorrendo linhas e colunas), definimos a variável `cost` via condicional ternário: se os caracteres comparados forem iguais, `cost = 0`; caso contrário, `cost = 1`.

Em seguida, pegamos o **mínimo entre as 3 operações** — porque queremos o caminho de menor custo para chegar ao resultado, já que é possível transformar uma string na outra de formas diferentes, mas com energias diferentes. Cada operação é representada por um dos 3 adjacentes da célula atual:

- **`[i-1][j]` → remoção:** avançamos apenas em uma das direções (str1), o que significa que aquele caractere deve ser ignorado — custo +1.
- **`[i][j-1]` → inserção:** avançamos apenas na direção de str2, ou seja, na string que estamos comparando falta 1 caractere — custo +1.
- **`[i-1][j-1]` → substituição (ou match):** voltamos um em ambas as direções, mantendo a sincronia — custo +1 se os caracteres forem diferentes, ou 0 se forem iguais.

Com o loop completo, a matriz está totalmente preenchida e o valor em `matrix[str1_length][str2_length]` é a distância final acumulada de todas as operações.

A partir disso, usamos um condicional ternário para identificar qual string é maior e usar seu tamanho como denominador no cálculo da porcentagem. Se as duas strings forem vazias, esse denominador fica `0`; nesse caso, a função libera a matriz e retorna `-4`, evitando divisão por zero. Por fim, no fluxo normal, liberamos o espaço alocado da matriz e retornamos a similaridade como `double`.

**Códigos de retorno:**

- `0` a `100`: porcentagem de similaridade calculada com sucesso.
- `-1`: falha na alocação de memória da matriz.
- `-2`: alguma das strings passou de `STR_BUFFER`, indicando overflow detectado por `my_strlen`.
- `-3`: alguma das entradas é `NULL`.
- `-4`: as duas strings são vazias, deixando o denominador do cálculo igual a `0`.

**OBS sobre strings vazias:** Como string vazia não passa de `STR_BUFFER`, uma string vazia contra outra com conteúdo ainda pode seguir para o cálculo e tende a retornar `0%`. O caso em que as duas strings são vazias é tratado separado com `-4`, porque não existe tamanho máximo para usar como base da porcentagem.

**OBS:** Os códigos de erro agora são negativos justamente para não confundir com porcentagens válidas. Como o retorno normal fica entre `0` e `100`, qualquer valor negativo indica que a função parou antes do cálculo final.

</details>
