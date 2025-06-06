/*
CSV loading and parsing logic
Statistics calculation methods
Display and menu methods
All the algorithms you implemented
Clean main function that just handles the user interface and menu system.
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iomanip>
#include <chrono>

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

    // ===== PARSEAR UNA LÍNEA DEL CSV =====
    VentaRecord parsearLinea(const string& linea) {
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
    void actualizarEstadisticas(const VentaRecord& venta) {
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

public:
    // ===== FUNCIÓN PRINCIPAL: CARGAR CSV =====
    void cargarCSV(const string& nombreArchivo) {
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
    
    // ===== REPROCESAR DATOS (después de modificaciones) =====
    void reprocesarDatos() {
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
    void mostrarTop5Ciudades(const string& pais) {
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
    void mostrarProductosExtremos() {
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
    void mostrarDiaMayorVenta() {
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
    void mostrarMenu() {
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
};

// ===== FUNCIÓN PRINCIPAL =====
int main() {
    SistemaVentas sistema;
    int opcion;
    string nombreArchivo, pais;
    
    do {
        sistema.mostrarMenu();
        cin >> opcion;
        
        switch (opcion) {
            case 1:
                cout << "Ingrese el nombre del archivo CSV: ";
                cin >> nombreArchivo;
                sistema.cargarCSV(nombreArchivo);
                break;
                
            case 2:
                cout << "Ingrese el país: ";
                cin >> pais;
                sistema.mostrarTop5Ciudades(pais);
                break;
                
            case 3:
                sistema.mostrarProductosExtremos();
                break;
                
            case 4:
                sistema.mostrarDiaMayorVenta();
                break;
                
            case 5:
                cout << "¡Gracias por usar el sistema!" << endl;
                break;
                
            default:
                cout << "Opción inválida. Intente nuevamente." << endl;
        }
        
        if (opcion != 5) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while (opcion != 5);
    
    return 0;
}