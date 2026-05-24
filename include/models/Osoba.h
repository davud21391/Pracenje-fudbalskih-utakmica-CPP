#pragma once

#include <string>

class Osoba {
public:
    Osoba(std::string ime, std::string prezime);
    virtual ~Osoba() = default;

    const std::string& getIme() const;
    const std::string& getPrezime() const;
    std::string getPunoIme() const;

protected:
    std::string ime;
    std::string prezime;
};
