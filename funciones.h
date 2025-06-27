#ifndef FUNCIONES_H
#define FUNCIONES_H

struct Cliente {
    char nombre[30];
    int cedula;
};

struct Repuesto {
    char id[10];  // Cambiado de int a char[10]
    char nombre[30];
    int cantidad;
    float precio;
};

struct DetalleVenta {
    char idRepuesto[10];  // Cambiado de int a char[10]
    char nombre[30];
    int cantidad;
    float precioUnitario;
};

struct Venta {
    struct Cliente cliente;
    struct DetalleVenta repuestos[10];
    int numRepuestos;
    float totalVenta;
};

void leerCadena(char *cadena, int num);
int menu();
void inicializarInventario();
void mostrarInventario();
void realizarVenta();
void guardarVenta(struct Venta);
void guardarCliente(struct Cliente);
int clienteRegistrado(int);
void reabastecerInventario();
void estadisticaVentas();
void buscarVentas();
void mostrarClientes();

#endif
