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
    printf("3. Gestion Inventario\n");
    printf("4. Estadistica de Ventas\n");
    printf("5. Listado de Clientes\n");
    printf("6. Buscar Ventas\n");
    printf("7. Salir\n");
    printf(">> ");
    scanf("%d", &opc);
    getchar();
    return opc;
}

void inicializarInventario() {
    struct Repuesto inventario[5];
    int cantidadActual = 0;

    // Inicializar array con ceros primero
    for (int i = 0; i < 5; i++) {
        strcpy(inventario[i].id, "");  // Inicializa ID como cadena vacía
        inventario[i].cantidad = 0;
        inventario[i].precio = 0.0;
        strcpy(inventario[i].nombre, "");
    }

    // Verificar si ya existe inventario
    FILE *f = fopen("inventario.dat", "rb");
    if (f != NULL) {
        fread(inventario, sizeof(struct Repuesto), 5, f);
        fclose(f);
        
        // Contar elementos existentes
        for (int i = 0; i < 5; i++) {
            if (strlen(inventario[i].id) > 0)  // Verifica si hay un ID no vacío
                cantidadActual++;
        }
        
        if (cantidadActual >= 5) {
            printf("El inventario ya fue inicializado completamente. Para modificar use la opción 3.\n");
            return;
        }
    }

    int num;
    int espaciosDisponibles = 5 - cantidadActual;
    printf("Ingrese cuantos repuestos desea agregar (max %d): ", espaciosDisponibles);
    scanf("%d", &num);
    
    if (num > espaciosDisponibles) {
        num = espaciosDisponibles;
        printf("Se limitará a %d repuestos por espacio disponible.\n", num);
    }
    
    getchar();

    // Buscar los primeros espacios vacíos para agregar nuevos repuestos
    int agregados = 0;
    for (int i = 0; i < 5 && agregados < num; i++) {
        if (strlen(inventario[i].id) == 0) {  // Si el ID está vacío
            printf("\n--- Repuesto %d ---\n", agregados + 1);
            printf("ID del repuesto: ");
            leerCadena(inventario[i].id, 10);  // Usa leerCadena en vez de scanf
            printf("Nombre del repuesto: ");
            leerCadena(inventario[i].nombre, 30);
            printf("Cantidad inicial: ");
            scanf("%d", &inventario[i].cantidad);
            printf("Precio: ");
            scanf("%f", &inventario[i].precio);
            getchar();
            agregados++;
        }
    }

    // Guardar todo el inventario
    f = fopen("inventario.dat", "wb");
    fwrite(inventario, sizeof(struct Repuesto), 5, f);
    fclose(f);

    printf("Inventario actualizado correctamente. Se agregaron %d repuestos.\n", agregados);
}

void mostrarInventario() {
    FILE *f = fopen("inventario.dat", "rb");
    if (f == NULL) {
        printf("Inventario no disponible. Debe inicializar primero.\n");
        return;
    }

    struct Repuesto repuestos[5];
    
    // Inicializar array
    for (int i = 0; i < 5; i++) {
        strcpy(repuestos[i].id, "");  // ID como cadena vacía
        repuestos[i].cantidad = 0;
        repuestos[i].precio = 0.0;
        strcpy(repuestos[i].nombre, "");
    }
    
    int leidos = fread(repuestos, sizeof(struct Repuesto), 5, f);
    fclose(f);
    
    if (leidos == 0) {
        printf("Archivo de inventario vacío o corrupto.\n");
        return;
    }

    printf("\n=== INVENTARIO ===\n");
    printf("#\tID\tRepuesto\t\tCantidad\tPrecio\n");
    printf("---------------------------------------------------------------\n");
    
    int contador = 0;
    for (int i = 0; i < 5; i++) {
        // Mostrar solo si el ID no está vacío
        if (strlen(repuestos[i].id) > 0) {
            contador++;
            printf("%d\t%s\t%-20s%d\t\t%.2f\n", 
                   contador, 
                   repuestos[i].id, 
                   repuestos[i].nombre, 
                   repuestos[i].cantidad,
                   repuestos[i].precio);
        }
    }
    
    if (contador == 0) {
        printf("No hay repuestos en el inventario.\n");
    }
}

void realizarVenta() {
    struct Repuesto inventario[5];
    
    // Inicializar array
    for (int i = 0; i < 5; i++) {
        strcpy(inventario[i].id, "");  // ID como cadena vacía
        inventario[i].cantidad = 0;
        inventario[i].precio = 0.0;
        strcpy(inventario[i].nombre, "");
    }
    
    FILE *finv = fopen("inventario.dat", "rb");
    if (finv == NULL) {
        printf("No hay inventario inicializado.\n");
        return;
    }
    
    int leidos = fread(inventario, sizeof(struct Repuesto), 5, finv);
    fclose(finv);
    
    if (leidos == 0) {
        printf("Error al leer el inventario.\n");
        return;
    }
    
    // Verificar si hay productos en el inventario
    int hayProductos = 0;
    for (int i = 0; i < 5; i++) {
        if (strlen(inventario[i].id) > 0) {  // Si hay ID
            hayProductos = 1;
            break;
        }
    }
    
    if (!hayProductos) {
        printf("No hay productos disponibles en el inventario.\n");
        return;
    }

    struct Venta nuevaVenta;
    printf("Nombre del cliente: ");
    leerCadena(nuevaVenta.cliente.nombre, 30);
    printf("Cedula del cliente: ");
    scanf("%d", &nuevaVenta.cliente.cedula);
    getchar(); // Limpiar buffer

    // Mostrar inventario para que el cliente pueda ver lo que hay
    mostrarInventario();
    
    int validoNumRepuestos = 0;
    do {
        printf("Cuantos productos desea comprar: ");
        if (scanf("%d", &nuevaVenta.numRepuestos) != 1) {
            printf("Entrada inválida. Ingrese un número.\n");
            while(getchar() != '\n'); // Limpiar buffer
        } else if (nuevaVenta.numRepuestos <= 0) {
            printf("Cantidad debe ser mayor a cero.\n");
        } else {
            validoNumRepuestos = 1;
        }
    } while (!validoNumRepuestos);
    
    getchar(); // Limpiar buffer

    nuevaVenta.totalVenta = 0;
    for (int i = 0; i < nuevaVenta.numRepuestos; i++) {
        char id[10];
        int cantidad = 0;
        int encontrado = -1;
        
        // Obtener ID del repuesto
        printf("ID del repuesto #%d: ", i + 1);
        leerCadena(id, 10);  // Usa leerCadena para ID alfanumérico
        
        // Obtener cantidad con validación
        int entradaValida = 0;
        do {
            printf("Cantidad: ");
            if (scanf("%d", &cantidad) != 1) {
                printf("Entrada inválida. Ingrese una cantidad numérica.\n");
                while(getchar() != '\n'); // Limpiar buffer
            } else if (cantidad <= 0) {
                printf("La cantidad debe ser mayor a cero.\n");
            } else {
                entradaValida = 1;
            }
        } while (!entradaValida);
        
        getchar(); // Limpiar buffer

        // Buscar el repuesto en el inventario por ID (ahora como string)
        for (int j = 0; j < 5; j++) {
            if (strcmp(inventario[j].id, id) == 0) {
                encontrado = j;
                break;
            }
        }

        if (encontrado == -1) {
            printf("No se encontró el repuesto con ID %s.\n", id);
            i--;
            continue;
        }
        
        if (inventario[encontrado].cantidad < cantidad) {
            printf("No hay suficiente inventario. Disponible: %d unidades de %s.\n", 
                   inventario[encontrado].cantidad, inventario[encontrado].nombre);
            i--;
            continue;
        }

        inventario[encontrado].cantidad -= cantidad;

        strcpy(nuevaVenta.repuestos[i].idRepuesto, inventario[encontrado].id);
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
    
    // Inicializar array
    for (int i = 0; i < 5; i++) {
        repuestos[i].id[0] = '\0';  // ID como cadena vacía
        repuestos[i].cantidad = 0;
        repuestos[i].precio = 0.0;
        strcpy(repuestos[i].nombre, "");
    }
    
    FILE *f = fopen("inventario.dat", "rb");
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
            if (repuestos[i].id[0] != '\0') {
                int add;
                printf("Cantidad para %s: ", repuestos[i].nombre);
                scanf("%d", &add);
                repuestos[i].cantidad += add;
            }
        }
    } else {
        // Encontrar el repuesto por su posición visual
        int contador = 0;
        int indiceReal = -1;
        for (int i = 0; i < 5; i++) {
            if (repuestos[i].id[0] != '\0') {
                contador++;
                if (contador == opc) {
                    indiceReal = i;
                    break;
                }
            }
        }
        
        if (indiceReal == -1) {
            printf("Selección inválida.\n");
            return;
        }
        
        int add;
        printf("Cantidad para %s: ", repuestos[indiceReal].nombre);
        scanf("%d", &add);
        repuestos[indiceReal].cantidad += add;
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
        char id[10];
        printf("Ingrese el ID del producto: ");
        leerCadena(id, 10);  // Usa leerCadena para ID alfanumérico
        
        int totalCant = 0;
        float totalPrecio = 0;
        char nombreProducto[30] = "";

        while (fread(&v, sizeof(struct Venta), 1, f)) {
            for (int i = 0; i < v.numRepuestos; i++) {
                if (strcmp(v.repuestos[i].idRepuesto, id) == 0) {  // Compara strings
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

void mostrarClientes() {
    FILE *f = fopen("clientes.dat", "rb");
    if (f == NULL) {
        printf("No hay clientes registrados.\n");
        return;
    }

    struct Cliente cliente;
    int contador = 0;
    
    printf("\n=== LISTADO DE CLIENTES ===\n");
    printf("#\tCédula\t\tNombre\n");
    printf("-----------------------------------\n");
    
    while (fread(&cliente, sizeof(struct Cliente), 1, f)) {
        printf("%d\t%d\t\t%s\n", ++contador, cliente.cedula, cliente.nombre);
    }
    
    fclose(f);
    
    if (contador == 0) {
        printf("No hay clientes registrados.\n");
    } else {
        printf("\nTotal de clientes: %d\n", contador);
    }
}