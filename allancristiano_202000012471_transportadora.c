#include <stdio.h>

typedef struct
{
    char codigo[20];
    double valor;
    int peso;
    int volume;
} Pacote;

typedef struct
{
    char placa[10];
    int peso;
    int volume;
    Pacote **pacotes;
    int num_pacotes;
    double valor_total;
    int peso_total;
    int volume_total;
} Caminhao;

double max(double a, double b)
{
    return (a > b) ? a : b;
}

void prepararCaminhao(Caminhao *caminhao, Pacote pacotes[], int total_pacotes, FILE *saida)
{
    double matrizValor[total_pacotes + 1][caminhao->peso + 1][caminhao->volume + 1];
    int pacotesSelecionados[total_pacotes + 1][caminhao->peso + 1][caminhao->volume + 1];

    for (int i = 0; i <= total_pacotes; i++)
    {
        for (int w = 0; w <= caminhao->peso; w++)
        {
            for (int v = 0; v <= caminhao->volume; v++)
            {
                if (i == 0 || w == 0 || v == 0)
                {
                    matrizValor[i][w][v] = 0;
                    pacotesSelecionados[i][w][v] = 0;
                }
                else if (pacotes[i - 1].peso <= w && pacotes[i - 1].volume <= v)
                {
                    double incluirItem = pacotes[i - 1].valor + matrizValor[i - 1][w - pacotes[i - 1].peso][v - pacotes[i - 1].volume];
                    double naoIncluirItem = matrizValor[i - 1][w][v];

                    if (incluirItem > naoIncluirItem)
                    {
                        matrizValor[i][w][v] = incluirItem;
                        pacotesSelecionados[i][w][v] = 1;
                    }
                    else
                    {
                        matrizValor[i][w][v] = naoIncluirItem;
                        pacotesSelecionados[i][w][v] = 0;
                    }
                }
                else
                {
                    matrizValor[i][w][v] = matrizValor[i - 1][w][v];
                    pacotesSelecionados[i][w][v] = 0;
                }
            }
        }
    }

    
    for (int i = total_pacotes, w = caminhao->peso, v = caminhao->volume; i > 0 && w > 0 && v > 0;)
    {
        if (pacotesSelecionados[i][w][v] == 1)
        {
            printf("Pacote %s foi selecionado. Peso: %d, Volume: %d\n",pacotes[i - 1].codigo , pacotes[i - 1].peso, pacotes[i - 1].volume);
            w -= pacotes[i - 1].peso;
            v -= pacotes[i - 1].volume;
            i--;
        }
        else
        {
            i--;
        }
    }
}

int main(int argc, char const *argv[])
{
    (void)argc;
    FILE *arquivo = fopen(argv[1], "r");
    FILE *saida = fopen(argv[2], "w");

    int total_caminhoes, total_pacotes;

    fscanf(arquivo, "%d", &total_caminhoes);
    // array todos os caminhoes
    Caminhao caminhoes[total_caminhoes];

    for (int i = 0; i < total_caminhoes; i++)
    {
        fscanf(arquivo, "%s %d %d", caminhoes[i].placa, &caminhoes[i].peso, &caminhoes[i].volume);
        caminhoes[i].pacotes = NULL;
        caminhoes[i].num_pacotes = 0;
        caminhoes[i].valor_total = 0;
        caminhoes[i].peso_total = 0;
        caminhoes[i].volume_total = 0;
    }

    fscanf(arquivo, "%d", &total_pacotes);
    // array de todos os pacotes
    Pacote pacotes[total_pacotes];

    for (int i = 0; i < total_pacotes; i++)
    {
        fscanf(arquivo, "%s %lf %d %d", pacotes[i].codigo, &pacotes[i].valor, &pacotes[i].peso, &pacotes[i].volume);
    }

    prepararCaminhao(&caminhoes[0], pacotes, total_pacotes, saida);

    fclose(arquivo);
    fclose(saida);
    return 0;
}
