#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>

void mudarCor(int corTexto, int corFundo) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (corFundo << 4) | corTexto);
}

typedef struct {
  char marca[50];
  char modelo[50];
  int anoFabricacao;
  int capacidaDeMemoria;
  float preco;
} Smartphone;

Smartphone smartphones[100];
int total = 0;

void inserirDadoOrdenado(Smartphone s) {
  int i = total - 1;
  while (i >= 0 && smartphones[i].preco > s.preco) {
    smartphones[i + 1] = smartphones[i];
    i--;
  }
  smartphones[i + 1] = s;
  total++;
}

void carregarArquivo() {
  //r = read
  FILE *file = fopen("smartphones.txt", "r");
  if (!file) {
    printf("Erro ao abrir arquivo\n");
    return;
  }

  Smartphone s;
  char linha[100];
  while (fgets(s.marca, sizeof(s.marca), file) != NULL) {
    int len = strlen(s.marca);
    if (len > 0 && s.marca[len - 1] == '\n') {
      s.marca[len - 1] = '\0';
    }

    if (fgets(s.modelo, sizeof(s.modelo), file) == NULL) break;
    len = strlen(s.modelo);
    if (len > 0 && s.modelo[len - 1] == '\n') {
      s.modelo[len - 1] = '\0';
    }

    if (fgets(linha, sizeof(linha), file) == NULL) break;
    s.anoFabricacao = atoi(linha);

    if (fgets(linha, sizeof(linha), file) == NULL) break;
    s.capacidaDeMemoria = atoi(linha);

    if (fgets(linha, sizeof(linha), file) == NULL) break;
    s.preco = atof(linha);

    inserirDadoOrdenado(s);
  }

  fclose(file);
}

void salvarArquivo() {
  //w = write
  FILE *file = fopen("smartphones.txt", "w");
  if (!file) {
    printf("Erro ao abrir arquivo para escrita\n");
    return;
  }

  for (int i = 0; i < total; i++) {
    fprintf(file, "%s\n%s\n%d\n%d\n%.2f\n",
            smartphones[i].marca,
            smartphones[i].modelo,
            smartphones[i].anoFabricacao,
            smartphones[i].capacidaDeMemoria,
            smartphones[i].preco);
  }

  fclose(file);
}


void exibirTodosSmartphones() {
  mudarCor(0, 10);
  printf("                Lista de Smartphones                \n");
  for (int i = 0; i < total; i++) {
    mudarCor(15, 0);
    printf("[%d] %s %s %d %dGB R$%.2f\n", i + 1,
           smartphones[i].marca, smartphones[i].modelo,
           smartphones[i].anoFabricacao, smartphones[i].capacidaDeMemoria,
           smartphones[i].preco);
  }
}

void exibirPorMarca() {
  mudarCor(0, 9);
  printf("                 Listar por marca                   \n");
  char marcaBusca[50];
  mudarCor(7, 0);
  printf("Digite a marca:");
  scanf(" %50[^\n]", marcaBusca);
  printf("Smartphones da marca %s:\n", marcaBusca);
  for (int i = 0; i < total; i++) {
    if (strcmp(smartphones[i].marca, marcaBusca) == 0) {
      printf("[%d] %s %s %d %dGB R$%.2f\n", i + 1,
             smartphones[i].marca, smartphones[i].modelo,
             smartphones[i].anoFabricacao, smartphones[i].capacidaDeMemoria,
             smartphones[i].preco);
    }
  }
}

void exibirPorPreco() {
  mudarCor(0, 13);
  printf("                 Listar por preço                   \n");
  mudarCor(7, 0);
  float precoMin, precoMax;
  printf("Digite o preço mínimo e máximo (preço mínimo + ESPAÇO + preço máximo):");
  scanf("%f %f", &precoMin, &precoMax);
  for (int i = 0; i < total; i++) {
    if (smartphones[i].preco >= precoMin && smartphones[i].preco <= precoMax) {
      printf("[%d] %s %s %d %dGB R$%.2f\n", i + 1,
             smartphones[i].marca, smartphones[i].modelo,
             smartphones[i].anoFabricacao, smartphones[i].capacidaDeMemoria,
             smartphones[i].preco);
    }
  }
}

void inserirSmartphoneNovo() {
  mudarCor(0, 14);
  printf("              Inserir novo Smartphone               \n");
  mudarCor(7, 0);
  Smartphone s;
  printf("Digite a marca:");
  scanf(" %50[^\n]", s.marca);
  printf("Digite o modelo:");
  scanf(" %50[^\n]", s.modelo);
  printf("Digite o ano:");
  scanf("%d", &s.anoFabricacao);
  printf("Digite a memória (GB):");
  scanf("%d", &s.capacidaDeMemoria);
  printf("Digite o preço:");
  scanf("%f", &s.preco);
  inserirDadoOrdenado(s);
  salvarArquivo();
  printf("Smartphone inserido com sucesso!\n");
}

void removerMemoriaInferior() {
  mudarCor(0, 11);
  printf("     Remover por memória inferior à especificada    \n");
  mudarCor(7, 0);
  int minMemoria;
  printf("Digite a memória mínima (GB):");
  scanf("%d", &minMemoria);
  int i = 0;
  while (i < total) {
    if (smartphones[i].capacidaDeMemoria < minMemoria) {
      for (int j = i; j < total - 1; j++) {
        smartphones[j] = smartphones[j + 1];
      }
      total--;
    } else {
      i++;
    }
  }
  salvarArquivo();
  printf("Smartphones com memória inferior a %dGB removidos!\n", minMemoria);
}

void sairPrograma() {
  mudarCor(0, 4);
  printf("Saindo do Programa...");
  exit(0);
}

void menu() {
  int opcao;
  do {
    mudarCor(1, 15); // Texto azul e fundo preto
    printf("                       MENU                         \n");

    mudarCor(2, 15); // Texto azul e fundo preto
    printf(" 1 -> Exibir todos os smartphones                   \n");

    mudarCor(1, 15); // Texto verde e fundo preto
    printf(" 2 -> Exibir por marca                              \n");

    mudarCor(5, 15); // Texto cyan e fundo preto
    printf(" 3 -> Exibir por faixa de preço                     \n");

    mudarCor(6, 15); // Texto amarelo e fundo preto
    printf(" 4 -> Inserir novo smartphone                       \n");

    mudarCor(3, 15); // Texto magenta e fundo preto
    printf(" 5 -> Remover por memória inferior à especificada   \n");

    mudarCor(0, 4); // Texto vermelho e fundo preto
    printf(" 6 -> Sair \n");

    mudarCor(7, 0); // Texto branco e fundo preto
    printf("Escolha uma opção:");
    scanf("%d", &opcao);

    if (opcao == 1) {
      exibirTodosSmartphones();
    }
    else if (opcao == 2) {
      exibirPorMarca();
    }
    else if (opcao == 3) {
      exibirPorPreco();
    }
    else if (opcao == 4) {
      inserirSmartphoneNovo();
    }
    else if (opcao == 5) {
      removerMemoriaInferior();
    }
    else if (opcao == 6) {
      sairPrograma();
    }
    else {
      printf("Inválido, Tente novamente.\n");
    }
  } while (opcao != 6);
}

int main() {
  setlocale(LC_ALL, "Portuguese_Brazil");
  carregarArquivo();
  menu();

  system("pause");
  return 0;
}