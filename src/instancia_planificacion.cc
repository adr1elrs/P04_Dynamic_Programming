/**
* Archivo: instancia_planificacion.cc
* Descripción: Implementación de la clase InstanciaPlanificacion.
*/

#include "instancia_planificacion.h"

using json = nlohmann::json;

/**
 * @brief Constructor de Carga: Lee el JSON y carga los datos en estructuras compartidas.
 */
InstanciaPlanificacion::InstanciaPlanificacion(const std::string& ruta_fichero) 
  : turnos_(std::make_shared<std::vector<std::string>>()),
    empleados_(std::make_shared<std::vector<std::string>>()),
    dias_libres_requeridos_(std::make_shared<std::vector<int>>()),
    satisfaccion_(std::make_shared<std::vector<std::vector<std::vector<int>>>>()),
    empleados_requeridos_(std::make_shared<std::vector<std::vector<int>>>()),
    dia_inicio_(0) {
  
  std::ifstream f(ruta_fichero);
  if (!f.is_open()) {
    throw std::runtime_error("Error crítico: No se pudo abrir el archivo JSON en " + ruta_fichero);
  }
  
  json data;
  try {
    data = json::parse(f);
  } catch (const json::parse_error& e) {
    throw std::runtime_error(std::string("Error al parsear el JSON: ") + e.what());
  }

  num_dias_ = data["planningHorizon"];

  // 1. Cargar metadatos básicos (Turnos y Empleados)
  for (const auto& t : data["shifts"]) {
    turnos_->push_back(t);
  }
  for (const auto& e : data["employees"]) {
    empleados_->push_back(e["name"]);
    dias_libres_requeridos_->push_back(e["freeDays"]);
  }

  // 2. Redimensionar las matrices tridimensionales y bidimensionales
  satisfaccion_->resize(GetNumEmpleados(), std::vector<std::vector<int>>(num_dias_, std::vector<int>(GetNumTurnos(), 0)));
  empleados_requeridos_->resize(num_dias_, std::vector<int>(GetNumTurnos(), 0));

  // 3. Poblar las matrices desreferenciando los punteros inteligentes
  for (const auto& s : data["satisfaction"]) {
    (*satisfaccion_)[s["employee"]][s["day"]][s["shift"]] = s["value"];
  }

  for (const auto& req : data["requiredEmployees"]) {
    (*empleados_requeridos_)[req["day"]][req["shift"]] = req["value"];
  }
}

/**
 * @brief Constructor de División: Crea una nueva instancia que representa una ventana temporal específica (un conjunto de los días).
 * El costo computacional es O(1) ya que solo se copian los punteros inteligentes, no los datos.
 */
InstanciaPlanificacion::InstanciaPlanificacion(int dia_inicio, int num_dias, const InstanciaPlanificacion& padre)
  : turnos_(padre.turnos_),
    empleados_(padre.empleados_),
    dias_libres_requeridos_(padre.dias_libres_requeridos_),
    satisfaccion_(padre.satisfaccion_),
    empleados_requeridos_(padre.empleados_requeridos_),
    dia_inicio_(dia_inicio), 
    num_dias_(num_dias) {}


/** 
 * @brief Métodos de acceso: Permiten obtener información específica de la instancia.
 * El costo computacional es O(1) para cada método, ya que acceden directamente a los datos sin necesidad de iterar.
 */
int InstanciaPlanificacion::GetSatisfaccion(int empleado, int dia_relativo, int turno) const {
  // Mapeamos el día del subproblema al día real del JSON
  return (*satisfaccion_)[empleado][dia_inicio_ + dia_relativo][turno];
}

/**
 * @brief GetEmpleadosRequeridos: Devuelve el número de empleados requeridos para un turno específico en un día relativo dentro de la ventana temporal.
 * El costo computacional es O(1) ya que accede directamente a la matriz de turnos requeridos.
 */
int InstanciaPlanificacion::GetEmpleadosRequeridos(int dia_relativo, int turno) const {
  return (*empleados_requeridos_)[dia_inicio_ + dia_relativo][turno];
}

/**
 * @brief GetDiasLibresRequeridos: Devuelve el número de días libres requeridos para un empleado específico.
 * El costo computacional es O(1) ya que accede directamente al vector de días libres requeridos.
 */
int InstanciaPlanificacion::GetDiasLibresRequeridos(int empleado) const {
  return (*dias_libres_requeridos_)[empleado];
}

/**
  * @brief GetNombreEmpleado y GetNombreTurno: Devuelven el nombre del empleado o turno dado su índice.
  */
std::string InstanciaPlanificacion::GetNombreEmpleado(int empleado) const {
  return (*empleados_)[empleado];
}

std::string InstanciaPlanificacion::GetNombreTurno(int turno) const {
  return (*turnos_)[turno];
}

/**
 * @brief Print: Imprime un resumen legible de la instancia, incluyendo el contexto temporal, dimensiones del problema, análisis de carga total y una matriz de demanda.
 */
void InstanciaPlanificacion::Print(std::ostream& os) const {
  os << "\n==================================================\n";
  os << "      RESUMEN DE INSTANCIA DE PLANIFICACIÓN       \n";
  os << "==================================================\n";
  
  // 1. Contexto temporal
  os << "Ventana temporal : Día " << dia_inicio_ << " al " << dia_inicio_ + num_dias_ - 1 
    << " (" << num_dias_ << " días en esta división)\n";
  
  // 2. Dimensiones del problema
  os << "Plantilla        : " << GetNumEmpleados() << " empleados registrados.\n";
  os << "Tipos de turno   : " << GetNumTurnos() << " (";
  for (int t = 0; t < GetNumTurnos(); ++t) {
    os << GetNombreTurno(t) << (t < GetNumTurnos() - 1 ? ", " : ")\n");
  }

  // 3. Análisis de carga total
  int demanda_total_subinstancia = 0;
  for (int d = 0; d < num_dias_; ++d) {
    for (int t = 0; t < GetNumTurnos(); ++t) {
      demanda_total_subinstancia += GetEmpleadosRequeridos(d, t);
    }
  }
  
  os << "Carga de trabajo : " << demanda_total_subinstancia 
      << " asignaciones requeridas en esta ventana.\n\n";

  // 4. Matriz de demanda (Mínimo de trabajadores por turno)
  os << "--- Demanda diaria (Trabajadores mínimos por turno) ---\n";
  
  // Calculamos el límite de columnas a mostrar (máximo 10)
  int turnos_a_mostrar = std::min(10, GetNumTurnos());
  
  // Cabecera de la tabla
  os << std::left << std::setw(12) << "Fecha";
  for (int t = 0; t < turnos_a_mostrar; ++t) {
    os << "| " << std::setw(10) << GetNombreTurno(t);
  }
  if (GetNumTurnos() > 10) {
    os << "| " << std::setw(5) << "..."; // Indicador de recorte
  }
  os << "\n--------------------------------------------------\n";

  // Filas de la tabla
  for (int d = 0; d < num_dias_; ++d) {
    // Imprimimos el día real (absoluto)
    os << std::left << std::setw(12) << ("Día " + std::to_string(dia_inicio_ + d));
    
    for (int t = 0; t < turnos_a_mostrar; ++t) {
      // Imprimimos cuántos trabajadores hacen falta
      os << "| " << std::setw(10) << GetEmpleadosRequeridos(d, t);
    }
    
    if (GetNumTurnos() > 10) {
      os << "| " << std::setw(5) << "..."; 
    }
    os << "\n";
  }
  os << "==================================================\n";
}