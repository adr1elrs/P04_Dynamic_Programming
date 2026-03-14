#include "../lib/algoritmo_factory.h"
#include "../lib/planificacion_dyv.h"
#include "../lib/algoritmo_voraz.h"
#include "../lib/programacion_dinamica.h"

#include <string>
#include <stdexcept>

std::shared_ptr<Algoritmo> AlgoritmoFactory::Crear(const nlohmann::json& config) {
  if (!config.contains("class")) {
    throw std::runtime_error("El JSON de configuración no contiene el campo 'class'.");
  }

  std::string tipo = config["class"];

  if (tipo == "Greedy") {
    return std::make_shared<AlgoritmoVoraz>();
  } 
  else if (tipo == "DynamicProgramming") {
    return std::make_shared<ProgramacionDinamica>();
  } 
  else if (tipo == "BinaryDivideAndConquer") {
    if (!config.contains("smallSolver")) {
      throw std::runtime_error("BinaryDivideAndConquer requiere un 'smallSolver'.");
    }
    
    // Llamada recursiva para construir el algoritmo base
    std::shared_ptr<Algoritmo> small_solver = Crear(config["smallSolver"]);
    
    // Si el JSON especifica cuándo parar la división, lo leemos; por defecto es 1 día
    int umbral = config.value("isSmall", 1); 
    
    return std::make_shared<PlanificacionDyV>(small_solver, umbral);
  }

  throw std::runtime_error("Clase de algoritmo desconocida en el Factory: " + tipo);
}