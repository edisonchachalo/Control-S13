struct Repuesto {
    int id;
    char nombre[30];
    int cantidad;
    float precio;
};

struct Cliente {
    char nombre[30];
    int cedula;
};

struct DetalleVenta {
    int idRepuesto;
    char nombre[30];
    int cantidad;
    float precioUnitario;
};

struct Venta {
    struct Cliente cliente;
    int numRepuestos;
    struct DetalleVenta repuestos[5];
    float totalVenta;
};

void leerCadena(char *cadena, int num);
int menu();

void inicializarInventario();
void mostrarInventario();
void reabastecerInventario();

void realizarVenta();
void guardarVenta(struct Venta nuevaVenta);
void guardarCliente(struct Cliente c);
int clienteRegistrado(int cedula);

void estadisticaVentas();
void mostrarClientes();
void buscarVentas();
