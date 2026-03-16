#ifndef CREADOR_ALGORITMO_H
#define CREADOR_ALGORITMO_H

#include "algoritmo.h"
#include <nlohmann/json.hpp>
#include <memory>

// Clase abstracta que define el contrato para construir algoritmos
class CreadorAlgoritmo {
 public:
  virtual std::shared_ptr<Algoritmo> build(const nlohmann::json& config) const = 0;
  virtual ~CreadorAlgoritmo() = default;
};

#endif