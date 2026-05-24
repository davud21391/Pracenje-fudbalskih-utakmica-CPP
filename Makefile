CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET := build/FootballResultsTracker

SOURCES := \
	src/main.cpp \
	src/models/Osoba.cpp \
	src/models/Igrac.cpp \
	src/models/Tim.cpp \
	src/models/Strijelac.cpp \
	src/models/Utakmica.cpp \
	src/repositories/TimRepository.cpp \
	src/repositories/UtakmicaRepository.cpp \
	src/services/TimService.cpp \
	src/ui/Application.cpp

.PHONY: run build clean

run: $(TARGET)
	./$(TARGET)

build: $(TARGET)

$(TARGET): $(SOURCES)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -rf build
