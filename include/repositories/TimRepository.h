#pragma once

#include "models/Tim.h"

#include <map>
#include <string>
#include <vector>

class TimRepository {
public:
    TimRepository() = default;
    ~TimRepository();

    TimRepository(const TimRepository&) = delete;
    TimRepository& operator=(const TimRepository&) = delete;

    void add(Tim* tim);
    Tim* findById(int idTima) const;
    Tim* findByName(const std::string& naziv) const;
    const std::vector<Tim*>& getAll() const;
    bool remove(int idTima);
    void clear();

private:
    std::vector<Tim*> timovi;
    std::map<int, Tim*> timoviPoId;
};
