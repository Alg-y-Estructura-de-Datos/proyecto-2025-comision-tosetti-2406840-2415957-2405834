 #include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Venta {
    int id;
    string fecha, pais, ciudad, cliente, producto, categoria;
    int cantidad;
    double precio_unitario, monto_total;
    string medio_envio, estado_envio;
};

vector<Venta> leerVentas(const string& nombreArchivo) {
    vector<Venta> ventas;
    ifstream archivo(nombreArchivo);
    string linea;

    // Saltar encabezado
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        Venta v;

        getline(ss, campo, ','); v.id = stoi(campo);
        getline(ss, v.fecha, ',');
        getline(ss, v.pais, ',');
        getline(ss, v.ciudad, ',');
        getline(ss, v.cliente, ',');
        getline(ss, v.producto, ',');
        getline(ss, v.categoria, ',');
        getline(ss, campo, ','); v.cantidad = stoi(campo);
        getline(ss, campo, ','); v.precio_unitario = stod(campo);
        getline(ss, campo, ','); v.monto_total = stod(campo);
        getline(ss, v.medio_envio, ',');
        getline(ss, v.estado_envio, '\n');

        ventas.push_back(v);
    }

    return ventas;
}

// Ejemplo de uso

