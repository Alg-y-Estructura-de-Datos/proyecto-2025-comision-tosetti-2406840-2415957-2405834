#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <set>
#include "Lista/Lista.h"
#include "Pila/Pila.h"
#include "Cola/Cola.h"
#include "HashMap/HashMap.h"
#include <set>
#include <vector>
#include <algorithm>

    

using namespace std;

struct CiudadMonto {
    string ciudad;
    double monto;
    CiudadMonto(string c, double m) : ciudad(c), monto(m) {}
    CiudadMonto() : ciudad("", 0.0) {}

};

Lista<Venta> ventas;

/*
template<typename K>
unsigned int hashString(K clave) {
    unsigned int hash = 0;
    for (unsigned char c : clave) {
        hash = hash * 31 + c;
    }
    return hash;
}*/

//hash correcta:
unsigned int hashString(string clave) {
    unsigned int hash = 0;
    for (unsigned char c : clave) {
        hash = hash * 31 + c;
    }
    return hash;
}

// HashMap global para almacenar toas las ventas por Pais, Ciudad


// Estructura para asociar país y su HashMap

HashMap<string, HashMap<string, double>> ciudadesPorPais(300, hashString);
HashMap<string, HashMap<string, double>> productoPorPais(300, hashString);
HashMap<string, HashMap<string, int>> categoriaPorPais(300, hashString);
HashMap<string, HashMap<string, int>> medioPorPais(300, hashString);
HashMap<string, HashMap<string, int>> estadoPorPais(300, hashString);
HashMap<string, HashMap<string, int>> medioPorCategoria(300, hashString);
HashMap<string, double> montoPorFecha(3000, hashString);
HashMap<string, int> cantidadPorProducto(3000, hashString);                   // producto -> cantidad total

// Función genérica para imprimir cualquier hash de hash

void actualizarHashmapsAgregar(const Venta& v);
void actualizarHashmapsEliminar(const Venta& v);


// 1. Top 5 ciudades por país (usando ciudadesPorPais)
void mostrarTop5CiudadesPorPais_Hash() {
    Lista<string> paises = ciudadesPorPais.claves();
    std::set<string> paisesUnicos;
    for (int i = 0; i < paises.getTamanio(); i++) paisesUnicos.insert(paises.getDato(i));
    for (const auto& pais : paisesUnicos) {
        try {
            HashMap<string, double> mapaCiudades = ciudadesPorPais.get(pais);
            Lista<string> ciudades = mapaCiudades.claves();
            std::vector<CiudadMonto> lista;
            std::set<string> ciudadesUnicas;
            for (int i = 0; i < ciudades.getTamanio(); i++) ciudadesUnicas.insert(ciudades.getDato(i));
            for (const auto& ciudad : ciudadesUnicas) {
                try {
                    double monto = mapaCiudades.get(ciudad);
                    lista.push_back(CiudadMonto(ciudad, monto));
                } catch (...) {}
            }
            std::sort(lista.begin(), lista.end(), [](const CiudadMonto& a, const CiudadMonto& b) {
                return a.monto > b.monto;
            });
            cout << "\nTop 5 ciudades con mas ventas en " << pais << ":\n";
            cout << "----------------------------------------\n";
            for (int i = 0; i < std::min(5, (int)lista.size()); i++) {
                cout << i + 1 << ". " << left << setw(20) << lista[i].ciudad
                     << " - $" << fixed << setprecision(2) << lista[i].monto << endl;
            }
            cout << "----------------------------------------\n";
        } catch (...) {}
    }
}

// 2. Monto total por producto y país (usando productoPorPais)
void mostrarMontoTotalPorProductoYPais_Hash() {
    Lista<string> paises = productoPorPais.claves();
    std::set<string> paisesUnicos;
    for (int i = 0; i < paises.getTamanio(); i++) paisesUnicos.insert(paises.getDato(i));
    for (const auto& pais : paisesUnicos) {
        cout << "\n=== Pais: " << pais << " ===\n";
        try {
            HashMap<string, double> montoPorProducto = productoPorPais.get(pais);
            Lista<string> productos = montoPorProducto.claves();
            std::set<string> productosUnicos;
            for (int i = 0; i < productos.getTamanio(); i++) productosUnicos.insert(productos.getDato(i));
            cout << left << setw(25) << "Producto" << right << setw(18) << "Monto Total" << endl;
            cout << string(43, '-') << endl;
            for (const auto& producto : productosUnicos) {
                try {
                    double monto = montoPorProducto.get(producto);
                    cout << left << setw(25) << producto << right << " $" << fixed << setprecision(2) << setw(15) << monto << endl;
                } catch (...) {}
            }
        } catch (...) {
            cout << "(Sin datos)\n";
        }
    }
    cout << endl;
}

// 3. Medio de envio mas utilizado por país (usando medioPorPais)
void mostrarMedioEnvioMasUtilizadoPorPais_Hash() {
    Lista<string> paises = medioPorPais.claves();
    std::set<string> paisesUnicos;
    for (int i = 0; i < paises.getTamanio(); i++) paisesUnicos.insert(paises.getDato(i));
    for (const auto& pais : paisesUnicos) {
        cout << "\n=== Pais: " << pais << " ===\n";
        try {
            HashMap<string, int> medioCantidad = medioPorPais.get(pais);
            Lista<string> medios = medioCantidad.claves();
            std::set<string> mediosUnicos;
            for (int i = 0; i < medios.getTamanio(); i++) mediosUnicos.insert(medios.getDato(i));
            string mejorMedio = "";
            int maxCantidad = -1;
            for (const auto& medio : mediosUnicos) {
                try {
                    int cantidad = medioCantidad.get(medio);
                    if (cantidad > maxCantidad) {
                        maxCantidad = cantidad;
                        mejorMedio = medio;
                    }
                } catch (...) {}
            }
            cout << "\n=== Medio de envio mas utilizado en " << pais << " ===\n";
            if (mejorMedio != "") {
                cout << "Medio: " << mejorMedio << endl;
                cout << "Cantidad: " << maxCantidad << endl;
            } else {
                cout << "No hay datos de envios para este pais." << endl;
            }
        } catch (...) {
            cout << "(Sin datos)\n";
        }
    }
    cout << endl;
}

// 4. Cantidad de ventas por categoria y país (usando categoriaPorPais)
void mostrarPromedioPorCategoriaPorPais_Hash() {
    Lista<string> paises = categoriaPorPais.claves();
    std::set<string> paisesUnicos;
    for (int i = 0; i < paises.getTamanio(); i++) paisesUnicos.insert(paises.getDato(i));
    for (const auto& pais : paisesUnicos) {
        cout << "\n=== Cantidad de ventas por categoria en " << pais << " ===\n";
        try {
            HashMap<string, int> cantidadPorCategoria = categoriaPorPais.get(pais);
            Lista<string> categorias = cantidadPorCategoria.claves();
            std::set<string> categoriasUnicas;
            for (int i = 0; i < categorias.getTamanio(); i++) categoriasUnicas.insert(categorias.getDato(i));
            cout << left << setw(25) << "Categoria" << right << setw(18) << "Cantidad Ventas" << endl;
            cout << string(43, '-') << endl;
            for (const auto& categoria : categoriasUnicas) {
                try {
                    int cantidad = cantidadPorCategoria.get(categoria);
                    cout << left << setw(25) << categoria << right << setw(15) << cantidad << endl;
                } catch (...) {}
            }
        } catch (...) {
            cout << "(Sin datos)\n";
        }
    }
    cout << endl;
}

// 5. Medio de envio mas utilizado por categoria (usando medioPorCategoria)
void mostrarMedioDeEnvioMasUtilizadoPorCategoria_Hash() {
    Lista<string> categorias = medioPorCategoria.claves();
    std::set<string> categoriasUnicas;
    for (int i = 0; i < categorias.getTamanio(); i++) categoriasUnicas.insert(categorias.getDato(i));
    for (const auto& categoria : categoriasUnicas) {
        cout << "\n=== Categoria: " << categoria << " ===\n";
        try {
            HashMap<string, int> medioCantidad = medioPorCategoria.get(categoria);
            Lista<string> medios = medioCantidad.claves();
            std::set<string> mediosUnicos;
            for (int i = 0; i < medios.getTamanio(); i++) mediosUnicos.insert(medios.getDato(i));
            string mejorMedio = "";
            int maxCantidad = -1;
            for (const auto& medio : mediosUnicos) {
                try {
                    int cantidad = medioCantidad.get(medio);
                    if (cantidad > maxCantidad) {
                        maxCantidad = cantidad;
                        mejorMedio = medio;
                    }
                } catch (...) {}
            }
            cout << "Medio de envio mas utilizado: ";
            if (mejorMedio != "") {
                cout << mejorMedio << " (" << maxCantidad << " ventas)" << endl;
            } else {
                cout << "No hay datos para esta categoria." << endl;
            }
        } catch (...) {
            cout << "(Sin datos)\n";
        }
    }
    cout << endl;
}

// 6. Día con mayor monto total de ventas (usando montoPorFecha)
void mostrarDiaConMayorCantidadDeVentas_Hash() {
    Lista<string> fechas = montoPorFecha.claves();
    std::set<string> fechasUnicas;
    for (int i = 0; i < fechas.getTamanio(); i++) fechasUnicas.insert(fechas.getDato(i));
    string mejorFecha = "";
    double maxMonto = -1.0;
    for (const auto& fecha : fechasUnicas) {
        try {
            double monto = montoPorFecha.get(fecha);
            if (monto > maxMonto) {
                maxMonto = monto;
                mejorFecha = fecha;
            }
        } catch (...) {}
    }
    cout << "\n=== Dia con mayor monto total de ventas ===\n";
    if (mejorFecha != "") {
        cout << "Fecha: " << mejorFecha << endl;
        cout << "Monto total de ventas: $" << fixed << setprecision(2) << maxMonto << endl;
    } else {
        cout << "No hay datos de ventas." << endl;
    }
    cout << endl;
}

// 7. Estado de envio más frecuente por país (usando estadoPorPais)
void estadoDeEnvioMasFrecuentePorPais_Hash() {
    Lista<string> paises = estadoPorPais.claves();
    std::set<string> paisesUnicos;
    for (int i = 0; i < paises.getTamanio(); i++) paisesUnicos.insert(paises.getDato(i));
    for (const auto& pais : paisesUnicos) {
        cout << "\n=== Pais: " << pais << " ===\n";
        try {
            HashMap<string, int> estadoCantidad = estadoPorPais.get(pais);
            Lista<string> estados = estadoCantidad.claves();
            std::set<string> estadosUnicos;
            for (int i = 0; i < estados.getTamanio(); i++) estadosUnicos.insert(estados.getDato(i));
            string mejorEstado = "";
            int maxCantidad = -1;
            for (const auto& estado : estadosUnicos) {
                try {
                    int cantidad = estadoCantidad.get(estado);
                    if (cantidad > maxCantidad) {
                        maxCantidad = cantidad;
                        mejorEstado = estado;
                    }
                } catch (...) {}
            }
            if (mejorEstado != "") {
                cout << "Estado de envio mas frecuente: " << mejorEstado << " (" << maxCantidad << " ventas)" << endl;
            } else {
                cout << "No hay datos de estados de envio para este pais." << endl;
            }
        } catch (...) {
            cout << "(Sin datos)\n";
        }
    }
    cout << endl;
}

// 8. Producto más vendido por cantidad (usando cantidadPorProducto)
void mostrarProductoMasVendidoPorCantidad_Hash() {
    Lista<string> productos = cantidadPorProducto.claves();
    std::set<string> productosUnicos;
    for (int i = 0; i < productos.getTamanio(); i++) productosUnicos.insert(productos.getDato(i));
    string mejorProducto = "";
    int maxCantidad = -1;
    for (const auto& producto : productosUnicos) {
        try {
            int cantidad = cantidadPorProducto.get(producto);
            if (cantidad > maxCantidad) {
                maxCantidad = cantidad;
                mejorProducto = producto;
            }
        } catch (...) {}
    }
    cout << "\n=== Producto más vendido por cantidad ===\n";
    if (mejorProducto != "") {
        cout << "Producto: " << mejorProducto << " (" << maxCantidad << " unidades)" << endl;
    } else {
        cout << "No hay datos de productos." << endl;
    }
    cout << endl;
}

// 9. Producto menos vendido por cantidad (usando cantidadPorProducto)
void mostrarProductoMenosVendidoPorCantidad_Hash() {
    Lista<string> productos = cantidadPorProducto.claves();
    std::set<string> productosUnicos;
    for (int i = 0; i < productos.getTamanio(); i++) productosUnicos.insert(productos.getDato(i));
    string peorProducto = "";
    int minCantidad = 999999;
    for (const auto& producto : productosUnicos) {
        try {
            int cantidad = cantidadPorProducto.get(producto);
            if (cantidad < minCantidad) {
                minCantidad = cantidad;
                peorProducto = producto;
            }
        } catch (...) {}
    }
    cout << "\n=== Producto menos vendido por cantidad ===\n";
    if (peorProducto != "") {
        cout << "Producto: " << peorProducto << " (" << minCantidad << " unidades)" << endl;
    } else {
        cout << "No hay datos de productos." << endl;
    }
    cout << endl;
}

void AgregarVenta() {
    Venta v;
    string input;

    // Generar id automáticamente (uno más que el mayor id actual)
    int maxId = 0;
    for (int i = 0; i < ventas.getTamanio(); i++) {
        int actualId = 0;
        try {
            actualId = stoi(ventas.getDato(i).id);
        } catch (...) {
            continue;
        }
        if (actualId > maxId) maxId = actualId;
    }
    v.id = to_string(maxId + 1);
    cout << "\n--- Ingreso de nueva venta ---" << endl;
    cout << "ID de venta asignado: " << v.id << endl;

    cout << "Fecha (ej: 31/12/2024): ";
    getline(cin >> ws, v.fecha);

    cout << "Pais: ";
    getline(cin, v.pais);

    cout << "Ciudad: ";
    getline(cin, v.ciudad);

    cout << "Cliente: ";
    getline(cin, v.cliente);

    cout << "Producto: ";
    getline(cin, v.producto);

    cout << "Categoria: ";
    getline(cin, v.categoria);

    cout << "Cantidad: ";
    getline(cin, input);

    // Validar precio_unitario (float)

    cout << "Precio unitario: ";
    getline(cin, input);

    // Validar monto_total (float)
    
    cout << "Monto total: ";
    getline(cin, input);


    cout << "Medio de envio: ";
    getline(cin, v.medio_envio);

    cout << "Estado de envio: ";
    getline(cin, v.estado_envio);

    // ...existing code...
    ventas.insertarUltimo(v);
    actualizarHashmapsAgregar(v);
    cout << "Venta agregada correctamente.\n" << endl;
}

void actualizarHashmapsAgregar(const Venta& v) {

    // 1. ciudad -> monto por país
    try {
        auto mapa = ciudadesPorPais.get(v.pais);
        try {
            double actual = mapa.get(v.ciudad);
            mapa.put(v.ciudad, actual + v.monto_total);
        } catch (...) {
            mapa.put(v.ciudad, v.monto_total);
        }
        ciudadesPorPais.put(v.pais, mapa);
    } catch (...) {
        HashMap<string, double> nuevo(100, hashString);
        nuevo.put(v.ciudad, v.monto_total);
        ciudadesPorPais.put(v.pais, nuevo);
    }

    // 2. producto -> monto por país
    try {
        auto mapa = productoPorPais.get(v.pais);
        try {
            double actual = mapa.get(v.producto);
            mapa.put(v.producto, actual + v.monto_total);
        } catch (...) {
            mapa.put(v.producto, v.monto_total);
        }
        productoPorPais.put(v.pais, mapa);
    } catch (...) {
        HashMap<string, double> nuevo(100, hashString);
        nuevo.put(v.producto, v.monto_total);
        productoPorPais.put(v.pais, nuevo);
    }

    // 3. categoria -> cantidad por país
    try {
        auto mapa = categoriaPorPais.get(v.pais);
        try {
            int actual = mapa.get(v.categoria);
            mapa.put(v.categoria, actual + 1);
        } catch (...) {
            mapa.put(v.categoria, 1);
        }
        categoriaPorPais.put(v.pais, mapa);
    } catch (...) {
        HashMap<string, int> nuevo(100, hashString);
        nuevo.put(v.categoria, 1);
        categoriaPorPais.put(v.pais, nuevo);
    }

    // 4. medio de envío -> cantidad por país
    try {
        auto mapa = medioPorPais.get(v.pais);
        try {
            int actual = mapa.get(v.medio_envio);
            mapa.put(v.medio_envio, actual + 1);
        } catch (...) {
            mapa.put(v.medio_envio, 1);
        }
        medioPorPais.put(v.pais, mapa);
    } catch (...) {
        HashMap<string, int> nuevo(100, hashString);
        nuevo.put(v.medio_envio, 1);
        medioPorPais.put(v.pais, nuevo);
    }

    // 5. estado de envío -> cantidad por país
    try {
        auto mapa = estadoPorPais.get(v.pais);
        try {
            int actual = mapa.get(v.estado_envio);
            mapa.put(v.estado_envio, actual + 1);
        } catch (...) {
            mapa.put(v.estado_envio, 1);
        }
        estadoPorPais.put(v.pais, mapa);
    } catch (...) {
        HashMap<string, int> nuevo(100, hashString);
        nuevo.put(v.estado_envio, 1);
        estadoPorPais.put(v.pais, nuevo);
    }

    // 6. medio de envío -> cantidad por categoría
    try {
        auto mapa = medioPorCategoria.get(v.categoria);
        try {
            int actual = mapa.get(v.medio_envio);
            mapa.put(v.medio_envio, actual + 1);
        } catch (...) {
            mapa.put(v.medio_envio, 1);
        }
        medioPorCategoria.put(v.categoria, mapa);
    } catch (...) {
        HashMap<string, int> nuevo(100, hashString);
        nuevo.put(v.medio_envio, 1);
        medioPorCategoria.put(v.categoria, nuevo);
    }

    // 7. monto total por fecha
    try {
        double actual = montoPorFecha.get(v.fecha);
        montoPorFecha.put(v.fecha, actual + v.monto_total);
    } catch (...) {
        montoPorFecha.put(v.fecha, v.monto_total);
    }

    // 8. cantidad total por producto
    try {
        int actual = cantidadPorProducto.get(v.producto);
        cantidadPorProducto.put(v.producto, actual + v.cantidad);
    } catch (...) {
        cantidadPorProducto.put(v.producto, v.cantidad);
    }
}

void calcularEstadisticasVentas() {
    mostrarTop5CiudadesPorPais_Hash();
    mostrarMontoTotalPorProductoYPais_Hash();
    mostrarMedioEnvioMasUtilizadoPorPais_Hash();
    mostrarPromedioPorCategoriaPorPais_Hash();
    mostrarMedioDeEnvioMasUtilizadoPorCategoria_Hash();
    mostrarDiaConMayorCantidadDeVentas_Hash();
    estadoDeEnvioMasFrecuentePorPais_Hash();
    mostrarProductoMasVendidoPorCantidad_Hash();
    mostrarProductoMenosVendidoPorCantidad_Hash();
    
}

void procesarEstadisticas() {
    calcularEstadisticasVentas();
}

void cargarVentasDesdeCSV(string nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string linea;
    getline(archivo, linea); // Encabezado
    while (getline(archivo, linea)) {
        stringstream stream(linea);
        Venta v;
        string cantidad, precio, monto;
        getline(stream, v.id, ',');
        getline(stream, v.fecha, ',');
        getline(stream, v.pais, ',');
        getline(stream, v.ciudad, ',');
        getline(stream, v.cliente, ',');
        getline(stream, v.producto, ',');
        getline(stream, v.categoria, ',');
        getline(stream, cantidad, ',');
        getline(stream, precio, ',');
        getline(stream, monto, ',');
        getline(stream, v.medio_envio, ',');
        getline(stream, v.estado_envio, ',');
        v.cantidad = stoi(cantidad);
        v.precio_unitario = stof(precio);
        v.monto_total = stof(monto);
        ventas.insertarUltimo(v);
    }
    archivo.close();
    
}



void calcularEstadisticasOptimo(
    HashMap<string, HashMap<string, double>>& ciudadesPorPais,
    HashMap<string, HashMap<string, double>>& productoPorPais,
    HashMap<string, HashMap<string, int>>& categoriaPorPais,
    HashMap<string, HashMap<string, int>>& medioPorPais,
    HashMap<string, HashMap<string, int>>& estadoPorPais,
    HashMap<string, HashMap<string, int>>& medioPorCategoria,
    HashMap<string, double>& montoPorFecha,
    HashMap<string, int>& cantidadPorProducto
) {
    // cout << "[DEBUG] Limpiando hashmaps..." << endl;
    ciudadesPorPais = HashMap<string, HashMap<string, double>>(300, hashString);
    productoPorPais = HashMap<string, HashMap<string, double>>(300, hashString);
    categoriaPorPais = HashMap<string, HashMap<string, int>>(300, hashString);
    medioPorPais = HashMap<string, HashMap<string, int>>(300, hashString);
    estadoPorPais = HashMap<string, HashMap<string, int>>(300, hashString);
    medioPorCategoria = HashMap<string, HashMap<string, int>>(300, hashString);
    montoPorFecha = HashMap<string, double>(3000, hashString);
    cantidadPorProducto = HashMap<string, int>(3000, hashString);

    // cout << "[DEBUG] Comenzando a recorrer ventas. Total: " << ventas.getTamanio() << endl;
    for (int i = 0; i < ventas.getTamanio(); i++) {
        Venta v = ventas.getDato(i);
        // cout << "[DEBUG] Venta #" << i << ": " << v.pais << " - " << v.ciudad << " - " << v.producto << endl;

        
        // 1. ciudad -> monto por país
        try {
            // cout << "[DEBUG] 1. Buscando mapa de ciudades para pais: " << v.pais << endl;
            HashMap<string, double> mapa = ciudadesPorPais.get(v.pais);
            try {
                double actual = mapa.get(v.ciudad);
                mapa.put(v.ciudad, actual + v.monto_total);
                // cout << "[DEBUG]    Sumo monto ciudad existente: " << v.ciudad << " + " << v.monto_total << endl;
            } catch (...) {
                mapa.put(v.ciudad, v.monto_total);
                // cout << "[DEBUG]    Nueva ciudad: " << v.ciudad << " monto: " << v.monto_total << endl;
            }
            ciudadesPorPais.put(v.pais, mapa);
        } catch (...) {
            // cout << "[DEBUG] 1. Nuevo pais para ciudades: " << v.pais << endl;
            HashMap<string, double> nuevo(100, hashString);
            nuevo.put(v.ciudad, v.monto_total);
            ciudadesPorPais.put(v.pais, nuevo);
        }

        // 2. producto -> monto por país
        try {
            // cout << "[DEBUG] 2. Buscando mapa de productos para pais: " << v.pais << endl;
            HashMap<string, double> mapa = productoPorPais.get(v.pais);
            try {
                double actual = mapa.get(v.producto);
                mapa.put(v.producto, actual + v.monto_total);
                //cout << "[DEBUG]    Sumo monto producto existente: " << v.producto << " + " << v.monto_total << endl;
            } catch (...) {
                mapa.put(v.producto, v.monto_total);
                //cout << "[DEBUG]    Nuevo producto: " << v.producto << " monto: " << v.monto_total << endl;
            }
            productoPorPais.put(v.pais, mapa);
        } catch (...) {
            //cout << "[DEBUG] 2. Nuevo pais para productos: " << v.pais << endl;
            HashMap<string, double> nuevo(100, hashString);
            nuevo.put(v.producto, v.monto_total);
            productoPorPais.put(v.pais, nuevo);
        }

        // 3. categoria -> cantidad por país
        try {
            //cout << "[DEBUG] 3. Buscando mapa de categorias para pais: " << v.pais << endl;
            HashMap<string, int> mapa = categoriaPorPais.get(v.pais);
            try {
                int actual = mapa.get(v.categoria);
                mapa.put(v.categoria, actual + 1);
                //cout << "[DEBUG]    Sumo categoria existente: " << v.categoria << " +1" << endl;
            } catch (...) {
                mapa.put(v.categoria, 1);
                //cout << "[DEBUG]    Nueva categoria: " << v.categoria << endl;
            }
            categoriaPorPais.put(v.pais, mapa);
        } catch (...) {
            //cout << "[DEBUG] 3. Nuevo pais para categorias: " << v.pais << endl;
            HashMap<string, int> nuevo(100, hashString);
            nuevo.put(v.categoria, 1);
            categoriaPorPais.put(v.pais, nuevo);
        }

        // 4. medio de envío -> cantidad por país
        try {
            //cout << "[DEBUG] 4. Buscando mapa de medios para pais: " << v.pais << endl;
            HashMap<string, int> mapa = medioPorPais.get(v.pais);
            try {
                int actual = mapa.get(v.medio_envio);
                mapa.put(v.medio_envio, actual + 1);
                //cout << "[DEBUG]    Sumo medio existente: " << v.medio_envio << " +1" << endl;
            } catch (...) {
                mapa.put(v.medio_envio, 1);
                //cout << "[DEBUG]    Nuevo medio: " << v.medio_envio << endl;
            }
            medioPorPais.put(v.pais, mapa);
        } catch (...) {
            //cout << "[DEBUG] 4. Nuevo pais para medios: " << v.pais << endl;
            HashMap<string, int> nuevo(100, hashString);
            nuevo.put(v.medio_envio, 1);
            medioPorPais.put(v.pais, nuevo);
        }

        // 5. estado de envío -> cantidad por país
        try {
            //cout << "[DEBUG] 5. Buscando mapa de estados para pais: " << v.pais << endl;
            HashMap<string, int> mapa = estadoPorPais.get(v.pais);
            try {
                int actual = mapa.get(v.estado_envio);
                mapa.put(v.estado_envio, actual + 1);
                //cout << "[DEBUG]    Sumo estado existente: " << v.estado_envio << " +1" << endl;
            } catch (...) {
                mapa.put(v.estado_envio, 1);
                //cout << "[DEBUG]    Nuevo estado: " << v.estado_envio << endl;
            }
            estadoPorPais.put(v.pais, mapa);
        } catch (...) {
            //cout << "[DEBUG] 5. Nuevo pais para estados: " << v.pais << endl;
            HashMap<string, int> nuevo(100, hashString);
            nuevo.put(v.estado_envio, 1);
            estadoPorPais.put(v.pais, nuevo);
        }

        // 6. medio de envío -> cantidad por categoría
        try {
            //cout << "[DEBUG] 6. Buscando mapa de medios para categoria: " << v.categoria << endl;
            HashMap<string, int> mapa = medioPorCategoria.get(v.categoria);
            try {
                int actual = mapa.get(v.medio_envio);
                mapa.put(v.medio_envio, actual + 1);
                //cout << "[DEBUG]    Sumo medio existente: " << v.medio_envio << " +1" << endl;
            } catch (...) {
                mapa.put(v.medio_envio, 1);
                //cout << "[DEBUG]    Nuevo medio: " << v.medio_envio << endl;
            }
            medioPorCategoria.put(v.categoria, mapa);
        } catch (...) {
            //cout << "[DEBUG] 6. Nueva categoria para medios: " << v.categoria << endl;
            HashMap<string, int> nuevo(100, hashString);
            nuevo.put(v.medio_envio, 1);
            medioPorCategoria.put(v.categoria, nuevo);
        }

        // 7. monto total por fecha
        try {
            double actual = montoPorFecha.get(v.fecha);
            montoPorFecha.put(v.fecha, actual + v.monto_total);
            //cout << "[DEBUG] 7. Sumo monto por fecha existente: " << v.fecha << " + " << v.monto_total << endl;
        } catch (...) {
            montoPorFecha.put(v.fecha, v.monto_total);
            //cout << "[DEBUG] 7. Nueva fecha: " << v.fecha << " monto: " << v.monto_total << endl;
        }

        // 8. cantidad total por producto
        try {
            int actual = cantidadPorProducto.get(v.producto);
            cantidadPorProducto.put(v.producto, actual + v.cantidad);
            //cout << "[DEBUG] 8. Sumo cantidad producto existente: " << v.producto << " + " << v.cantidad << endl;
        } catch (...) {
            cantidadPorProducto.put(v.producto, v.cantidad);
            //cout << "[DEBUG] 8. Nuevo producto: " << v.producto << " cantidad: " << v.cantidad << endl;
        }

    }
    //cout << "[DEBUG] Fin de calcularEstadisticasOptimo" << endl;
}

void mostrarMenuConsultas() {
    cout << "\n----- CONSULTAS DINAMICAS -----" << endl;
    cout << "1. Listado de ventas por ciudad especifica" << endl;
    cout << "2. Listado de ventas por rango de fechas y pais" << endl;
    cout << "3. Comparacion entre dos paises" << endl;
    cout << "4. Comparacion entre dos productos" << endl;
    cout << "5. Productos vendidos por debajo de un monto (por pais)" << endl;
    cout << "6. Productos vendidos por encima de un monto (por pais)" << endl;
    cout << "7. Volver al menu principal" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Seleccione una opcion: ";
}

void eliminarVenta() {
    string pais, ciudad, idEliminar;
    cout << "\n--- Eliminar venta ---" << endl;
    cout << "Ingrese pais (deje en blanco para ignorar): ";
    getline(cin >> ws, pais);
    cout << "Ingrese ciudad (deje en blanco para ignorar): ";
    getline(cin, ciudad);

    // Filtrar ventas
    Lista<int> indicesFiltrados;
    cout << "\nVentas encontradas:\n";
    cout << left << setw(5) << "ID" << setw(12) << "Fecha" << setw(15) << "Pais" << setw(15) << "Ciudad"
         << setw(15) << "Producto" << setw(10) << "Cantidad" << endl;
    cout << string(70, '-') << endl;
    for (int i = 0; i < ventas.getTamanio(); i++) {
        Venta v = ventas.getDato(i);
        bool coincide = true;
        if (!pais.empty() && v.pais != pais) coincide = false;
        if (!ciudad.empty() && v.ciudad != ciudad) coincide = false;
        if (coincide) {
            indicesFiltrados.insertarUltimo(i);
            cout << left << setw(5) << v.id << setw(12) << v.fecha << setw(15) << v.pais
                 << setw(15) << v.ciudad << setw(15) << v.producto << setw(10) << v.cantidad << endl;
        }
    }
    if (indicesFiltrados.getTamanio() == 0) {
        cout << "No se encontraron ventas con esos filtros.\n";
        return;
    }

    cout << "\nIngrese el ID de la venta a eliminar (deje en blanco para cancelar): ";
    getline(cin, idEliminar);
    if (idEliminar.empty()) {
        cout << "Eliminacion cancelada.\n";
        return;
    }

    // Buscar y eliminar
    bool eliminado = false;
    for (int j = 0; j < indicesFiltrados.getTamanio(); j++) {
        int idx = indicesFiltrados.getDato(j);
        Venta v = ventas.getDato(idx);
        if (v.id == idEliminar) {
            actualizarHashmapsEliminar(v);
            ventas.remover(idx);
            cout << "Venta eliminada correctamente.\n";
            eliminado = true;
            break;
        }
    }
    if (!eliminado) {
        cout << "No se encontro una venta con ese ID en los resultados filtrados.\n";
    }


}

void actualizarHashmapsEliminar(const Venta& v) {

    // 1. ciudad -> monto por país
    try {
        auto mapa = ciudadesPorPais.get(v.pais);
        try {
            double actual = mapa.get(v.ciudad);
            double nuevoMonto = actual - v.monto_total;
            if (nuevoMonto <= 0.0001) {
                mapa.remove(v.ciudad);
            } else {
                mapa.put(v.ciudad, nuevoMonto);
            }
        } catch (...) {}
        ciudadesPorPais.put(v.pais, mapa);
    } catch (...) {}

    // 2. producto -> monto por país
    try {
        auto mapa = productoPorPais.get(v.pais);
        try {
            double actual = mapa.get(v.producto);
            double nuevoMonto = actual - v.monto_total;
            if (nuevoMonto <= 0.0001) {
                mapa.remove(v.producto);
            } else {
                mapa.put(v.producto, nuevoMonto);
            }
        } catch (...) {}
        productoPorPais.put(v.pais, mapa);
    } catch (...) {}

    // 3. categoria -> cantidad por país
    try {
        auto mapa = categoriaPorPais.get(v.pais);
        try {
            int actual = mapa.get(v.categoria);
            int nuevo = actual - 1;
            if (nuevo <= 0) {
                mapa.remove(v.categoria);
            } else {
                mapa.put(v.categoria, nuevo);
            }
        } catch (...) {}
        categoriaPorPais.put(v.pais, mapa);
    } catch (...) {}

    // 4. medio de envío -> cantidad por país
    try {
        auto mapa = medioPorPais.get(v.pais);
        try {
            int actual = mapa.get(v.medio_envio);
            int nuevo = actual - 1;
            if (nuevo <= 0) {
                mapa.remove(v.medio_envio);
            } else {
                mapa.put(v.medio_envio, nuevo);
            }
        } catch (...) {}
        medioPorPais.put(v.pais, mapa);
    } catch (...) {}

    // 5. estado de envío -> cantidad por país
    try {
        auto mapa = estadoPorPais.get(v.pais);
        try {
            int actual = mapa.get(v.estado_envio);
            int nuevo = actual - 1;
            if (nuevo <= 0) {
                mapa.remove(v.estado_envio);
            } else {
                mapa.put(v.estado_envio, nuevo);
            }
        } catch (...) {}
        estadoPorPais.put(v.pais, mapa);
    } catch (...) {}

    // 6. medio de envío -> cantidad por categoría
    try {
        auto mapa = medioPorCategoria.get(v.categoria);
        try {
            int actual = mapa.get(v.medio_envio);
            int nuevo = actual - 1;
            if (nuevo <= 0) {
                mapa.remove(v.medio_envio);
            } else {
                mapa.put(v.medio_envio, nuevo);
            }
        } catch (...) {}
        medioPorCategoria.put(v.categoria, mapa);
    } catch (...) {}

    // 7. monto total por fecha
    try {
        double actual = montoPorFecha.get(v.fecha);
        double nuevoMonto = actual - v.monto_total;
        if (nuevoMonto <= 0.0001) {
            montoPorFecha.remove(v.fecha);
        } else {
            montoPorFecha.put(v.fecha, nuevoMonto);
        }
    } catch (...) {}

    // 8. cantidad total por producto
    try {
        int actual = cantidadPorProducto.get(v.producto);
        int nuevo = actual - v.cantidad;
        if (nuevo <= 0) {
            cantidadPorProducto.remove(v.producto);
        } else {
            cantidadPorProducto.put(v.producto, nuevo);
        }
    } catch (...) {}
}

void ModificarVenta() {
    string idModificar;
    int idx = -1;

    // Buscar ID válido o cancelar
    while (true) {
        cout << "\n--- Modificar venta ---" << endl;
        cout << "Ingrese el ID de la venta a modificar (x para cancelar): ";
        getline(cin >> ws, idModificar);
        if (idModificar == "x" || idModificar == "X") {
            cout << "Modificación cancelada.\n";
            return;
        }
        for (int i = 0; i < ventas.getTamanio(); i++) {
            if (ventas.getDato(i).id == idModificar) {
                idx = i;
                break;
            }
        }
        if (idx != -1) break;
        cout << "ID no encontrado. Intente de nuevo.\n";
    }

    Venta v = ventas.getDato(idx);
    string input;

    cout << "Fecha actual: " << v.fecha << endl;
    cout << "Nueva fecha (x para dejar igual): ";
    getline(cin, input);
    if (input != "x" && input != "X" && !input.empty()) v.fecha = input;

    cout << "Pais actual: " << v.pais << endl;
    cout << "Nuevo pais (x para dejar igual): ";
    getline(cin, input);
    if (input != "x" && input != "X" && !input.empty()) v.pais = input;

    cout << "Ciudad actual: " << v.ciudad << endl;
    cout << "Nueva ciudad (x para dejar igual): ";
    getline(cin, input);
    if (input != "x" && input != "X" && !input.empty()) v.ciudad = input;

    cout << "Cliente actual: " << v.cliente << endl;
    cout << "Nuevo cliente (x para dejar igual): ";
    getline(cin, input);
    if (input != "x" && input != "X" && !input.empty()) v.cliente = input;

    cout << "Producto actual: " << v.producto << endl;
    cout << "Nuevo producto (x para dejar igual): ";
    getline(cin, input);
    if (input != "x" && input != "X" && !input.empty()) v.producto = input;

    cout << "Categoria actual: " << v.categoria << endl;
    cout << "Nueva categoria (x para dejar igual): ";
    getline(cin, input);
    if (input != "x" && input != "X" && !input.empty()) v.categoria = input;

    // Validar cantidad (int)
    while (true) {
        cout << "Cantidad actual: " << v.cantidad << endl;
        cout << "Nueva cantidad (x para dejar igual): ";
        getline(cin, input);
        if (input == "x" || input == "X" || input.empty()) break;
        try {
            int nuevaCantidad = stoi(input);
            if (nuevaCantidad < 0) throw invalid_argument("negativo");
            v.cantidad = nuevaCantidad;
            break;
        } catch (...) {
            cout << "Cantidad invalida. Debe ser un numero entero positivo o x." << endl;
        }
    }

    // Validar precio_unitario (float)
    while (true) {
        cout << "Precio unitario actual: " << v.precio_unitario << endl;
        cout << "Nuevo precio unitario (x para dejar igual): ";
        getline(cin, input);
        if (input == "x" || input == "X" || input.empty()) break;
        try {
            float nuevoPrecio = stof(input);
            if (nuevoPrecio < 0) throw invalid_argument("negativo");
            v.precio_unitario = nuevoPrecio;
            break;
        } catch (...) {
            cout << "Precio invalido. Debe ser un numero positivo o x." << endl;
        }
    }

    // Validar monto_total (float)
    while (true) {
        cout << "Monto total actual: " << v.monto_total << endl;
        cout << "Nuevo monto total (x para dejar igual): ";
        getline(cin, input);
        if (input == "x" || input == "X" || input.empty()) break;
        try {
            float nuevoMonto = stof(input);
            if (nuevoMonto < 0) throw invalid_argument("negativo");
            v.monto_total = nuevoMonto;
            break;
        } catch (...) {
            cout << "Monto invalido. Debe ser un numero positivo o x." << endl;
        }
    }

    cout << "Medio de envio actual: " << v.medio_envio << endl;
    cout << "Nuevo medio de envio (x para dejar igual): ";
    getline(cin, input);
    if (input != "x" && input != "X" && !input.empty()) v.medio_envio = input;

    cout << "Estado de envio actual: " << v.estado_envio << endl;
    cout << "Nuevo estado de envio (x para dejar igual): ";
    getline(cin, input);
    if (input != "x" && input != "X" && !input.empty()) v.estado_envio = input;

    actualizarHashmapsEliminar(ventas.getDato(idx));
    // ...modificás v...
    ventas.reemplazar(idx, v);
    actualizarHashmapsAgregar(v);
    cout << "Venta modificada correctamente.\n" << endl;
}

/*dinamicas: */
//nueva:
void ventasEnCiudad() {
    string ciudad;
    cout << "\n--- Listado de ventas por ciudad ---" << endl;
    cout << "Ingrese el nombre de la ciudad: ";
    getline(cin >> ws, ciudad);

    bool hayVentas = false;
    cout << left << setw(5) << "ID" << setw(12) << "Fecha" << setw(15) << "Pais" << setw(15) << "Ciudad"
         << setw(15) << "Cliente" << setw(15) << "Producto" << setw(10) << "Cantidad" << setw(10) << "Monto" << endl;
    cout << string(100, '-') << endl;
    for (int i = 0; i < ventas.getTamanio(); i++) {
        Venta v = ventas.getDato(i);
        if (v.ciudad == ciudad) {
            hayVentas = true;
            cout << left << setw(5) << v.id << setw(12) << v.fecha << setw(15) << v.pais << setw(15) << v.ciudad
                 << setw(15) << v.cliente << setw(15) << v.producto << setw(10) << v.cantidad
                 << setw(10) << fixed << setprecision(2) << v.monto_total << endl;
        }
    }
    if (!hayVentas) {
        cout << "No se encontraron ventas para la ciudad ingresada." << endl;
    }
    cout << endl;
}

string normalizarPais(string pais) {
    // Quita espacios y pasa a minúsculas
    string res;
    for (char c : pais) {
        if (!isspace(c)) res += tolower(c);
    }
    return res;
}

string fechaAFormatoOrdenable(string fecha) {
    // Convierte dd/mm/yyyy a yyyymmdd para comparar como string
    if (fecha.length() != 10) return "";
    string dd = fecha.substr(0, 2);
    string mm = fecha.substr(3, 2);
    string yyyy = fecha.substr(6, 4);
    return yyyy + mm + dd;
}

void ventasPorRangoFechasYPais() {
    string pais, fechaInicio, fechaFin;
    cout << "\n--- Listado de ventas por rango de fechas y pais ---" << endl;
    cout << "Ingrese el pais: ";
    getline(cin >> ws, pais);
    cout << "Ingrese la fecha de inicio (dd/mm/yyyy): ";
    getline(cin, fechaInicio);
    cout << "Ingrese la fecha de fin (dd/mm/yyyy): ";
    getline(cin, fechaFin);

    string paisNorm = normalizarPais(pais);
    string fIni = fechaAFormatoOrdenable(fechaInicio);
    string fFin = fechaAFormatoOrdenable(fechaFin);

    bool hayVentas = false;
    cout << left << setw(5) << "ID" << setw(12) << "Fecha" << setw(15) << "Pais" << setw(15) << "Ciudad"
         << setw(15) << "Producto" << setw(10) << "Cantidad" << setw(10) << "Monto" << endl;
    cout << string(80, '-') << endl;
    for (int i = 0; i < ventas.getTamanio(); i++) {
        Venta v = ventas.getDato(i);
        string paisVentaNorm = normalizarPais(v.pais);
        string fechaVenta = fechaAFormatoOrdenable(v.fecha);
        if (paisVentaNorm == paisNorm && fechaVenta >= fIni && fechaVenta <= fFin) {
            hayVentas = true;
            cout << left << setw(5) << v.id << setw(12) << v.fecha << setw(15) << v.pais
                 << setw(15) << v.ciudad << setw(15) << v.producto
                 << setw(10) << v.cantidad
                 << setw(10) << fixed << setprecision(2) << v.monto_total
                 << endl;
        }
    }
    if (!hayVentas) {
        cout << "No se encontraron ventas en el rango de fechas y pais ingresados." << endl;
    }
    cout << endl;
}

void comparacion2paises() {
    string pais1, pais2;
    cout << "\n--- Comparación entre dos países ---" << endl;
    cout << "Ingrese el primer país: ";
    getline(cin >> ws, pais1);
    cout << "Ingrese el segundo país: ";
    getline(cin, pais2);

    auto mostrarDatosPais = [](const string& pais) {
        double montoTotal = 0;
        string productoMasVendido = "";
        int maxCantidad = -1;
        string medioMasUsado = "";
        int maxMedio = -1;

        // Monto total de ventas
        try {
            HashMap<string, double> mapaCiudades = ciudadesPorPais.get(pais);
            Lista<string> ciudades = mapaCiudades.claves();
            for (int i = 0; i < ciudades.getTamanio(); i++) {
                try {
                    montoTotal += mapaCiudades.get(ciudades.getDato(i));
                } catch (...) {}
            }
        } catch (...) {}

        // Producto más vendido (por cantidad)
        try {
            HashMap<string, int> mapaCant = categoriaPorPais.get(pais);
            Lista<string> productos = cantidadPorProducto.claves();
            for (int i = 0; i < productos.getTamanio(); i++) {
                string prod = productos.getDato(i);
                int cantidad = 0;
                // Sumar solo si el producto pertenece al país
                try {
                    HashMap<string, double> mapaProd = productoPorPais.get(pais);
                    mapaProd.get(prod); // Si existe en el país
                    cantidad = cantidadPorProducto.get(prod);
                } catch (...) { continue; }
                if (cantidad > maxCantidad) {
                    maxCantidad = cantidad;
                    productoMasVendido = prod;
                }
            }
        } catch (...) {}

        // Medio de envío más usado
        try {
            HashMap<string, int> mapaMedio = medioPorPais.get(pais);
            Lista<string> medios = mapaMedio.claves();
            for (int i = 0; i < medios.getTamanio(); i++) {
                string medio = medios.getDato(i);
                int cantidad = mapaMedio.get(medio);
                if (cantidad > maxMedio) {
                    maxMedio = cantidad;
                    medioMasUsado = medio;
                }
            }
        } catch (...) {}

        cout << "\n=== País: " << pais << " ===" << endl;
        cout << "Monto total de ventas: $" << fixed << setprecision(2) << montoTotal << endl;
        cout << "Producto más vendido: " << (productoMasVendido.empty() ? "(Sin datos)" : productoMasVendido) << endl;
        cout << "Medio de envío más usado: " << (medioMasUsado.empty() ? "(Sin datos)" : medioMasUsado) << endl;
    };

    mostrarDatosPais(pais1);
    mostrarDatosPais(pais2);
    cout << endl;
}

void comparacion2productos() {
    string prod1, prod2;
    cout << "\n--- Comparación entre dos productos por país ---" << endl;
    cout << "Ingrese el primer producto: ";
    getline(cin >> ws, prod1);
    cout << "Ingrese el segundo producto: ";
    getline(cin, prod2);

    // Normaliza productos para comparar (sin espacios y minúsculas)
    auto normalizar = [](const string& s) {
        string res;
        for (char c : s) if (!isspace(c)) res += tolower(c);
        return res;
    };
    string prod1Norm = normalizar(prod1);
    string prod2Norm = normalizar(prod2);

    // Obtener todos los países
    set<string> paisesUnicos;
    for (int i = 0; i < ventas.getTamanio(); i++)
        paisesUnicos.insert(ventas.getDato(i).pais);

    cout << left << setw(15) << "Pais"
         << setw(20) << prod1 + " (cant)"
         << setw(20) << prod1 + " ($)"
         << setw(20) << prod2 + " (cant)"
         << setw(20) << prod2 + " ($)" << endl;
    cout << string(95, '-') << endl;

    for (const auto& pais : paisesUnicos) {
        int cant1 = 0, cant2 = 0;
        double monto1 = 0, monto2 = 0;

        for (int i = 0; i < ventas.getTamanio(); i++) {
            Venta v = ventas.getDato(i);
            if (v.pais == pais) {
                string vProdNorm = normalizar(v.producto);
                if (vProdNorm == prod1Norm) {
                    cant1 += v.cantidad;
                    monto1 += v.monto_total;
                }
                if (vProdNorm == prod2Norm) {
                    cant2 += v.cantidad;
                    monto2 += v.monto_total;
                }
            }
        }

        if (cant1 > 0 || cant2 > 0 || monto1 > 0 || monto2 > 0) {
            cout << left << setw(15) << pais
                 << setw(20) << cant1
                 << setw(20) << fixed << setprecision(2) << monto1
                 << setw(20) << cant2
                 << setw(20) << fixed << setprecision(2) << monto2 << endl;
        }
    }
    cout << endl;
}

void productosPorUmbralPorPais(const string& modo) {
    string pais;
    double umbral;
    cout << "\n--- Productos vendidos por " << (modo == "debajo" ? "debajo" : "arriba") << " de un monto promedio en un país ---" << endl;
    cout << "Ingrese el país: ";
    getline(cin >> ws, pais);
    cout << "Ingrese el monto umbral: ";
    cin >> umbral;
    cin.ignore();

    HashMap<string, double> sumaPorProducto(200, hashString);
    HashMap<string, int> cantidadPorProductoAux(200, hashString);

    for (int i = 0; i < ventas.getTamanio(); i++) {
        Venta v = ventas.getDato(i);
        if (v.pais == pais) {
            try {
                double sumaActual = sumaPorProducto.get(v.producto);
                int cantidadActual = cantidadPorProductoAux.get(v.producto);
                sumaPorProducto.put(v.producto, sumaActual + v.monto_total);
                cantidadPorProductoAux.put(v.producto, cantidadActual + v.cantidad);
            } catch (...) {
                sumaPorProducto.put(v.producto, v.monto_total);
                cantidadPorProductoAux.put(v.producto, v.cantidad);
            }
        }
    }

    cout << left << setw(25) << "Producto" << right << setw(18) << "Promedio Venta" << endl;
    cout << string(43, '-') << endl;
    bool hay = false;
    Lista<string> productos = sumaPorProducto.claves();
    for (int i = 0; i < productos.getTamanio(); i++) {
        string producto = productos.getDato(i);
        double suma = 0.0;
        int cantidad = 0;
        try { suma = sumaPorProducto.get(producto); } catch (...) { suma = 0.0; }
        try { cantidad = cantidadPorProductoAux.get(producto); } catch (...) { cantidad = 0; }
        double promedio = (cantidad > 0) ? (suma / cantidad) : 0.0;
        if ((modo == "debajo" && promedio < umbral) ||
            (modo == "arriba" && promedio > umbral)) {
            cout << left << setw(25) << producto << right << " $" << fixed << setprecision(2) << setw(15) << promedio << endl;
            hay = true;
        }
    }
    cout << string(43, '-') << endl;
    if (!hay) {
        cout << "No hay productos con promedio " << (modo == "debajo" ? "por debajo" : "por arriba") << " del umbral en ese país." << endl;
    }
    cout << endl;
}

void mostrarMenu() {
    cout << "----- MENU DE ANALISIS DE VENTAS -----" << endl;
    cout << "1. Agregar una venta" << endl;
    cout << "2. Eliminar una venta" << endl;
    cout << "3. Modificar una venta" << endl;
    cout << "4. Consultas dinamicas solicitadas por el usuario" << endl;
    cout << "5. Salir" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    int opcion;
    cargarVentasDesdeCSV("ventas_sudamerica_ascii.csv");
    cout << "\n----- Calculando Estadisticas -----" << endl;
    calcularEstadisticasOptimo(
            ciudadesPorPais,
            productoPorPais,
            categoriaPorPais,
            medioPorPais,
            estadoPorPais,
            medioPorCategoria,
            montoPorFecha,
            cantidadPorProducto
        );
    do {
        cout << "\n----- Mostrando estadisticas -----" << endl;
        calcularEstadisticasVentas();
        cout << "\n----- Finalizada la muestra de estadisticas -----" << endl;
        mostrarMenu();
        cin >> opcion;
        switch (opcion) {
            case 1:
                AgregarVenta();
                
                break;
            case 2:
                eliminarVenta();
                
                break;
            case 3:
                ModificarVenta();
                
                break;
            case 4:
                int subopcion;
                do {
                    mostrarMenuConsultas();
                    cin >> subopcion;
                    switch (subopcion) {
                        case 1:
                            ventasEnCiudad();
                            break;
                        case 2:
                            ventasPorRangoFechasYPais();
                            break;
                        case 3:
                            comparacion2paises();
                            break;
                        case 4:
                            comparacion2productos();
                            break;
                        case 5:
                            productosPorUmbralPorPais("debajo");
                            break;
                        case 6:
                            productosPorUmbralPorPais("arriba");
                            break;
                        case 7:
                            break;
                        default:
                            cout << "Opcion no valida." << endl;
                    }
                } while (subopcion != 7);
                break;
            case 5:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opcion != 5);
    return 0;
}




/*The end*/
