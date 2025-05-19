#include<iostream>
#include"file.h"

int main() {
    string nombreArchivo = "data/ventas_sudamerica.csv";
    vector<Venta> ventas = leerVentas(nombreArchivo);

    for (const Venta& v : ventas) {
        cout << "ID: " << v.id << ", Cliente: " << v.cliente
             << ", País: " << v.pais << ", Producto: " << v.producto
             << ", Monto: $" << v.monto_total << endl;
    }

    return 0;
}