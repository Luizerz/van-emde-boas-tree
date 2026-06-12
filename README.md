# Árvore van Emde Boas com Espaço Linear

Implementação de uma van Emde Boas para palavras de 32 bits (universo de 2³² valores)
com espaço linear no número de elementos: os clusters de cada nó são guardados em uma
tabela de dispersão própria com table doubling/halving, em vez de um array de tamanho √u.

## Linguagem

C++17 (testado com g++/Apple clang 17, compatível com qualquer g++ com suporte a C++17).

## Como compilar

```
make build
```

## Como executar

```
make run
```
ou, para usar outro arquivo de entrada:
```
make run INPUT=nome_do_arquivo.txt
```

O programa recebe um arquivo `.txt` com uma operação por linha (`INC x`, `REM x`,
`SUC x`, `PRE x`, `IMP`) e imprime os resultados no terminal.

- `SUC x` imprime `+INF` quando não há valor maior que x na estrutura.
- `PRE x` imprime `-INF` quando não há valor menor que x.
- `IMP` imprime o min do primeiro nível e, para cada cluster não vazio (em ordem
  crescente de índice), os valores low armazenados nele em ordem crescente.
  Com a estrutura vazia, imprime `Min: +INF`.

## Estruturas e funções

### veb.h / veb.cpp

- **`struct VEB`** — nó da árvore para um universo de `2^w` valores. Campos:
  `w` (largura em bits), `min`/`max` (`-1` = vazio), `summary` (vEB de `w/2` bits
  com os índices dos clusters não vazios) e `clusters` (tabela de dispersão
  índice → `VEB*`). O `min` não é armazenado recursivamente e clusters/summary
  só são criados quando recebem elementos, o que dá o espaço O(n).
  - `insert(x)` — operação INC, O(log w). Em cluster vazio a inserção é O(1)
    (só ajusta min/max), de modo que apenas uma chamada recursiva "cara" é feita.
  - `remove(x)` — operação REM, O(log w). Quando um cluster esvazia, ele é
    desalocado e removido da tabela e do summary, mantendo o espaço linear.
  - `successor(x)` / `predecessor(x)` — operações SUC/PRE, O(log w).
    Retornam `-1` quando não existe (impresso como `+INF`/`-INF`).
  - `print()` — operação IMP; percorre o summary para visitar os clusters em ordem.
  - `high(x)`, `low(x)`, `index(h, l)` — aritmética de bits para dividir/recompor
    a chave (`x = high · 2^(w/2) + low`).
  - Construtor e destrutor (libera recursivamente summary e clusters).

### hashtable.h / hashtable.cpp

- **`class HashTable`** — tabela de dispersão com endereçamento aberto e sondagem
  linear, mapeando índice de cluster (`uint32_t`) para `VEB*`. Campos: `slots`
  (vetor de `Slot`), `capacity` (potência de 2), `lg` (log₂ da capacidade),
  `count` (slots ocupados) e `used` (ocupados + removidos).
  - **`struct Slot`** — entrada da tabela: `key`, `value` e `state`
    (`EMPTY`, `OCCUPIED` ou `DELETED`).
  - `insert(key, value)` — insere/atualiza; faz **doubling** quando o fator de
    carga passa de 1/2 (ou reconstrói na mesma capacidade quando o excesso é só
    de slots `DELETED`).
  - `remove(key)` — marca o slot como `DELETED`; faz **halving** quando a
    ocupação cai a 1/8 da capacidade.
  - `find(key)` — busca por sondagem linear; retorna `nullptr` se ausente.
  - `hashOf(key)` — hashing multiplicativo (constante de Fibonacci), usa os
    `lg` bits mais significativos do produto.
  - `resize(newCapacity)` — realoca e reinsere os elementos, eliminando os
    marcadores `DELETED`.

### main.cpp

- **`main(argc, argv)`** — recebe o caminho do arquivo de entrada como argumento,
  lê as operações linha a linha e as despacha para a `VEB` raiz (criada com
  `w = 32`), imprimindo os resultados de SUC/PRE/IMP.

## Testes

```
make build
./programa.exe entrada.txt > saida.txt
diff saida.txt saida_esperada.txt
```

O repositório inclui `entrada.txt`/`saida_esperada.txt` com o exemplo do enunciado.
A corretude também foi verificada com um stress test de 50.000 operações aleatórias
comparado a um oráculo (lista ordenada) e com AddressSanitizer (sem vazamentos).
