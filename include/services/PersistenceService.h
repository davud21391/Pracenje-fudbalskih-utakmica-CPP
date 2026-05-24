#pragma once

#include "repositories/TimRepository.h"
#include "repositories/UtakmicaRepository.h"
#include "services/TimService.h"
#include "services/UtakmicaService.h"

#include <string>

class PersistenceService {
public:
    PersistenceService(TimRepository& timRepository, UtakmicaRepository& utakmicaRepository, TimService& timService, UtakmicaService& utakmicaService);

    void spremiPodatke(const std::string& direktorij = "data") const;
    void ucitajPodatke(const std::string& direktorij = "data");

private:
    TimRepository& timRepository;
    UtakmicaRepository& utakmicaRepository;
    TimService& timService;
    UtakmicaService& utakmicaService;
};
