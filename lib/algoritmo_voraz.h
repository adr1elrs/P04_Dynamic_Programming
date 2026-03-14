#ifndef ALGORITMO_VORAZ_H
#define ALGORITMO_VORAZ_H

#include "algoritmo.h"
#include <memory>

class AlgoritmoVoraz : public Algoritmo {
 public:
  AlgoritmoVoraz() = default;
  ~AlgoritmoVoraz() override = default;

  std::shared_ptr<Solucion> solve(std::shared_ptr<Instancia> instancia) override;
};

#endif