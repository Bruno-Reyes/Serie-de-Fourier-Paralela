#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define RANGO 101
#define n_valores 12
#define pi 3.1415
#define MAX_THREADS 10

double matriz[RANGO][n_valores];
double x[RANGO];
int n[n_valores];

double constante();
void mostrarMatriz(double m[RANGO][n_valores]);
void guardarComoCSV(double arreglo[], int longitud, const char* nombreArchivo);

void *termino_variable(void *argumentoN){
    int *n_valor = (int *) argumentoN;
    printf("Hilo con n=%d\n", *n_valor);    
    for(int k=0; k<RANGO; k++){
        matriz[k][*n_valor-1] = ((2*pow(-1, *n_valor)-2)*cos((*n_valor)*x[k]))/(pi*pow(*n_valor,2));
    }
    pthread_exit(NULL);
}

int main(){

    for(int i = 0; i<n_valores; i++){
        n[i] = i+1;
    }
    // Creamos el vector de valores de x
    double aux = -10.0;
    double incremento = 0.2;
    for(int i=0; i<RANGO; i++){
        x[i] = aux;
        aux += incremento;
    }

    // Calcular el término constante
    double termino_constante = constante();

    // Declaracion de hilos
    pthread_t thread_id[MAX_THREADS];
    
    // Creacion de hilos 
    for(int i=0; i<n_valores; i++){
        pthread_create(&thread_id[i], NULL,termino_variable, &n[i]);
    }

    // Método de espera para los hilos
    for(int i=0; i<n_valores; i++){
        pthread_join(thread_id[i], NULL);
    }

    // Creamos el vector de salida   
    double y[RANGO];

    // Inicializamos el vector de salida con 0's
    for(int i=0; i<RANGO; i++){
        y[i] = 0;
    }  

    // Sumamos todas las filas y almacenamos el resultado en la lista "y"
    for(int i=0; i<RANGO; i++){
        for(int j=0; j<n_valores; j++){
            y[i] += matriz[i][j];
        }
        y[i] += termino_constante;
    }
    
    
    printf("\n");

    // Mostramos la matriz resultante
    mostrarMatriz(matriz);

    // Guardamos los datos en un archivo para su visualización 
    int longitud = sizeof(y) / sizeof(y[0]);
    const char* nombreArchivo = "y1.csv";
    guardarComoCSV(y, longitud, nombreArchivo);

    return 0;
}

double constante(){
    return pi/2;
}

void mostrarMatriz(double m[RANGO][n_valores]){
    for(int i=0; i<RANGO; i++){
        for(int j=0; j<n_valores; j++){
            printf("%f ", m[i][j]);
        }
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
