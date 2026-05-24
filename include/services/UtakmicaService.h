#pragma once

#include "repositories/TimRepository.h"
#include "repositories/UtakmicaRepository.h"

#include <string>

class UtakmicaService {
public:
    UtakmicaService(UtakmicaRepository& utakmicaRepository, TimRepository& timRepository);

    Utakmica* dodajUtakmicu(int idDomacina, int idGosta, const std::string& datum, int rezultatDomacin, int rezultatGost, int kolo);
    const std::vector<Utakmica*>& vratiSveUtakmice() const;

private:
    UtakmicaRepository& utakmicaRepository;
    TimRepository& timRepository;
    int nextUtakmicaId;

    void validirajUtakmicu(int idDomacina, int idGosta, const std::string& datum, int rezultatDomacin, int rezultatGost, int kolo) const;
    std::string normalizujDatum(const std::string& datum) const;
    bool validanDatumFormat(const std::string& datum) const;
};
