#ifndef PROGRAMACION_DINAMICA_H
#define PROGRAMACION_DINAMICA_H

#include "algoritmo.h"
#include <memory>

class ProgramacionDinamica : public Algoritmo {
 public:
  ProgramacionDinamica() = default;
  ~ProgramacionDinamica() override = default;

  std::shared_ptr<Solucion> solve(std::shared_ptr<Instancia> instancia) override;
};

#endif