#pragma once

#include <string>
#include <nlohmann/json.hpp>


class LevelLoader {
public:
    LevelLoader();
    ~LevelLoader();

    bool LoadLevel(std::string path);
    void FreeModelAndTextures();

private:

};