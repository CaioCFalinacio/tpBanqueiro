#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

/* Constantes definidas no roteiro do trabalho */
#define NUMBER_OF_CUSTOMERS 5  
#define NUMBER_OF_RESOURCES 3

 // Montante disponível de cada recurso 
int available[NUMBER_OF_RESOURCES];  
// Demanda máxima de cada cliente                    
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];  
// Montante correntemente alocado a cada cliente        
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];   
// Necessidade remanescente de cada cliente    
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];  

/* Mutex para evitar condição de corrida */
pthread_mutex_t banco_mutex;

/* Protótipos exigidos pelo trabalho */
int request_resources(int customer_num, int request[]); 
int release_resources(int customer_num, int release[]); 

/* Protótipos auxiliares */

// Para isolar a lógica do algoritmo do banqueiro
bool is_safe(); 
// Comportamento das thread
void* customer_thread(void* customer_id); 

int main(int argc, char *argv[]){

    // TODO: Validar e ler os argumentos da linha de comando (argv) para preencher 'available'
    
    // TODO: Inicializar matrizes maximum, allocation e need
    
    // TODO: Inicializar o mutex
    
    // TODO: Criar as N threads

    return 0;
}

bool is_safe() {
    //Inicializando os vetores Trabalho (work) e Término (finish)
    int work[NUMBER_OF_RESOURCES];
    bool finish[NUMBER_OF_CUSTOMERS];

    // Trabalho = Disponível
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        work[i] = available[i];
    }

    // Término[i] = false para todo i
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        finish[i] = false;
    }

    // Encontrando uma sequência segura
    int count = 0; // Conta quantos clientes conseguiram terminar
    while (count < NUMBER_OF_CUSTOMERS) {
        bool found = false; // Flag para verificar se foi encontrado um cliente nesta rodada

        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish[i]) {
                // Necessidade_i <= Trabalho
                bool can_allocate = true;
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = false;
                        break; // Se um recurso não for suficiente, já quebra o laço
                    }
                }

                // Se puder alocar (Passo 3)
                if (can_allocate) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                        work[j] += allocation[i][j]; // Trabalho = Trabalho + Alocação_i
                    }
                    finish[i] = true; // Término[i] = true
                    found = true;
                    count++;
                }
            }
        }

        // Se passar por todos os clientes e não achar ninguém que possa terminar, o laço é quebrado.
        if (!found) {
            break;
        }
    }

    // Se Término[i] == true para todo i, então o estado é seguro
    // Como usamos a variável 'count', basta checar se ela chegou ao total de clientes
    if (count == NUMBER_OF_CUSTOMERS) {
        return true;  // Estado Seguro
    } else {
        return false; // Estado Inseguro
    }
}

int request_resources(int customer_num, int request[]){
    pthread_mutex_lock(&banco_mutex);

    // Verificar se o cliente está pedindo mais do que ele disse que precisaria (need)
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num][i]) {
            // O cliente excedeu sua demanda máxima
            pthread_mutex_unlock(&banco_mutex);
            return -1; // Nega o pedido
        }
    }

    // Verificar se o banco tem os recursos disponíveis no momento
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > available[i]) {
            // Os recursos não estão disponíveis; o cliente deve esperar
            pthread_mutex_unlock(&banco_mutex);
            return -1; // Nega o pedido
        }
    }

    // Fingir a alocação dos recursos para testar a segurança
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    // Rodar o Algoritmo de Segurança com o estado alterado
    if (is_safe()) {
        // O estado é seguro.
        pthread_mutex_unlock(&banco_mutex);
        return 0; 
    } else {
        // O estado é inseguro. 
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        
        // Desbloqueamos o mutex e negamos o pedido
        pthread_mutex_unlock(&banco_mutex);
        return -1; // Falha [cite: 33]
    }
}

