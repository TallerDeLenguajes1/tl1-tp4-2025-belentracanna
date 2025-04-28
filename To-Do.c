#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tarea {
    int TareaID;//Numérico autoincremental comenzando en 1000
    char *Descripcion;
    int Duracion; // entre 10 – 100
} Tarea;

typedef struct Nodo {
    Tarea T;
    struct Nodo *Siguiente;
} Nodo;

// Variable global para el ID de la próxima tarea
int nextTareaID = 1000;

/*------------------- funciones -----------------------------------*/
Nodo *CrearListaVacia();
Nodo *CrearNodoTarea(char *descripcion, int duracion);
void InsertarNodoTarea(Nodo **lista, Nodo *nuevoNodo);
void MostrarListaTareas(Nodo *lista, char *nombreLista);
Nodo *BuscarNodoTareaPorID(Nodo *lista, int tareaID);
void MoverTareaRealizada(Nodo **pendientes, Nodo **realizadas, int tareaID);
void LiberarListaTareas(Nodo *lista);

int main() {
    Nodo *listaPendientes = CrearListaVacia();
    Nodo *listaRealizadas = CrearListaVacia();
    char respuesta;

    printf("--- Carga de Tareas Pendientes ---\n");

    do {
        char descripcion[100];
        int duracion;

        printf("\nIngrese la descripción de la tarea: ");
        fgets(descripcion, sizeof(descripcion), stdin);
        descripcion[strcspn(descripcion, "\n")] = 0;

        printf("Ingrese la duración de la tarea (10-100): ");
        if (scanf("%d", &duracion) != 1 || duracion < 10 || duracion > 100) {
            printf("Duración inválida.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        Nodo *nuevaTareaNodo = CrearNodoTarea(descripcion, duracion);
        if (nuevaTareaNodo != NULL) {
            InsertarNodoTarea(&listaPendientes, nuevaTareaNodo);
        }

        printf("\n¿Desea ingresar otra tarea? (s/n): ");
        scanf("%c", &respuesta);
        while (getchar() != '\n');

    } while (respuesta == 's' || respuesta == 'S');

    printf("\n--- Lista de Tareas Pendientes Inicial ---\n");
    MostrarListaTareas(listaPendientes, "Tareas Pendientes");
    printf("\n--- Lista de Tareas Realizadas Inicial ---\n");
    MostrarListaTareas(listaRealizadas, "Tareas Realizadas");

    int idTareaRealizada;
    printf("\nIngrese el ID de la tarea que desea marcar como realizada: ");
    if (scanf("%d", &idTareaRealizada) == 1) {
        MoverTareaRealizada(&listaPendientes, &listaRealizadas, idTareaRealizada);
    } else {
        printf("Entrada invalida para el ID.\n");
        while (getchar() != '\n'); // obtener un caracter
    }

    printf("\n--- Lista de Tareas Pendientes Actualizada ---\n");
    MostrarListaTareas(listaPendientes, "Tareas Pendientes");
    printf("\n--- Lista de Tareas Realizadas Actualizada ---\n");
    MostrarListaTareas(listaRealizadas, "Tareas Realizadas");

    LiberarListaTareas(listaPendientes);
    LiberarListaTareas(listaRealizadas);

    return 0;
}

Nodo *CrearListaVacia() {
    return NULL;
}

Nodo *CrearNodoTarea(char *descripcion, int duracion) {
    Nodo *nuevoNodo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevoNodo == NULL) {
        printf("Error al asignar memoria para el nodo.\n");
        return NULL;
    }

    Tarea *nuevaTarea = (Tarea *)malloc(sizeof(Tarea));
    if (nuevaTarea == NULL) {
        printf("Error al asignar memoria para la tarea.\n");
        free(nuevoNodo);
        return NULL;
    }

    nuevaTarea->TareaID = nextTareaID++;
    nuevaTarea->Descripcion = (char *)malloc(strlen(descripcion) + 1);
    //nuevaTarea->Descripcion = strdup(descripcion);
    if (nuevaTarea->Descripcion == NULL) {
        printf("Error al asignar memoria para la descripción.\n");
        free(nuevaTarea);
        free(nuevoNodo);
        return NULL;
    }
    nuevaTarea->Duracion = duracion;

    nuevoNodo->T = *nuevaTarea;
    nuevoNodo->Siguiente = NULL;

    printf("Tarea con ID %d creada.\n", nuevaTarea->TareaID);

    return nuevoNodo;
}

void InsertarNodoTarea(Nodo **lista, Nodo *nuevoNodo) {
    if (nuevoNodo != NULL) {
        nuevoNodo->Siguiente = *lista;
        *lista = nuevoNodo;
        printf("Tarea insertada en la lista.\n");
    }
}

void MostrarListaTareas(Nodo *lista, char *nombreLista) {
    printf("--- %s ---\n", nombreLista);
    Nodo *actual = lista;
    if (actual == NULL) {
        printf("La lista está vacía.\n");
        return;
    }
    while (actual != NULL) {
        printf("ID: %d, Descripción: %s, Duración: %d minutos\n",
               actual->T.TareaID, actual->T.Descripcion, actual->T.Duracion);
        actual = actual->Siguiente;
    }
}

Nodo *BuscarNodoTareaPorID(Nodo *lista, int tareaID) {
    Nodo *actual = lista;
    while (actual != NULL) {
        if (actual->T.TareaID == tareaID) {
            return actual;
        }
        actual = actual->Siguiente;
    }
    return NULL;
}

void MoverTareaRealizada(Nodo **pendientes, Nodo **realizadas, int tareaID) {
    Nodo *nodoAMover = NULL;
    Nodo *anterior = NULL;
    Nodo *actualPendiente = *pendientes;

    // Buscar el nodo en la lista de pendientes
    while (actualPendiente != NULL) {
        if (actualPendiente->T.TareaID == tareaID) {
            nodoAMover = actualPendiente;
            break;
        }
        anterior = actualPendiente;
        actualPendiente = actualPendiente->Siguiente;
    }

    if (nodoAMover != NULL) {
        // Desconectar el nodo de la lista de pendientes
        if (anterior == NULL) {
            *pendientes = nodoAMover->Siguiente;
        } else {
            anterior->Siguiente = nodoAMover->Siguiente;
        }

        // Insertar el nodo al principio de la lista de realizadas
        nodoAMover->Siguiente = *realizadas;
        *realizadas = nodoAMover;
        printf("Tarea con ID %d movida a la lista de realizadas.\n", tareaID);
    } else {
        printf("No se encontró la tarea con ID %d en la lista de pendientes.\n", tareaID);
    }
}

void LiberarListaTareas(Nodo *lista) {
    Nodo *actual = lista;
    Nodo *siguiente;
    while (actual != NULL) {
        siguiente = actual->Siguiente;
        free(actual->T.Descripcion);
        free(actual);
        actual = siguiente;
    }
}