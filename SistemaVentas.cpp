/*
The implementation file containing all the method definitions:

CSV loading and parsing logic
Statistics calculation methods
Display and menu methods
All the algorithms you implemented
*/

#include "SistemaVentas.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>

using namespace std;

// ===== FUNCIÓN PRINCIPAL: CARGAR CSV =====
void SistemaVentas::cargarCSV(const string& nombreArchivo) {
    cout << "Cargando datos del archivo: " << nombreArchivo << "..." << endl;
    
    auto inicio = chrono::high_resolution_clock::now();
    int contadorIfs = 0;
    
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }
    
    string linea;
    // Saltar la primera línea (encabezados)
    getline(archivo, linea);
    
    int numVentas = 0;
    while (getline(archivo, linea)) {
        VentaRecord venta = parsearLinea(linea);
        
        // 1. Guardar en la caja principal
        todasLasVentas.push_back(venta);
        
        // 2. Actualizar todas las cajas auxiliares
        actualizarEstadisticas(venta);
        
        numVentas++;
        contadorIfs++; // Contar el if del while
    }
    
    archivo.close();
    
    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);
    
    cout << "✓ Cargados " << numVentas << " registros exitosamente!" << endl;
    cout << "✓ Tiempo de ejecución: " << duracion.count() << " ms" << endl;
    cout << "✓ Condicionales utilizados: " << contadorIfs << endl;
    cout << "✓ Algoritmo: Lectura secuencial con HashMap para agregaciones (O(n))" << endl;
    cout << "----------------------------------------" << endl;
}

// ===== PARSEAR UNA LÍNEA DEL CSV =====
VentaRecord SistemaVentas::parsearLinea(const string& linea) {
    VentaRecord venta;
    stringstream ss(linea);
    string campo;
    int columna = 0;
    
    while (getline(ss, campo, ',')) {
        // Limpiar comillas si las hay
        if (!campo.empty() && campo.front() == '"') {
            campo = campo.substr(1);
        }
        if (!campo.empty() && campo.back() == '"') {
            campo.pop_back();
        }
        
        switch (columna) {
            case 0: venta.id_venta = stoi(campo); break;
            case 1: venta.fecha = campo; break;
            case 2: venta.pais = campo; break;
            case 3: venta.ciudad = campo; break;
            case 4: venta.cliente = campo; break;
            case 5: venta.producto = campo; break;
            case 6: venta.categoria = campo; break;
            case 7: venta.cantidad = stoi(campo); break;
            case 8: venta.precio_unitario = stod(campo); break;
            case 9: venta.monto_total = stod(campo); break;
            case 10: venta.medio_envio = campo; break;
            case 11: venta.estado_envio = campo; break;
        }
        columna++;
    }
    
    return venta;
}

// ===== ACTUALIZAR TODAS LAS ESTADÍSTICAS =====
void SistemaVentas::actualizarEstadisticas(const VentaRecord& venta) {
    // 1. Ventas por ciudad
    ventasPorCiudad[venta.pais][venta.ciudad] += venta.monto_total;
    
    // 2. Ventas por producto
    ventasPorProducto[venta.pais][venta.producto] += venta.monto_total;
    
    // 3. Cantidad por producto (para producto más/menos vendido)
    cantidadPorProducto[venta.producto] += venta.cantidad;
    
    // 4. Ventas por categoría (para promedio)
    ventasPorCategoria[venta.pais][venta.categoria].first += venta.monto_total;
    ventasPorCategoria[venta.pais][venta.categoria].second += 1;
    
    // 5. Medio de envío por país
    enviosPorPais[venta.pais][venta.medio_envio]++;
    
    // 6. Medio de envío por categoría
    enviosPorCategoria[venta.categoria][venta.medio_envio]++;
    
    // 7. Ventas por fecha
    ventasPorFecha[venta.fecha] += venta.monto_total;
    
    // 8. Estados por país
    estadosPorPais[venta.pais][venta.estado_envio]++;
}

// ===== REPROCESAR DATOS (después de modificaciones) =====
void SistemaVentas::reprocesarDatos() {
    cout << "Reprocesando datos..." << endl;
    auto inicio = chrono::high_resolution_clock::now();
    
    // Limpiar todas las estructuras auxiliares
    ventasPorCiudad.clear();
    ventasPorProducto.clear();
    cantidadPorProducto.clear();
    ventasPorCategoria.clear();
    enviosPorPais.clear();
    enviosPorCategoria.clear();
    ventasPorFecha.clear();
    estadosPorPais.clear();
    
    // Recalcular todo
    for (const auto& venta : todasLasVentas) {
        actualizarEstadisticas(venta);
    }
    
    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);
    
    cout << "✓ Datos reprocesados en " << duracion.count() << " ms" << endl;
}

// ===== MOSTRAR TOP 5 CIUDADES POR PAÍS =====
void SistemaVentas::mostrarTop5Ciudades(const string& pais) {
    cout << "\n=== TOP 5 CIUDADES CON MAYOR MONTO DE VENTAS EN " << pais << " ===" << endl;
    
    if (ventasPorCiudad.find(pais) == ventasPorCiudad.end()) {
        cout << "No se encontraron datos para el país: " << pais << endl;
        return;
    }
    
    // Convertir a vector para ordenar
    vector<pair<double, string>> ciudades;
    for (const auto& ciudad : ventasPorCiudad[pais]) {
        ciudades.push_back({ciudad.second, ciudad.first});
    }
    
    // Ordenar por monto (descendente)
    sort(ciudades.rbegin(), ciudades.rend());
    
    // Mostrar top 5
    int limite = min(5, (int)ciudades.size());
    for (int i = 0; i < limite; i++) {
        cout << (i+1) << ". " << ciudades[i].second 
             << " - $" << fixed << setprecision(2) << ciudades[i].first << endl;
    }
    cout << "Algoritmo utilizado: HashMap + Ordenamiento (O(n log n))" << endl;
}

// ===== MOSTRAR PRODUCTO MÁS Y MENOS VENDIDO =====
void SistemaVentas::mostrarProductosExtremos() {
    cout << "\n=== PRODUCTOS MÁS Y MENOS VENDIDOS (POR CANTIDAD) ===" << endl;
    
    if (cantidadPorProducto.empty()) {
        cout << "No hay datos disponibles." << endl;
        return;
    }
    
    string productoMas, productoMenos;
    int cantidadMas = 0, cantidadMenos = INT_MAX;
    
    for (const auto& producto : cantidadPorProducto) {
        if (producto.second > cantidadMas) {
            cantidadMas = producto.second;
            productoMas = producto.first;
        }
        if (producto.second < cantidadMenos) {
            cantidadMenos = producto.second;
            productoMenos = producto.first;
        }
    }
    
    cout << "Producto MÁS vendido: " << productoMas << " (" << cantidadMas << " unidades)" << endl;
    cout << "Producto MENOS vendido: " << productoMenos << " (" << cantidadMenos << " unidades)" << endl;
    cout << "Algoritmo utilizado: Búsqueda lineal en HashMap (O(n))" << endl;
}

// ===== MOSTRAR DÍA CON MAYOR VENTA =====
void SistemaVentas::mostrarDiaMayorVenta() {
    cout << "\n=== DÍA CON MAYOR MONTO DE VENTAS ===" << endl;
    
    if (ventasPorFecha.empty()) {
        cout << "No hay datos disponibles." << endl;
        return;
    }
    
    string mejorFecha;
    double mejorMonto = 0;
    
    for (const auto& fecha : ventasPorFecha) {
        if (fecha.second > mejorMonto) {
            mejorMonto = fecha.second;
            mejorFecha = fecha.first;
        }
    }
    
    cout << "Fecha con mayor monto de ventas: " << mejorFecha 
         << " - $" << fixed << setprecision(2) << mejorMonto << endl;
    cout << "Algoritmo utilizado: Búsqueda del máximo en HashMap (O(n))" << endl;
}

// ===== MOSTRAR MENÚ PRINCIPAL =====
void SistemaVentas::mostrarMenu() {
    cout << "\n========================================" << endl;
    cout << "    SISTEMA DE ANÁLISIS DE VENTAS" << endl;
    cout << "========================================" << endl;
    cout << "1. Cargar datos desde CSV" << endl;
    cout << "2. Mostrar top 5 ciudades por país" << endl;
    cout << "3. Mostrar productos más y menos vendidos" << endl;
    cout << "4. Mostrar día con mayor venta" << endl;
    cout << "5. Salir" << endl;
    cout << "========================================" << endl;
    cout << "Seleccione una opción: ";
}