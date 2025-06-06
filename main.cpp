/*4. main.cpp
Clean main function that just handles the user interface and menu system.*/

#include <iostream>
#include "SistemaVentas.h"

using namespace std;

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