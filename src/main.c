/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/wait.h>

pthread_mutex_t trava;
int count = 0;

// Funcao que verifica se o numero recebido como parametro eh primo
// Se for, retorna 1, caso contrario retorna 0
int is_prime(long unsigned int num){
	if(num < 2)
		return 0;
	if(num > 1 && num < 4)
		return 1;
	for(unsigned int i = 2; i <= num/2; i++)
		if(num % i == 0)
			return 0;
	return 1;
}

void *thread(void *args){
	long unsigned int k = *((long unsigned int *)args);
	int a = is_prime(k);
	pthread_mutex_lock(&trava);
	count += a;
	pthread_mutex_unlock(&trava);
	return NULL;
}


int main() {
	int child = 0; // numero de filhos ativos
	
	long unsigned int c = 0;
	long unsigned int list[4] = {0, 0, 0, 0}; 
	pthread_t t[4] = {0, 0, 0, 0};
 	
	do{
		// Recebe 1 inteiro da entrada 
		scanf("%lu", &c);

		// Se os 4 filhos estiverem ocupadas, espera um deles acabar (e limpa todos os filhos que acabaram antes)
		if(child == 4){
			for(int i = 0; i < 4; i++){
				pthread_join(t[i], NULL);
				t[i] = 0;
				child--;
			}
				
		}
		
		int index = 0;
		//Cria uma nova thread para verificar se o numero eh primo
		for(int i = 0; i < 4; i++){
			if(t[i] == 0){
				index = i;
				break;
			}
		}
		
		child++;
		list[index] = c;
		pthread_create(&(t[index]), NULL, thread, ((void *) (&list[index])));


	}while(getchar() != '\n');
	
	// Espera que todas os processos filhos acabem
	for(int i = 0; i < 4; i++)
		pthread_join(t[i], NULL);	
	printf("%d\n", count);

	return 0;
}
