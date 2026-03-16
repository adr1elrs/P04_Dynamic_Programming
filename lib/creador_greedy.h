#ifndef CREADOR_GREEDY_H
#define CREADOR_GREEDY_H

#include "creador_algoritmo.h"
#include "algoritmo_voraz.h"

class CreadorGreedy : public CreadorAlgoritmo {
 public:
  std::shared_ptr<Algoritmo> build(const nlohmann::json& config) const override {
    return std::make_shared<AlgoritmoVoraz>();
  }
};

#endif