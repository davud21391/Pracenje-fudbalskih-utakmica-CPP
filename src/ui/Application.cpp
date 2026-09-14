#include "ui/Application.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

Application::Application()
    : timRepository(),
      utakmicaRepository(),
      timService(timRepository),
      utakmicaService(utakmicaRepository, timRepository),
      persistenceService(timRepository, utakmicaRepository, timService, utakmicaService) {}

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
    std::cout << "1. Timovi\n";
    std::cout << "2. Utakmice\n";
    std::cout << "3. Statistika\n";
    std::cout << "4. Podaci\n";
    std::cout << "0. Izlaz\n";
}

void Application::obradiIzbor(int izbor, bool& running) {
    try {
        switch (izbor) {
            case 1:
                meniTimovi();
                break;
            case 2:
                meniUtakmice();
                break;
            case 3:
                meniStatistika();
                break;
            case 4:
                meniPodaci();
                break;
            case 0:
                running = false;
                std::cout << "Zatvaranje aplikacije.\n";
                break;
            default:
                std::cout << "Nepostojeca opcija. Pokusajte ponovo.\n";
                break;
        }
    } catch (const std::exception& ex) {
        std::cout << "Greska: " << ex.what() << "\n";
    }
}

void Application::meniTimovi() {
    bool nazad = false;

    while (!nazad) {
        std::cout << "\n--- Timovi ---\n";
        std::cout << "1. Dodaj tim\n";
        std::cout << "2. Dodaj igraca timu\n";
        std::cout << "3. Prikazi timove\n";
        std::cout << "4. Prikazi igrace tima\n";
        std::cout << "0. Nazad\n";

        const int izbor = ucitajInt("Odaberite opciju: ");
        switch (izbor) {
            case 1:
                dodajTim();
                break;
            case 2:
                dodajIgracaUTim();
                break;
            case 3:
                prikaziTimove();
                break;
            case 4:
                prikaziIgraceTima();
                break;
            case 0:
                nazad = true;
                break;
            default:
                std::cout << "Nepostojeca opcija za timove.\n";
                break;
        }
    }
}

void Application::meniUtakmice() {
    bool nazad = false;

    while (!nazad) {
        std::cout << "\n--- Utakmice ---\n";
        std::cout << "1. Dodaj utakmicu\n";
        std::cout << "2. Uredi utakmicu\n";
        std::cout << "3. Obrisi utakmicu\n";
        std::cout << "4. Evidentiraj strijelce\n";
        std::cout << "5. Pretraga utakmica\n";
        std::cout << "6. Pregled utakmica tima\n";
        std::cout << "7. Prikazi utakmice\n";
        std::cout << "8. Sortiraj utakmice\n";
        std::cout << "0. Nazad\n";

        const int izbor = ucitajInt("Odaberite opciju: ");
        switch (izbor) {
            case 1:
                dodajUtakmicu();
                break;
            case 2:
                urediUtakmicu();
                break;
            case 3:
                obrisiUtakmicu();
                break;
            case 4:
                evidentirajStrijelca();
                break;
            case 5:
                pretraziUtakmice();
                break;
            case 6:
                prikaziUtakmiceTima();
                break;
            case 7:
                prikaziUtakmice();
                break;
            case 8:
                sortirajUtakmice();
                break;
            case 0:
                nazad = true;
                break;
            default:
                std::cout << "Nepostojeca opcija za utakmice.\n";
                break;
        }
    }
}

void Application::meniStatistika() const {
    bool nazad = false;

    while (!nazad) {
        std::cout << "\n--- Statistika ---\n";
        std::cout << "1. Prikaz tabele\n";
        std::cout << "2. Lista strijelaca\n";
        std::cout << "0. Nazad\n";

        const int izbor = ucitajInt("Odaberite opciju: ");
        switch (izbor) {
            case 1:
                prikaziTabelu();
                break;
            case 2:
                prikaziListuStrijelaca();
                break;
            case 0:
                nazad = true;
                break;
            default:
                std::cout << "Nepostojeca opcija za statistiku.\n";
                break;
        }
    }
}

void Application::meniPodaci() {
    bool nazad = false;

    while (!nazad) {
        std::cout << "\n--- Podaci ---\n";
        std::cout << "1. Spremi podatke u datoteku\n";
        std::cout << "2. Ucitaj podatke iz datoteke\n";
        std::cout << "0. Nazad\n";

        const int izbor = ucitajInt("Odaberite opciju: ");
        switch (izbor) {
            case 1:
                spremiPodatke();
                break;
            case 2:
                ucitajPodatke();
                break;
            case 0:
                nazad = true;
                break;
            default:
                std::cout << "Nepostojeca opcija za podatke.\n";
                break;
        }
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

void Application::urediUtakmicu() {
    if (utakmicaService.vratiSveUtakmice().empty()) {
        std::cout << "Nema unesenih utakmica za uredjivanje.\n";
        return;
    }

    prikaziUtakmice();

    std::string datum;
    const int idUtakmice = ucitajInt("Unesite ID utakmice za uredjivanje: ");
    Utakmica* postojecaUtakmica = utakmicaService.pronadjiUtakmicu(idUtakmice);
    if (postojecaUtakmica == nullptr) {
        std::cout << "Utakmica sa zadanim ID-em ne postoji.\n";
        return;
    }

    const int idDomacina = ucitajInt("Unesite novi ID domaceg tima: ");
    const int idGosta = ucitajInt("Unesite novi ID gostujuceg tima: ");

    std::cout << "Unesite novi datum utakmice (dd.mm.gggg): ";
    std::getline(std::cin >> std::ws, datum);

    const int rezultatDomacin = ucitajInt("Unesite novi broj golova domacina: ");
    const int rezultatGost = ucitajInt("Unesite novi broj golova gosta: ");
    const int kolo = ucitajInt("Unesite novo kolo: ");

    const bool obrisaniStrijelci = utakmicaService.urediUtakmicu(idUtakmice, idDomacina, idGosta, datum, rezultatDomacin, rezultatGost, kolo);
    std::cout << "Utakmica uspjesno uredjena.\n";

    if (obrisaniStrijelci) {
        std::cout << "Napomena: strijelci su obrisani jer su timovi ili rezultat promijenjeni.\n";
    }
}

void Application::sortirajUtakmice() const {
    if (utakmicaService.vratiSveUtakmice().empty()) {
        std::cout << "Nema unesenih utakmica za sortiranje.\n";
        return;
    }

    std::cout << "\nSortiranje utakmica:\n";
    std::cout << "1. Po datumu\n";
    std::cout << "2. Po kolu\n";

    const int kriterij = ucitajInt("Odaberite kriterij sortiranja: ");
    switch (kriterij) {
        case 1:
            ispisiListuUtakmica(utakmicaService.sortirajPoDatumu(), "Sortirane utakmice po datumu");
            break;
        case 2:
            ispisiListuUtakmica(utakmicaService.sortirajPoKolu(), "Sortirane utakmice po kolu");
            break;
        default:
            std::cout << "Nepostojeci kriterij sortiranja.\n";
            break;
    }
}

void Application::obrisiUtakmicu() {
    if (utakmicaService.vratiSveUtakmice().empty()) {
        std::cout << "Nema unesenih utakmica za brisanje.\n";
        return;
    }

    prikaziUtakmice();

    const int idUtakmice = ucitajInt("Unesite ID utakmice za brisanje: ");
    Utakmica* utakmica = utakmicaService.pronadjiUtakmicu(idUtakmice);
    if (utakmica == nullptr) {
        std::cout << "Utakmica sa zadanim ID-em ne postoji.\n";
        return;
    }

    if (!ucitajDaNe("Potvrdite brisanje utakmice? (d/n): ")) {
        std::cout << "Brisanje utakmice otkazano.\n";
        return;
    }

    if (utakmicaService.obrisiUtakmicu(idUtakmice)) {
        std::cout << "Utakmica uspjesno obrisana.\n";
        return;
    }

    std::cout << "Brisanje utakmice nije uspjelo.\n";
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

void Application::spremiPodatke() const {
    persistenceService.spremiPodatke();
    std::cout << "Podaci su uspjesno spremljeni u direktorij 'data'.\n";
}

void Application::ucitajPodatke() {
    if (timService.vratiSveTimove().empty() && utakmicaService.vratiSveUtakmice().empty()) {
        persistenceService.ucitajPodatke();
        std::cout << "Podaci su uspjesno ucitani iz direktorija 'data'.\n";
        return;
    }

    if (!ucitajDaNe("Ucitaj podatke i prepisi trenutno stanje? (d/n): ")) {
        std::cout << "Ucitavanje podataka otkazano.\n";
        return;
    }

    persistenceService.ucitajPodatke();
    std::cout << "Podaci su uspjesno ucitani iz direktorija 'data'.\n";
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

    std::vector<const Utakmica*> prikaz;
    for (const Utakmica* utakmica : utakmice) {
        prikaz.push_back(utakmica);
    }

    ispisiListuUtakmica(prikaz, "Lista utakmica");
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

void Application::prikaziUtakmiceTima() const {
    if (timService.vratiSveTimove().empty()) {
        std::cout << "Nema unesenih timova.\n";
        return;
    }

    const int idTima = ucitajInt("Unesite ID tima za pregled utakmica: ");
    const Tim* trazeniTim = timService.pronadjiTim(idTima);
    if (trazeniTim == nullptr) {
        std::cout << "Tim sa zadanim ID-em ne postoji.\n";
        return;
    }

    const std::vector<const Utakmica*> utakmice = utakmicaService.vratiUtakmiceTima(idTima);
    if (utakmice.empty()) {
        std::cout << "Tim " << trazeniTim->getNaziv() << " nema evidentiranih utakmica.\n";
        return;
    }

    ispisiListuUtakmica(utakmice, std::string("Utakmice tima: ") + trazeniTim->getNaziv());
}

void Application::pretraziUtakmice() const {
    if (utakmicaService.vratiSveUtakmice().empty()) {
        std::cout << "Nema unesenih utakmica za pretragu.\n";
        return;
    }

    std::cout << "\nPretraga utakmica:\n";
    std::cout << "1. Po timu\n";
    std::cout << "2. Po datumu\n";
    std::cout << "3. Po kolu\n";

    const int kriterij = ucitajInt("Odaberite kriterij pretrage: ");

    switch (kriterij) {
        case 1: {
            const int idTima = ucitajInt("Unesite ID tima: ");
            const Tim* tim = timService.pronadjiTim(idTima);
            if (tim == nullptr) {
                std::cout << "Tim sa zadanim ID-em ne postoji.\n";
                return;
            }

            const std::vector<const Utakmica*> rezultat = utakmicaService.vratiUtakmiceTima(idTima);
            if (rezultat.empty()) {
                std::cout << "Nema utakmica za trazeni tim.\n";
                return;
            }

            ispisiListuUtakmica(rezultat, std::string("Pretraga po timu: ") + tim->getNaziv());
            break;
        }
        case 2: {
            std::string datum;
            std::cout << "Unesite datum (dd.mm.gggg): ";
            std::getline(std::cin >> std::ws, datum);

            const std::vector<const Utakmica*> rezultat = utakmicaService.pretraziPoDatumu(datum);
            if (rezultat.empty()) {
                std::cout << "Nema utakmica za trazeni datum.\n";
                return;
            }

            ispisiListuUtakmica(rezultat, std::string("Pretraga po datumu: ") + datum);
            break;
        }
        case 3: {
            const int kolo = ucitajInt("Unesite kolo: ");
            const std::vector<const Utakmica*> rezultat = utakmicaService.pretraziPoKolu(kolo);
            if (rezultat.empty()) {
                std::cout << "Nema utakmica za trazeno kolo.\n";
                return;
            }

            ispisiListuUtakmica(rezultat, std::string("Pretraga po kolu: ") + std::to_string(kolo));
            break;
        }
        default:
            std::cout << "Nepostojeci kriterij pretrage.\n";
            break;
    }
}

void Application::ispisiListuUtakmica(const std::vector<const Utakmica*>& utakmice, const std::string& naslov) const {
    std::cout << "\n--- " << naslov << " ---\n";
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

        std::cout << "\n";
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
