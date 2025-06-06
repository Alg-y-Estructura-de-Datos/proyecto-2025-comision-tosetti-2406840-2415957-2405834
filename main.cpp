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