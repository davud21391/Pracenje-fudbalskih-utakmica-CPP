#pragma once

#include "services/TimService.h"

class Application {
public:
    Application();
    void run();

private:
    TimRepository timRepository;
    TimService timService;

    void prikaziMeni() const;
    void obradiIzbor(int izbor, bool& running);
    void dodajTim();
    void prikaziTimove() const;
    int ucitajInt(const char* prompt) const;
};
