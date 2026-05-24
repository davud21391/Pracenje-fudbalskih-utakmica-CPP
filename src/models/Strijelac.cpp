#include "models/Strijelac.h"

Strijelac::Strijelac(int idStrijelca, int idUtakmice, int idIgraca, int minuta, bool autoGol)
    : idStrijelca(idStrijelca),
      idUtakmice(idUtakmice),
      idIgraca(idIgraca),
      minuta(minuta),
      autoGol(autoGol) {}

int Strijelac::getIdStrijelca() const {
    return idStrijelca;
}

int Strijelac::getIdUtakmice() const {
    return idUtakmice;
}

int Strijelac::getIdIgraca() const {
    return idIgraca;
}

int Strijelac::getMinuta() const {
    return minuta;
}

bool Strijelac::isAutoGol() const {
    return autoGol;
}
