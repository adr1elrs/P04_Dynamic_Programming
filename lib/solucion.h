/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Diseño y Análisis de Algoritmos 2025-2026
  * Curso: 3º
  * Práctica 3 - Experimentación DYV
  * 
  * @author Sergio Rosales Calzadilla
  * @author Adriel Reyes Suárez
  * @date 26/02/2026
  * Archivo instruccion.h: 
  */

#ifndef SOLUCION_H
#define SOLUCION_H

#include <iostream>

class Solucion {
 public:
  virtual ~Solucion() noexcept = 0;
  virtual void Print(std::ostream& os) const = 0;
  friend std::ostream& operator<<(std::ostream& os, const Solucion& solucion) {
    solucion.Print(os);
    return os;
  }
};

inline Solucion::~Solucion() { }

#endif