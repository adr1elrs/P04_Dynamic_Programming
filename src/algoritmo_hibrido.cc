#include "../lib/algoritmo_hibrido.h"
#include "../lib/instancia_planificacion.h"
#include "../lib/solucion_planificacion.h"
#include <vector>
#include <algorithm>

struct EvaluacionTurno {
  int satisfaccion;
  int turno;
  int empleado;
};

std::shared_ptr<Solucion> AlgoritmoHibrido::solve(std::shared_ptr<Instancia> instancia) {
  auto inst_dyv = std::dynamic_pointer_cast<InstanciaPlanificacion>(instancia);
  if (!inst_dyv) return nullptr;

  int num_empleados = inst_dyv->GetNumEmpleados();
  int num_turnos = inst_dyv->GetNumTurnos();
  int num_dias = inst_dyv->GetNumDias();

  auto solucion = std::make_shared<SolucionPlanificacion>(
      inst_dyv->GetDiaInicio(), num_dias, num_empleados, num_turnos,
      inst_dyv->GetDiasTotales(),
      inst_dyv->GetPtrDiasLibres(),
      inst_dyv->GetPtrSatisfaccion(),
      inst_dyv->GetPtrEmpleadosRequeridos());

  // Resolvemos día por día
  for (int d = 0; d < num_dias; ++d) {
    const std::vector<int>& empleados_requeridos = inst_dyv->GetCapacidadesTurnosDia(d);
    std::vector<bool> empleado_ocupado(num_empleados, false);
    std::vector<EvaluacionTurno> satisfacciones_ordenadas;

    for (int e = 0; e < num_empleados; ++e) {
      for (int t = 0; t < num_turnos; ++t) {
        satisfacciones_ordenadas.push_back({inst_dyv->GetSatisfaccion(e, d, t), t, e});
      }
    }

    std::sort(satisfacciones_ordenadas.begin(), satisfacciones_ordenadas.end(),
              [](const EvaluacionTurno& a, const EvaluacionTurno& b) {
                return a.satisfaccion > b.satisfaccion;
              });

    std::vector<int> empleados_pendientes_por_turno = empleados_requeridos;
    int suma_total_empleados_requeridos = 0;
    for (int req : empleados_requeridos) suma_total_empleados_requeridos += req;

    int personas_trabajando = 0;
    for (size_t s = 0; s < satisfacciones_ordenadas.size(); ++s) {
      int e = satisfacciones_ordenadas[s].empleado;
      int t = satisfacciones_ordenadas[s].turno;
      int sat = satisfacciones_ordenadas[s].satisfaccion;

      if (!empleado_ocupado[e] && empleados_pendientes_por_turno[t] > 0) {
        solucion->AsignarEmpleado(d, t, e, sat);
        empleado_ocupado[e] = true;
        --empleados_pendientes_por_turno[t];

        if (empleados_pendientes_por_turno[t] == 0) {
          solucion->RegistrarTurnoCubierto();
        }
        
        ++personas_trabajando;
      }

      if (personas_trabajando == suma_total_empleados_requeridos) break;
    }
    bool mejora = true;
    while (mejora) {
      mejora = false;
      
      // Comparamos pares de turnos en el mismo día
      for (int t1 = 0; t1 < num_turnos; ++t1) {
        for (int t2 = t1 + 1; t2 < num_turnos; ++t2) {
          
          int req_t1 = inst_dyv->GetEmpleadosRequeridos(d, t1);
          int req_t2 = inst_dyv->GetEmpleadosRequeridos(d, t2);
          if (req_t1 == 0 || req_t2 == 0) continue;

          // Hacemos copia de los asignados para poder iterar sin que se rompa al desasignar
          std::vector<int> emp_t1 = solucion->GetAsignaciones(d, t1);
          std::vector<int> emp_t2 = solucion->GetAsignaciones(d, t2);

          for (int e1 : emp_t1) {
            for (int e2 : emp_t2) {
              if (e1 == e2) continue; // Por seguridad

              int sat_e1_t1 = inst_dyv->GetSatisfaccion(e1, d, t1);
              int sat_e2_t2 = inst_dyv->GetSatisfaccion(e2, d, t2);
              int sat_actual = sat_e1_t1 + sat_e2_t2;

              int sat_e1_t2 = inst_dyv->GetSatisfaccion(e1, d, t2);
              int sat_e2_t1 = inst_dyv->GetSatisfaccion(e2, d, t1);
              int sat_cruzada = sat_e1_t2 + sat_e2_t1;

              // Si el intercambio mejora la satisfacción, lo aplicamos
              if (sat_cruzada > sat_actual) {
                // Desasignar (pasando todos tus argumentos requeridos)
                solucion->DesasignarEmpleado(d, t1, e1, sat_e1_t1, req_t1);
                solucion->DesasignarEmpleado(d, t2, e2, sat_e2_t2, req_t2);
                
                // Asignar cruzados
                solucion->AsignarEmpleado(d, t1, e2, sat_e2_t1);
                solucion->AsignarEmpleado(d, t2, e1, sat_e1_t2);

                int asignaciones_actuales_t1 = solucion->GetAsignaciones(d, t1).size();
                if (asignaciones_actuales_t1 == req_t1) {
                  solucion->RegistrarTurnoCubierto();
                }

                int asignaciones_actuales_t2 = solucion->GetAsignaciones(d, t2).size();
                if (asignaciones_actuales_t2 == req_t2) {
                  solucion->RegistrarTurnoCubierto();
                }
                
                mejora = true;
                break; // Rompemos para reiniciar la búsqueda con el estado nuevo
              }
            }
            if (mejora) break;
          }
          if (mejora) break;
        }
        if (mejora) break;
      }
    }
  }
  solucion->AjustarDescansos();
  return solucion;
}