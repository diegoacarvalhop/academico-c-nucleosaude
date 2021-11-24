#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    char nome[30];
    int sexo;
    Data dataNascimento;
    char cpf[11];
} Dependente;

typedef struct {
    char nome[25], email[35], telefone[11];
    int sexo, plano;
    float valorPlano;
    Data dataNascimento, dataVencimento;
    Dependente dependentes[10];
    char cpf[11];
} Titular;

Titular titular, titular_aux;

FILE *base_clientes;

SYSTEMTIME time;

int idade, qtdDependentes, posicao, opcao, resposta, temDependente, confirmar, cpfExiste, plano;

float valorPlano;

char cpf[11];

void linha();

void cabecalho();

void abre_arquivo();

Dependente popularDependente();

void inserir();

float calculaValorPlano(Titular titular);

void listagem();

void listagemDependentes();

int procura(char cpf[11]);

void mostre(int pos);

void remover();

void alterar();

void listagemPorPlano();

void listagemVencimento();

int main() {
    setlocale(LC_ALL, "Portuguese");
    GetSystemTime(&time);
    abre_arquivo();
    do {
        system("cls");
        cabecalho();
        printf("\nOpcoes: \n\n");
        printf("1 - Cadastrar novo Cliente\n");
        printf("2 - Editar Cliente\n");
        printf("3 - Remover Cliente\n");
        printf("4 - Listagem Geral\n");
        printf("5 - Listagem por Plano\n");
        printf("6 - Listagem dos Vencimentos do Plano de Saude do Mes\n");
        printf("7 - (BONUS) Listagem dos Dependentes por Titular\n");
        printf("0 - Sair\n");
        linha();
        printf("\n\nInforme a opcao desejada: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                inserir();
                break;
            case 2:
                alterar();
                break;
            case 3:
                remover();
                break;
            case 4:
                listagem();
                break;
            case 5:
                listagemPorPlano();
                break;
            case 6:
                listagemVencimento();
                break;
            case 7:
                listagemDependentes();
                break;
            case 0:
                fclose(base_clientes);
                printf("\nAte a Proxima !\n\n");
                system("pause");
                break;
            default:
                printf("\n\nOpcao invalida!");
                break;
        }
    } while (opcao != 0);
    return 0;
}

void linha() {
    for (int i = 1; i <= 130; i++) {
        printf("_");
    }
}

void cabecalho() {
    printf("\n");
    linha();
    printf("                                                            Nucleo Saude\n");
    linha();
}

void abre_arquivo() {
    base_clientes = fopen("..\\base_clientes.txt", "r+b");
    if (base_clientes == NULL) {
        base_clientes = fopen("..\\base_clientes.txt", "w+b");
    }
}

Dependente popularDependente() {
    system("cls");
    cabecalho();
    linha();
    Dependente dependente;
    printf("\nCadastrar novo dependente\n");
    linha();
    printf("\n\nCPF: ");
    scanf("%s", &dependente.cpf);
    printf("\nNome: ");
    fflush(stdin);
    gets(dependente.nome);
    printf("\nSexo:\n1 - Feminino\n2 - Masculino\nEscolha:");
    scanf("%d", &dependente.sexo);
    printf("\nData de Nascimento:\nDia: ");
    scanf("%d", &dependente.dataNascimento.dia);
    printf("Mes: ");
    scanf("%d", &dependente.dataNascimento.mes);
    printf("Ano: ");
    scanf("%d", &dependente.dataNascimento.ano);
    return dependente;
}

void inserir() {
    do {
        system("cls");
        cabecalho();
        linha();
        printf("Cadastrar Novo Cliente\n");
        linha();
        do {
            cpfExiste = 0;
            printf("\n\nCPF: ");
            scanf("%s", &cpf);
            rewind(base_clientes);
            fread(&titular, sizeof(Titular), 1, base_clientes);
            while (feof(base_clientes) == 0) {
                if (strcmp(titular.cpf, cpf) == 0) {
                    printf("\nCPF %s ja cadastrado! Informe um CPF diferente!", cpf);
                    cpfExiste = 1;
                }
                fread(&titular, sizeof(Titular), 1, base_clientes);
            }
            if (cpfExiste == 0) {
                memset(&titular, 0, sizeof(Titular));
                strcpy(&titular.cpf, cpf);
            }
        } while (cpfExiste == 1);
        printf("\nNome: ");
        fflush(stdin);
        gets(titular.nome);
        printf("\nSexo:\n1 - Feminino\n2 - Masculino\nEscolha:");
        scanf("%d", &titular.sexo);
        printf("\nTelefone: ");
        scanf("%s", &titular.telefone);
        printf("\nE-mail: ");
        fflush(stdin);
        gets(titular.email);
        printf("\nData de Nascimento:\nDia: ");
        scanf("%d", &titular.dataNascimento.dia);
        printf("Mes: ");
        scanf("%d", &titular.dataNascimento.mes);
        printf("Ano: ");
        scanf("%d", &titular.dataNascimento.ano);
        printf("\nPlano: \n1 - Ouro\n2 - Diamante\n3 - Prata\n4 - Esmeralda\nEscolha: ");
        scanf("%d", &titular.plano);
        int x = 0;
        do {
            printf("\nDeseja cadastrar um dependente?\n1 - Sim\n0 - Nao\nResposta: ");
            scanf("%d", &temDependente);
            if (temDependente == 1) {
                titular.dependentes[x] = popularDependente();
            }
            x++;
        } while (temDependente == 1);
        titular.valorPlano = calculaValorPlano(titular);
        titular.dataVencimento.dia = time.wDay;
        titular.dataVencimento.mes = time.wMonth;
        titular.dataVencimento.ano = (time.wYear + 1);
        fseek(base_clientes, 0, SEEK_END);
        fwrite(&titular, sizeof(Titular), 1, base_clientes);
        system("cls");
        cabecalho();
        printf("\n\nCliente cadastrado com sucesso!\n");
        printf("\nDeseja cadastrar outro Cliente?\n1 - Sim\n0 - Nao\nResposta: ");
        scanf("%d", &resposta);
        if (resposta == 1) {
            memset(titular.dependentes, 0, sizeof(Dependente));
        }
    } while (resposta == 1);
}

float calculaValorPlano(Titular titular) {
    for (qtdDependentes = 0; qtdDependentes < sizeof(titular.dependentes) / sizeof(titular.dependentes[0]);) {
        if (strcmp(titular.dependentes[qtdDependentes].cpf, "")) {
            qtdDependentes++;
        } else {
            break;
        }
    }
    switch (titular.plano) {
        case 1:
            valorPlano = 300.00;
            break;
        case 2:
            valorPlano = 400.00;
            break;
        case 3:
            valorPlano = 200.00;
            break;
        case 4:
            valorPlano = 500.00;
            break;
        default:
            break;
    }
    idade = 365 * time.wYear + 30 * time.wMonth + time.wDay - 365 * titular.dataNascimento.ano - 30 * titular.dataNascimento.mes - titular.dataNascimento.dia;
    idade /= 365;
    //Aqui faz a validação somente do titular e aplica os acréscimos e descontos
    if (titular.sexo == 1 && (idade >= 13 && idade <= 35)) {
        valorPlano *= (qtdDependentes + 1) * (1 + 0.30);
    } else if (idade < 13) {
        valorPlano *= (qtdDependentes + 1) * (1 - 0.30);
    } else if (idade >= 60) {
        valorPlano *= (qtdDependentes + 1) * (1 + 0.40);
    } else {
        valorPlano *= (qtdDependentes + 1);
    }
    //Aqui faz a validação de todos os dependentes do titular e aplica os acréscimos e descontos.
    //Para o primeiro IF, tive que adicionar a variável SEXO no dependente.
    for (int x = 0; x < qtdDependentes; x++) {
        idade = 365 * time.wYear + 30 * time.wMonth + time.wDay - 365 * titular.dependentes[x].dataNascimento.ano - 30 * titular.dependentes[x].dataNascimento.mes - titular.dependentes[x].dataNascimento.dia;
        idade /= 365;
        if (titular.dependentes[x].sexo == 1 && (idade >= 13 && idade <= 35)) {
            valorPlano *= (1 + 0.30);
        } else if (idade < 13) {
            valorPlano *= (1 - 0.30);
        } else if (idade >= 60) {
            valorPlano *= (1 + 0.40);
        }
    }
    if (qtdDependentes > 1) {
        valorPlano *= (1 - 0.20);
    }
    return valorPlano;
}

void listagem() {
    system("cls");
    cabecalho();
    linha();
    printf("                                                           Listagem Geral\n");
    linha();
    printf("CPF         Nome                      Sexo Fone        Email                               Idade Plano Dep.      Valor Venc.\n");
    linha();
    rewind(base_clientes);
    fread(&titular, sizeof(Titular), 1, base_clientes);
    while (feof(base_clientes) == 0) {
        if (titular.sexo != 0) {
            idade = 365 * time.wYear + 30 * time.wMonth + time.wDay - 365 * titular.dataNascimento.ano - 30 * titular.dataNascimento.mes - titular.dataNascimento.dia;
            idade /= 365;
            for (qtdDependentes = 0;
                 qtdDependentes < sizeof(titular.dependentes) / sizeof(titular.dependentes[0]);) {
                if (strcmp(titular.dependentes[qtdDependentes].cpf, "") != 0) {
                    qtdDependentes++;
                } else {
                    break;
                }
            }
            printf("%-11s %-25s %4d %-11s %-35s %5d %5d %-4d %10.2f %d/%d/%d\n",
                   titular.cpf,
                   titular.nome,
                   titular.sexo,
                   titular.telefone,
                   titular.email,
                   idade,
                   titular.plano,
                   qtdDependentes,
                   titular.valorPlano,
                   titular.dataVencimento.dia,
                   titular.dataVencimento.mes,
                   titular.dataVencimento.ano);
        }
        fread(&titular, sizeof(Titular), 1, base_clientes);
    }
    if (titular.sexo == 0) {
        printf("\n\nNao Existem Clientes Cadastrados !\n");
    }
    linha();
    printf("\n\n");
    system("pause");
}

void listagemDependentes() {
    system("cls");
    cabecalho();
    linha();
    printf("\nInforme o CPF do Titular\n");
    linha();
    printf("\n\nCPF: ");
    scanf("%s", &cpf);
    posicao = procura(cpf);
    fseek(base_clientes, posicao * sizeof(Titular), SEEK_SET);
    fread(&titular, sizeof(Titular), 1, base_clientes);
    system("cls");
    cabecalho();
    printf("\nTitular: %s", titular.nome);
    printf("\n");
    linha();
    printf("\n                                                   Listar Dependentes por Titular\n");
    linha();
    printf("CPF         Nome                      Sexo Idade\n");
    linha();
    for (int x = 0; x < sizeof(titular.dependentes) / sizeof(titular.dependentes[0]); x++) {
        if (strcmp(titular.dependentes[x].cpf, "") != 0) {
            idade = 365 * time.wYear + 30 * time.wMonth + time.wDay - 365 * titular.dependentes[x].dataNascimento.ano - 30 * titular.dependentes[x].dataNascimento.mes - titular.dependentes[x].dataNascimento.dia;
            idade /= 365;
            printf("%11s %-25s %4d %-4d\n",
                   titular.dependentes[x].cpf,
                   titular.dependentes[x].nome,
                   titular.dependentes[x].sexo,
                   idade);
        } else {
            break;
        }
    }
    linha();
    printf("\n\n");
    system("pause");
}

int procura(char cpf[11]) {
    posicao = 0;
    rewind(base_clientes);
    fread(&titular, sizeof(Titular), 1, base_clientes);
    while (feof(base_clientes) == 0) {
        if (strcmp(titular.cpf, cpf) == 0) {
            return posicao;
        } else {
            fread(&titular, sizeof(Titular), 1, base_clientes);
            posicao++;
        }
    }
    return -1;
}

void mostre(int posicao) {
    fseek(base_clientes, posicao * sizeof(Titular), SEEK_SET);
    fread(&titular, sizeof(Titular), 1, base_clientes);
    system("cls");
    cabecalho();
    linha();
    printf("CPF         Nome                      Sexo Fone        Email                               Idade Plano Dep.      Valor Venc.\n");
    linha();
    idade = 365 * time.wYear + 30 * time.wMonth + time.wDay - 365 * titular.dataNascimento.ano - 30 * titular.dataNascimento.mes - titular.dataNascimento.dia;
    idade /= 365;
    for (qtdDependentes = 0; qtdDependentes < sizeof(titular.dependentes) / sizeof(titular.dependentes[0]);) {
        if (strcmp(titular.dependentes[qtdDependentes].cpf, "") != 0) {
            qtdDependentes++;
        } else {
            break;
        }
    }
    printf("%11s %-25s %4d %-11s %-35s %5d %5d %-4d %10.2f %d/%d/%d\n",
           titular.cpf,
           titular.nome,
           titular.sexo,
           titular.telefone,
           titular.email,
           idade,
           titular.plano,
           qtdDependentes,
           titular.valorPlano,
           titular.dataVencimento.dia,
           titular.dataVencimento.mes,
           titular.dataVencimento.ano);
    linha();
}

void remover() {
    do {
        system("cls");
        cabecalho();
        linha();
        printf("\nRemover Cliente\n");
        linha();
        printf("\n\nCPF: ");
        scanf("%s", &cpf);
        posicao = procura(cpf);
        if (posicao == -1) {
            printf("\nCliente nao Encontrado!!\n");
        } else {
            mostre(posicao);
            printf("\n\nDeseja Remover o Cliente?\n1 - Sim\n0 - Nao\nResposta: ");
            scanf("%d", &confirmar);
            if (confirmar == 1) {
                fseek(base_clientes, posicao * sizeof(Titular), SEEK_SET);
                fwrite(&titular_aux, sizeof(Titular), 1, base_clientes);
                system("cls");
                cabecalho();
                printf("\n\nCliente removido com sucesso!\n");
            } else {
                system("cls");
                cabecalho();
                printf("\n\nRemocao Cancelada!\n");
            }
        }
        printf("\nDeseja Remover Outro?\n1 - Sim\n0 - Nao\nResposta: ");
        scanf("%d", &resposta);
    } while (resposta == 1);
}

void alterar() {
    do {
        system("cls");
        cabecalho();
        linha();
        printf("\nAlterar Nome do Cliente\n");
        linha();
        printf("\n\nCPF: ");
        scanf("%s", &cpf);
        posicao = procura(cpf);
        if (posicao == -1) {
            printf("\nCliente nao encontrado!!\n");
        } else {
            mostre(posicao);
            printf("\n\nAlterar o Nome do Cliente?\n1 - Sim \n0 - Nao\nResposta: ");
            scanf("%d", &confirmar);
            if (confirmar == 1) {
                printf("\nNovo Nome: ");
                fflush(stdin);
                gets(titular.nome);
                fseek(base_clientes, posicao * sizeof(Titular), SEEK_SET);
                fwrite(&titular, sizeof(Titular), 1, base_clientes);
                system("cls");
                mostre(posicao);
                printf("\nNome do Cliente Alterado com Sucesso!\n");
            } else {
                system("cls");
                cabecalho();
                printf("\n\nAlteracao Cancelada!\n");
            }
        }
        printf("\nDeseja Alterar Outro?\n1 - Sim \n0 - Nao\nResposta: ");
        scanf("%d", &resposta);
    } while (resposta == 1);
}

void listagemPorPlano() {
    system("cls");
    cabecalho();
    linha();
    printf("\nInsira o Plano Para a Consulta\n1 - Ouro\n2 - Diamante\n3 - Prata\n4 - Esmeralda\nEscolha: ");
    scanf("%d", &plano);
    system("cls");
    cabecalho();
    linha();
    printf("                                                         Listagem Por Plano\n");
    linha();
    printf("CPF         Nome                      Sexo Fone        Email                               Idade Plano Dep.      Valor Venc.\n");
    linha();
    rewind(base_clientes);
    fread(&titular_aux, sizeof(Titular), 1, base_clientes);
    while (feof(base_clientes) == 0) {
        if (titular_aux.sexo != 0) {
            if (titular_aux.plano == plano) {
                idade = 365 * time.wYear + 30 * time.wMonth + time.wDay - 365 * titular.dataNascimento.ano - 30 * titular.dataNascimento.mes - titular.dataNascimento.dia;
                idade /= 365;
                for (qtdDependentes = 0;
                     qtdDependentes < sizeof(titular_aux.dependentes) / sizeof(titular_aux.dependentes[0]);) {
                    if (strcmp(titular_aux.dependentes[qtdDependentes].cpf, "") != 0) {
                        qtdDependentes++;
                    } else {
                        break;
                    }
                }
                printf("%-11s %-25s %4d %-11s %-35s %5d %5d %-4d %10.2f %d/%d/%d\n",
                       titular_aux.cpf,
                       titular_aux.nome,
                       titular_aux.sexo,
                       titular_aux.telefone,
                       titular_aux.email,
                       idade,
                       titular_aux.plano,
                       qtdDependentes,
                       titular_aux.valorPlano,
                       titular_aux.dataVencimento.dia,
                       titular_aux.dataVencimento.mes,
                       titular_aux.dataVencimento.ano);
            }
        }
        fread(&titular_aux, sizeof(Titular), 1, base_clientes);
    }
    if (titular_aux.sexo == 0) {
        printf("\n\nNao Existem Clientes Cadastrados !\n");
    }
    linha();
    printf("\n\n");
    system("pause");
}

void listagemVencimento() {
    system("cls");
    cabecalho();
    linha();
    printf("                                         Listagem dos Vencimentos do Plano de Saude do Mes\n");
    linha();
    printf("CPF         Nome                      Sexo Fone        Email                               Idade Plano Dep.      Valor Venc.\n");
    linha();
    rewind(base_clientes);
    fread(&titular_aux, sizeof(Titular), 1, base_clientes);
    while (feof(base_clientes) == 0) {
        if (titular_aux.sexo != 0) {
            if (time.wMonth == titular_aux.dataVencimento.mes && time.wYear == titular_aux.dataVencimento.ano) {
                idade = 365 * time.wYear + 30 * time.wMonth + time.wDay - 365 * titular.dataNascimento.ano - 30 * titular.dataNascimento.mes - titular.dataNascimento.dia;
                idade /= 365;
                for (qtdDependentes = 0;
                     qtdDependentes < sizeof(titular_aux.dependentes) / sizeof(titular_aux.dependentes[0]);) {
                    if (strcmp(titular_aux.dependentes[qtdDependentes].cpf, "") != 0) {
                        qtdDependentes++;
                    } else {
                        break;
                    }
                }
                printf("%-11s %-25s %4d %-11s %-35s %5d %5d %-4d %10.2f %d/%d/%d\n",
                       titular_aux.cpf,
                       titular_aux.nome,
                       titular_aux.sexo,
                       titular_aux.telefone,
                       titular_aux.email,
                       idade,
                       titular_aux.plano,
                       qtdDependentes,
                       titular_aux.valorPlano,
                       titular_aux.dataVencimento.dia,
                       titular_aux.dataVencimento.mes,
                       titular_aux.dataVencimento.ano);
            }
        }
        fread(&titular_aux, sizeof(Titular), 1, base_clientes);
    }
    if (titular_aux.sexo == 0) {
        printf("\n\nNao Existem Clientes Cadastrados !\n");
    }
    linha();
    printf("\n\n");
    system("pause");
}