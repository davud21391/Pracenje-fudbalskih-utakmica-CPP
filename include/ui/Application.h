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
    void prikaziTimove() const;
    void prikaziIgraceTima() const;
    void prikaziUtakmice() const;
    int ucitajInt(const char* prompt) const;
};
