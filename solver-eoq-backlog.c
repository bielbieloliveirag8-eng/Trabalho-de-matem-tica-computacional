/*
integrantes:
Camily Leal Silva -0117239
Felipe de Castro Leal -0117037
Gabriel Mendonça de Oliveira -0117251
Sofia Maria de Jesus Leal -

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

        if (fabs(DeltaX)<=epsilon && (fabs(EquacaoDerivadaParcial(p,x))<=epsilon))
        {
            return x;
        } else if (EquacaoDerivadaParcial(p,a)*EquacaoDerivadaParcial(p,x)>0)
        {
            a=x;
        } else
        {
            b=x;
        }
        
        iter++;

        
    }
    return x;
}

int main(int argc, char const *argv[])
{

    return 0;
}
