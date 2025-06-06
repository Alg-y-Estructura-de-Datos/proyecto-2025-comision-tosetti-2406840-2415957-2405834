//Contains the structure definition for individual sales records. 
//This is kept separate as it's a data structure that might be used by other parts of the system.

#ifndef VENTA_RECORD_H
#define VENTA_RECORD_H

#include <string>

using namespace std;

// ===== ESTRUCTURA PARA CADA VENTA =====
struct VentaRecord {
    int id_venta;
    string fecha;
    string pais;
    string ciudad;
    string cliente;
    string producto;
    string categoria;
    int cantidad;
    double precio_unitario;
    double monto_total;
    string medio_envio;
    string estado_envio;
};

#endif // VENTA_RECORD_H