#include "../lib/algoritmo_voraz.h"
#include "../lib/instancia_planificacion.h"
#include "../lib/solucion_planificacion.h"
#include <vector>
#include <algorithm>

struct EvaluacionTurno {
  int satisfaccion;
  int turno;
  int empleado;
};

std::shared_ptr<Solucion> AlgoritmoVoraz::solve(std::shared_ptr<Instancia> instancia) {
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
  }

  // Ajuste final para cumplir restricciones globales
  solucion->AjustarDescansos();
  return solucion;
}