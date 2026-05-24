#pragma once

#include "models/Utakmica.h"

#include <map>
#include <vector>

class UtakmicaRepository {
public:
    UtakmicaRepository() = default;
    ~UtakmicaRepository();

    UtakmicaRepository(const UtakmicaRepository&) = delete;
    UtakmicaRepository& operator=(const UtakmicaRepository&) = delete;

    void add(Utakmica* utakmica);
    Utakmica* findById(int idUtakmice) const;
    const std::vector<Utakmica*>& getAll() const;
    bool remove(int idUtakmice);
    void clear();

private:
    std::vector<Utakmica*> utakmice;
    std::map<int, Utakmica*> utakmicePoId;
};
