#pragma once
#include <string>
#include <vector>

struct PlayerPings {
    int allInPings = 0;
    int assistMePings = 0;
    int commandPings = 0;
    int enemyMissingPings = 0;
    int enemyVisionPings = 0;
    int holdPings = 0;
    int getBackPings = 0;
    int needVisionPings = 0;
    int onMyWayPings = 0;
    int pushPings = 0;
    int visionClearedPings = 0;
};

struct MatchSummary {
    std::string matchId;
    int mapId;                                  //Para diferenciar los tipos de mapas (Abismo, puente...)
    long long gameDuration = 0;                 // In seconds
    long long gameEndTimestamp = 0;             // In milliseconds
    std::string playerChampionName = "Unknown";
    bool playerSurrendered = false;
    bool playerWin = false;
    std::vector<std::string> gameModeMutators; // Guardará la lista de mutadores del mapa, ej para el cruce de Koeshin: ["mapskin_map12_bloom"]
    PlayerPings pings;
};