/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos 2025-2026
 *
 * @author Sergio Rosales Calzadilla
 * @author Adriel Reyes Suárez
 * @date 26/02/2026
 * @brief Archivo main para la ejecución del algoritmo Divide y Vencerás
 */

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "lib/instancia_planificacion.h"
#include "lib/solucion_planificacion.h"
#include "lib/algoritmo_factory.h"

using json = nlohmann::json;

// Función auxiliar para leer un JSON y devolver el objeto parseado
json LeerConfiguracion(const std::string& ruta) {
  std::ifstream archivo(ruta);
  if (!archivo.is_open()) {
    throw std::runtime_error("No se pudo abrir el archivo de configuracion: " + ruta);
  }
  json config;
  archivo >> config;
  return config;
}

// NUEVO: Función recursiva para imprimir el árbol de algoritmos
void ImprimirJerarquiaAlgoritmos(const json& config, int nivel = 0, std::string prefijo = "") {
  if (!config.contains("class")) return;

  std::string tipo = config["class"];
  
  // Imprimimos la rama y el nombre del algoritmo
  std::cout << prefijo << (nivel > 0 ? "└── " : "") << "[" << tipo << "]";

  if (tipo == "BinaryDivideAndConquer") {
    // Mostramos su umbral
    std::cout << " (Umbral 'isSmall': " << config.value("isSmall", 1) << " dias)\n";
    
    // Si tiene un sub-algoritmo, nos llamamos recursivamente
    if (config.contains("smallSolver")) {
      std::string nuevo_prefijo = prefijo + (nivel > 0 ? "    " : "");
      ImprimirJerarquiaAlgoritmos(config["smallSolver"], nivel + 1, nuevo_prefijo);
    }
  } else {
    // Si es un caso base (Greedy o DP), simplemente saltamos de línea
    std::cout << "\n";
  }
}

int main(int argc, char* argv[]) {
  // Control estricto de los parámetros de entrada
  if (argc != 3) {
    std::cerr << "Uso incorrecto del programa.\n";
    std::cerr << "Formato esperado: " << argv[0] << " <ruta_configuracion.json> <ruta_instancia.json>\n";
    std::cerr << "Ejemplo: ./experimento_dyv input_algorithm/divide_and_greedy.json input_data/instance_000.json\n";
    return 1;
  }

  // Recogemos los argumentos
  std::string ruta_config = argv[1];
  std::string ruta_instancia = argv[2];

  try {
    std::cout << "[INFO] Cargando configuracion desde: " << ruta_config << "\n";
    json config_json = LeerConfiguracion(ruta_config);
    auto algoritmo = AlgoritmoFactory::CrearAlgoritmos(config_json);

    std::cout << "[INFO] Cargando instancia desde: " << ruta_instancia << "\n";
    auto instancia = std::make_shared<InstanciaPlanificacion>(ruta_instancia);

    // Imprimimos el resumen de la instancia leída
    instancia->Print(std::cout);

    std::cout << "[INFO] Resolviendo planificacion...\n";

    // Medición precisa del tiempo (en microsegundos)
    auto inicio = std::chrono::high_resolution_clock::now();
    auto solucion = algoritmo->solve(instancia);
    auto fin = std::chrono::high_resolution_clock::now();

    // Calculamos en microsegundos
    auto tiempo_us = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();

    // Extraemos la calidad haciendo downcast seguro
    auto sol_plan = std::dynamic_pointer_cast<SolucionPlanificacion>(solucion);
    int calidad = sol_plan ? sol_plan->GetCalidad() : 0;

    // Reporte de la ejecución
    std::cout << "\n================ RESULTADOS ================\n";
    std::cout << "Jerarquia de Algoritmos:\n";
    
    // Llamada a la nueva función recursiva
    ImprimirJerarquiaAlgoritmos(config_json);
    std::cout << "--------------------------------------------\n";
    
    std::cout << "Empleados             : " << instancia->GetNumEmpleados() << "\n";
    std::cout << "Dias planificados     : " << instancia->GetNumDias() << "\n";
    std::cout << "Turnos por dia        : " << instancia->GetNumTurnos() << "\n";
    std::cout << "Calidad de solucion   : " << calidad << "\n";
    std::cout << "Tiempo de ejecucion   : " << tiempo_us << " microsegundos (us)\n";
    std::cout << "============================================\n\n";

    // --- NUEVO: Escribir la solución en salid.txt ---
    if (sol_plan) {
      std::ofstream archivo_salida("salida.txt");
      if (archivo_salida.is_open()) {
        sol_plan->Print(archivo_salida);
        archivo_salida.close();
        std::cout << "[INFO] La matriz de horarios detallada se ha guardado correctamente en 'salid.txt'\n";
      } else {
        std::cerr << "[ADVERTENCIA] No se pudo crear o abrir el archivo 'salid.txt' para guardar la solucion.\n";
      }
    }

  } catch (const std::exception& e) {
    std::cerr << "\n[ERROR CRITICO]: " << e.what() << "\n";
    return 1;
  }

  return 0;
}