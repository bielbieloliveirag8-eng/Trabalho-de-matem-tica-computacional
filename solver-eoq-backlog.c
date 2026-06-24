/*INSTITUTO FEDERAL DE MINAS GERAIS - CAMPUS FORMIGA
 Bacharelado em Ciência da Computação
 Disciplina: Matemática Computacional
 Professor: Diego Mello da Silva

 Integrantes do Grupo:
 1. Camily Leal silva - Matrícula: 0117239
 2. Gabriel Mendonça de Oliveira - Matrícula: 0117251
 3. Felipe de Castro Leal Ribeiro - Matrícula: 0117037
 4. Sofia Maria Jesus Leal - Matrícula: 0120376

 Instruções de compilação:
  ->Linux:
    -gcc solver-eoq-backlog.c -lm -o solver-eoq-backlog.bin 
    -./solver-eoq-backlog.bin  <D> <H> <S> <PI> 
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*
struct de parametros de entrada 
*/
typedef struct
{
    double d;  // Demanda anual
    double s;  // Custo de Setup
    double h;  // Custo de estocagem
    double pi; // Custo de falta
}Parametros;

double EquacaoDerivadaParcial(Parametros p, double q);
double MetodoDaBisseccao(Parametros p, double a, double b, double epsilon, int maxIter);
double EquacaoPolinomio(Parametros p, double q);
int Tabelamento(double *a, double *b, double passo, Parametros p);
double EquacaoB_Isolado(Parametros p, double q);
double EquacaoCustoTotal(Parametros p, double q, double b);
double EstoqueMaximo(double q, double b);
void saida(Parametros p, double a, double b, double q, double b_faltas, double custo_total, double estoque_max);

int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        printf("Numero de parametros inválido, programa encerrando");
        return 0;
    }

    Parametros p;
    p.d = atof(argv[1]);
    p.h = atof(argv[2]);
    p.s = atof(argv[3]);
    p.pi = atof(argv[4]);

    double a = 0, b = 0;
 
    if(!Tabelamento(&a, &b, 1, p))
    {
        printf("Nao foi possivel encontrar um intervalo de isolamento, programa encerrando");
        return 0;
    }

    double q = MetodoDaBisseccao(p, a, b, 0.0000001, 500);
    double b_faltas = EquacaoB_Isolado(p,q);
    double custo_total = EquacaoCustoTotal(p, q, b_faltas);
    double estoque_max = EstoqueMaximo(q, b_faltas);

    saida(p, a, b, q, b_faltas, custo_total, estoque_max);

    return 0;
}

/**
 * Nome: EquacaoDerivadaParcial
 * Objetivo:
 *    retornar a função da derivada parcial
 * Parâmetros:
 *    - struct de parametros p
 *    - variavel q: tamanho otimo do estoque de produção
 * Retorno: o resultado da função.
 */
double EquacaoDerivadaParcial(Parametros p, double q)
{
    return (-(p.d * p.s) / (q * q)) + (p.h / 2) * (1 - (p.h / (p.h + p.pi)));
}

/**
 * Nome: MetodoDaBisseccao
 * Objetivo:
 *    Encontrar a raiz (q) para tamanho otimo do estoque de produção
 * Parâmetros:
 *    - struct de parametros p
 *    - variavel a,b : intervalo [a,b] 
 *    - variavel epsilon: tamanho do erro
 *    - variavel maxIter: maximo de iterações
 *
 * Retorno: raiz.
 */
double MetodoDaBisseccao(Parametros p, double a, double b, double epsilon, int maxIter)
{

    if (EquacaoDerivadaParcial(p, a) * EquacaoDerivadaParcial(p, b) > 0)
    {
        printf("\n  A Funcao nao muda de sinal nos extremos do intervalo");

        return 0;
    }

    double x;
    double DeltaX;
    int iter = 0;

    while (iter < maxIter)
    {
        x = (a + b) / 2;
        DeltaX = (b - a) / 2;

        if (fabs(DeltaX) <= epsilon && (fabs(EquacaoDerivadaParcial(p, x)) <= epsilon))
        {
            return x;
        }
        else if (EquacaoDerivadaParcial(p, a) * EquacaoDerivadaParcial(p, x) > 0)
        {
            a = x;
        }
        else
        {
            b = x;
        }

        iter++;
    }
    return x;
}

/**
 * Nome: EquacaoPolinomio
 * Objetivo:
 *    retornar a função do polinomio 
 * Parâmetros:
 *    - struct de parametros p
 *    - variavel q: tamanho otimo do estoque de produção
 * Retorno: o resultado da função.
 */
double EquacaoPolinomio(Parametros p, double q)
{
    return (((p.h / 2.0) * (1 - (p.h / (p.h + p.pi)))) * pow(q, 2) - (p.d * p.s));
}

/**
 * Nome: Tabelamento
 * Objetivo:
 *    Encontrar os intervalos de isolamento usados no metodo da bissecção
 * Parâmetros:
 *   - variavel a,b : intervalo [a,b] 
 *   - variavel passo: tamanho do deslocamento 
 *   - struct de parametros p
 *
 * Retorno: 1 = existe raiz
 *          0 = não existe raiz.
 *
 * Observações:
 *   - a = 0
 *   - b = a + passo
 */
int Tabelamento(double *a, double *b, double passo, Parametros p)
{

    double q = *a;
    double q_prox = q + passo;

    while (q < q_prox)
    {
        double fx = EquacaoPolinomio(p, q);
        double fx_prox = EquacaoPolinomio(p, q_prox);

        if (fx * fx_prox < 0)
        {
            *a = q;
            *b = q_prox;
            return 1;
        }

        q = q_prox;
        q_prox = q + passo;
    }

    return 0;
}

/**
 * Nome: EquacaoB_Isolado
 * Objetivo:
 *    retornar a função do B isolado.
 * Parâmetros:
 *    - struct de parametros p
 *    - variavel q: tamanho otimo do estoque de produção
 * Retorno: o resultado da função.
 */
double EquacaoB_Isolado(Parametros p, double q)
{
    return (q * (p.h / (p.h + p.pi)));
}

/**
 * Nome: EquacaoCustoTotal
 * Objetivo:
 *    retornar a função do custo total.
 * Parâmetros:
 *    - struct de parametros p
 *    - variavel q: tamanho otimo do estoque de produção
 *    - variavel b: faltas planejadas
 * Retorno: o resultado da função.
 */
double EquacaoCustoTotal(Parametros p, double q, double b)
{
    return (((p.d / q) * p.s) + (pow((q - b), 2) / (2 * q)) * p.h + (pow(b, 2) / (2 * q)) * p.pi);
}

/**
 * Nome: EstoqueMaximo
 * Objetivo:
 *    retornar a função do estoque maximo.
 * Parâmetros:
 *    - variavel q: tamanho otimo do estoque de produção
 *    - variavel b: faltas planejadas
 * Retorno: o resultado da função.
 */
double EstoqueMaximo(double q, double b)
{
    return q - b;
}

/**
 * Nome: EstoqueMaximo
 * Objetivo:
 *    imprimir o relatorio do software
 * Parâmetros:
 *    - variavel q: tamanho otimo do estoque de produção
 *    - variavel b_faltas: faltas planejadas
 *    - variavel a,b : intervalo [a,b] 
 *    - variavel custo_total: custo total adquirido pela função EquacaoCustoTotal
 *    - variavel estoque_max: estoque maximo adquirido pela função EstoqueMaximo
 * Retorno: sem retorno
 */
void saida(Parametros p, double a, double b, double q, double b_faltas, double custo_total, double estoque_max){
    printf("========================================================\n");
    printf("RELATORIO DE DIMENSIONAMENTO DE ESTOQUES (RAIZES)\n");
    printf("========================================================\n\n");
    printf(">> PARAMETROS DE ENTRADA:\n");
    printf("Demanda Anual (D).........: %.2f\n", p.d);
    printf("Custo de Estocagem (H)....: %.2f\n", p.h);
    printf("Custo de Setup (S)........: %.2f\n", p.s);
    printf("Custo de Falta (PI).......: %.2f\n", p.pi);
    printf("--------------------------------------------------------\n");
    printf(">> INTERVALO DE ISOLAMENTO:\n");
    printf("[a, b]....................: [%.2f, %.2f]\n", a, b);
    printf("--------------------------------------------------------\n");
    printf(">> RESULTADOS DA OTIMIZACAO:\n");
    printf("Lote Otimo (Q*)..........: %.2f\n", q);
    printf("Faltas Planejadas (B*)...: %.2f\n", b_faltas);
    printf("Estoque Maximo (Imax)....: %.2f\n", estoque_max);
    printf("Custo Total (CT).........: R$ %.2f\n", custo_total);
    printf("========================================================\n");
}