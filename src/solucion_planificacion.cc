/**
 * @file solucion_planificacion.cc
 * @brief Implementación de la clase SolucionPlanificacion.
 * @author Sergio Rosales Calzadilla
 * @author Adriel Reyes Suárez
 * @date 26/02/2026
 * * @details Gestiona la matriz de asignaciones de turnos y mantiene una caché
 * de orden O(1) para el cálculo instantáneo de la función objetivo.
 */

#include "../lib/solucion_planificacion.h"
#include <iomanip>

/**
 * @brief Constructor. Reserva la memoria exacta para la ventana temporal.
 * @param dia_inicio Día absoluto de inicio.
 * @param num_dias Tamaño de la ventana de tiempo.
 * @param num_empleados Total de empleados en la plantilla.
 * @param num_turnos Total de turnos disponibles por día.
 */
SolucionPlanificacion::SolucionPlanificacion(int dia_inicio, int num_dias, int num_empleados, int num_turnos,int dias_totales,
      std::shared_ptr<std::vector<int>> dias_libres,
      std::shared_ptr<std::vector<std::vector<std::vector<int>>>> satisfaccion,
      std::shared_ptr<std::vector<std::vector<int>>> empleados_requeridos)
    : dia_inicio_(dia_inicio), 
      num_dias_(num_dias),
      num_turnos_(num_turnos),
      suma_satisfaccion_(0),
      turnos_cubiertos_(0),
      dias_totales_(dias_totales),
      dias_libres_(dias_libres),
      satisfaccion_(satisfaccion),
      empleados_requeridos_(empleados_requeridos) {
  
  // Reserva de memoria O(D * T) para evitar realojamientos dinámicos
  asignaciones_.resize(num_dias_, std::vector<std::vector<int>>(num_turnos));
  dias_trabajados_.resize(num_empleados, 0);
}

/**
 * @brief Asigna un empleado a un turno y actualiza la caché en O(1).
 * @param dia_relativo Índice del día dentro de esta subsolución (0 a num_dias_-1).
 * @param turno Identificador del turno.
 * @param empleado Identificador del empleado.
 * @param satisfaccion Valor de satisfacción aportado por esta asignación.
 */
void SolucionPlanificacion::AsignarEmpleado(int dia_relativo, int turno, int empleado, int satisfaccion) {
  asignaciones_[dia_relativo][turno].push_back(empleado);
  dias_trabajados_[empleado]++;
  suma_satisfaccion_ += satisfaccion;
}


void SolucionPlanificacion::DesasignarEmpleado(int dia_relativo, int turno, int empleado, int satisfaccion, int requeridos) {
  auto& lista_trabajadores = asignaciones_[dia_relativo][turno];

  auto it = std::find(lista_trabajadores.begin(), lista_trabajadores.end(), empleado);

  if (it != lista_trabajadores.end()) {
    bool estaba_cubierto = (static_cast<int>(lista_trabajadores.size()) >= requeridos);

    lista_trabajadores.erase(it);

    bool esta_cubierto = (static_cast<int>(lista_trabajadores.size()) >= requeridos);

    if (estaba_cubierto && !esta_cubierto) {
      --turnos_cubiertos_;
    }

    suma_satisfaccion_ -= satisfaccion;
    --dias_trabajados_[empleado];
  }
}

/**
 * @brief Incrementa el contador de turnos que han alcanzado el mínimo requerido.
 */
void SolucionPlanificacion::RegistrarTurnoCubierto() {
  turnos_cubiertos_++;
}

/**
 * @brief Fusiona esta solución con otra posterior cronológicamente.
 * Complejidad temporal lineal respecto al tamaño de la solución entrante.
 * @param otra Objeto SolucionPlanificacion correspondiente al siguiente bloque de días.
 */
void SolucionPlanificacion::Concatenar(const SolucionPlanificacion& otra) {
  // 1. Inserción de las matrices 3D
  asignaciones_.insert(asignaciones_.end(), otra.asignaciones_.begin(), otra.asignaciones_.end());
  
  // 2. Actualización de las dimensiones temporales
  num_dias_ += otra.num_dias_;
  
  // 3. Suma vectorial de los días trabajados por cada empleado
  for (size_t e = 0; e < dias_trabajados_.size(); ++e) {
    dias_trabajados_[e] += otra.dias_trabajados_[e];
  }
  
  // 4. Unificación de la caché de calidad
  suma_satisfaccion_ += otra.suma_satisfaccion_;
  turnos_cubiertos_ += otra.turnos_cubiertos_;
}

/**
 * @brief Recupera la lista de empleados trabajando en un momento dado en O(1).
 * @param dia_relativo Índice del día en la subsolución.
 * @param turno Identificador del turno.
 * @return Referencia constante al vector de empleados asignados.
 */
const std::vector<int>& SolucionPlanificacion::GetAsignaciones(int dia_relativo, int turno) const {
  return asignaciones_[dia_relativo][turno];
}

/**
 * @brief Consulta la carga de trabajo acumulada de un empleado en O(1).
 * @param empleado Identificador del empleado.
 * @return Número de días que ha trabajado en esta ventana temporal.
 */
int SolucionPlanificacion::GetDiasTrabajados(int empleado) const {
  return dias_trabajados_[empleado];
}

/**
 * @brief Consulta si el empleado trabaja en un dia dado.
 * @param dia_relativo Índice del día en la subsolución.
 * @param empleado Identificador del empleado.
 * @return Verdadero si trabaja o falso si no.
 */
bool SolucionPlanificacion::TrabajaEnDia(int dia_relativo, int empleado) const {
  for (int t = 0; t < num_turnos_; ++t) {
    const auto& empleados = asignaciones_[dia_relativo][t];

    if (std::find(empleados.begin(), empleados.end(), empleado) != empleados.end()) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Devuelve el valor de la función objetivo f(x) instantáneamente.
 * @return Puntuación total (Satisfacción + Turnos cubiertos * 100).
 */
int SolucionPlanificacion::GetCalidad() const {
  return suma_satisfaccion_ + (turnos_cubiertos_ * 100);
}

/**
 * @brief Imprime la matriz de horarios por consola de forma tabulada.
 * @param os Flujo de salida (ej. std::cout).
 */
/**
void SolucionPlanificacion::Print(std::ostream& os) const {
  os << "\n--- Planificación de Empleados (Días " << dia_inicio_ << " a "
     << (dia_inicio_ + num_dias_ - 1) << ") ---\n";
  os << "Calidad de la solución: " << GetCalidad() << " (Satisfacción: "
     << suma_satisfaccion_ << ", Turnos cubiertos: " << turnos_cubiertos_ << ")\n\n";

  if (asignaciones_.empty() || asignaciones_[0].empty()) return;

  // Extraemos las dimensiones
  int num_turnos = asignaciones_[0].size();
  
  // Definimos una caja rígida para todas las columnas
  const int ANCHO_COLUMNA = 10; 

  // 1. Imprimir la cabecera
  os << std::left << std::setw(6) << "Día";
  for (int t = 0; t < num_turnos; ++t) {
    std::string header = "Turno " + std::to_string(t);
    os << "| " << std::left << std::setw(ANCHO_COLUMNA) << header;
  }
  os << "|\n"; 

  // 2. Imprimir la línea separadora adaptativa (calcula el ancho total matemáticamente)
  int ancho_total = 6 + (ANCHO_COLUMNA + 2) * num_turnos + 1;
  os << std::string(ancho_total, '-') << "\n";

  // 3. Imprimir el contenido de la matriz
  for (int d = 0; d < num_dias_; ++d) {
    // Imprimimos el número del día absoluto
    os << std::left << std::setw(6) << (dia_inicio_ + d);
    
    for (int t = 0; t < num_turnos; ++t) {
      const auto& asignados = asignaciones_[d][t];
      std::string celda = "";
      
      if (asignados.empty()) {
        // Quitamos la tilde para evitar el desbordamiento de bytes en std::setw()
        celda = "Vacio"; 
      } else {
        // Si hay varios empleados en el mismo turno, los unimos por comas "1, 4"
        for (size_t i = 0; i < asignados.size(); ++i) {
          celda += "e" + std::to_string(asignados[i]);
          if (i < asignados.size() - 1) celda += ", ";
        }
      }
      
      // Imprimimos la celda ya construida, forzándola a medir exactamente ANCHO_COLUMNA
      os << "| " << std::left << std::setw(ANCHO_COLUMNA) << celda;
    }
    os << "|\n"; // Cerramos la última columna de la fila
  }

  // ==========================================
  // FASE 2: RESUMEN GLOBAL Y AUDITORÍA
  // ==========================================
  // --- Cálculos extra para el resumen global ---
  int total_turnos_ventana = num_dias_ * num_turnos_;
  int total_asignaciones_objetivo = 0;
  int asignaciones_actuales = 0;

  for (int d = 0; d < num_dias_; ++d) {
    int dia_absoluto = dia_inicio_ + d;
    for (int t = 0; t < num_turnos_; ++t) {
      total_asignaciones_objetivo += (*empleados_requeridos_)[dia_absoluto][t];
      asignaciones_actuales += asignaciones_[d][t].size();
    }
  }
  // ---------------------------------------------
  os << "\n==================================================\n";
  os << "              RESUMEN GLOBAL                      \n";
  os << "==================================================\n";
  os << "Días planificados: " << dia_inicio_ << " a " << (dia_inicio_ + num_dias_ - 1) << "\n";
  os << "Calidad de la solución: " << GetCalidad() << "\n";
  os << "  -> Satisfacción acumulada: " << suma_satisfaccion_ << "\n";
  os << "  -> Turnos cubiertos: " << turnos_cubiertos_ << " de " << total_turnos_ventana 
     << " (Multiplicador x100)\n";
  os << "  -> Volumen de plantilla: " << asignaciones_actuales << " asignaciones hechas de " 
     << total_asignaciones_objetivo << " requeridas en total\n";
  os << "--------------------------------------------------\n";
  os << "Auditoría de Descansos (Ventana actual: " << num_dias_ << " días):\n";

  for (int e = 0; e < num_empleados; ++e) {
    // Calculamos el límite local proporcional idéntico al que usa el Combine
    double ratio_trabajo = 1.0 - (static_cast<double>((*dias_libres_)[e]) / static_cast<double>(dias_totales_));
    int limite_local = std::ceil(ratio_trabajo * num_dias_);

    os << "Empleado " << std::left << std::setw(3) << e << ": " 
       << std::setw(3) << dias_trabajados_[e] << " días trabajados ";

    if (dias_trabajados_[e] <= limite_local) {
      os << "[OK - Máx permitido: " << limite_local << "]\n";
    } else {
      os << "[EXCEDE - Máx permitido: " << limite_local << "]\n";
    }
  }
  os << "==================================================\n\n";
}
*/

void SolucionPlanificacion::Print(std::ostream& os) const {
  if (asignaciones_.empty() || asignaciones_[0].empty()) return;

  int num_empleados = dias_trabajados_.size();
  const int ANCHO_COLUMNA = 10;
  const int ANCHO_EMPLEADO = 15;

  // ==========================================
  // FASE 1: DESGLOSE DÍA A DÍA (Mapa Visual)
  // ==========================================
  for (int d = 0; d < num_dias_; ++d) {
    int dia_absoluto = dia_inicio_ + d;
    os << "\n=== DÍA " << dia_absoluto << " ===\n";

    // 1. Cabecera de turnos
    os << std::left << std::setw(ANCHO_EMPLEADO) << ""; // Hueco vacío esquina superior izquierda
    for (int t = 0; t < num_turnos_; ++t) {
      os << std::left << std::setw(ANCHO_COLUMNA) << ("Turno " + std::to_string(t));
    }
    os << "\n";

    // 2. Imprimir matriz cruzada: Filas (Empleados) x Columnas (Turnos)
    for (int e = 0; e < num_empleados; ++e) {
      os << std::left << std::setw(ANCHO_EMPLEADO) << ("Empleado " + std::to_string(e));

      for (int t = 0; t < num_turnos_; ++t) {
        const auto& asignados = asignaciones_[d][t];
        
        // Comprobamos si el empleado 'e' está en el vector de este turno
        bool trabaja = (std::find(asignados.begin(), asignados.end(), e) != asignados.end());

        // Imprimimos el bloque o el guion. 
        // Nota: Añadimos espacios manuales porque '█' suele ocupar varios bytes en UTF-8 
        // y a veces despista al std::setw en algunas terminales.
        if (trabaja) {
          os << "█         "; 
        } else {
          os << "-         ";
        }
      }
      os << "\n";
    }
  }

  // ==========================================
  // FASE 2: RESUMEN GLOBAL Y AUDITORÍA
  // ==========================================
  // --- Cálculos extra para el resumen global ---
  int total_turnos_ventana = num_dias_ * num_turnos_;
  int total_asignaciones_objetivo = 0;
  int asignaciones_actuales = 0;

  for (int d = 0; d < num_dias_; ++d) {
    int dia_absoluto = dia_inicio_ + d;
    for (int t = 0; t < num_turnos_; ++t) {
      total_asignaciones_objetivo += (*empleados_requeridos_)[dia_absoluto][t];
      asignaciones_actuales += asignaciones_[d][t].size();
    }
  }
  // ---------------------------------------------
  os << "\n==================================================\n";
  os << "              RESUMEN GLOBAL                      \n";
  os << "==================================================\n";
  os << "Días planificados: " << dia_inicio_ << " a " << (dia_inicio_ + num_dias_ - 1) << "\n";
  os << "Calidad de la solución: " << GetCalidad() << "\n";
  os << "  -> Satisfacción acumulada: " << suma_satisfaccion_ << "\n";
  os << "  -> Turnos cubiertos: " << turnos_cubiertos_ << " de " << total_turnos_ventana 
     << " (Multiplicador x100)\n";
  os << "  -> Volumen de plantilla: " << asignaciones_actuales << " asignaciones hechas de " 
     << total_asignaciones_objetivo << " requeridas en total\n";
  os << "--------------------------------------------------\n";
  os << "Auditoría de Descansos (Ventana actual: " << num_dias_ << " días):\n";

  for (int e = 0; e < num_empleados; ++e) {
    // Calculamos el límite local proporcional idéntico al que usa el Combine
    double ratio_trabajo = 1.0 - (static_cast<double>((*dias_libres_)[e]) / static_cast<double>(dias_totales_));
    int limite_local = std::ceil(ratio_trabajo * num_dias_);

    os << "Empleado " << std::left << std::setw(3) << e << ": " 
       << std::setw(3) << dias_trabajados_[e] << " días trabajados ";

    if (dias_trabajados_[e] <= limite_local) {
      os << "[OK - Máx permitido: " << limite_local << "]\n";
    } else {
      os << "[EXCEDE - Máx permitido: " << limite_local << "]\n";
    }
  }
  os << "==================================================\n\n";
}

/**
 * @brief Puesto que vamos a implementar Varios algoritmos para resolver varios días
 *        para no tener que estar definiendoen cada uno de ellos la lógica del ajuste de descansos
 *        (violando el principio DRY), encapsulamos dicha lógica en la propia solución, así únicamente
 *        tendremos que llamar al método. Todo cambio que se deba hacer únicamente tendremos que 
 *        modificar 1 código
 */
void SolucionPlanificacion::AjustarDescansos() {
  int num_empleados = GetNumEmpleados();
  int dias_totales = GetDiasTotales();

  for (int e = 0; e < num_empleados; ++e) {
    double ratio_trabajo_empleado = 1.0 - (static_cast<double>(GetDiasLibresExigidos(e)) / static_cast<double>(dias_totales));
    double maximo_dias_trabajados_en_llamada_actual = std::ceil(ratio_trabajo_empleado * GetNumDias());

    // Si el empleado trabaja más de lo permitido en esta ventana de días
    while (GetDiasTrabajados(e) > maximo_dias_trabajados_en_llamada_actual) {
      int peor_dia_relativo = -1;
      int peor_turno = -1;
      int peor_sat = 999999;

      // 1. Encontrar el turno donde el empleado aporta la menor satisfacción
      for (int d = 0; d < GetNumDias(); ++d) {
        for (int t = 0; t < GetNumTurnos(); ++t) {
          const auto& empleados_asignados = GetAsignaciones(d, t);
          
          if (std::find(empleados_asignados.begin(), empleados_asignados.end(), e) != empleados_asignados.end()) {
            int dia_absoluto = GetDiaInicio() + d;
            int sat = GetSatisfaccion(e, dia_absoluto, t);
            
            if (sat < peor_sat) {
              peor_dia_relativo = d;
              peor_turno = t;
              peor_sat = sat;
            }
          }
        }
      }

      if (peor_dia_relativo != -1) {
        // 2. Buscar un empleado sustituto óptimo
        std::vector<std::pair<int,int>> posibles_empleados_sustitutos;
        
        for (int e_s = 0; e_s < num_empleados; ++e_s) {
          int dias_trabajados_e_s = GetDiasTrabajados(e_s);
          double ratio_trabajo_empleado_s = 1.0 - (static_cast<double>(GetDiasLibresExigidos(e_s)) / static_cast<double>(dias_totales));
          double maximo_dias_trabajados_llamada_actual_e_s = std::ceil(ratio_trabajo_empleado_s * GetNumDias());

          if (!TrabajaEnDia(peor_dia_relativo, e_s) && ((dias_trabajados_e_s + 1) <= maximo_dias_trabajados_llamada_actual_e_s)) {
            int satisfaccion_en_dia = GetSatisfaccion(e_s, GetDiaInicio() + peor_dia_relativo, peor_turno);
            posibles_empleados_sustitutos.push_back({e_s, satisfaccion_en_dia});
          }
        }

        // Ordenamos sustitutos de mayor a menor satisfacción
        std::sort(posibles_empleados_sustitutos.begin(), posibles_empleados_sustitutos.end(),
                  [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                    return a.second > b.second;
                  });

        // 3. Asignar al nuevo y desasignar al antiguo
        if (posibles_empleados_sustitutos.size() > 0) {
          AsignarEmpleado(peor_dia_relativo, peor_turno, posibles_empleados_sustitutos[0].first, posibles_empleados_sustitutos[0].second);
        }

        int dia_absoluto = GetDiaInicio() + peor_dia_relativo;
        int requeridos = GetRequeridos(dia_absoluto, peor_turno);
        DesasignarEmpleado(peor_dia_relativo, peor_turno, e, peor_sat, requeridos);
        
      } else {
        // Mecanismo de seguridad para evitar bucles infinitos
        break;
      }
    }
  }
}
