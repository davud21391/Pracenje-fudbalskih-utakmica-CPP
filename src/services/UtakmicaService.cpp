#include "services/UtakmicaService.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <stdexcept>

UtakmicaService::UtakmicaService(UtakmicaRepository& utakmicaRepository, TimRepository& timRepository)
    : utakmicaRepository(utakmicaRepository), timRepository(timRepository), nextUtakmicaId(1) {}

Utakmica* UtakmicaService::dodajUtakmicu(int idDomacina, int idGosta, const std::string& datum, int rezultatDomacin, int rezultatGost, int kolo) {
    const std::string normalizovanDatum = normalizujDatum(datum);
    validirajUtakmicu(idDomacina, idGosta, normalizovanDatum, rezultatDomacin, rezultatGost, kolo);

    Utakmica* utakmica = new Utakmica(nextUtakmicaId++, idDomacina, idGosta, normalizovanDatum, rezultatDomacin, rezultatGost, kolo);
    utakmicaRepository.add(utakmica);
    return utakmica;
}

const std::vector<Utakmica*>& UtakmicaService::vratiSveUtakmice() const {
    return utakmicaRepository.getAll();
}

void UtakmicaService::validirajUtakmicu(int idDomacina, int idGosta, const std::string& datum, int rezultatDomacin, int rezultatGost, int kolo) const {
    if (timRepository.findById(idDomacina) == nullptr) {
        throw std::runtime_error("Domaci tim ne postoji.");
    }

    if (timRepository.findById(idGosta) == nullptr) {
        throw std::runtime_error("Gostujuci tim ne postoji.");
    }

    if (idDomacina == idGosta) {
        throw std::runtime_error("Tim ne moze igrati sam protiv sebe.");
    }

    if (!validanDatumFormat(datum)) {
        throw std::runtime_error("Datum mora biti u formatu dd.mm.gggg.");
    }

    if (rezultatDomacin < 0 || rezultatGost < 0) {
        throw std::runtime_error("Rezultat ne moze biti negativan.");
    }

    if (kolo < 1) {
        throw std::runtime_error("Kolo mora biti veci broj od 0.");
    }
}

std::string UtakmicaService::normalizujDatum(const std::string& datum) const {
    std::string rezultat = datum;

    while (!rezultat.empty() && std::isspace(static_cast<unsigned char>(rezultat.front()))) {
        rezultat.erase(rezultat.begin());
    }

    while (!rezultat.empty() && std::isspace(static_cast<unsigned char>(rezultat.back()))) {
        rezultat.pop_back();
    }

    if (!rezultat.empty() && rezultat.back() == '.') {
        rezultat.pop_back();
    }

    return rezultat;
}

bool UtakmicaService::validanDatumFormat(const std::string& datum) const {
    const std::string normalizovanDatum = normalizujDatum(datum);

    if (normalizovanDatum.size() != 10) {
        return false;
    }

    for (int index = 0; index < static_cast<int>(normalizovanDatum.size()); ++index) {
        if (index == 2 || index == 5) {
            if (normalizovanDatum[index] != '.') {
                return false;
            }
            continue;
        }

        if (!std::isdigit(static_cast<unsigned char>(normalizovanDatum[index]))) {
            return false;
        }
    }

    const int dan = std::stoi(normalizovanDatum.substr(0, 2));
    const int mjesec = std::stoi(normalizovanDatum.substr(3, 2));
    const int godina = std::stoi(normalizovanDatum.substr(6, 4));

    if (godina < 1900 || godina > 2100) {
        return false;
    }

    if (mjesec < 1 || mjesec > 12) {
        return false;
    }

    if (dan < 1 || dan > 31) {
        return false;
    }

    return true;
}
