/**
  * Archivo: instancia_planificacion.h
  * Descripción: Declaración de la clase InstanciaPlanificacion para el problema
  * de asignación de turnos mediante Divide y Vencerás.
  */

#ifndef INSTANCIA_PLANIFICACION_H
#define INSTANCIA_PLANIFICACION_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <iomanip>
#include "instancia.h"

/**
 * @brief Representa una instancia del problema de planificación.
* * En lugar de duplicar matrices pesadas en cada llamada recursiva, utiliza 
* punteros inteligentes compartidos (std::shared_ptr) para que todas las 
* subinstancias accedan a la misma memoria, ajustando únicamente su "ventana" 
* de tiempo mediante dia_inicio_ y num_dias_.
*/
class InstanciaPlanificacion : public Instancia {
 public:
  /**
    * @brief Constructor Raíz. 
    * Carga el JSON, reserva la memoria principal y la puebla.
    * @param ruta_fichero Ruta al archivo JSON con los datos del problema.
    */
  InstanciaPlanificacion(const std::string& ruta_fichero);
  
  /**
    * @brief Constructor de Subinstancia (Para la fase Divide).
    * Copia los punteros inteligentes (O(1)) y ajusta la ventana de tiempo.
    * @param dia_inicio Desplazamiento del día inicial de esta división.
    * @param num_dias Tamaño del bloque de días.
    * @param padre Instancia de la que se heredan las referencias a los datos.
    */
  InstanciaPlanificacion(int dia_inicio, int num_dias, const InstanciaPlanificacion& padre);

  /**
    * @brief Imprime la información básica de la instancia por consola.
    */
  void Print(std::ostream& os) const override;

  // Getters
  int GetNumDias() const { return num_dias_; }
  int GetDiaInicio() const { return dia_inicio_; }
  int GetNumEmpleados() const { return empleados_->size(); }
  int GetNumTurnos() const { return turnos_->size(); }
  const std::vector<int>& GetCapacidadesTurnosDia(int dia_relativo) const { return (*empleados_requeridos_)[dia_inicio_ + dia_relativo]; }
  
  int GetDiasTotales() const { return empleados_requeridos_->size(); }
  std::shared_ptr<std::vector<int>> GetPtrDiasLibres() const { return dias_libres_requeridos_; }
  std::shared_ptr<std::vector<std::vector<std::vector<int>>>> GetPtrSatisfaccion() const { return satisfaccion_; }
  std::shared_ptr<std::vector<std::vector<int>>> GetPtrEmpleadosRequeridos() const { return empleados_requeridos_; }
  

  int GetSatisfaccion(int empleado, int dia_relativo, int turno) const;
  int GetEmpleadosRequeridos(int dia_relativo, int turno) const;
  int GetDiasLibresRequeridos(int empleado) const;
  std::string GetNombreEmpleado(int empleado) const;
  std::string GetNombreTurno(int turno) const;

 private:
  // Punteros compartidos hacia los datos globales (Heap).
  std::shared_ptr<std::vector<std::string>> turnos_;
  std::shared_ptr<std::vector<std::string>> empleados_;
  std::shared_ptr<std::vector<int>> dias_libres_requeridos_;
  
  // Matriz 3D: [empleado][dia_absoluto][turno]
  std::shared_ptr<std::vector<std::vector<std::vector<int>>>> satisfaccion_;
  
  // Matriz 2D: [dia_absoluto][turno]
  std::shared_ptr<std::vector<std::vector<int>>> empleados_requeridos_;

  int dia_inicio_; ///< Día absoluto en el que comienza esta subinstancia.
  int num_dias_;   ///< Cantidad de días que abarca esta subinstancia.
};

#endif 