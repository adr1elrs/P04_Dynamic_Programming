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

#ifndef PLANIFICACION_DYV
#define PLANIFICACION_DYV

#include "divide_y_venceras.h"

class PlanificacionDyV : public DivideYVenceras {
 public:
  PlanificacionDyV(std::shared_ptr<Algoritmo> small_solver, int umbral_small = 1);
  
  bool Small(std::shared_ptr<Instancia> instancia) override;
  // Función vitual para resolver cuando es suficientemente pequeña
  std::shared_ptr<Solucion> SolveSmall(std::shared_ptr<Instancia> instancia) override;
  // Función virtual para dividir la instancia en subinstancias
  std::pair<std::shared_ptr<Instancia>, std::shared_ptr<Instancia>> Divide(std::shared_ptr<Instancia> instancia) override;
  // Función virtual para combinar las subinstancias
  std::shared_ptr<Solucion> Combine(std::shared_ptr<Solucion> solucion_1, std::shared_ptr<Solucion> solucion_2) override;

 private:
  std::shared_ptr<Algoritmo> small_solver_; // Algoritmo inyectado para resolver el caso pequeño
  int umbral_small_; // Umbral que determinará cuando llamar a 'small_solver_'

};

#endif