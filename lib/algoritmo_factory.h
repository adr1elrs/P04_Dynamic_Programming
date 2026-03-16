#ifndef ALGORITMO_FACTORY_H
#define ALGORITMO_FACTORY_H

#include "algoritmo.h"
#include <nlohmann/json.hpp>
#include <memory>

class AlgoritmoFactory {
 public:
  static std::shared_ptr<Algoritmo> CrearAlgoritmos(const nlohmann::json& config);
};

#endif