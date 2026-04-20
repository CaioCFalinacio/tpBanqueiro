# Trabalho Prático 1: Algoritmo do Banqueiro

## Sobre o projeto

Este projeto implementa o Algoritmo do Banqueiro, com o objetivo de gerenciar a alocação de recursos a múltiplos clientes (threads) garantindo que o sistema permaneça sempre em um estado seguro, prevenindo *deadlocks*. O projeto foi desenvolvido em **C**, utilizando a biblioteca **Pthreads** e *locks mutex* para evitar condições de corrida.

## Estrutura de arquivos

- `src/main.c`: Código-fonte principal com a lógica do Banqueiro e das threads.
- `docs/`: Diretório contendo o relatório final em PDF.

## Tecnologias Utilizadas
Linguagem C 

Biblioteca Pthreads (POSIX Threads) 

Mutex para sincronização e prevenção de condições de corrida

## Pré-requisitos
- Compilador **GCC** (GNU Compiler Collection).
- Biblioteca **pthread** (nativa em sistemas baseados em Unix/Linux).
- **Nota para ambientes Windows:** Como o projeto utiliza a biblioteca nativa do padrão POSIX, recomenda-se a compilação e execução através do **WSL (Windows Subsystem for Linux)** ou de ambientes compatíveis (como MSYS2/MinGW).

## Como Compilar

Para compilar o código fonte, certifique-se de ter o compilador GCC instalado. Abra o terminal na raiz do projeto e execute o seguinte comando:

```bash
gcc -o banqueiro src/main.c -lpthread -Wall
```

## Como Executar

Formato:
```bash
./banqueiro [recurso_1] [recurso_2] [recurso_3]
```

Exemplo prático:
```bash
./banqueiro 10 5 7
```

## Funcionamento
Ao iniciar, o programa cria 5 threads de clientes. Cada cliente solicitará e liberará recursos em um loop contínuo. O "Banqueiro" processa essas solicitações através das funções:
request_resources(): Avalia se a solicitação mantém o sistema em um estado seguro antes de concedê-la.

release_resources(): Libera os recursos utilizados de volta para o banco.

## Autores

- Caio César Falinacio dos Santos
- Luiz Fernando Cunha Maia
