#pragma once

#include "models/Igrac.h"

#include <string>
#include <vector>

class Tim {
public:
    Tim(int idTima, std::string naziv, std::string grad, std::string trener, int godinaOsnivanja);
    ~Tim();

    Tim(const Tim&) = delete;
    Tim& operator=(const Tim&) = delete;

    int getIdTima() const;
    const std::string& getNaziv() const;
    const std::string& getGrad() const;
    const std::string& getTrener() const;
    int getGodinaOsnivanja() const;
    const std::vector<Igrac*>& getIgraci() const;

    void setNaziv(const std::string& noviNaziv);
    void setGrad(const std::string& noviGrad);
    void setTrener(const std::string& noviTrener);
    void setGodinaOsnivanja(int novaGodina);

    void dodajIgraca(Igrac* igrac);
    bool ukloniIgraca(int idIgraca);
    Igrac* pronadjiIgracaPoId(int idIgraca) const;
    bool postojiBrojDresa(int brojDresa) const;

private:
    int idTima;
    std::string naziv;
    std::string grad;
    std::string trener;
    int godinaOsnivanja;
    std::vector<Igrac*> igraci;
};
