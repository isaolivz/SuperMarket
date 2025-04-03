# Sistema Gerenciador de Supermercado

## Sobre o Projeto

Este projeto foi desenvolvido como parte do trabalho semestral da disciplina de **Estrutura de Dados**. O objetivo principal é implementar um Sistema Gerenciador de Supermercado utilizando conceitos fundamentais de estruturas de dados, como pilhas, listas encadeadas e manipulação de arquivos.

## Funcionalidades

**O sistema permite:**

Adicionar produtos ao estoque (armazenados em pilhas, simulando prateleiras)

Adicionar produtos ao carrinho (utilizando listas encadeadas para gerenciar os itens)

Remover produtos do carrinho

Listar os produtos disponíveis e no carrinho

Salvar e carregar produtos em arquivos, garantindo persistência de dados

## Estruturas de Dados Utilizadas

Pilhas: Utilizadas para armazenar produtos nas prateleiras do supermercado.

Listas Encadeadas: Gerenciamos os produtos no carrinho de compras.

Filas: Controlamos a ordem de atendimento dos clientes no caixa.

Busca Binária: Aplicada para localizar rapidamente produtos cadastrados.

Manipulação de Arquivos: Utilizada para salvar e recuperar informações dos produtos.

## Como Executar o Projeto

Clone o repositório (se aplicável):

git clone https://github.com/isaolivz/SuperMarket.git

Compile o código utilizando um compilador de C:

gcc testefinalfinalfinalprojeto.c -o SuperMercado


**Execute o programa:**

./Super.Market

## Estrutura do Código

SuperMarket.c → Arquivo principal do sistema

estoque.txt → Arquivo onde os produtos são armazenados

## Tecnologias Utilizadas

Linguagem: C

Conceitos Aplicados: Estruturas de Dados (pilhas, filas e listas encadeadas), manipulação de arquivos

Autor

Desenvolvido por Isabella Oliveira durante o curso de Análise e Desenvolvimento de Sistemas no IFSP.
