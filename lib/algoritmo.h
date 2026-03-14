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

#ifndef ALGORITMO_H
#define ALGORITMO_H

#include <iostream>
#include <memory>

#include "solucion.h"
#include "instancia.h"

class Algoritmo {
 public:
  virtual std::shared_ptr<Solucion> solve(std::shared_ptr<Instancia> instancia) = 0;
  virtual ~Algoritmo() = 0;
};

inline Algoritmo::~Algoritmo() { }

#endif