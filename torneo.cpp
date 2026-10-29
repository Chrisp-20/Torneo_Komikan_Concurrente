#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

#define BUFFER_SIZE 1024

int N;
int M;
int min_ms = 50;
int max_ms = 300;
int seed = 1;

// cola de jugadores que esperan (la misma idea que pc.c)
int cola[BUFFER_SIZE];
int front = 0, tail = 0, cant = 0;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_cola = PTHREAD_MUTEX_INITIALIZER;

// mesas
sem_t mesas;
pthread_mutex_t mutex_mesas = PTHREAD_MUTEX_INITIALIZER;
int *mesa_ocupada;
pthread_t *jueces;

struct Partida {
    int mesa;
    int jugador_puma;
    int jugador_perros;
};

void leer_config(string archivo) {
    ifstream f(archivo);
    if (!f) {
        cout << "No se pudo abrir " << archivo << endl;
        exit(1);
    }
    string nombre;
    int valor;
    while (f >> nombre >> valor) {
        if (nombre == "min_ms") {
            min_ms = valor;
        } else if (nombre == "max_ms") {
            max_ms = valor;
        } else if (nombre == "seed") {
            seed = valor;
        }
    }
}

void enqueue(int n) {
    cola[tail] = n;
    tail = (tail + 1) % BUFFER_SIZE;
    cant++;
}

int dequeue() {
    int val = cola[front];
    front = (front + 1) % BUFFER_SIZE;
    cant--;
    return val;
}

void llega_jugador(int id) {
    pthread_mutex_lock(&mutex_cola);
    while (cant == BUFFER_SIZE) {
        pthread_cond_wait(&not_full, &mutex_cola);
    }
    enqueue(id);
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mutex_cola);
}

int saca_jugador() {
    pthread_mutex_lock(&mutex_cola);
    while (cant == 0) {
        pthread_cond_wait(&not_empty, &mutex_cola);
    }
    int id = dequeue();
    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&mutex_cola);
    return id;
}

void *juez(void *arg) {
    Partida *p = (Partida *)arg;
    printf("Mesa %d: Puma=jugador %d, Perros=jugador %d\n",
           p->mesa, p->jugador_puma, p->jugador_perros);

    usleep(500000); // aqui va la partida de verdad

    printf("Mesa %d: termino\n", p->mesa);
    pthread_mutex_lock(&mutex_mesas);
    mesa_ocupada[p->mesa] = 0;
    pthread_mutex_unlock(&mutex_mesas);
    free(p);
    sem_post(&mesas);
    return NULL;
}

void *matchmaker(void *arg) {
    (void)arg;
    for (int k = 0; k < N / 2; k++) {
        int a = saca_jugador();
        int b = saca_jugador();

        sem_wait(&mesas); // si las M mesas estan ocupadas, espera aqui

        pthread_mutex_lock(&mutex_mesas);
        int m = 0;
        while (mesa_ocupada[m]) m++;
        mesa_ocupada[m] = 1;
        pthread_mutex_unlock(&mutex_mesas);

        Partida *p = (Partida *)malloc(sizeof(Partida));
        p->mesa = m;
        p->jugador_puma = a;
        p->jugador_perros = b;
        pthread_create(&jueces[k], NULL, juez, p);
    }
    return NULL;
}





int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 4) {
        cout << "Uso: ./torneo N M [config.txt]" << endl;
        return 1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    if (N <= 0 || M <= 0) {
        cout << "N y M deben ser mayores a 0" << endl;
        return 1;
    }

    if (argc == 4) {
        leer_config(argv[3]);
    }

    if (min_ms < 0 || min_ms > max_ms) {
        cout << "Retardos invalidos: min_ms debe ser >= 0 y <= max_ms" << endl;
        return 1;
    }

    srand(seed);

    cout << "Jugadores (N): " << N << endl;
    cout << "Mesas maximas (M): " << M << endl;
    cout << "Retardo: " << min_ms << " a " << max_ms << " ms" << endl;
    cout << "Seed: " << seed << endl;

    mesa_ocupada = (int *)calloc(M, sizeof(int));
    jueces = (pthread_t *)malloc((N / 2 + 1) * sizeof(pthread_t));
    sem_init(&mesas, 0, M);

    pthread_t mm;
    pthread_create(&mm, NULL, matchmaker, NULL);

    for (int i = 0; i < N; i++) {
        usleep((rand() % 20) * 1000); // los jugadores llegan de a poco
        llega_jugador(i);
    }

    pthread_join(mm, NULL);
    for (int k = 0; k < N / 2; k++) {
        pthread_join(jueces[k], NULL);
    }
    if (N % 2 == 1) {
        cout << "Quedo 1 jugador sin pareja" << endl;
    }

    sem_destroy(&mesas);
    free(mesa_ocupada);
    free(jueces);
    return 0;
}
