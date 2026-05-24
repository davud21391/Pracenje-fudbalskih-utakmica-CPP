#pragma once

#include "repositories/TimRepository.h"
#include "repositories/UtakmicaRepository.h"

#include <map>
#include <string>
#include <vector>

struct StatistikaStrijelca {
    int idIgraca;
    std::string punoIme;
    int brojGolova;
};

struct StatistikaTima {
    int idTima;
    std::string nazivTima;
    int odigrane;
    int pobjede;
    int nerijesene;
    int porazi;
    int datiGolovi;
    int primljeniGolovi;
    int golRazlika;
    int bodovi;
};

class UtakmicaService {
public:
    UtakmicaService(UtakmicaRepository& utakmicaRepository, TimRepository& timRepository);

    Utakmica* dodajUtakmicu(int idDomacina, int idGosta, const std::string& datum, int rezultatDomacin, int rezultatGost, int kolo);
    Strijelac dodajStrijelca(int idUtakmice, int idIgraca, int minuta, bool autoGol);
    bool urediUtakmicu(int idUtakmice, int idDomacina, int idGosta, const std::string& datum, int rezultatDomacin, int rezultatGost, int kolo);
    bool obrisiUtakmicu(int idUtakmice);
    const std::vector<Utakmica*>& vratiSveUtakmice() const;
    Utakmica* pronadjiUtakmicu(int idUtakmice) const;
    std::vector<StatistikaStrijelca> vratiListuStrijelaca() const;
    std::vector<StatistikaTima> vratiTabelu() const;
    std::vector<const Utakmica*> vratiUtakmiceTima(int idTima) const;
    std::vector<const Utakmica*> pretraziPoDatumu(const std::string& datum) const;
    std::vector<const Utakmica*> pretraziPoKolu(int kolo) const;
    std::vector<const Utakmica*> sortirajPoDatumu() const;
    std::vector<const Utakmica*> sortirajPoKolu() const;
    void postaviSljedeceIdVrijednosti(int sljedecaUtakmicaId, int sljedeciStrijelacId);


private:
    UtakmicaRepository& utakmicaRepository;
    TimRepository& timRepository;
    int nextUtakmicaId;
    int nextStrijelacId;

    void validirajUtakmicu(int idDomacina, int idGosta, const std::string& datum, int rezultatDomacin, int rezultatGost, int kolo) const;
    void validirajStrijelca(const Utakmica& utakmica, int idIgraca, int minuta) const;
    std::string normalizujDatum(const std::string& datum) const;
    bool validanDatumFormat(const std::string& datum) const;
    int vrijednostDatumaZaSort(const std::string& datum) const;
    Igrac* pronadjiIgracaUMecu(const Utakmica& utakmica, int idIgraca) const;
    int ukupanBrojEvidentiranihGolova(const Utakmica& utakmica) const;
};
