#include "ui/Application.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

Application::Application()
    : timRepository(), utakmicaRepository(), timService(timRepository), utakmicaService(utakmicaRepository, timRepository) {}

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
    std::cout << "2. Dodaj igraca timu\n";
    std::cout << "3. Dodaj utakmicu\n";
    std::cout << "4. Obrisi utakmicu\n";
    std::cout << "5. Evidentiraj strijelce\n";
    std::cout << "6. Pretraga utakmica\n";
    std::cout << "7. Prikaz tabele\n";
    std::cout << "8. Pregled utakmica tima\n";
    std::cout << "9. Lista strijelaca\n";
    std::cout << "10. Spremi podatke u datoteku\n";
    std::cout << "11. Ucitaj podatke iz datoteke\n";
    std::cout << "12. Prikazi timove\n";
    std::cout << "13. Prikazi igrace tima\n";
    std::cout << "14. Prikazi utakmice\n";
    std::cout << "0. Izlaz\n";
}

void Application::obradiIzbor(int izbor, bool& running) {
    try {
        switch (izbor) {
            case 1:
                dodajTim();
                break;
            case 2:
                dodajIgracaUTim();
                break;
            case 3:
                dodajUtakmicu();
                break;
            case 5:
                evidentirajStrijelca();
                break;
            case 7:
                prikaziTabelu();
                break;
            case 9:
                prikaziListuStrijelaca();
                break;
            case 12:
                prikaziTimove();
                break;
            case 13:
                prikaziIgraceTima();
                break;
            case 14:
                prikaziUtakmice();
                break;
            case 0:
                running = false;
                std::cout << "Zatvaranje aplikacije.\n";
                break;
            case 4:
            case 6:
            case 8:
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

void Application::dodajIgracaUTim() {
    if (timService.vratiSveTimove().empty()) {
        std::cout << "Prvo morate dodati barem jedan tim.\n";
        return;
    }

    std::string ime;
    std::string prezime;
    std::string pozicija;

    const int idTima = ucitajInt("Unesite ID tima: ");

    std::cout << "Unesite ime igraca: ";
    std::getline(std::cin >> std::ws, ime);

    std::cout << "Unesite prezime igraca: ";
    std::getline(std::cin, prezime);

    const int brojDresa = ucitajInt("Unesite broj dresa: ");

    std::cout << "Unesite poziciju igraca: ";
    std::getline(std::cin >> std::ws, pozicija);

    Igrac* igrac = timService.dodajIgracaUTim(idTima, ime, prezime, brojDresa, pozicija);
    std::cout << "Igrac uspjesno dodat. ID igraca: " << igrac->getIdIgraca() << "\n";
}

void Application::dodajUtakmicu() {
    if (timService.vratiSveTimove().size() < 2) {
        std::cout << "Morate imati najmanje dva tima za unos utakmice.\n";
        return;
    }

    std::string datum;
    const int idDomacina = ucitajInt("Unesite ID domaceg tima: ");
    const int idGosta = ucitajInt("Unesite ID gostujuceg tima: ");

    std::cout << "Unesite datum utakmice (dd.mm.gggg): ";
    std::getline(std::cin >> std::ws, datum);

    const int rezultatDomacin = ucitajInt("Unesite broj golova domacina: ");
    const int rezultatGost = ucitajInt("Unesite broj golova gosta: ");
    const int kolo = ucitajInt("Unesite kolo: ");

    Utakmica* utakmica = utakmicaService.dodajUtakmicu(idDomacina, idGosta, datum, rezultatDomacin, rezultatGost, kolo);
    std::cout << "Utakmica uspjesno dodata. ID utakmice: " << utakmica->getIdUtakmice() << "\n";
}

void Application::evidentirajStrijelca() {
    if (utakmicaService.vratiSveUtakmice().empty()) {
        std::cout << "Prvo morate unijeti barem jednu utakmicu.\n";
        return;
    }

    const int idUtakmice = ucitajInt("Unesite ID utakmice: ");
    Utakmica* utakmica = utakmicaService.pronadjiUtakmicu(idUtakmice);
    if (utakmica == nullptr) {
        std::cout << "Utakmica sa zadanim ID-em ne postoji.\n";
        return;
    }

    const Tim* domacin = timService.pronadjiTim(utakmica->getIdDomacina());
    const Tim* gost = timService.pronadjiTim(utakmica->getIdGosta());

    std::cout << "Dostupni igraci za ovu utakmicu:\n";
    if (domacin != nullptr) {
        std::cout << "- " << domacin->getNaziv() << ":\n";
        for (const Igrac* igrac : domacin->getIgraci()) {
            std::cout << "  ID " << igrac->getIdIgraca() << " | " << igrac->getPunoIme() << " | DRES " << igrac->getBrojDresa() << "\n";
        }
    }

    if (gost != nullptr) {
        std::cout << "- " << gost->getNaziv() << ":\n";
        for (const Igrac* igrac : gost->getIgraci()) {
            std::cout << "  ID " << igrac->getIdIgraca() << " | " << igrac->getPunoIme() << " | DRES " << igrac->getBrojDresa() << "\n";
        }
    }

    const int idIgraca = ucitajInt("Unesite ID igraca koji je postigao gol: ");
    const int minuta = ucitajInt("Unesite minutu gola: ");
    const bool autoGol = ucitajDaNe("Da li je autogol? (d/n): ");

    const Strijelac strijelac = utakmicaService.dodajStrijelca(idUtakmice, idIgraca, minuta, autoGol);
    std::cout << "Gol evidentiran. ID strijelca: " << strijelac.getIdStrijelca() << "\n";
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

void Application::prikaziIgraceTima() const {
    if (timService.vratiSveTimove().empty()) {
        std::cout << "Nema unesenih timova.\n";
        return;
    }

    const int idTima = ucitajInt("Unesite ID tima za pregled igraca: ");
    Tim* tim = timService.pronadjiTim(idTima);

    if (tim == nullptr) {
        std::cout << "Tim sa zadanim ID-em ne postoji.\n";
        return;
    }

    const std::vector<Igrac*>& igraci = timService.vratiIgraceTima(idTima);
    if (igraci.empty()) {
        std::cout << "Tim " << tim->getNaziv() << " nema evidentiranih igraca.\n";
        return;
    }

    std::cout << "\n--- Igraci tima: " << tim->getNaziv() << " ---\n";
    for (const Igrac* igrac : igraci) {
        std::cout << "ID: " << igrac->getIdIgraca()
                  << " | Ime i prezime: " << igrac->getPunoIme()
                  << " | Broj dresa: " << igrac->getBrojDresa()
                  << " | Pozicija: " << igrac->getPozicija()
                  << "\n";
    }
}

void Application::prikaziUtakmice() const {
    const std::vector<Utakmica*>& utakmice = utakmicaService.vratiSveUtakmice();

    if (utakmice.empty()) {
        std::cout << "Nema unesenih utakmica.\n";
        return;
    }

    std::cout << "\n--- Lista utakmica ---\n";
    for (const Utakmica* utakmica : utakmice) {
        const Tim* domacin = timService.pronadjiTim(utakmica->getIdDomacina());
        const Tim* gost = timService.pronadjiTim(utakmica->getIdGosta());

        std::cout << "ID: " << utakmica->getIdUtakmice()
                  << " | Kolo: " << utakmica->getKolo()
                  << " | Datum: " << utakmica->getDatum()
                  << " | " << (domacin != nullptr ? domacin->getNaziv() : "Nepoznat domacin")
                  << " " << utakmica->getRezultatDomacin()
                  << ":" << utakmica->getRezultatGost() << " "
                  << (gost != nullptr ? gost->getNaziv() : "Nepoznat gost");

        if (!utakmica->getStrijelci().empty()) {
            std::cout << " | Strijelci: ";
            bool prvi = true;
            for (const Strijelac& strijelac : utakmica->getStrijelci()) {
                Igrac* igrac = timService.pronadjiIgraca(strijelac.getIdIgraca());
                if (!prvi) {
                    std::cout << ", ";
                }

                std::cout << (igrac != nullptr ? igrac->getPunoIme() : "Nepoznat igrac")
                          << " (" << strijelac.getMinuta() << "')";

                if (strijelac.isAutoGol()) {
                    std::cout << " [AG]";
                }

                prvi = false;
            }
        }

        std::cout
                  << "\n";
    }
}

void Application::prikaziTabelu() const {
    const std::vector<StatistikaTima> tabela = utakmicaService.vratiTabelu();

    if (tabela.empty()) {
        std::cout << "Nema unesenih timova za prikaz tabele.\n";
        return;
    }

    std::cout << "\n--- Tabela ---\n";
    std::cout << "Poz | Tim | OU | P | N | I | DG | PG | GR | B\n";

    int pozicija = 1;
    for (const StatistikaTima& red : tabela) {
        std::cout << pozicija++
                  << ". | " << red.nazivTima
                  << " | " << red.odigrane
                  << " | " << red.pobjede
                  << " | " << red.nerijesene
                  << " | " << red.porazi
                  << " | " << red.datiGolovi
                  << " | " << red.primljeniGolovi
                  << " | " << red.golRazlika
                  << " | " << red.bodovi
                  << "\n";
    }
}

void Application::prikaziListuStrijelaca() const {
    const std::vector<StatistikaStrijelca> statistika = utakmicaService.vratiListuStrijelaca();
    if (statistika.empty()) {
        std::cout << "Nema evidentiranih strijelaca.\n";
        return;
    }

    std::cout << "\n--- Lista strijelaca ---\n";
    int redniBroj = 1;
    for (const StatistikaStrijelca& stavka : statistika) {
        std::cout << redniBroj++ << ". " << stavka.punoIme
                  << " | Golovi: " << stavka.brojGolova
                  << " | ID igraca: " << stavka.idIgraca << "\n";
    }
}

bool Application::ucitajDaNe(const char* prompt) const {
    while (true) {
        std::string unos;
        std::cout << prompt;
        std::getline(std::cin >> std::ws, unos);

        if (unos == "d" || unos == "D") {
            return true;
        }

        if (unos == "n" || unos == "N") {
            return false;
        }

        std::cout << "Molimo unesite 'd' ili 'n'.\n";
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
