#pragma once

#include "MatchSummary.hpp"
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

// To store pentakills
struct PentaKillEvent {
    std::string championName;
    std::string matchId;
};

// --- Multi-Kills resume  ---
struct MaxKillsMatch {
    std::string matchId;
    std::string championName;
    int kills = 0; // El número de kills en esa partida
};

class StatsAnalyzer {
public:
    // El constructor sigue igual
    explicit StatsAnalyzer();

    // Acumula todas las estadísticas (pings y kills) de una partida
    void analyzeMatch(const MatchSummary& match);
    
    // Imprime el informe completo de todas las estadísticas
    void printFinalReport() const;

    // --- API DEVELOPMENT ---
    // Genera un objeto JSON con todas las estadísticas calculadas
    nlohmann::json generateReportJson() const;

private:
    PlayerPings pingTotals_; 

    // --- kill stats ---
    std::vector<PentaKillEvent> pentakills_;
    int totalQuadraKills_ = 0;
    int totalTripleKills_ = 0;
    int totalDoubleKills_ = 0;
    int largestKillingSpree_ = 0;
    MaxKillsMatch maxKillsMatch_;
    MaxKillsMatch maxDeathsMatch_; 
    std::vector<std::string> zeroDeathMatches_;

    // --- Variables de Partida ---
    MatchSummary longestGame_;
    MatchSummary shortestGame_;
    int surrenderCount_ = 0;
    int winCount_ = 0;
    int lossCount_ = 0;

    // --- Variables de Mapa ---
    int map_howling_abyss_count_ = 0;
    int map_koshin_crossing_count_ = 0;
    int map_butchers_bridge_count_ = 0;
    int map_other_count_ = 0;
};