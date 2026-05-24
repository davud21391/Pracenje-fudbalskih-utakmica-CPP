#include "models/Tim.h"

#include <algorithm>
#include <utility>

Tim::Tim(int idTima, std::string naziv, std::string grad, std::string trener, int godinaOsnivanja)
    : idTima(idTima),
      naziv(std::move(naziv)),
      grad(std::move(grad)),
      trener(std::move(trener)),
      godinaOsnivanja(godinaOsnivanja) {}

Tim::~Tim() {
    for (Igrac* igrac : igraci) {
        delete igrac;
    }
}

int Tim::getIdTima() const {
    return idTima;
}

const std::string& Tim::getNaziv() const {
    return naziv;
}

const std::string& Tim::getGrad() const {
    return grad;
}

const std::string& Tim::getTrener() const {
    return trener;
}

int Tim::getGodinaOsnivanja() const {
    return godinaOsnivanja;
}

const std::vector<Igrac*>& Tim::getIgraci() const {
    return igraci;
}

void Tim::setNaziv(const std::string& noviNaziv) {
    naziv = noviNaziv;
}

void Tim::setGrad(const std::string& noviGrad) {
    grad = noviGrad;
}

void Tim::setTrener(const std::string& noviTrener) {
    trener = noviTrener;
}

void Tim::setGodinaOsnivanja(int novaGodina) {
    godinaOsnivanja = novaGodina;
}

void Tim::dodajIgraca(Igrac* igrac) {
    igraci.push_back(igrac);
}

bool Tim::ukloniIgraca(int idIgraca) {
    const auto it = std::find_if(igraci.begin(), igraci.end(), [idIgraca](Igrac* igrac) {
        return igrac->getIdIgraca() == idIgraca;
    });

    if (it == igraci.end()) {
        return false;
    }

    delete *it;
    igraci.erase(it);
    return true;
}

Igrac* Tim::pronadjiIgracaPoId(int idIgraca) const {
    const auto it = std::find_if(igraci.begin(), igraci.end(), [idIgraca](Igrac* igrac) {
        return igrac->getIdIgraca() == idIgraca;
    });

    return it == igraci.end() ? nullptr : *it;
}

bool Tim::postojiBrojDresa(int brojDresa) const {
    return std::any_of(igraci.begin(), igraci.end(), [brojDresa](Igrac* igrac) {
        return igrac->getBrojDresa() == brojDresa;
    });
}
