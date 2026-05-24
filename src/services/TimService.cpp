#include "services/TimService.h"

#include <stdexcept>

TimService::TimService(TimRepository& timRepository)
    : timRepository(timRepository), nextTimId(1) {}

Tim* TimService::dodajTim(const std::string& naziv, const std::string& grad, const std::string& trener, int godinaOsnivanja) {
    validirajTim(naziv, grad, trener, godinaOsnivanja);

    Tim* tim = new Tim(nextTimId++, naziv, grad, trener, godinaOsnivanja);
    timRepository.add(tim);
    return tim;
}

const std::vector<Tim*>& TimService::vratiSveTimove() const {
    return timRepository.getAll();
}

Tim* TimService::pronadjiTim(int idTima) const {
    return timRepository.findById(idTima);
}

void TimService::validirajTim(const std::string& naziv, const std::string& grad, const std::string& trener, int godinaOsnivanja) const {
    if (naziv.empty()) {
        throw std::runtime_error("Naziv tima je obavezan.");
    }

    if (grad.empty()) {
        throw std::runtime_error("Grad je obavezan.");
    }

    if (trener.empty()) {
        throw std::runtime_error("Trener je obavezan.");
    }

    if (godinaOsnivanja < 1800 || godinaOsnivanja > 2100) {
        throw std::runtime_error("Godina osnivanja nije validna.");
    }

    if (timRepository.findByName(naziv) != nullptr) {
        throw std::runtime_error("Tim sa tim nazivom vec postoji.");
    }
}
