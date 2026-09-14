#pragma once

#include "services/PersistenceService.h"
#include "services/UtakmicaService.h"
#include "services/TimService.h"

class Application {
public:
    Application();
    void run();

private:
    TimRepository timRepository;
    UtakmicaRepository utakmicaRepository;
    TimService timService;
    UtakmicaService utakmicaService;
    PersistenceService persistenceService;

    void prikaziMeni() const;
    void obradiIzbor(int izbor, bool& running);
    void meniTimovi();
    void meniStatistika() const;
    void meniPodaci();
    void dodajTim();
    void dodajIgracaUTim();
    void meniUtakmice();
    void dodajUtakmicu();
    void urediUtakmicu();
    void sortirajUtakmice() const;
    void obrisiUtakmicu();
    void evidentirajStrijelca();
    void spremiPodatke() const;
    void ucitajPodatke();
    void prikaziTimove() const;
    void prikaziIgraceTima() const;
    void prikaziTabelu() const;
    void prikaziUtakmiceTima() const;
    void pretraziUtakmice() const;
    void prikaziUtakmice() const;
    void prikaziListuStrijelaca() const;
    void ispisiListuUtakmica(const std::vector<const Utakmica*>& utakmice, const std::string& naslov) const;
    bool ucitajDaNe(const char* prompt) const;
    int ucitajInt(const char* prompt) const;
};
