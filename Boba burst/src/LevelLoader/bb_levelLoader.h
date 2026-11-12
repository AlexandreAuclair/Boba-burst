#pragma once

#include <string>
#include <nlohmann/json.hpp>


class LevelLoader {
public:
    LevelLoader();
    ~LevelLoader();

    bool LoadLevel(int n, std::string path);

private:

};