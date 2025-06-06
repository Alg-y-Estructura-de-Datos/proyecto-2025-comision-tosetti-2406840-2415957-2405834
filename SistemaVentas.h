/*
The header file for the main class containing:

All private member variables (the hash maps for quick lookups)
Method declarations (both public and private)
Proper include guards to prevent multiple inclusions 
*/

#ifndef SISTEMA_VENTAS_H
#define SISTEMA_VENTAS_H

#include <vector>
#include <unordered_map>
#include <string>
#include "VentaRecord.h"

using namespace std;

// ===== CLASE PRINCIPAL DEL SISTEMA =====
class SistemaVentas {
private:
    // ===== ALMACENAMIENTO PRINCIPAL =====
    vector<VentaRecord> todasLasVentas;  // La "caja grande" con todos los datos
    
    // ===== ALMACENAMIENTO PARA RESPUESTAS RÁPIDAS =====
    // Ventas por ciudad en cada país: ventasPorCiudad["Peru"]["Lima"] = 50000
    unordered_map<string, unordered_map<string, double>> ventasPorCiudad;
    
    // Ventas por producto en cada país: ventasPorProducto["Peru"]["Laptop"] = 25000
    unordered_map<string, unordered_map<string, double>> ventasPorProducto;
    
    // Cantidad de productos vendidos: cantidadPorProducto["Laptop"] = 150
    unordered_map<string, int> cantidadPorProducto;
    
    // Ventas por categoría en cada país: ventasPorCategoria["Peru"]["Electronics"] = {total, count}
    unordered_map<string, unordered_map<string, pair<double, int>>> ventasPorCategoria;
    
    // Medio de envío más usado por país: enviosPorPais["Peru"]["Express"] = 45
    unordered_map<string, unordered_map<string, int>> enviosPorPais;
    
    // Medio de envío más usado por categoría: enviosPorCategoria["Electronics"]["Express"] = 30
    unordered_map<string, unordered_map<string, int>> enviosPorCategoria;
    
    // Ventas por fecha: ventasPorFecha["2024-01-15"] = 15000
    unordered_map<string, double> ventasPorFecha;
    
    // Estados de envío por país: estadosPorPais["Peru"]["Delivered"] = 40
    unordered_map<string, unordered_map<string, int>> estadosPorPais;

    // ===== MÉTODOS PRIVADOS =====
    VentaRecord parsearLinea(const string& linea);
    void actualizarEstadisticas(const VentaRecord& venta);

public:
    // ===== MÉTODOS PÚBLICOS =====
    void cargarCSV(const string& nombreArchivo);
    void reprocesarDatos();
    void mostrarTop5Ciudades(const string& pais);
    void mostrarProductosExtremos();
    void mostrarDiaMayorVenta();
    void mostrarMenu();
};

#endif // SISTEMA_VENTAS_H