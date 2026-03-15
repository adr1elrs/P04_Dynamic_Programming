#ifndef ALGORITMO_HIBRIDO_H
#define ALGORITMO_HIBRIDO_H

#include "algoritmo.h"
#include <memory>

class AlgoritmoHibrido : public Algoritmo {
 public:
  AlgoritmoHibrido() = default;
  ~AlgoritmoHibrido() override = default;

  std::shared_ptr<Solucion> solve(std::shared_ptr<Instancia> instancia) override;
};

#endif