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

    vector<string> top5Ciudades;

    

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

    void actualizar_estadisticas(){

    }
    
    // ===== REPROCESAR DATOS (después de modificaciones) =====
    
    
    // ===== MOSTRAR TOP 5 CIUDADES POR PAÍS =====
    void mostrarTop5Ciudades(const string& pais) {
        // Convertir a vector para ordenar
        // Ordenar por monto (descendente)
        // Mostrar top 5
    }
    
    // ===== MOSTRAR PRODUCTO MÁS Y MENOS VENDIDO =====
    
    
    // ===== MOSTRAR DÍA CON MAYOR VENTA =====
    
    
    // ===== MOSTRAR MENÚ PRINCIPAL =====
    void mostrarMenu() {
        cout << "\n========================================" << endl;
        cout << "    SISTEMA DE ANÁLISIS DE VENTAS" << endl;
        cout << "========================================" << endl;
        cout << "1. Agregar Venta" << endl;
        cout << "2. Eliminar Una Venta" << endl;
        cout << "3. Modificar Una Venta" << endl;
        cout << "4. Ventas de una Ciudad Especificas" << endl;
        cout << "5. Ventas en rango de Fecha por Pais" << endl;
        cout << "6. Comparacio 2 paises(monto total, prod + vend, envio + usado)" << endl;
        cout << "7. Comparacio 2 prod(cant total vendida y monto total / cada pais)" << endl;
        cout << "8. Productos vendidos en promedio por debajo de(por pais): " << endl;
        cout << "========================================" << endl;
        cout << "Seleccione una opción: ";
    }

    // ===== MOSTRAR TODAS LAS VENTAS =====
void mostrarTodasLasVentas() const {
    cout << left << setw(5) << "ID"
         << setw(12) << "Fecha"
         << setw(15) << "País"
         << setw(18) << "Ciudad"
         << setw(18) << "Cliente"
         << setw(18) << "Producto"
         << setw(15) << "Categoría"
         << setw(10) << "Cantidad"
         << setw(15) << "P.Unitario"
         << setw(15) << "Total"
         << setw(12) << "Medio"
         << setw(15) << "Estado" << endl;

    cout << string(168, '-') << endl;

    for (const VentaRecord& v : todasLasVentas) {
        cout << left << setw(5) << v.id_venta
             << setw(12) << v.fecha
             << setw(15) << v.pais
             << setw(18) << v.ciudad
             << setw(18) << v.cliente
             << setw(18) << v.producto
             << setw(15) << v.categoria
             << setw(10) << v.cantidad
             << setw(15) << fixed << setprecision(2) << v.precio_unitario
             << setw(15) << fixed << setprecision(2) << v.monto_total
             << setw(12) << v.medio_envio
             << setw(15) << v.estado_envio
             << endl;
    }
}


};

// ===== FUNCIÓN PRINCIPAL =====
int main() {
    SistemaVentas sistema;
    int opcion;
    string nombreArchivo, pais;
    sistema.cargarCSV("./data/ventas_sudamerica.csv");
    do {
        sistema.mostrarMenu();
        cin >> opcion;
        cout << "option recieved";
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
                
                break;
                
            case 4:
                
                break;
                
            case 5:
                cout << "¡Gracias por usar el sistema!" << endl;
                break;
            case 6:
                sistema.mostrarTodasLasVentas();
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

/* A ELIMINAR*/
