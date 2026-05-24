#pragma once

#include "models/Osoba.h"

#include <string>

class Igrac : public Osoba {
public:
    Igrac(int idIgraca, std::string ime, std::string prezime, int brojDresa, std::string pozicija, int idTima);

    int getIdIgraca() const;
    int getBrojDresa() const;
    const std::string& getPozicija() const;
    int getIdTima() const;

    void setBrojDresa(int noviBrojDresa);
    void setPozicija(const std::string& novaPozicija);

private:
    int idIgraca;
    int brojDresa;
    std::string pozicija;
    int idTima;
};
