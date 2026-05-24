#include "services/UtakmicaService.h"

#include <algorithm>
#include <cctype>
#include <map>
#include <string>
#include <stdexcept>

namespace {
bool statistikaPoredjenje(const StatistikaStrijelca& lijevo, const StatistikaStrijelca& desno) {
    if (lijevo.brojGolova != desno.brojGolova) {
        return lijevo.brojGolova > desno.brojGolova;
    }

    return lijevo.punoIme < desno.punoIme;
}

bool tabelaPoredjenje(const StatistikaTima& lijevo, const StatistikaTima& desno) {
    if (lijevo.bodovi != desno.bodovi) {
        return lijevo.bodovi > desno.bodovi;
    }

    if (lijevo.golRazlika != desno.golRazlika) {
        return lijevo.golRazlika > desno.golRazlika;
    }

    if (lijevo.datiGolovi != desno.datiGolovi) {
        return lijevo.datiGolovi > desno.datiGolovi;
    }

    return lijevo.nazivTima < desno.nazivTima;
}
}

UtakmicaService::UtakmicaService(UtakmicaRepository& utakmicaRepository, TimRepository& timRepository)
    : utakmicaRepository(utakmicaRepository), timRepository(timRepository), nextUtakmicaId(1), nextStrijelacId(1) {}

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

Utakmica* UtakmicaService::pronadjiUtakmicu(int idUtakmice) const {
    return utakmicaRepository.findById(idUtakmice);
}

Strijelac UtakmicaService::dodajStrijelca(int idUtakmice, int idIgraca, int minuta, bool autoGol) {
    Utakmica* utakmica = pronadjiUtakmicu(idUtakmice);
    if (utakmica == nullptr) {
        throw std::runtime_error("Utakmica sa zadanim ID-em ne postoji.");
    }

    validirajStrijelca(*utakmica, idIgraca, minuta);

    const Strijelac strijelac(nextStrijelacId++, idUtakmice, idIgraca, minuta, autoGol);
    utakmica->dodajStrijelca(strijelac);
    return strijelac;
}

std::vector<StatistikaStrijelca> UtakmicaService::vratiListuStrijelaca() const {
    std::map<int, StatistikaStrijelca> statistikaPoIgracu;

    for (const Utakmica* utakmica : utakmicaRepository.getAll()) {
        for (const Strijelac& strijelac : utakmica->getStrijelci()) {
            if (strijelac.isAutoGol()) {
                continue;
            }

            Igrac* igrac = pronadjiIgracaUMecu(*utakmica, strijelac.getIdIgraca());
            if (igrac == nullptr) {
                continue;
            }

            auto it = statistikaPoIgracu.find(igrac->getIdIgraca());
            if (it == statistikaPoIgracu.end()) {
                statistikaPoIgracu[igrac->getIdIgraca()] = StatistikaStrijelca{igrac->getIdIgraca(), igrac->getPunoIme(), 1};
            } else {
                it->second.brojGolova += 1;
            }
        }
    }

    std::vector<StatistikaStrijelca> rezultat;
    for (const auto& [_, statistika] : statistikaPoIgracu) {
        rezultat.push_back(statistika);
    }

    std::sort(rezultat.begin(), rezultat.end(), statistikaPoredjenje);
    return rezultat;
}

std::vector<StatistikaTima> UtakmicaService::vratiTabelu() const {
    std::map<int, StatistikaTima> statistikaPoTimu;

    for (const Tim* tim : timRepository.getAll()) {
        statistikaPoTimu[tim->getIdTima()] = StatistikaTima{
            tim->getIdTima(),
            tim->getNaziv(),
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        };
    }

    for (const Utakmica* utakmica : utakmicaRepository.getAll()) {
        auto domacinIt = statistikaPoTimu.find(utakmica->getIdDomacina());
        auto gostIt = statistikaPoTimu.find(utakmica->getIdGosta());
        if (domacinIt == statistikaPoTimu.end() || gostIt == statistikaPoTimu.end()) {
            continue;
        }

        StatistikaTima& domacin = domacinIt->second;
        StatistikaTima& gost = gostIt->second;

        domacin.odigrane += 1;
        gost.odigrane += 1;

        domacin.datiGolovi += utakmica->getRezultatDomacin();
        domacin.primljeniGolovi += utakmica->getRezultatGost();
        gost.datiGolovi += utakmica->getRezultatGost();
        gost.primljeniGolovi += utakmica->getRezultatDomacin();

        if (utakmica->getRezultatDomacin() > utakmica->getRezultatGost()) {
            domacin.pobjede += 1;
            domacin.bodovi += 3;
            gost.porazi += 1;
        } else if (utakmica->getRezultatDomacin() < utakmica->getRezultatGost()) {
            gost.pobjede += 1;
            gost.bodovi += 3;
            domacin.porazi += 1;
        } else {
            domacin.nerijesene += 1;
            gost.nerijesene += 1;
            domacin.bodovi += 1;
            gost.bodovi += 1;
        }
    }

    std::vector<StatistikaTima> tabela;
    for (auto& [_, statistika] : statistikaPoTimu) {
        statistika.golRazlika = statistika.datiGolovi - statistika.primljeniGolovi;
        tabela.push_back(statistika);
    }

    std::sort(tabela.begin(), tabela.end(), tabelaPoredjenje);
    return tabela;
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

void UtakmicaService::validirajStrijelca(const Utakmica& utakmica, int idIgraca, int minuta) const {
    if (pronadjiIgracaUMecu(utakmica, idIgraca) == nullptr) {
        throw std::runtime_error("Igrac ne pripada timovima izabrane utakmice.");
    }

    if (minuta < 1 || minuta > 130) {
        throw std::runtime_error("Minuta gola mora biti izmedju 1 i 130.");
    }

    const int maksimalanBrojGolova = utakmica.getRezultatDomacin() + utakmica.getRezultatGost();
    if (ukupanBrojEvidentiranihGolova(utakmica) >= maksimalanBrojGolova) {
        throw std::runtime_error("Svi golovi za ovu utakmicu su vec evidentirani.");
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

Igrac* UtakmicaService::pronadjiIgracaUMecu(const Utakmica& utakmica, int idIgraca) const {
    Tim* domacin = timRepository.findById(utakmica.getIdDomacina());
    if (domacin != nullptr) {
        Igrac* igrac = domacin->pronadjiIgracaPoId(idIgraca);
        if (igrac != nullptr) {
            return igrac;
        }
    }

    Tim* gost = timRepository.findById(utakmica.getIdGosta());
    if (gost != nullptr) {
        return gost->pronadjiIgracaPoId(idIgraca);
    }

    return nullptr;
}

int UtakmicaService::ukupanBrojEvidentiranihGolova(const Utakmica& utakmica) const {
    return static_cast<int>(utakmica.getStrijelci().size());
}
