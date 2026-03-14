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
  * Archivo solucion_secuencia.h: 
  */

#ifndef DIVIDE_Y_VENCERAS_H
#define DIVIDE_Y_VENCERAS_H

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include "instancia.h"
#include "solucion.h"
#include "algoritmo.h"


class DivideYVenceras : public Algoritmo {
 public:
  ~DivideYVenceras() noexcept = default;
  std::shared_ptr<Solucion> solve(std::shared_ptr<Instancia> instancia) override;
  // Función virtual para determinar si la instancia es lo suficientemente pequeña para resolver
  virtual bool Small(std::shared_ptr<Instancia> instancia) = 0;
  // Función vitual para resolver cuando es suficientemente pequeña
  virtual std::shared_ptr<Solucion> SolveSmall(std::shared_ptr<Instancia> instancia) = 0;
  // Función virtual para dividir la instancia en subinstancias
  virtual std::pair<std::shared_ptr<Instancia>, std::shared_ptr<Instancia>> Divide(std::shared_ptr<Instancia> instancia) = 0;
  // Función virtual para combinar las subinstancias
  virtual std::shared_ptr<Solucion> Combine(std::shared_ptr<Solucion> solucion_1, std::shared_ptr<Solucion> solucion_2) = 0;
};

#endif