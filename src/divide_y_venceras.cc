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
  * Archivo divide_y_venceras.cc: 
  */

#include "../lib/divide_y_venceras.h"

std::shared_ptr<Solucion> DivideYVenceras::solve(std::shared_ptr<Instancia> instancia) {
  if (Small(instancia)) {
    return SolveSmall(instancia);
  } else {
    std::pair<std::shared_ptr<Instancia>, std::shared_ptr<Instancia>> partes = Divide(instancia);
    std::shared_ptr<Solucion> solucion_izquierda = solve(partes.first);
    std::shared_ptr<Solucion> solucion_derecha = solve(partes.second);
    std::shared_ptr<Solucion> solucion_combinada = Combine(solucion_izquierda, solucion_derecha);
    return solucion_combinada;
  }
}
