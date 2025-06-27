#include <stdio.h>
#include <string.h>
#include "funciones.h"

int main() {
    int opc;
    do {
        opc = menu();
        switch (opc) {
            case 1:
                inicializarInventario();
                break;
            case 2:
                realizarVenta();
                break;
            case 3: {
                int sub;
                printf("1. Mostrar Inventario\n");
                printf("2. Reabastecer Inventario\n>> ");
                scanf("%d", &sub);
                getchar();
                if (sub == 1)
                    mostrarInventario();
                else if (sub == 2)
                    reabastecerInventario();
                else
                    printf("Opción inválida.\n");
                break;
            }
            case 4:
                estadisticaVentas();
                break;
            case 5:
                mostrarClientes();
                break;
            case 6:
                buscarVentas();
                break;
            case 7:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opc != 7);
    return 0;
}