#include <stdio.h>
#include <string.h>
#include <locale.h>

#define limiteItens 100
#define DESCONTO 0.10f

struct DadosItem {
    int codigoItem;
    int quantidadeItem;
    char nomeItem[50];
    char marcaItem[50];
    char dataVenda[50];
    float precoUnit;
};

// Cálculo com desconto, se aplicável
float calcularTotal(struct DadosItem item) {
    if (item.quantidadeItem >= 3)
        return item.quantidadeItem * item.precoUnit * (1.0f - DESCONTO);
    else
        return item.quantidadeItem * item.precoUnit;
}

// Função auxiliar para exibir item
void exibirItem(struct DadosItem item, int indice) {
    printf("\nItem %d:\n", indice + 1);
    printf("Nome: %s\n", item.nomeItem);
    printf("Marca: %s\n", item.marcaItem);
    printf("Quantidade: %d\n", item.quantidadeItem);
    printf("Preço unitário: %.2f\n", item.precoUnit);
    printf("Total da venda: R$ %.2f\n", calcularTotal(item));
}


// Exibe total de venda por item
float totalVenda(int quantidadeItem, float precoUnit) {
    struct DadosItem item = {.quantidadeItem = quantidadeItem, .precoUnit = precoUnit};
    float totalPorVenda = calcularTotal(item);

    if (quantidadeItem < 3)
        printf("Sua venda ficou com um total de: R$ %.2f!\n", totalPorVenda);
    else
        printf("Sua compra gerou um desconto de 10%%, ficando um total de: R$ %.2f!\n", totalPorVenda);

    return totalPorVenda;
}

// Soma no total do dia
float totalVendasDia(float acumulado, int quantidadeItem, float precoUnit) {
    struct DadosItem item = {.quantidadeItem = quantidadeItem, .precoUnit = precoUnit};
    return acumulado + calcularTotal(item);
}

// Ordena vendas por valor total
void ordenarPorValorTotal(struct DadosItem vendas[], int total) {
    int trocou;
    for (int i = 0; i < total - 1; i++) {
        trocou = 0;
        for (int j = 0; j < total - i - 1; j++) {
            if (calcularTotal(vendas[j]) < calcularTotal(vendas[j + 1])) {
                struct DadosItem temp = vendas[j];
                vendas[j] = vendas[j + 1];
                vendas[j + 1] = temp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

// Gera relatório por data
void gerarRelatorioPorData(struct DadosItem vendas[], int total) {
    char dataBusca[50];
    int encontrou = 0;
    float totalDoDia = 0;
    int maisVendidoIndex = -1;
    int menosVendidoIndex = -1;

    printf("\nDigite a data para consultar (dd/mm/aaaa): ");
    scanf(" %[^\n]", dataBusca);

    for (int i = 0; i < total; i++) {
        if (strcmp(vendas[i].dataVenda, dataBusca) == 0) {
            if (!encontrou) {
                printf("\n--- Itens vendidos em %s ---\n", dataBusca);
                encontrou = 1;
            }

            exibirItem(vendas[i], i);
            totalDoDia += calcularTotal(vendas[i]);

            if (maisVendidoIndex == -1 || vendas[i].quantidadeItem > vendas[maisVendidoIndex].quantidadeItem)
                maisVendidoIndex = i;
            if (menosVendidoIndex == -1 || vendas[i].quantidadeItem < vendas[menosVendidoIndex].quantidadeItem)
                menosVendidoIndex = i;
        }
    }

    if (!encontrou) {
        printf("\nNenhuma venda encontrada para a data %s.\n", dataBusca);
    } else {
        printf("\nTotal vendido em %s: R$ %.2f\n", dataBusca, totalDoDia);

        printf("\n--- Item mais vendido em %s ---\n", dataBusca);
        exibirItem(vendas[maisVendidoIndex], maisVendidoIndex);

        printf("\n--- Item menos vendido em %s ---\n", dataBusca);
        exibirItem(vendas[menosVendidoIndex], menosVendidoIndex);
    }
}

// Função principal
int main(void) {
    setlocale(LC_ALL, "Portuguese");

    struct DadosItem vendas[limiteItens];
    int total = 0, clientes = 0;
    float totalDia = 0.0f;
    char continuar, consultarOutroDia;

    do {
        if (total >= limiteItens) {
            printf("Limite de itens atingido.\n");
            break;
        }

        printf("Código (somente números): ");
        scanf(" %d", &vendas[total].codigoItem);

        printf("Nome: ");
        scanf(" %[^\n]", vendas[total].nomeItem);

        printf("Marca: ");
        scanf(" %[^\n]", vendas[total].marcaItem);

        printf("Data (dd/mm/aaaa): ");
        scanf(" %[^\n]", vendas[total].dataVenda);

        printf("Quantidade: ");
        scanf("%d", &vendas[total].quantidadeItem);

        printf("Preço unitário: ");
        scanf("%f", &vendas[total].precoUnit);

        totalDia = totalVendasDia(totalDia, vendas[total].quantidadeItem, vendas[total].precoUnit);
        totalVenda(vendas[total].quantidadeItem, vendas[total].precoUnit);

        total++;
        clientes++;

        printf("Deseja cadastrar outro item vendido? (s/n): ");
        scanf(" %c", &continuar);
        printf("\n\n");

    } while (continuar == 's' || continuar == 'S');

    printf("____________________________________________\n\n");
    printf("Relatório geral do dia: \n\n");

    printf("\nForam cadastrados %d itens.\n", total);
    printf("Quantidade de clientes do dia: %d\n", clientes);
    printf("Valor total de vendas do dia: %.2f", totalDia);

    ordenarPorValorTotal(vendas, total);

    printf("\n\nItens ordenados por valor total da venda (maior para menor):\n");
    for (int i = 0; i < total; i++) {
        exibirItem(vendas[i], i);
    }

    if (total > 0) {
        int indiceMaisVendido = 0;
        int indiceMenosVendido = 0;

        for (int i = 1; i < total; i++) {
            if (vendas[i].quantidadeItem > vendas[indiceMaisVendido].quantidadeItem)
                indiceMaisVendido = i;
            if (vendas[i].quantidadeItem < vendas[indiceMenosVendido].quantidadeItem)
                indiceMenosVendido = i;
        }

        printf("\n\n------Item mais vendido: ------\n");
        exibirItem(vendas[indiceMaisVendido], indiceMaisVendido);

        printf("\n\n------Item menos vendido: ------\n");
        exibirItem(vendas[indiceMenosVendido], indiceMenosVendido);

        do {
            printf("\nDeseja consultar outro dia? (s/n): ");
            scanf(" %c", &consultarOutroDia);

            if (consultarOutroDia == 's' || consultarOutroDia == 'S') {
                gerarRelatorioPorData(vendas, total);
            }
        } while (consultarOutroDia == 's' || consultarOutroDia == 'S');
    }

    return 0;
}
