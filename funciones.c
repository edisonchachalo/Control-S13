#include <stdio.h>
#include <string.h>
#include "funciones.h"

void leerCadena(char *cadena, int num) {
    fflush(stdin);
    fgets(cadena, num, stdin);
    int len = strlen(cadena);
    if (len > 0 && cadena[len - 1] == '\n') {
        cadena[len - 1] = '\0';
    }
}

int menu() {
    int opc;
    printf("\n== GESTSYS UDLA ==\n");
    printf("1. Inventario Inicial\n");
    printf("2. Realizar Ventas\n");
    printf("3. Gestión Inventario\n");
    printf("4. Estadística de Ventas\n");
    printf("5. Listado de Clientes\n");
    printf("6. Buscar Ventas\n");
    printf("7. Salir\n");
    printf(">> ");
    scanf("%d", &opc);
    getchar();
    return opc;
}

void inicializarInventario() {
    FILE *f = fopen("inventario.dat", "rb+");
    struct Repuesto inventario[5];
    int cantidadActual = 0;

    if (f != NULL) {
        fread(inventario, sizeof(struct Repuesto), 5, f);
        fclose(f);
        for (int i = 0; i < 5; i++) {
            if (inventario[i].id != 0)
                cantidadActual++;
        }
        if (cantidadActual >= 5) {
            printf("El inventario ya fue inicializado. Para agregar más use la opción 3.\n");
            return;
        }
    }

    f = fopen("inventario.dat", "rb+");
    if (f == NULL) f = fopen("inventario.dat", "wb+");

    if (cantidadActual > 0)
        fread(inventario, sizeof(struct Repuesto), 5, f);

    int num;
    printf("Ingrese cuántos repuestos desea agregar (máx %d): ", 5 - cantidadActual);
    scanf("%d", &num);
    getchar();

    for (int i = cantidadActual; i < cantidadActual + num; i++) {
        printf("ID del repuesto: ");
        scanf("%d", &inventario[i].id);
        getchar();
        printf("Nombre del repuesto: ");
        leerCadena(inventario[i].nombre, 30);
        printf("Cantidad inicial: ");
        scanf("%d", &inventario[i].cantidad);
        printf("Precio: ");
        scanf("%f", &inventario[i].precio);
        getchar();
    }

    f = fopen("inventario.dat", "wb");
    fwrite(inventario, sizeof(struct Repuesto), 5, f);
    fclose(f);

    printf("Inventario inicializado correctamente.\n");
}

void mostrarInventario() {
    FILE *f = fopen("inventario.dat", "rb");
    if (f == NULL) {
        printf("Inventario no disponible.\n");
        return;
    }

    struct Repuesto repuestos[5];
    fread(repuestos, sizeof(struct Repuesto), 5, f);
    fclose(f);

    printf("#\tID\tRepuesto\t\tCantidad\n");
    for (int i = 0; i < 5; i++) {
        if (repuestos[i].id != 0)
            printf("%d\t%d\t%-20s%d\n", i + 1, repuestos[i].id, repuestos[i].nombre, repuestos[i].cantidad);
    }
}

void realizarVenta() {
    struct Repuesto inventario[5];
    FILE *finv = fopen("inventario.dat", "rb+");
    if (finv == NULL) {
        printf("No hay inventario inicializado.\n");
        return;
    }
    fread(inventario, sizeof(struct Repuesto), 5, finv);
    fclose(finv);

    struct Venta nuevaVenta;
    printf("Nombre del cliente: ");
    leerCadena(nuevaVenta.cliente.nombre, 30);
    printf("Cédula del cliente: ");
    scanf("%d", &nuevaVenta.cliente.cedula);
    getchar();

    mostrarInventario();
    printf("¿Cuántos productos desea comprar? ");
    scanf("%d", &nuevaVenta.numRepuestos);
    getchar();

    nuevaVenta.totalVenta = 0;
    for (int i = 0; i < nuevaVenta.numRepuestos; i++) {
        int id, cantidad;
        int encontrado = -1;
        printf("ID del repuesto #%d: ", i + 1);
        scanf("%d", &id);
        printf("Cantidad: ");
        scanf("%d", &cantidad);
        getchar();

        for (int j = 0; j < 5; j++) {
            if (inventario[j].id == id) {
                encontrado = j;
                break;
            }
        }

        if (encontrado == -1 || inventario[encontrado].cantidad < cantidad) {
            printf("No se puede vender el repuesto con ID %d. Inventario insuficiente.\n", id);
            i--;
            continue;
        }

        inventario[encontrado].cantidad -= cantidad;

        nuevaVenta.repuestos[i].idRepuesto = inventario[encontrado].id;
        strcpy(nuevaVenta.repuestos[i].nombre, inventario[encontrado].nombre);
        nuevaVenta.repuestos[i].cantidad = cantidad;
        nuevaVenta.repuestos[i].precioUnitario = inventario[encontrado].precio;

        nuevaVenta.totalVenta += cantidad * inventario[encontrado].precio;
    }

    // Guardar venta
    guardarVenta(nuevaVenta);

    // Guardar cliente si no existe
    if (!clienteRegistrado(nuevaVenta.cliente.cedula)) {
        guardarCliente(nuevaVenta.cliente);
    }

    // Actualizar inventario
    finv = fopen("inventario.dat", "wb");
    fwrite(inventario, sizeof(struct Repuesto), 5, finv);
    fclose(finv);

    printf("Venta realizada exitosamente. Total: %.2f\n", nuevaVenta.totalVenta);
}

void guardarVenta(struct Venta nuevaVenta) {
    FILE *f = fopen("ventas.dat", "ab");
    if (f != NULL) {
        fwrite(&nuevaVenta, sizeof(struct Venta), 1, f);
        fclose(f);
    }
}

void guardarCliente(struct Cliente c) {
    FILE *f = fopen("clientes.dat", "ab");
    if (f != NULL) {
        fwrite(&c, sizeof(struct Cliente), 1, f);
        fclose(f);
    }
}

int clienteRegistrado(int cedula) {
    FILE *f = fopen("clientes.dat", "rb");
    if (f == NULL) return 0;
    struct Cliente c;
    while (fread(&c, sizeof(struct Cliente), 1, f)) {
        if (c.cedula == cedula) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void reabastecerInventario() {
    struct Repuesto repuestos[5];
    FILE *f = fopen("inventario.dat", "rb+");
    if (f == NULL) {
        printf("No hay inventario disponible.\n");
        return;
    }

    fread(repuestos, sizeof(struct Repuesto), 5, f);
    fclose(f);

    mostrarInventario();
    int opc;
    printf("Seleccione el repuesto a reabastecer o 0 para reabastecer todos:\n>> ");
    scanf("%d", &opc);
    getchar();

    if (opc == 0) {
        for (int i = 0; i < 5; i++) {
            int add;
            printf("Cantidad para %s: ", repuestos[i].nombre);
            scanf("%d", &add);
            repuestos[i].cantidad += add;
        }
    } else {
        if (opc < 1 || opc > 5) {
            printf("Selección inválida.\n");
            return;
        }
        int add;
        printf("Cantidad para %s: ", repuestos[opc - 1].nombre);
        scanf("%d", &add);
        repuestos[opc - 1].cantidad += add;
    }

    f = fopen("inventario.dat", "wb");
    fwrite(repuestos, sizeof(struct Repuesto), 5, f);
    fclose(f);
    printf("Inventario actualizado correctamente.\n");
}

void estadisticaVentas() {
    FILE *f = fopen("ventas.dat", "rb");
    if (f == NULL) {
        printf("No hay ventas registradas.\n");
        return;
    }

    struct Venta v;
    int contador = 0;
    float totalGeneral = 0;
    struct DetalleVenta repuestosVendidos[50];
    int numRepuestos = 0;

    printf("\n#\tCliente\t\tTotalVenta\n");
    while (fread(&v, sizeof(struct Venta), 1, f)) {
        printf("%d\t%-15s%.2f\n", ++contador, v.cliente.nombre, v.totalVenta);
        totalGeneral += v.totalVenta;

        for (int i = 0; i < v.numRepuestos; i++) {
            int encontrado = -1;
            for (int j = 0; j < numRepuestos; j++) {
                if (repuestosVendidos[j].idRepuesto == v.repuestos[i].idRepuesto) {
                    encontrado = j;
                    break;
                }
            }
            if (encontrado != -1) {
                repuestosVendidos[encontrado].cantidad += v.repuestos[i].cantidad;
            } else {
                repuestosVendidos[numRepuestos] = v.repuestos[i];
                numRepuestos++;
            }
        }
    }
    fclose(f);

    // Buscar el repuesto más vendido
    int max = 0;
    int pos = -1;
    for (int i = 0; i < numRepuestos; i++) {
        if (repuestosVendidos[i].cantidad > max) {
            max = repuestosVendidos[i].cantidad;
            pos = i;
        }
    }

    if (pos != -1) {
        float totalRepuesto = repuestosVendidos[pos].cantidad * repuestosVendidos[pos].precioUnitario;
        printf("\nEl repuesto más vendido es \"%s\" con %d unidades vendidas. Total: $%.2f\n",
               repuestosVendidos[pos].nombre, repuestosVendidos[pos].cantidad, totalRepuesto);
    }

    printf("TOTAL DE VENTAS GENERAL: $%.2f\n", totalGeneral);
}

void buscarVentas() {
    printf("\n1. Buscar por producto\n");
    printf("2. Buscar por cliente\n>> ");
    int opc;
    scanf("%d", &opc);
    getchar();

    FILE *f = fopen("ventas.dat", "rb");
    if (f == NULL) {
        printf("No hay ventas registradas.\n");
        return;
    }

    struct Venta v;

    if (opc == 1) {
        int id;
        printf("Ingrese el ID del producto: ");
        scanf("%d", &id);
        getchar();
        int totalCant = 0;
        float totalPrecio = 0;
        char nombreProducto[30] = "";

        while (fread(&v, sizeof(struct Venta), 1, f)) {
            for (int i = 0; i < v.numRepuestos; i++) {
                if (v.repuestos[i].idRepuesto == id) {
                    if (strlen(nombreProducto) == 0)
                        strcpy(nombreProducto, v.repuestos[i].nombre);
                    totalCant += v.repuestos[i].cantidad;
                    totalPrecio += v.repuestos[i].cantidad * v.repuestos[i].precioUnitario;
                }
            }
        }

        if (totalCant > 0) {
            printf("\nProducto: %s\n", nombreProducto);
            printf("Cantidad total vendida: %d\n", totalCant);
            printf("Total recaudado: $%.2f\n", totalPrecio);
        } else {
            printf("No se encontraron ventas para ese ID.\n");
        }
    }

    else if (opc == 2) {
        int cedula;
        printf("Ingrese la cédula del cliente: ");
        scanf("%d", &cedula);
        getchar();

        int encontrado = 0;
        while (fread(&v, sizeof(struct Venta), 1, f)) {
            if (v.cliente.cedula == cedula) {
                encontrado = 1;
                printf("\nCliente: %s\nCédula: %d\n", v.cliente.nombre, v.cliente.cedula);
                for (int i = 0; i < v.numRepuestos; i++) {
                    printf("- %s  Cant: %d  Precio: %.2f\n",
                           v.repuestos[i].nombre,
                           v.repuestos[i].cantidad,
                           v.repuestos[i].precioUnitario);
                }
            }
        }
        if (!encontrado)
            printf("No se encontraron ventas para esa cédula.\n");
    }
    else {
        printf("Opción inválida.\n");
    }

    fclose(f);
}