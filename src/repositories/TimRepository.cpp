#include "repositories/TimRepository.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace {
std::string toLowerCopy(const std::string& value) {
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return result;
}
}

TimRepository::~TimRepository() {
    clear();
}

void TimRepository::add(Tim* tim) {
    if (tim == nullptr) {
        throw std::invalid_argument("Tim ne moze biti null.");
    }

    if (timoviPoId.find(tim->getIdTima()) != timoviPoId.end()) {
        throw std::runtime_error("Tim sa istim ID-em vec postoji.");
    }

    timovi.push_back(tim);
    timoviPoId[tim->getIdTima()] = tim;
}

Tim* TimRepository::findById(int idTima) const {
    const auto it = timoviPoId.find(idTima);
    return it == timoviPoId.end() ? nullptr : it->second;
}

Tim* TimRepository::findByName(const std::string& naziv) const {
    const std::string trazeniNaziv = toLowerCopy(naziv);

    const auto it = std::find_if(timovi.begin(), timovi.end(), [&trazeniNaziv](Tim* tim) {
        return toLowerCopy(tim->getNaziv()) == trazeniNaziv;
    });

    return it == timovi.end() ? nullptr : *it;
}

const std::vector<Tim*>& TimRepository::getAll() const {
    return timovi;
}

bool TimRepository::remove(int idTima) {
    const auto mapIt = timoviPoId.find(idTima);
    if (mapIt == timoviPoId.end()) {
        return false;
    }

    Tim* tim = mapIt->second;
    timoviPoId.erase(mapIt);

    const auto vectorIt = std::find(timovi.begin(), timovi.end(), tim);
    if (vectorIt != timovi.end()) {
        timovi.erase(vectorIt);
    }

    delete tim;
    return true;
}

void TimRepository::clear() {
    for (Tim* tim : timovi) {
        delete tim;
    }

    timovi.clear();
    timoviPoId.clear();
}
