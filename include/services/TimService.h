#pragma once

#include "repositories/TimRepository.h"

#include <string>

class TimService {
public:
    explicit TimService(TimRepository& timRepository);

    Tim* dodajTim(const std::string& naziv, const std::string& grad, const std::string& trener, int godinaOsnivanja);
    const std::vector<Tim*>& vratiSveTimove() const;
    Tim* pronadjiTim(int idTima) const;

private:
    TimRepository& timRepository;
    int nextTimId;

    void validirajTim(const std::string& naziv, const std::string& grad, const std::string& trener, int godinaOsnivanja) const;
};
