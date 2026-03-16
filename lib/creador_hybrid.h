#ifndef CREADOR_HYBRID_H
#define CREADOR_HYBRID_H

#include "creador_algoritmo.h"
#include "algoritmo_hibrido.h"

class CreadorHybrid : public CreadorAlgoritmo {
 public:
  std::shared_ptr<Algoritmo> build(const nlohmann::json& config) const override {
    return std::make_shared<AlgoritmoHibrido>();
  }
};

#endif