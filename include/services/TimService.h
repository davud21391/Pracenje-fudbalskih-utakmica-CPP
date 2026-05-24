#pragma once

#include "repositories/TimRepository.h"

#include <string>

class TimService {
public:
    explicit TimService(TimRepository& timRepository);

    Tim* dodajTim(const std::string& naziv, const std::string& grad, const std::string& trener, int godinaOsnivanja);
    Igrac* dodajIgracaUTim(int idTima, const std::string& ime, const std::string& prezime, int brojDresa, const std::string& pozicija);
    const std::vector<Tim*>& vratiSveTimove() const;
    Tim* pronadjiTim(int idTima) const;
    Igrac* pronadjiIgraca(int idIgraca) const;
    const std::vector<Igrac*>& vratiIgraceTima(int idTima) const;

private:
    TimRepository& timRepository;
    int nextTimId;
    int nextIgracId;

    void validirajTim(const std::string& naziv, const std::string& grad, const std::string& trener, int godinaOsnivanja) const;
    void validirajIgraca(const Tim& tim, const std::string& ime, const std::string& prezime, int brojDresa, const std::string& pozicija) const;
};
