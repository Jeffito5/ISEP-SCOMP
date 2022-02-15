#include <stdio.h>
#include <string.h>
#include <unistd.h>	   
#include <sys/types.h> 
#include <stdlib.h>	   

#define TAMANHO_DATA 5



int main(){
	
	struct product{
		char nome[50];
		float preco;
		int cod_barras;
	};
	
	struct product base_dados_produtos[TAMANHO_DATA];
	strcpy(base_dados_produtos[0].nome, "Batata");
	base_dados_produtos[0].preco = 1.5;
	base_dados_produtos[0].cod_barras = 1;
	strcpy(base_dados_produtos[1].nome, "Camisinha");
	base_dados_produtos[1].preco = 1.99;
	base_dados_produtos[1].cod_barras = 2;
	strcpy(base_dados_produtos[2].nome, "Bolacha");
	base_dados_produtos[2].preco = 2.20;
	base_dados_produtos[2].cod_barras = 3;
	strcpy(base_dados_produtos[3].nome, "Pizza");
	base_dados_produtos[3].preco = 2.19;
	base_dados_produtos[3].cod_barras = 4;
	strcpy(base_dados_produtos[4].nome, "Sopa koka");
	base_dados_produtos[4].preco = 0.89;
	base_dados_produtos[4].cod_barras = 5;
	int fd[6][2], i, j, indice_, exst;
	pid_t pid;
	int cod_barras;
	
	for(i = 0; i < 6; i++){
		if(pipe(fd[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
	
	for(i = 0; i < 5; i++){
		pid = fork();
		if(pid == 0){
			close(fd[0][0]);
			close(fd[i+1][1]);
			printf("Leitos %d - Digite o codigo de barras para a analise (1, 2, 3, 4 ou 5):\n", i+1);
			scanf("%d", &cod_barras);
			write(fd[0][1], &cod_barras, sizeof(int));
			write(fd[0][1], &i, sizeof(int));
			struct product prod;
			read(fd[i+1][0], &prod, sizeof(struct product));
			printf("Leitor %d - Nome: %s\nPreco: %.2f£\n", i+1, prod.nome, prod.preco);
			close(fd[0][1]);
			close(fd[i+1][0]);
			exit(0);
		}
	}

	for(i = 0; i < 5; i++){
		close(fd[0][1]);
		close(fd[i+1][0]);
		read(fd[0][0], &cod_barras, sizeof(int));
		read(fd[0][0], &indice_, sizeof(int));
		exst = 0;
		
		for(j = 0; j < TAMANHO_DATA; j++){
			if(cod_barras == base_dados_produtos[j].cod_barras){
				write(fd[indice_+1][1], &base_dados_produtos[j], sizeof(struct product));
				exst = 1;
			}
		}
		
		if(exst == 0){
			struct product prod;
			strcpy(prod.nome, "produto não encontrado na base de dados.");
			prod.preco = 0;
			write(fd[indice_+1][1], &prod, sizeof(struct product));
		}
	}

	return 0;
}
