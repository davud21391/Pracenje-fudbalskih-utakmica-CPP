#include "models/Osoba.h"

#include <utility>

Osoba::Osoba(std::string ime, std::string prezime)
    : ime(std::move(ime)), prezime(std::move(prezime)) {}

const std::string& Osoba::getIme() const {
    return ime;
}

const std::string& Osoba::getPrezime() const {
    return prezime;
}

std::string Osoba::getPunoIme() const {
    return ime + " " + prezime;
}
