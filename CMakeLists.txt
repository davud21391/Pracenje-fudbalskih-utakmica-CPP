cmake_minimum_required(VERSION 3.16)
project(FootballResultsTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(FootballResultsTracker
    src/main.cpp
    src/models/Osoba.cpp
    src/models/Igrac.cpp
    src/models/Tim.cpp
    src/models/Strijelac.cpp
    src/models/Utakmica.cpp
    src/repositories/TimRepository.cpp
    src/repositories/UtakmicaRepository.cpp
    src/services/TimService.cpp
    src/services/PersistenceService.cpp
    src/services/UtakmicaService.cpp
    src/ui/Application.cpp
)
