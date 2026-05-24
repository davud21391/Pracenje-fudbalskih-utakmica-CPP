#pragma once

#include "models/Strijelac.h"

#include <string>
#include <vector>

class Utakmica {
public:
    Utakmica(int idUtakmice, int idDomacina, int idGosta, std::string datum, int rezultatDomacin, int rezultatGost, int kolo);

    int getIdUtakmice() const;
    int getIdDomacina() const;
    int getIdGosta() const;
    const std::string& getDatum() const;
    int getRezultatDomacin() const;
    int getRezultatGost() const;
    int getKolo() const;
    const std::vector<Strijelac>& getStrijelci() const;

    void setIdDomacina(int noviIdDomacina);
    void setIdGosta(int noviIdGosta);
    void setDatum(const std::string& noviDatum);
    void setRezultatDomacin(int noviRezultatDomacin);
    void setRezultatGost(int noviRezultatGost);
    void setKolo(int novoKolo);

    void dodajStrijelca(const Strijelac& strijelac);
    void obrisiStrijelce();

private:
    int idUtakmice;
    int idDomacina;
    int idGosta;
    std::string datum;
    int rezultatDomacin;
    int rezultatGost;
    int kolo;
    std::vector<Strijelac> strijelci;
};
