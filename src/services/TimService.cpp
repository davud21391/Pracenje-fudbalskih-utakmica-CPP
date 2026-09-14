#include "services/TimService.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace {
std::string uMalaSlova(const std::string& tekst) {
    std::string rezultat = tekst;
    std::transform(rezultat.begin(), rezultat.end(), rezultat.begin(), [](unsigned char znak) {
        return static_cast<char>(std::tolower(znak));
    });
    return rezultat;
}
}

TimService::TimService(TimRepository& timRepository)
    : timRepository(timRepository), nextTimId(1), nextIgracId(1) {}

Tim* TimService::dodajTim(const std::string& naziv, const std::string& grad, const std::string& trener, int godinaOsnivanja) {
    validirajTim(naziv, grad, trener, godinaOsnivanja);

    Tim* tim = new Tim(nextTimId++, naziv, grad, trener, godinaOsnivanja);
    timRepository.add(tim);
    return tim;
}

bool TimService::urediTim(int idTima, const std::string& naziv, const std::string& grad, const std::string& trener, int godinaOsnivanja) {
    Tim* tim = pronadjiTim(idTima);
    if (tim == nullptr) {
        return false;
    }

    validirajIzmjenuTima(idTima, naziv, grad, trener, godinaOsnivanja);

    tim->setNaziv(naziv);
    tim->setGrad(grad);
    tim->setTrener(trener);
    tim->setGodinaOsnivanja(godinaOsnivanja);
    return true;
}

bool TimService::obrisiTim(int idTima) {
    return timRepository.remove(idTima);
}

Igrac* TimService::dodajIgracaUTim(int idTima, const std::string& ime, const std::string& prezime, int brojDresa, const std::string& pozicija) {
    Tim* tim = pronadjiTim(idTima);
    if (tim == nullptr) {
        throw std::runtime_error("Tim sa zadanim ID-em ne postoji.");
    }

    validirajIgraca(*tim, ime, prezime, brojDresa, pozicija);

    Igrac* igrac = new Igrac(nextIgracId++, ime, prezime, brojDresa, pozicija, tim->getIdTima());
    tim->dodajIgraca(igrac);
    return igrac;
}

const std::vector<Tim*>& TimService::vratiSveTimove() const {
    return timRepository.getAll();
}

Tim* TimService::pronadjiTim(int idTima) const {
    return timRepository.findById(idTima);
}

Igrac* TimService::pronadjiIgraca(int idIgraca) const {
    const std::vector<Tim*>& timovi = timRepository.getAll();

    for (Tim* tim : timovi) {
        Igrac* igrac = tim->pronadjiIgracaPoId(idIgraca);
        if (igrac != nullptr) {
            return igrac;
        }
    }

    return nullptr;
}

std::vector<const Tim*> TimService::pretraziTimove(const std::string& pojam) const {
    const std::string trazeniPojam = uMalaSlova(pojam);
    if (trazeniPojam.empty()) {
        throw std::runtime_error("Pojam za pretragu ne smije biti prazan.");
    }

    std::vector<const Tim*> rezultat;
    for (const Tim* tim : timRepository.getAll()) {
        const std::string naziv = uMalaSlova(tim->getNaziv());
        const std::string grad = uMalaSlova(tim->getGrad());

        if (naziv.find(trazeniPojam) != std::string::npos || grad.find(trazeniPojam) != std::string::npos) {
            rezultat.push_back(tim);
        }
    }

    return rezultat;
}

const std::vector<Igrac*>& TimService::vratiIgraceTima(int idTima) const {
    Tim* tim = pronadjiTim(idTima);
    if (tim == nullptr) {
        throw std::runtime_error("Tim sa zadanim ID-em ne postoji.");
    }

    return tim->getIgraci();
}

void TimService::postaviSljedeceIdVrijednosti(int sljedeciTimId, int sljedeciIgracId) {
    nextTimId = sljedeciTimId;
    nextIgracId = sljedeciIgracId;
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

void TimService::validirajIzmjenuTima(int idTima, const std::string& naziv, const std::string& grad, const std::string& trener, int godinaOsnivanja) const {
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

    Tim* timSaIstimNazivom = timRepository.findByName(naziv);
    if (timSaIstimNazivom != nullptr && timSaIstimNazivom->getIdTima() != idTima) {
        throw std::runtime_error("Tim sa tim nazivom vec postoji.");
    }
}

void TimService::validirajIgraca(const Tim& tim, const std::string& ime, const std::string& prezime, int brojDresa, const std::string& pozicija) const {
    if (ime.empty()) {
        throw std::runtime_error("Ime igraca je obavezno.");
    }

    if (prezime.empty()) {
        throw std::runtime_error("Prezime igraca je obavezno.");
    }

    if (pozicija.empty()) {
        throw std::runtime_error("Pozicija igraca je obavezna.");
    }

    if (brojDresa < 1 || brojDresa > 99) {
        throw std::runtime_error("Broj dresa mora biti izmedju 1 i 99.");
    }

    if (tim.postojiBrojDresa(brojDresa)) {
        throw std::runtime_error("Igrac sa tim brojem dresa vec postoji u timu.");
    }
}
