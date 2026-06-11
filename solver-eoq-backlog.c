/*
integrantes:
Camily Leal Silva -0117239
Felipe de Castro Leal -0117037
Gabriel Mendonça de Oliveira -0117251
Sofia Maria de Jesus Leal -120376

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{

    double d;  // Demanda anual
    double s;  // Custo de Setup
    double h;  // Custo de estocagem
    double pi; // Custo de falta

} Parametros;

double EquacaoDerivadaParcial(Parametros p, double q)
{
    return (-(p.d * p.s) / (q * q)) + (p.h / 2) * (1 - (p.h / (p.h + p.pi)));
}

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

double EquacaoPolinomio(Parametros p, double q)
{
    return (((p.h / 2.0) * (1 - (p.h / (p.h + p.pi)))) * q * q - p.d * p.s);
}

int Tabelamento(int *a, int *b, int passo, Parametros p)
{

    int q = a;
    int q_prox = q + passo;

    while (q < b)
    {
        double fx = EquacaoPolinomio(p, q);
        double fx_prox = EquacaoPolinomio(p, q_prox);

        if (fx * fx_prox < 0)
        {
            return 1;
        }

        q = q_prox;
        q_prox = q + passo;
    }
}

double EquacaoB_Isolado(Parametros p, double q)
{
    return (q * (p.h / (p.h + p.pi)));
}

double EquacaoCustoTotal(Parametros p, double q, double b)
{
    return (((p.d / q) * p.s) + (pow((q - b), 2) / 2 * q) * p.h + (pow(b, 2) / 2 * q) * p.pi);
}
int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        printf("Numero de parametros invalido, programa encerrando");
        return 0;
    }

    Parametros p;
    p.d = atof(argv[1]);
    p.h = atof(argv[2]);
    p.s = atof(argv[3]);
    p.pi = atof(argv[4]);
    return 0;
}