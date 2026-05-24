#pragma once

class Strijelac {
public:
    Strijelac(int idStrijelca, int idUtakmice, int idIgraca, int minuta, bool autoGol);

    int getIdStrijelca() const;
    int getIdUtakmice() const;
    int getIdIgraca() const;
    int getMinuta() const;
    bool isAutoGol() const;

private:
    int idStrijelca;
    int idUtakmice;
    int idIgraca;
    int minuta;
    bool autoGol;
};
