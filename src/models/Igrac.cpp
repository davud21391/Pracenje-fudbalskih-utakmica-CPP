#include "models/Igrac.h"

#include <utility>

Igrac::Igrac(int idIgraca, std::string ime, std::string prezime, int brojDresa, std::string pozicija, int idTima)
    : Osoba(std::move(ime), std::move(prezime)),
      idIgraca(idIgraca),
      brojDresa(brojDresa),
      pozicija(std::move(pozicija)),
      idTima(idTima) {}

int Igrac::getIdIgraca() const {
    return idIgraca;
}

int Igrac::getBrojDresa() const {
    return brojDresa;
}

const std::string& Igrac::getPozicija() const {
    return pozicija;
}

int Igrac::getIdTima() const {
    return idTima;
}

void Igrac::setBrojDresa(int noviBrojDresa) {
    brojDresa = noviBrojDresa;
}

void Igrac::setPozicija(const std::string& novaPozicija) {
    pozicija = novaPozicija;
}
