#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

int swap_Endians(int valor);

float hits = 0;
float miss_compulsorio = 0;
float miss_conflito = 0;
float miss_capacidade = 0;
float total_misses = 0;

float num_acessos;
float taxa_hits;
float taxa_miss;
float taxa_miss_compusorio;
float taxa_miss_capacidade;
float taxa_miss_conflito;

int main(int argc, char *argv[]) {
    
    if (argc != 7) {
        printf("Numero de argumentos incorreto. Utilize:\n");
        printf("./cache_simulator <nsets> <bsize> <assoc> <substituicao> <flag_saida> arquivo_de_entrada\n");
        exit(EXIT_FAILURE);
    }
    int nsets = atoi(argv[1]);
    int bsize = atoi(argv[2]);
    int assoc = atoi(argv[3]);
    char *subst = argv[4];
    int flagOut = atoi(argv[5]);
    char *arquivoEntrada = argv[6];
    
    int i,j;
    int via;
    int flag_deu_hit = 0;
    int nblocos_restantes = nsets * assoc; 
    int endereco;
    int tag;
    int indice;
    int cache_val[nsets][assoc];
    int cache_tag[nsets][assoc];
    int ordem_substituicao[nsets][assoc]; 
    int menor_via;
    srand(time(0));

    FILE *arquivo = fopen(arquivoEntrada, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int *buffer = (int*)malloc(sizeof(int) * 1000000); 
    if(buffer == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(EXIT_FAILURE);
    }  
     
    int tamanho_arquivo = fread(buffer,4,1000000,arquivo);//funciona com arquivos de até 1m de endereços

    for (i = 0; i < nsets; i++) {
        for (j = 0; j < assoc; j++) {
            cache_val[i][j] = 0; 
            cache_tag[i][j] = 0;
            ordem_substituicao[i][j] = 0; 
        }
    }

    for (i = 0; i < tamanho_arquivo; i++) {
        flag_deu_hit = 0;
        endereco = (buffer[i]);

        endereco = swap_Endians(endereco);
        tag = endereco / bsize; 
        indice = (endereco / bsize) % nsets;

        if (assoc == 1) { 
            if (cache_val[indice][0] == 0) {
                miss_compulsorio++;
                total_misses++;
                cache_val[indice][0] = 1;
                cache_tag[indice][0] = tag;
            } else {    
                if (cache_tag[indice][0] == tag) {
                    hits++;
                    flag_deu_hit = 1;
                } else {
                    miss_conflito++;
                    total_misses++;
                    cache_val[indice][0] = 1;
                    cache_tag[indice][0] = tag;
                }
            }
        } else { 
            for (j = 0; j < assoc; j++) {
                if ((cache_val[indice][j] == 1) && (cache_tag[indice][j] == tag)) {
                    hits++;
                    flag_deu_hit = 1;

                   if (strcmp(subst, "L") == 0)
                   ordem_substituicao[indice][j] = i + 1; 

                   break; 
                }
            }

            if (flag_deu_hit == 0) { 
                int via_vazia = -1;
                for (j = 0; j < assoc; j++) {
                    if (cache_val[indice][j] == 0) {
                        via_vazia = j;
                        break;
                    }
                }

                if (via_vazia != -1) { 
                    miss_compulsorio++;
                    total_misses++;
                    nblocos_restantes--;
                    cache_val[indice][via_vazia] = 1;
                    cache_tag[indice][via_vazia] = tag;
                    ordem_substituicao[indice][via_vazia] = i + 1; 
                } else { 
                    if(nblocos_restantes > 0) {
                        miss_conflito ++; 
                    } else  {
                        miss_capacidade ++;
                    }

                    total_misses++;
                    
                    if (strcmp(subst, "R") == 0) { 
                        via = rand() % assoc;
                        cache_tag[indice][via] = tag;
                     
                    } else if (strcmp(subst, "L") == 0) {
                        menor_via = 0;
                        for (j = 1; j < assoc; j++) {
                            if (ordem_substituicao[indice][j] < ordem_substituicao[indice][menor_via]) {
                                menor_via = j;
                            }
                        }

                        cache_tag[indice][menor_via] = tag;
                        ordem_substituicao[indice][menor_via] = i + 1; 
                        
                    } else if (strcmp(subst, "F") == 0) {
                        menor_via = 0;
                        for (j = 1; j < assoc; j++) {
                            if (ordem_substituicao[indice][j] < ordem_substituicao[indice][menor_via]) {
                                menor_via = j;
                            }
                        }

                        cache_tag[indice][menor_via] = tag;
                        ordem_substituicao[indice][menor_via] = i + 1; 
                        
                    }                   
                }
            }
        }
    }

  
    num_acessos = tamanho_arquivo;
    taxa_hits = hits/num_acessos;
    taxa_miss = total_misses/num_acessos;
    taxa_miss_compusorio = miss_compulsorio/total_misses;
    taxa_miss_capacidade = miss_capacidade/total_misses;
    taxa_miss_conflito = miss_conflito/total_misses;

    if(flagOut == 0){
        printf("nsets = %d\n", nsets);
        printf("bsize = %d\n", bsize);
        printf("assoc = %d\n", assoc);
        printf("subst = %s\n", subst);
        printf("flagOut = %d\n", flagOut);
        printf("arquivo = %s\n\n", arquivoEntrada);
        
        printf("bits do offset = %.0f\n", (log2(bsize)));
        printf("bits do indice = %.0f\n", (log2(nsets)));
        printf("bits da tag = %.0f\n", (32-log2(bsize)-log2(nsets)));
        printf("Assoc: %d\n", assoc);
        printf("Total de hits: %.0f\n", hits);
        printf("Total de misses: %.0f\n", total_misses);
        printf("Total de misses compulsorios: %.0f\n", miss_compulsorio);
        printf("Total de misses capacidade: %.0f\n", miss_capacidade);
        printf("Total de misses conflito: %.0f\n", miss_conflito);
        
        printf("Numero de acessos: %.0f\n",num_acessos);
        printf("Taxa de hits: %.2f%%\n", taxa_hits*100);
        printf("Taxa de misses: %.2f%%\n", taxa_miss*100);
        printf("Taxa de miss compulsorio: %.2f%%\n", taxa_miss_compusorio*100);
        printf("Taxa de miss de capacidade: %.2f%%\n", taxa_miss_capacidade*100);
        printf("Taxa de miss compulsorio: %.2f%%\n", taxa_miss_conflito*100);
        
    } else{ 
        printf("%.0f,%.2f,%.2f,%.2f,%.2f,%.2f\n",num_acessos, taxa_hits, taxa_miss, taxa_miss_compusorio, taxa_miss_capacidade, taxa_miss_conflito);
    }

    return 0;
}

int swap_Endians(int valor) {
    int esquerda;
    int meio_esquerda;
    int meio_direita;
    int direita;
    int resultado;

    esquerda = (valor & 0x000000FF) >> 0;
    meio_esquerda = (valor & 0x0000FF00) >> 8;
    meio_direita = (valor & 0x00FF0000) >> 16;
    direita = (valor & 0xFF000000) >> 24;
    
    esquerda <<= 24;
    meio_esquerda <<= 16;
    meio_direita <<= 8;
    direita <<= 0;

    resultado = (esquerda | meio_esquerda | meio_direita | direita);

    return resultado;
}