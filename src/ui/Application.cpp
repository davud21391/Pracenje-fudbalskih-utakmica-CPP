#include "ui/Application.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

Application::Application()
    : timRepository(), timService(timRepository) {}

void Application::run() {
    bool running = true;

    while (running) {
        prikaziMeni();
        const int izbor = ucitajInt("Odaberite opciju: ");
        obradiIzbor(izbor, running);
    }
}

void Application::prikaziMeni() const {
    std::cout << "\n=== Pracenje rezultata fudbalskih utakmica ===\n";
    std::cout << "1. Dodaj tim\n";
    std::cout << "2. Unesi rezultat utakmice\n";
    std::cout << "3. Uredi podatke o utakmici\n";
    std::cout << "4. Obrisi utakmicu\n";
    std::cout << "5. Evidentiraj strijelce\n";
    std::cout << "6. Pretraga utakmica\n";
    std::cout << "7. Prikaz tabele\n";
    std::cout << "8. Pregled utakmica tima\n";
    std::cout << "9. Lista strijelaca\n";
    std::cout << "10. Spremi podatke u datoteku\n";
    std::cout << "11. Ucitaj podatke iz datoteke\n";
    std::cout << "12. Prikazi timove\n";
    std::cout << "0. Izlaz\n";
}

void Application::obradiIzbor(int izbor, bool& running) {
    try {
        switch (izbor) {
            case 1:
                dodajTim();
                break;
            case 12:
                prikaziTimove();
                break;
            case 0:
                running = false;
                std::cout << "Zatvaranje aplikacije.\n";
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                std::cout << "Ova funkcionalnost je planirana za naredne faze implementacije.\n";
                break;
            default:
                std::cout << "Nepostojeca opcija. Pokusajte ponovo.\n";
                break;
        }
    } catch (const std::exception& ex) {
        std::cout << "Greska: " << ex.what() << "\n";
    }
}

void Application::dodajTim() {
    std::string naziv;
    std::string grad;
    std::string trener;

    std::cout << "Unesite naziv tima: ";
    std::getline(std::cin >> std::ws, naziv);

    std::cout << "Unesite grad: ";
    std::getline(std::cin, grad);

    std::cout << "Unesite ime trenera: ";
    std::getline(std::cin, trener);

    const int godinaOsnivanja = ucitajInt("Unesite godinu osnivanja: ");
    Tim* tim = timService.dodajTim(naziv, grad, trener, godinaOsnivanja);

    std::cout << "Tim uspjesno dodat. ID: " << tim->getIdTima() << "\n";
}

void Application::prikaziTimove() const {
    const std::vector<Tim*>& timovi = timService.vratiSveTimove();

    if (timovi.empty()) {
        std::cout << "Nema unesenih timova.\n";
        return;
    }

    std::cout << "\n--- Lista timova ---\n";
    for (const Tim* tim : timovi) {
        std::cout << "ID: " << tim->getIdTima()
                  << " | Naziv: " << tim->getNaziv()
                  << " | Grad: " << tim->getGrad()
                  << " | Trener: " << tim->getTrener()
                  << " | Godina osnivanja: " << tim->getGodinaOsnivanja()
                  << "\n";
    }
}

int Application::ucitajInt(const char* prompt) const {
    int vrijednost;

    while (true) {
        std::cout << prompt;
        if (std::cin >> vrijednost) {
            return vrijednost;
        }

        std::cout << "Molimo unesite broj.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
