# Local Discovery

## Visão geral

Este projeto implementa um serviço de descoberta local em rede usando UDP multicast em C para Linux. O programa escuta mensagens enviadas ao grupo multicast `239.0.0.1` na porta `5050`, identifica o cliente que enviou o datagrama e responde com o endereço IP local da máquina.

A ideia central é permitir que dispositivos ou processos na mesma rede local descubram uns aos outros sem depender de um servidor central. O projeto foi estruturado para crescer com módulos, documentação e testes futuros.

## Tecnologias

- C11
- GCC
- Make
- Uncrustify
- Sockets UDP multicast em Linux

## Estrutura do projeto

```text
.
├── Makefile
├── README.md
├── .gitignore
├── .uncrustify.cfg
├── bin/            Executáveis gerados
├── build/          Objetos e arquivos intermediários
├── docs/           Documentação complementar
├── include/        Headers públicos
├── src/            Código-fonte principal
├── tests/          Testes automatizados (ainda não implementados)
└──
```

## Como compilar

```bash
make
```

Esse comando gera o executável `bin/app` a partir dos arquivos em `src/`.

## Como executar

```bash
make run
```

O programa inicia a escuta em todas as interfaces IPv4 e imprime o IP detectado da máquina. Em seguida, fica em loop aguardando datagramas UDP do grupo multicast.

Quando uma mensagem chega, o servidor mostra o endereço do remetente e retorna o IP local da máquina em resposta.

> O endereço e a porta estão definidos em `src/main.c` através das macros `MULTICAST_ADDR` e `PORT`.

## Comandos disponíveis

```bash
make          # compila o projeto
make run      # executa o programa
make clean    # remove artefatos de compilação
make lint     # verifica formatação e estilo sem alterar arquivos
make format   # formata os arquivos C com Uncrustify
make test     # alvo preparado para testes; ainda não há testes
make help     # lista os comandos do projeto
make check    # formata, compila e executa o alvo de testes
```

## Lint e formatação

```bash
make lint
make format
```

O `lint` usa o Uncrustify para verificar a padronização dos arquivos C sem modificar o código. O `format` aplica a mesma configuração e substitui a formatação dos arquivos no local, sem criar backups.

## Testes

```bash
make test
```

Este alvo está preparado para receber testes automatizados, mas, no momento, não há casos de teste implementados.

## Observações de funcionamento

- O programa utiliza `socket(AF_INET, SOCK_DGRAM, 0)` para comunicação UDP.
- Ele se inscreve no grupo multicast `239.0.0.1` com `IP_ADD_MEMBERSHIP`.
- A resposta é enviada ao cliente que enviou a mensagem original, sem envolver um servidor central.
- O app continua executando em loop até ser interrompido com `Ctrl+C`.

## Fluxo de validação

Para validar o projeto após alterações, os comandos mais relevantes são:

```bash
make
make lint
make test
make clean
```

A execução do programa em modo interativo deve ser feita separadamente com `make run` porque ele permanece aguardando mensagens.
