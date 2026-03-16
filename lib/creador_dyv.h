#ifndef CREADOR_DYV_H
#define CREADOR_DYV_H
#include "creador_algoritmo.h"
#include "planificacion_dyv.h"

class CreadorDyV : public CreadorAlgoritmo {
 public:
  std::shared_ptr<Algoritmo> build(const nlohmann::json& config) const override {
    if (!config.contains("smallSolver")) {
      throw std::runtime_error("BinaryDivideAndConquer requiere un 'smallSolver'.");
    }
    
    std::shared_ptr<Algoritmo> small_solver = AlgoritmoFactory::CrearAlgoritmos(config["smallSolver"]);
    
    int umbral = config.value("isSmall", 1); 
    return std::make_shared<PlanificacionDyV>(small_solver, umbral);
  }
};

#endif