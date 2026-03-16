#ifndef CREADOR_DYNAMIC_H
#define CREADOR_DYNAMIC_H

#include "creador_algoritmo.h"
#include "programacion_dinamica.h"

class CreadorDynamic : public CreadorAlgoritmo {
 public:
  std::shared_ptr<Algoritmo> build(const nlohmann::json& config) const override {
    return std::make_shared<ProgramacionDinamica>();
  }
};

#endif
