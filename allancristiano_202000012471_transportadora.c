#include <stdio.h>
#include <stdlib.h>

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

void imprimirPacotes(Pacote pacotes[], int tamanho, FILE *saida)
{
    for (int i = 0; i < tamanho; i++)
    {
        fprintf(saida, "%s\n", pacotes[i].codigo);
    }
}

void imprimirCaminhao(Caminhao caminhao, FILE *saida)
{
    double porcentagem_peso = ((double)caminhao.peso_total / caminhao.peso) * 100;
    double porcentagem_volume = ((double)caminhao.volume_total / caminhao.volume) * 100;

    fprintf(saida, "[%s]", caminhao.placa);
    fprintf(saida, "R$%.2lf,", caminhao.valor_total);
    fprintf(saida, "%dKG(%0.lf%%)", caminhao.peso_total, porcentagem_peso);
    fprintf(saida, "%dL(%0.lf%%)\n", caminhao.volume_total, porcentagem_volume);

    for (int i = caminhao.num_pacotes - 1; i >= 0; i--)
    {
        Pacote pacote = *(caminhao.pacotes[i]);
        fprintf(saida, "%s\n", pacote.codigo);
    }
}

void removerPacote(Pacote pacotes[], int *total_pacotes, int indice)
{
    for (int i = indice; i < (*total_pacotes) - 1; i++)
    {
        pacotes[i] = pacotes[i + 1];
    }
    (*total_pacotes)--;
}

void prepararCaminhao(Caminhao *caminhao, Pacote pacotes[], int *total_pacotes)
{
    int capacidade_peso = caminhao->peso;
    int capacidade_volume = caminhao->volume;

    double ***tabela = (double ***)malloc(((*total_pacotes) + 1) * sizeof(double **));
    for (int i = 0; i <= (*total_pacotes); i++)
    {
        tabela[i] = (double **)malloc((capacidade_peso + 1) * sizeof(double *));
        for (int j = 0; j <= capacidade_peso; j++)
        {
            tabela[i][j] = (double *)malloc((capacidade_volume + 1) * sizeof(double));
        }
    }

   

    for (int i = 0; i <= (*total_pacotes); i++)
    {
        for (int w = 0; w <= capacidade_peso; w++)
        {
            for (int v = w; v <= capacidade_volume; v++)
            {
                if (i == 0 || w == 0 || v == 0)
                {
                    tabela[i][w][v] = 0;
                }
                else if (pacotes[i - 1].peso <= w && pacotes[i - 1].volume <= v)
                {
                    tabela[i][w][v] = max(pacotes[i - 1].valor + tabela[i - 1][w - pacotes[i - 1].peso][v - pacotes[i - 1].volume],
                                          tabela[i - 1][w][v]);
                }
                else
                {
                    tabela[i][w][v] = tabela[i - 1][w][v];
                }
            }
        }
    }
    

    int i = (*total_pacotes);
    int w = capacidade_peso;
    int v = capacidade_volume;

    while (i > 0 && w > 0 && v > 0)
    {
        if (tabela[i][w][v] != tabela[i - 1][w][v])
        {
            Pacote *novo_pacote = (Pacote *)malloc(sizeof(Pacote));
            *novo_pacote = pacotes[i - 1];
            caminhao->pacotes[caminhao->num_pacotes++] = novo_pacote;
            caminhao->valor_total += novo_pacote->valor;
            caminhao->peso_total += novo_pacote->peso;
            caminhao->volume_total += novo_pacote->volume;

            removerPacote(pacotes, total_pacotes, i - 1);

            i--;
            w -= novo_pacote->peso;
            v -= novo_pacote->volume;
        }
        else
        {
            i--;
        }
    }
    free(tabela);
}

int main(int argc, char const *argv[])
{
    (void)argc;
    FILE *arquivo = fopen(argv[1], "r");
    FILE *saida = fopen(argv[2], "w");
    int total_caminhoes, total_pacotes;

    fscanf(arquivo, "%d", &total_caminhoes);

    Caminhao caminhoes[total_caminhoes];

    for (int i = 0; i < total_caminhoes; i++)
    {
        fscanf(arquivo, "%s %d %d", caminhoes[i].placa, &caminhoes[i].peso, &caminhoes[i].volume);
        caminhoes[i].pacotes = (Pacote **)malloc(total_pacotes * sizeof(Pacote *));
        caminhoes[i].num_pacotes = 0;
        caminhoes[i].valor_total = 0;
        caminhoes[i].peso_total = 0;
        caminhoes[i].volume_total = 0;
    }

    fscanf(arquivo, "%d", &total_pacotes);

    Pacote pacotes[total_pacotes];

    for (int i = 0; i < total_pacotes; i++)
    {
        fscanf(arquivo, "%s %lf %d %d", pacotes[i].codigo, &pacotes[i].valor, &pacotes[i].peso, &pacotes[i].volume);
    }

    for (int i = 0; i < total_caminhoes; i++)
    {
        prepararCaminhao(&caminhoes[i], pacotes, &total_pacotes);
        imprimirCaminhao(caminhoes[i], saida);
    }

    double valor_pendente = 0;
    int quilos_pendentes = 0;
    int volume_pendente = 0;
    for (int i = 0; i < total_pacotes; i++)
    {
        valor_pendente += pacotes[i].valor;
        quilos_pendentes += pacotes[i].peso;
        volume_pendente += pacotes[i].volume;
    }

    fprintf(saida, "[PENDENTE]R$%.2lf,%dKG,%dL\n", valor_pendente, quilos_pendentes, volume_pendente);
    imprimirPacotes(pacotes, total_pacotes, saida);

    fclose(arquivo);
    fclose(saida);
    return 0;
}
