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
  * Archivo planificacion_dyv.cc: 
  */

#include "../lib/instancia_planificacion.h"
#include "../lib/solucion_planificacion.h"
#include "../lib/planificacion_dyv.h"


PlanificacionDyV::PlanificacionDyV(std::shared_ptr<Algoritmo> small_solver, int umbral_small)
  : small_solver_(small_solver), umbral_small_(umbral_small) {}


bool PlanificacionDyV::Small(std::shared_ptr<Instancia> instancia) {
  std::shared_ptr<InstanciaPlanificacion> instancia_dyv = std::dynamic_pointer_cast<InstanciaPlanificacion>(instancia);
  if (!instancia_dyv) return false;
  return (instancia_dyv->GetNumDias() <= umbral_small_);
}


struct EvualuacionTurno {
  int satisfaccion;
  int turno;
  int empleado;
};

// ./experimento_dyv input/instance_horizon7_employees10_shifts6_001.json

std::shared_ptr<Solucion> PlanificacionDyV::SolveSmall(std::shared_ptr<Instancia> instancia) {
  return small_solver_->solve(instancia);
}



std::pair<std::shared_ptr<Instancia>, std::shared_ptr<Instancia>> PlanificacionDyV::Divide(std::shared_ptr<Instancia> instancia) {
  std::shared_ptr<InstanciaPlanificacion> instancia_dyv = std::dynamic_pointer_cast<InstanciaPlanificacion>(instancia);
  if (!instancia_dyv) return {nullptr, nullptr};
  int mitad_dias = instancia_dyv->GetNumDias() / 2;
  auto primera_mitad_instancia = std::make_shared<InstanciaPlanificacion>(instancia_dyv->GetDiaInicio(), mitad_dias, *instancia_dyv);
  auto segunda_mitad_instancia = std::make_shared<InstanciaPlanificacion>(instancia_dyv->GetDiaInicio() + mitad_dias, instancia_dyv->GetNumDias() - mitad_dias, *instancia_dyv);
  return {primera_mitad_instancia, segunda_mitad_instancia};
}


std::shared_ptr<Solucion> PlanificacionDyV::Combine(std::shared_ptr<Solucion> solucion_1, std::shared_ptr<Solucion> solucion_2) {
  // Preparamos ambas soluciones que tendremos que concatenar
  auto solucion_izquierda = std::dynamic_pointer_cast<SolucionPlanificacion>(solucion_1);
  auto solucion_derecha = std::dynamic_pointer_cast<SolucionPlanificacion>(solucion_2);
  if (!solucion_izquierda || !solucion_derecha) return nullptr;
  
  // Creamos la solucion a partir de la izquierda para posteriormente concatenar la derecha
  auto sol_combinada = std::make_shared<SolucionPlanificacion>(*solucion_izquierda);
  sol_combinada->Concatenar(*solucion_derecha);

  sol_combinada->AjustarDescansos();
  
  return sol_combinada;
}

