#pragma once

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

    void prikaziMeni() const;
    void obradiIzbor(int izbor, bool& running);
    void dodajTim();
    void dodajIgracaUTim();
    void dodajUtakmicu();
    void evidentirajStrijelca();
    void prikaziTimove() const;
    void prikaziIgraceTima() const;
    void prikaziTabelu() const;
    void prikaziUtakmiceTima() const;
    void prikaziUtakmice() const;
    void prikaziListuStrijelaca() const;
    bool ucitajDaNe(const char* prompt) const;
    int ucitajInt(const char* prompt) const;
};
