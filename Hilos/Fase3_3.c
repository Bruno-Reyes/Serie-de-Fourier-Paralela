#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/ipc.h>

#define RANGO 101
#define n_valores 10
#define pi 3.1415
#define MAX_THREADS 10

double matriz[RANGO][n_valores];
double x[RANGO];
double constante[RANGO];
int n[n_valores];


void mostrarMatriz(double m[RANGO][n_valores]);
void guardarComoCSV(double arreglo[], int longitud, const char* nombreArchivo);

void *termino_variable(void *argumentoN){
    int *n_valor = (int *) argumentoN;
    printf("Hilo con n= %d\n", *n_valor);   
    
    for(int i=0; i<RANGO; i++){
        matriz[i][*n_valor-2] = 2*(pow((-1), *n_valor+1))/((pow(*n_valor, 2))-1)*cos(*n_valor*x[i]);
    }
    pthread_exit(NULL);
}

void *termino_constante(){
    printf("Hilo para termino constante inicializado \n");
    double term_constante = 0;
    for(int i=0; i<RANGO; i++){
        constante[i] = 1 - ((0.5)*(cos(x[i])));
    }
}

int main(){
	// Vector n
	for(int i=0; i<n_valores; i++){
        n[i] = i+2;
    }
	
	// Vector x
    double aux = -10;
	for(int i=0; i<RANGO; i++){
		x[i] = aux;
		aux += 0.2;
	}

    // Vector constante
	for(int i=0; i<RANGO+1; i++){
		constante[i] = 0;
	}
	
	// Declaración de hilos
    pthread_t thread_id[MAX_THREADS];
    pthread_t thread_cte;
    
    // Creación de hilos 
    for(int i=0; i<n_valores; i++){
        pthread_create(&thread_id[i], NULL,termino_variable, &n[i]);
    }
    pthread_create(&thread_cte, NULL, termino_constante, NULL);

    // Método de espera para los hilos
    for(int i=0; i<n_valores; i++){
        pthread_join(thread_id[i], NULL);
    }
    pthread_join(thread_cte, NULL);
    
    // Mostramos la matriz resultante
    mostrarMatriz(matriz);
    
    //  Vector y
    double y[RANGO];

    for(int i=0; i<RANGO; i++){
        y[i] = 0;
    }

    for(int i=0; i<RANGO; i++){
        for(int j=0; j<n_valores; j++){
            y[i] += matriz[i][j];
        }
        y[i] += constante[i];
    }
    
    // Exportamos el vector y en un archivo
    int longitud = sizeof(y) / sizeof(y[0]);
    const char* nombreArchivo = "y3.csv";
    guardarComoCSV(y, longitud, nombreArchivo);
    return 0;
}

void mostrarMatriz(double m[RANGO][n_valores]){
	for(int i=0; i<RANGO; i++){
        for(int j=0; j<n_valores; j++){
            printf("%f ", m[i][j]);
        }
        printf("%f", constante[i]);
        printf("\n");
    }   	
}

void guardarComoCSV(double arreglo[], int longitud, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }
    
    for (int i = 0; i < longitud; i++) {
        fprintf(archivo, "%f", arreglo[i]);
        
        // Agrega una coma después de cada elemento, excepto el último
        if (i < longitud - 1) {
            fprintf(archivo, "\n");
        }
    }
    
    fclose(archivo);
    printf("El arreglo se ha guardado como CSV en el archivo: %s\n", nombreArchivo);
}
