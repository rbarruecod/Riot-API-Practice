#pragma once
#include <string>

struct MatchSummary {
    std::string matchId;
    int mapId;                                  //Para diferenciar los tipos de mapas (Abismo, puente...)
    long long gameDuration = 0;                 // In seconds
    long long gameEndTimestamp = 0;             // In milliseconds
    std::string playerChampionName = "Unknown";
    bool playerSurrendered = false;
    bool playerWin = false;

    // Guardará la lista de mutadores del mapa, ej para el cruce de Koeshin: ["mapskin_map12_bloom"]
    std::vector<std::string> gameModeMutators;
};