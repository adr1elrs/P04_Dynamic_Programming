#ifndef ALGORITMO_FACTORY_H
#define ALGORITMO_FACTORY_H

#include "algoritmo.h"
#include <nlohmann/json.hpp>
#include <memory>

class AlgoritmoFactory {
 public:
  // Solo declaramos el método estático
  static std::shared_ptr<Algoritmo> Crear(const nlohmann::json& config);
};

#endif