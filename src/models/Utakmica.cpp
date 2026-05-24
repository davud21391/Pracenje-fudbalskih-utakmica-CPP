#include "models/Utakmica.h"

#include <utility>

Utakmica::Utakmica(int idUtakmice, int idDomacina, int idGosta, std::string datum, int rezultatDomacin, int rezultatGost, int kolo)
    : idUtakmice(idUtakmice),
      idDomacina(idDomacina),
      idGosta(idGosta),
      datum(std::move(datum)),
      rezultatDomacin(rezultatDomacin),
      rezultatGost(rezultatGost),
      kolo(kolo) {}

int Utakmica::getIdUtakmice() const {
    return idUtakmice;
}

int Utakmica::getIdDomacina() const {
    return idDomacina;
}

int Utakmica::getIdGosta() const {
    return idGosta;
}

const std::string& Utakmica::getDatum() const {
    return datum;
}

int Utakmica::getRezultatDomacin() const {
    return rezultatDomacin;
}

int Utakmica::getRezultatGost() const {
    return rezultatGost;
}

int Utakmica::getKolo() const {
    return kolo;
}

const std::vector<Strijelac>& Utakmica::getStrijelci() const {
    return strijelci;
}

void Utakmica::setDatum(const std::string& noviDatum) {
    datum = noviDatum;
}

void Utakmica::setRezultatDomacin(int noviRezultatDomacin) {
    rezultatDomacin = noviRezultatDomacin;
}

void Utakmica::setRezultatGost(int noviRezultatGost) {
    rezultatGost = noviRezultatGost;
}

void Utakmica::setKolo(int novoKolo) {
    kolo = novoKolo;
}

void Utakmica::dodajStrijelca(const Strijelac& strijelac) {
    strijelci.push_back(strijelac);
}

void Utakmica::obrisiStrijelce() {
    strijelci.clear();
}
