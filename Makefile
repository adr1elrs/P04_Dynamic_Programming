# Nombre del ejecutable final
TARGET = experimento_dyv

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -Ilib

# Directorios
SRCDIR = src
LIBDIR = lib
OBJDIR = obj

# Localización de archivos (Todo en una sola línea para evitar errores de salto)
SOURCES = main.cc src/instancia_planificacion.cc src/solucion_planificacion.cc src/divide_y_venceras.cc src/planificacion_dyv.cc src/algoritmo_voraz.cc src/programacion_dinamica.cc src/algoritmo_factory.cc

# Transformamos la lista de fuentes en una lista de objetos en la carpeta obj/
OBJECTS = $(SOURCES:%.cc=$(OBJDIR)/%.o)

# Regla por defecto
all: $(TARGET)

# Creación del ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar archivos .cc a .o
$(OBJDIR)/%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza de archivos temporales y ejecutable
.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "Limpieza completada: archivos .o y ejecutable eliminados."