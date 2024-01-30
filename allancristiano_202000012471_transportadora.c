#include <stdio.h>

typedef struct
{
    char placa[7];
    int peso;
    int volume;
} Caminhao;

typedef struct
{
    char cordigo[20];
    double valor;
    int peso;
    int volume;    
} Encomenda;


int main(int argc, char const *argv[])
{
    (void)argc;
    FILE *arquivo = fopen(argv[1], "r");


    fclose(arquivo);
    return 0;
}


