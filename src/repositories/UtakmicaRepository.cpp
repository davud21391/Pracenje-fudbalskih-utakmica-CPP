#include "repositories/UtakmicaRepository.h"

#include <algorithm>
#include <stdexcept>

UtakmicaRepository::~UtakmicaRepository() {
    clear();
}

void UtakmicaRepository::add(Utakmica* utakmica) {
    if (utakmica == nullptr) {
        throw std::invalid_argument("Utakmica ne moze biti null.");
    }

    if (utakmicePoId.find(utakmica->getIdUtakmice()) != utakmicePoId.end()) {
        throw std::runtime_error("Utakmica sa istim ID-em vec postoji.");
    }

    utakmice.push_back(utakmica);
    utakmicePoId[utakmica->getIdUtakmice()] = utakmica;
}

Utakmica* UtakmicaRepository::findById(int idUtakmice) const {
    const auto it = utakmicePoId.find(idUtakmice);
    return it == utakmicePoId.end() ? nullptr : it->second;
}

const std::vector<Utakmica*>& UtakmicaRepository::getAll() const {
    return utakmice;
}

bool UtakmicaRepository::remove(int idUtakmice) {
    const auto mapIt = utakmicePoId.find(idUtakmice);
    if (mapIt == utakmicePoId.end()) {
        return false;
    }

    Utakmica* utakmica = mapIt->second;
    utakmicePoId.erase(mapIt);

    const auto vectorIt = std::find(utakmice.begin(), utakmice.end(), utakmica);
    if (vectorIt != utakmice.end()) {
        utakmice.erase(vectorIt);
    }

    delete utakmica;
    return true;
}

void UtakmicaRepository::clear() {
    for (Utakmica* utakmica : utakmice) {
        delete utakmica;
    }

    utakmice.clear();
    utakmicePoId.clear();
}
