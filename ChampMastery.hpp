#pragma once
#include <string>

struct ChampMastery
{
    std::string championName;               //Campo derivado mediante mapa (id/nombre)                    
    long long championID;
    int championPoints;                     //Los campos long del JSON se mapean a long long en C++  
    int championLevel;                      //para asegurar que tenemos suficiente espacio,
    long long lastPlayTime;                 //especialmente para timestamps.               
    long long championPointsSinceLastLevel;
    long long championPointsUntilNextLevel;
    bool chest;  
    
};

// class ChampMastery{

// };