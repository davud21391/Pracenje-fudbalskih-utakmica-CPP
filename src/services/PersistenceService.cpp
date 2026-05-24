#include "services/PersistenceService.h"

#include "models/Igrac.h"
#include "models/Strijelac.h"
#include "models/Tim.h"
#include "models/Utakmica.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
struct TimRecord {
    int idTima;
    std::string naziv;
    std::string grad;
    std::string trener;
    int godinaOsnivanja;
};

struct IgracRecord {
    int idIgraca;
    int idTima;
    std::string ime;
    std::string prezime;
    int brojDresa;
    std::string pozicija;
};

struct UtakmicaRecord {
    int idUtakmice;
    int idDomacina;
    int idGosta;
    std::string datum;
    int rezultatDomacin;
    int rezultatGost;
    int kolo;
};

struct StrijelacRecord {
    int idStrijelca;
    int idUtakmice;
    int idIgraca;
    int minuta;
    bool autoGol;
};

std::vector<std::string> podijeli(const std::string& linija, char delimiter) {
    std::vector<std::string> rezultat;
    std::stringstream ss(linija);
    std::string dio;

    while (std::getline(ss, dio, delimiter)) {
        rezultat.push_back(dio);
    }

    return rezultat;
}

int parseInt(const std::string& vrijednost, const std::string& poruka) {
    try {
        return std::stoi(vrijednost);
    } catch (...) {
        throw std::runtime_error(poruka);
    }
}

bool parseBool(const std::string& vrijednost, const std::string& poruka) {
    if (vrijednost == "0") {
        return false;
    }

    if (vrijednost == "1") {
        return true;
    }

    throw std::runtime_error(poruka);
}
}

PersistenceService::PersistenceService(TimRepository& timRepository, UtakmicaRepository& utakmicaRepository, TimService& timService, UtakmicaService& utakmicaService)
    : timRepository(timRepository),
      utakmicaRepository(utakmicaRepository),
      timService(timService),
      utakmicaService(utakmicaService) {}

void PersistenceService::spremiPodatke(const std::string& direktorij) const {
    namespace fs = std::filesystem;
    const fs::path baza(direktorij);
    fs::create_directories(baza);

    std::ofstream timoviDat(baza / "timovi.txt");
    std::ofstream igraciDat(baza / "igraci.txt");
    std::ofstream utakmiceDat(baza / "utakmice.txt");
    std::ofstream strijelciDat(baza / "strijelci.txt");

    if (!timoviDat || !igraciDat || !utakmiceDat || !strijelciDat) {
        throw std::runtime_error("Nije moguce otvoriti datoteke za spremanje podataka.");
    }

    for (const Tim* tim : timRepository.getAll()) {
        timoviDat << tim->getIdTima() << ';'
                  << tim->getNaziv() << ';'
                  << tim->getGrad() << ';'
                  << tim->getTrener() << ';'
                  << tim->getGodinaOsnivanja() << '\n';

        for (const Igrac* igrac : tim->getIgraci()) {
            igraciDat << igrac->getIdIgraca() << ';'
                      << igrac->getIdTima() << ';'
                      << igrac->getIme() << ';'
                      << igrac->getPrezime() << ';'
                      << igrac->getBrojDresa() << ';'
                      << igrac->getPozicija() << '\n';
        }
    }

    for (const Utakmica* utakmica : utakmicaRepository.getAll()) {
        utakmiceDat << utakmica->getIdUtakmice() << ';'
                    << utakmica->getIdDomacina() << ';'
                    << utakmica->getIdGosta() << ';'
                    << utakmica->getDatum() << ';'
                    << utakmica->getRezultatDomacin() << ';'
                    << utakmica->getRezultatGost() << ';'
                    << utakmica->getKolo() << '\n';

        for (const Strijelac& strijelac : utakmica->getStrijelci()) {
            strijelciDat << strijelac.getIdStrijelca() << ';'
                         << strijelac.getIdUtakmice() << ';'
                         << strijelac.getIdIgraca() << ';'
                         << strijelac.getMinuta() << ';'
                         << (strijelac.isAutoGol() ? 1 : 0) << '\n';
        }
    }
}

void PersistenceService::ucitajPodatke(const std::string& direktorij) {
    namespace fs = std::filesystem;
    const fs::path baza(direktorij);
    const fs::path timoviPath = baza / "timovi.txt";
    const fs::path igraciPath = baza / "igraci.txt";
    const fs::path utakmicePath = baza / "utakmice.txt";
    const fs::path strijelciPath = baza / "strijelci.txt";

    if (!fs::exists(timoviPath) || !fs::exists(igraciPath) || !fs::exists(utakmicePath) || !fs::exists(strijelciPath)) {
        throw std::runtime_error("Datoteke za ucitavanje nisu pronadjene u direktoriju 'data'.");
    }

    std::ifstream timoviDat(timoviPath);
    std::ifstream igraciDat(igraciPath);
    std::ifstream utakmiceDat(utakmicePath);
    std::ifstream strijelciDat(strijelciPath);

    if (!timoviDat || !igraciDat || !utakmiceDat || !strijelciDat) {
        throw std::runtime_error("Nije moguce otvoriti datoteke za ucitavanje podataka.");
    }

    std::vector<TimRecord> timovi;
    std::vector<IgracRecord> igraci;
    std::vector<UtakmicaRecord> utakmice;
    std::vector<StrijelacRecord> strijelci;

    int maxTimId = 0;
    int maxIgracId = 0;
    int maxUtakmicaId = 0;
    int maxStrijelacId = 0;

    std::string linija;
    while (std::getline(timoviDat, linija)) {
        if (linija.empty()) {
            continue;
        }

        const std::vector<std::string> dijelovi = podijeli(linija, ';');
        if (dijelovi.size() != 5) {
            throw std::runtime_error("Neispravan zapis u timovi.txt.");
        }

        TimRecord record{
            parseInt(dijelovi[0], "Neispravan ID tima u timovi.txt."),
            dijelovi[1],
            dijelovi[2],
            dijelovi[3],
            parseInt(dijelovi[4], "Neispravna godina osnivanja u timovi.txt."),
        };
        maxTimId = std::max(maxTimId, record.idTima);
        timovi.push_back(record);
    }

    while (std::getline(igraciDat, linija)) {
        if (linija.empty()) {
            continue;
        }

        const std::vector<std::string> dijelovi = podijeli(linija, ';');
        if (dijelovi.size() != 6) {
            throw std::runtime_error("Neispravan zapis u igraci.txt.");
        }

        IgracRecord record{
            parseInt(dijelovi[0], "Neispravan ID igraca u igraci.txt."),
            parseInt(dijelovi[1], "Neispravan ID tima u igraci.txt."),
            dijelovi[2],
            dijelovi[3],
            parseInt(dijelovi[4], "Neispravan broj dresa u igraci.txt."),
            dijelovi[5],
        };
        maxIgracId = std::max(maxIgracId, record.idIgraca);
        igraci.push_back(record);
    }

    while (std::getline(utakmiceDat, linija)) {
        if (linija.empty()) {
            continue;
        }

        const std::vector<std::string> dijelovi = podijeli(linija, ';');
        if (dijelovi.size() != 7) {
            throw std::runtime_error("Neispravan zapis u utakmice.txt.");
        }

        UtakmicaRecord record{
            parseInt(dijelovi[0], "Neispravan ID utakmice u utakmice.txt."),
            parseInt(dijelovi[1], "Neispravan ID domacina u utakmice.txt."),
            parseInt(dijelovi[2], "Neispravan ID gosta u utakmice.txt."),
            dijelovi[3],
            parseInt(dijelovi[4], "Neispravan rezultat domacina u utakmice.txt."),
            parseInt(dijelovi[5], "Neispravan rezultat gosta u utakmice.txt."),
            parseInt(dijelovi[6], "Neispravno kolo u utakmice.txt."),
        };
        maxUtakmicaId = std::max(maxUtakmicaId, record.idUtakmice);
        utakmice.push_back(record);
    }

    while (std::getline(strijelciDat, linija)) {
        if (linija.empty()) {
            continue;
        }

        const std::vector<std::string> dijelovi = podijeli(linija, ';');
        if (dijelovi.size() != 5) {
            throw std::runtime_error("Neispravan zapis u strijelci.txt.");
        }

        StrijelacRecord record{
            parseInt(dijelovi[0], "Neispravan ID strijelca u strijelci.txt."),
            parseInt(dijelovi[1], "Neispravan ID utakmice u strijelci.txt."),
            parseInt(dijelovi[2], "Neispravan ID igraca u strijelci.txt."),
            parseInt(dijelovi[3], "Neispravna minuta u strijelci.txt."),
            parseBool(dijelovi[4], "Neispravna autogol vrijednost u strijelci.txt."),
        };
        maxStrijelacId = std::max(maxStrijelacId, record.idStrijelca);
        strijelci.push_back(record);
    }

    utakmicaRepository.clear();
    timRepository.clear();

    for (const TimRecord& record : timovi) {
        Tim* tim = new Tim(record.idTima, record.naziv, record.grad, record.trener, record.godinaOsnivanja);
        timRepository.add(tim);
    }

    for (const IgracRecord& record : igraci) {
        Tim* tim = timRepository.findById(record.idTima);
        if (tim == nullptr) {
            throw std::runtime_error("Igrac se odnosi na nepostojeci tim tokom ucitavanja.");
        }

        Igrac* igrac = new Igrac(record.idIgraca, record.ime, record.prezime, record.brojDresa, record.pozicija, record.idTima);
        tim->dodajIgraca(igrac);
    }

    for (const UtakmicaRecord& record : utakmice) {
        if (timRepository.findById(record.idDomacina) == nullptr || timRepository.findById(record.idGosta) == nullptr) {
            throw std::runtime_error("Utakmica se odnosi na nepostojeci tim tokom ucitavanja.");
        }

        Utakmica* utakmica = new Utakmica(record.idUtakmice, record.idDomacina, record.idGosta, record.datum, record.rezultatDomacin, record.rezultatGost, record.kolo);
        utakmicaRepository.add(utakmica);
    }

    for (const StrijelacRecord& record : strijelci) {
        Utakmica* utakmica = utakmicaRepository.findById(record.idUtakmice);
        if (utakmica == nullptr) {
            throw std::runtime_error("Strijelac se odnosi na nepostojecu utakmicu tokom ucitavanja.");
        }

        if (timService.pronadjiIgraca(record.idIgraca) == nullptr) {
            throw std::runtime_error("Strijelac se odnosi na nepostojeceg igraca tokom ucitavanja.");
        }

        utakmica->dodajStrijelca(Strijelac(record.idStrijelca, record.idUtakmice, record.idIgraca, record.minuta, record.autoGol));
    }

    timService.postaviSljedeceIdVrijednosti(maxTimId + 1, maxIgracId + 1);
    utakmicaService.postaviSljedeceIdVrijednosti(maxUtakmicaId + 1, maxStrijelacId + 1);
}
