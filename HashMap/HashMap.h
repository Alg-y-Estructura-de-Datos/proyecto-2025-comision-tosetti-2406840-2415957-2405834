#ifndef U05_HASH_HASHMAP_HASHMAP_H_
#define U05_HASH_HASHMAP_HASHMAP_H_

#include "HashEntry.h"
#include <iostream>

using namespace std;

struct Venta {
    string id;
    string fecha;
    string pais;
    string ciudad;
    string cliente;
    string producto;
    string categoria;
    int cantidad;
    float precio_unitario;
    float monto_total;
    string medio_envio;
    string estado_envio;
};

template <class K, class T>
class HashMap
{
private:
  HashEntry<K, T> **tabla;

  unsigned int tamanio;

  static unsigned int hashFunc(K clave);

  unsigned int (*hashFuncP)(K clave);

public:

  HashMap(const HashMap<K, T>& otro);

  HashMap<K, T>& operator=(const HashMap<K, T>& otro);

  HashMap();

  explicit HashMap(unsigned int k);

  HashMap(unsigned int k, unsigned int (*hashFuncP)(K clave));

  T get(K clave) const;

  void put(K clave, T valor);

  void remove(K clave);

  ~HashMap();

  bool esVacio();

  void print();

  Lista<K> claves() const;
};

// Constructor de copia
template <class K, class T>
HashMap<K, T>::HashMap(const HashMap<K, T>& otro) {
    tamanio = otro.tamanio;
    tabla = new HashEntry<K, T>*[tamanio];
    for (int i = 0; i < tamanio; i++) {
        if (otro.tabla[i] != NULL) {
            tabla[i] = new HashEntry<K, T>(*otro.tabla[i]);
        } else {
            tabla[i] = NULL;
        }
    }
    hashFuncP = otro.hashFuncP;
}

// Operador de asignación
template <class K, class T>
HashMap<K, T>& HashMap<K, T>::operator=(const HashMap<K, T>& otro) {
    if (this == &otro) return *this;
    for (int i = 0; i < tamanio; i++) {
        if (tabla[i] != NULL) delete tabla[i];
    }
    delete[] tabla;
    tamanio = otro.tamanio;
    tabla = new HashEntry<K, T>*[tamanio];
    for (int i = 0; i < tamanio; i++) {
        if (otro.tabla[i] != NULL) {
            tabla[i] = new HashEntry<K, T>(*otro.tabla[i]);
        } else {
            tabla[i] = NULL;
        }
    }
    hashFuncP = otro.hashFuncP;
    return *this;
}

template <class K, class T>
HashMap<K, T>::HashMap() {
    tamanio = 1000;
    tabla = new HashEntry<K, T> *[tamanio];
    for (int i = 0; i < tamanio; i++) {
        tabla[i] = NULL;
    }
    hashFuncP = hashFunc;
}

template <class K, class T>
HashMap<K, T>::HashMap(unsigned int k)
{
  tamanio = k;
  tabla = new HashEntry<K, T> *[tamanio];
  for (int i = 0; i < tamanio; i++)
  {
    tabla[i] = NULL;
  }
  hashFuncP = hashFunc;
}

template <class K, class T>
HashMap<K, T>::HashMap(unsigned int k, unsigned int (*fp)(K))
{
  tamanio = k;
  tabla = new HashEntry<K, T> *[tamanio];
  for (int i = 0; i < tamanio; i++)
  {
    tabla[i] = NULL;
  }
  hashFuncP = fp;
}

template <class K, class T>
HashMap<K, T>::~HashMap()
{
  for (int i = 0; i < tamanio; i++)
  {
    if (tabla[i] != NULL)
    {
      delete tabla[i];
    }
  }
}

template <class K, class T>
T HashMap<K, T>::get(K clave) const
{
    unsigned int pos = hashFuncP(clave) % tamanio;
    unsigned int originalPos = pos;
    while (tabla[pos] != NULL) {
        if (tabla[pos]->getClave() == clave) {
            return tabla[pos]->getValor();
        }
        pos = (pos + 1) % tamanio;
        if (pos == originalPos) break; // Volvió al inicio, no está
    }
    throw 404; // No encontrado
}

template <class K, class T>
void HashMap<K, T>::put(K clave, T valor)
{
    unsigned int pos = hashFuncP(clave) % tamanio;
    unsigned int originalPos = pos;
    while (tabla[pos] != NULL && tabla[pos]->getClave() != clave) {
        pos = (pos + 1) % tamanio;
        if (pos == originalPos) throw 409; // Tabla llena
    }
    if (tabla[pos] != NULL) {
        tabla[pos]->setValor(valor); // Actualiza si ya existe
    } else {
        tabla[pos] = new HashEntry<K, T>(clave, valor);
    }
}

template <class K, class T>
void HashMap<K, T>::remove(K clave)
{
    unsigned int pos = hashFuncP(clave) % tamanio;
    unsigned int originalPos = pos;
    while (tabla[pos] != NULL) {
        if (tabla[pos]->getClave() == clave) {
            delete tabla[pos];
            tabla[pos] = NULL;
            return;
        }
        pos = (pos + 1) % tamanio;
        if (pos == originalPos) break;
    }
}

template <class K, class T>
bool HashMap<K, T>::esVacio()
{
  for (int i = 0; i < tamanio; i++)
  {
    if (tabla[i] != NULL)
    {
      return false;
    }
  }
  return true;
}

template <class K, class T>
Lista<K> HashMap<K, T>::claves() const
{
    Lista<K> listaClaves;
    for (int i = 0; i < tamanio; i++) {
        if (tabla[i] != NULL) {
            listaClaves.insertarUltimo(tabla[i]->getClave());
        }
    }
    return listaClaves;
}

// Hash por defecto para int
template<>
unsigned int HashMap<int, int>::hashFunc(int clave) {
    return (unsigned int)clave;
}

template<>
unsigned int HashMap<string, int>::hashFunc(string clave) {
    unsigned int hash = 0;
    for (unsigned char c : clave) {
        hash = hash * 31 + c;
    }
    return hash;
}

// Agregar estas especializaciones al final de HashMap.h, después de las existentes:

// Especialización para HashMap<string, double>
template<>
unsigned int HashMap<string, double>::hashFunc(string clave) {
    unsigned int hash = 0;
    for (unsigned char c : clave) {
        hash = hash * 31 + c;
    }
    return hash;
}

// Especialización para HashMap<string, Lista<Venta>>
template<>
unsigned int HashMap<string, Lista<Venta>>::hashFunc(string clave) {
    unsigned int hash = 0;
    for (unsigned char c : clave) {
        hash = hash * 31 + c;
    }
    return hash;
}

#endif