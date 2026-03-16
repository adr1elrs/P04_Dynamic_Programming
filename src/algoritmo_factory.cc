#include "../lib/algoritmo_factory.h"
#include "../lib/creador_algoritmo.h"
#include "../lib/planificacion_dyv.h"
#include "../lib/algoritmo_voraz.h"
#include "../lib/algoritmo_hibrido.h"
#include "../lib/programacion_dinamica.h"
#include "../lib/creador_greedy.h"
#include "../lib/creador_dynamic.h"
#include "../lib/creador_hybrid.h"
#include "../lib/creador_dyv.h"


#include <string>
#include <stdexcept>

std::shared_ptr<Algoritmo> AlgoritmoFactory::CrearAlgoritmos(const nlohmann::json& config) {
  if (!config.contains("class")) {
    throw std::runtime_error("El JSON de configuración no contiene el campo 'class'.");
  }

  std::string tipo = config["class"];

  if (tipo == "Greedy") {
    CreadorGreedy creador;
    return creador.build(config);
  } 
  else if (tipo == "DynamicProgramming") {
    CreadorDynamic creador;
    return creador.build(config);
  }
  else if (tipo == "Hybrid") { 
    CreadorHybrid creador;
    return creador.build(config);
  } 
  else if (tipo == "BinaryDivideAndConquer") {
    CreadorDyV creador;
    return creador.build(config);
  }

  throw std::runtime_error("Clase de algoritmo desconocida en el Factory: " + tipo);
}