#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//ranking
char mejorJugador[50] = "Lucho";
int mejorPuntaje = 9999999;

//punteros char e int)
void inicializarTablero(char *t, int n, int *fb, int *cb);
void mostrarTablero(char *t, int n);
int realizarDisparo(char *t, int n, int fb, int cb, int *f_tiro, int *c_tiro);

int main() {
	int n;
	int barcoF, barcoC, opcion, tiros, gano;
	char nombreActual[50];
	
	srand(time(NULL));
	
	do {
		printf("\n=== TP2 INFORMATICA II===\n");
		if (mejorPuntaje < 9999998) printf("Record: %s con %d tiros\n", mejorJugador, mejorPuntaje);
		
		printf("1. Iniciar Partida\n2. Salir\nSeleccione: ");
		scanf("%d", &opcion);
		
		if (opcion == 1) {
			printf("Tamaño del tablero: ");
			scanf("%d", &n);
			
			//tablero
			char *tablero = (char *)malloc(n * n * sizeof(char));
			
			//null para realloc
			int *historialFilas = NULL;
			int *historialCols = NULL;
			
			inicializarTablero(tablero, n, &barcoF, &barcoC);
			tiros = 0;
			gano = 0;
			
			while (tiros < (n * n) && !gano) {
				mostrarTablero(tablero, n);
				printf("Disparo actual: %d\n", tiros + 1);
				
				int f_actual, c_actual;
				int res = realizarDisparo(tablero, n, barcoF, barcoC, &f_actual, &c_actual);
				
				if (res != -1) {
					tiros++;
					
					//realloc para cada arreglo por separado, va creciendo segun los tiros
					historialFilas = (int *)realloc(historialFilas, tiros * sizeof(int));
					historialCols = (int *)realloc(historialCols, tiros * sizeof(int));
					
					historialFilas[tiros - 1] = f_actual;
					historialCols[tiros - 1] = c_actual;
					
					if (res == 1) gano = 1;
				}
			}
			
			mostrarTablero(tablero, n);
			
			if (gano) {
				printf("Treinta anos navegando he visto ese tiro exactamente... una vez. Y era yo. Asi que no te pongas muy comodo, entendiste? \nLe diste en %d canonazos.\n", tiros);
				printf("Dime tu nombre muchacho, esto debe ser recordado: ");
				scanf("%s", nombreActual);
				if (tiros < mejorPuntaje) {
					mejorPuntaje = tiros;
					strcpy(mejorJugador, nombreActual);
				}
			}
			
			//historial basado en los arreglos
			printf("\n--- SECUENCIA DE DISPAROS ---\n");
			for (int i = 0; i < tiros; i++) {
				printf("Tiro %d: Fila %d, Columna %d\n", i + 1, historialFilas[i], historialCols[i]);
			}
			
			//LIBERAR TODO
			free(tablero);
			free(historialFilas);
			free(historialCols);
		}
	} while (opcion != 2);
	
	return 0;
}

//aritmetica de punteros
void inicializarTablero(char *t, int n, int *fb, int *cb) {
	for (int i = 0; i < (n * n); i++) {
		*(t + i) = '.'; 
	}
	*fb = rand() % n;
	*cb = rand() % n;
}

void mostrarTablero(char *t, int n) {
	printf("\n    ");
	for (int i = 0; i < n; i++) printf("%2d ", i);
	printf("\n");
	
	for (int i = 0; i < n; i++) {
		printf("%2d |", i);
		for (int j = 0; j < n; j++) {
			// Aritmética de punteros: base + (fila * ancho) + columna
			printf(" %c ", *(t + (i * n) + j));
		}
		printf("\n");
	}
}
// 					(tablero, n,   barcoF, barcoC,  &f_actual,     &c_actual)
int realizarDisparo(char *t, int n, int fb, int cb, int *f_tiro, int *c_tiro) {
	int f, c;
	printf("Coordenadas:");
	printf("\nFila: ");
	scanf("%d", &f);
	printf("Columna: ");
	scanf("%d", &c);
	
	if (f < 0 || f >= n || c < 0 || c >= n) {
		printf("Tecnicamente... no fallaste. Para fallar, la bala tiene que haber tenido alguna posibilidad de dar. Lo tuyo fue otra cosa. Lo tuyo fue... arte.\n!\n");
		return -1;// -1 para decir invalido
	}
	
	if (*(t + (f * n) + c) != '.') {
		printf("Ya has disparado ahi, y ya has fallado ahi! Te perdonare la bala por esta vez recluta.\n");
		return -1;// same
	}
	
	*f_tiro = f;
	*c_tiro = c;
	
	if (f == fb && c == cb) {
		*(t + (f * n) + c) = 'B';
		return 1;//barco yipieeee
	} else {
		*(t + (f * n) + c) = 'A';
		printf("\nAGUA!\n");
		return 0;//agua
	}
}
