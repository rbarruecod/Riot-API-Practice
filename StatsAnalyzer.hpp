#pragma once

#include "MatchSummary.hpp"
#include <vector>

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

    // --- MÉTODO ACTUALIZADO ---
    // Acumula todas las estadísticas (pings y kills) de una partida
    void analyzeMatch(const MatchSummary& match);
    
    // --- MÉTODO ACTUALIZADO ---
    // Imprime el informe completo de todas las estadísticas
    void printFinalReport() const;

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
};