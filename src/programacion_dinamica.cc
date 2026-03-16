#include "../lib/programacion_dinamica.h"
#include "../lib/instancia_planificacion.h"
#include "../lib/solucion_planificacion.h"
#include <vector>
#include <algorithm>

std::shared_ptr<Solucion> ProgramacionDinamica::solve(std::shared_ptr<Instancia> instancia) {
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

  // Vamos a resolver usando el algoritmo de programación dínamica los días de forma independiente
  for (int d = 0; d < num_dias; ++d) {
    // Creamos el vector de slots a cubrir por cada empleado requerido en el turno
    // El valor de cada índice será el número de turno, esto es para posteriormente
    // Calcular la satisfacción del empleado
    std::vector<int> slots;
    for (int t = 0; t < num_turnos; ++t) {
      int requeridos = inst_dyv->GetEmpleadosRequeridos(d, t);
      for (int i = 0; i < requeridos; ++i) {
        slots.push_back(t);
      }
    }

    int K = slots.size();
    int E = num_empleados;

    // Creamos la tabla inicializada todo a 0
    // Esto es por los casos base, se hace así y no con bucles para ahorrar complejidad
    std::vector<std::vector<int>> dp(E + 1, std::vector<int>(K + 1, 0));

    // Rellenamos la tabla
    // Al haber añadido una fila y columna extra para representar no contar empleado o no contar slot
    // podemos trabajar con índice 1-based ("empieza" en 1)
    for (int e = 1; e <= E; ++e) {
      for (int k = 1; k <= K; ++k) {
        int turno_asociado = slots[k - 1]; // Índice 0-based para el array puesto que en la matriz es 1-based
        int sat = inst_dyv->GetSatisfaccion(e - 1, d, turno_asociado);
        
        dp[e][k] = std::max(dp[e - 1][k], dp[e - 1][k - 1] + sat);
      }
    }

    // Reconstruimos la solución 
    int e_actual = E;
    int k_actual = K;
    std::vector<int> turnos_completados(num_turnos, 0);

    while (e_actual > 0 && k_actual > 0) {
      // Si el valor viene de la diagonal, significa que el empleado fue asignado
      if (dp[e_actual][k_actual] != dp[e_actual - 1][k_actual]) {
        int turno_asociado = slots[k_actual - 1];
        int sat = inst_dyv->GetSatisfaccion(e_actual - 1, d, turno_asociado);
        
        solucion->AsignarEmpleado(d, turno_asociado, e_actual - 1, sat);
        turnos_completados[turno_asociado]++;

        // Como lo asignamos, retrocedemos en empleado y en slot
        e_actual--;
        k_actual--;
      } else {
        // No fue asignado, miramos al empleado anterior
        e_actual--;
      }
    }

    // Registrar turnos cubiertos al 100%
    for (int t = 0; t < num_turnos; ++t) {
      if (turnos_completados[t] >= inst_dyv->GetEmpleadosRequeridos(d, t) && inst_dyv->GetEmpleadosRequeridos(d, t) > 0) {
        solucion->RegistrarTurnoCubierto();
      }
    }
  }

  // Ajuste global de descansos para toda la ventana de tiempo
  solucion->AjustarDescansos();
  return solucion;
}