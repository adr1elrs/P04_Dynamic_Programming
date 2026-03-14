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
  * Archivo solucion_planificacion.h: 
  */

/**
 * Archivo: solucion_planificacion.h
 * Descripción: Declaración de la clase SolucionPlanificacion para el problema
 * de asignación de turnos mediante Divide y Vencerás.
 */

#ifndef SOLUCION_PLANIFICACION_H
#define SOLUCION_PLANIFICACION_H

#include <vector>
#include <iostream>
#include <memory>
#include "solucion.h" 
#include "instancia_planificacion.h"

/**
 * @brief Representa una solución (o subsolución) del problema de planificación.
 * Almacena las asignaciones y mantiene un estado interno (caché) para que 
 * las operaciones de mezcla en el DyV sean eficientes.
 */
class SolucionPlanificacion : public Solucion {
 public:
  /**
   * @brief Constructor. Inicializa las estructuras vacías para la ventana de tiempo.
   */
  SolucionPlanificacion(int dia_inicio, int num_dias, int num_empleados, int num_turnos, int dias_totales_, 
      std::shared_ptr<std::vector<int>> dias_libres,
      std::shared_ptr<std::vector<std::vector<std::vector<int>>>> satisfaccion,
      std::shared_ptr<std::vector<std::vector<int>>> empleados_requeridos);
  
  // Asigna un empleado a un turno específico 
  void AsignarEmpleado(int dia_relativo, int turno, int empleado, int satisfaccion);
  
  void DesasignarEmpleado(int dia_relativo, int turno, int empleado, int satisfaccion, int requeridos);
  // Aumenta el valor de los turnos cubiertos
  void RegistrarTurnoCubierto();
  // Devuelve la lista de empleados que trabajan en el día y turno especificados
  const std::vector<int>& GetAsignaciones(int dia_relativo, int turno) const;
  
  
  // Cuántos días el empleado ha trabajado
  int GetDiasTrabajados(int empleado) const;

  // Devuelve true si el empleado trabaja en ese día (en cualquier turno)
  bool TrabajaEnDia(int dia_relativo, int empleado) const;
  // Calcula la función objetivo
  int GetCalidad() const;
  
  int GetDiaInicio() const { return dia_inicio_; }
  
  int GetNumDias() const { return num_dias_; }
  int GetNumTurnos() const { return num_turnos_; }

  int GetDiasTotales() const { return dias_totales_; }
  int GetNumEmpleados() const { return dias_trabajados_.size(); }
  int GetDiasLibresExigidos(int empleado) const { return (*dias_libres_)[empleado]; }
  int GetSatisfaccion(int empleado, int dia_absoluto, int turno) const { return (*satisfaccion_)[empleado][dia_absoluto][turno]; }
  int GetRequeridos(int dia_absoluto, int turno) const { return (*empleados_requeridos_)[dia_absoluto][turno]; }
  void Print(std::ostream& os) const override;
  void Concatenar(const SolucionPlanificacion& otra);

  // Ajusta a los empleados para que continúen teniendo los días de descanso que merecen
  void AjustarDescansos();

 private:
  int dia_inicio_; ///< Día absoluto en el que comienza esta subsolución.
  int num_dias_;   ///< Cantidad de días que abarca.
  int num_turnos_;  ///< Número de turnos por día
  // Matriz 3D: [dia_relativo][turno] -> Lista de IDs de empleados asignados
  // Se usa un vector interno porque un turno puede requerir múltiples empleados.
  std::vector<std::vector<std::vector<int>>> asignaciones_;

  // Vector: [ID_empleado] -> Cuántos días ha trabajado en esta subsolución
  std::vector<int> dias_trabajados_;

  // Caché para la función objetivo f(x)
  int suma_satisfaccion_;
  int turnos_cubiertos_;

  int dias_totales_; 
  std::shared_ptr<std::vector<int>> dias_libres_;
  std::shared_ptr<std::vector<std::vector<std::vector<int>>>> satisfaccion_;
  std::shared_ptr<std::vector<std::vector<int>>> empleados_requeridos_;
};

#endif