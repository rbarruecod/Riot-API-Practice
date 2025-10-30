#pragma once

#include "MatchSummary.hpp"
#include <vector>

class StatsAnalyzer {
public:
    // El constructor sigue igual
    explicit StatsAnalyzer();

    // Acumula los pings de una partida en la variable 'totals'
    void accumulatePings(const MatchSummary& match);
    
    // Imprime el total acumulado. 'const' significa que este método no modifica la clase.
    void printTotalPingStatistics() const;

private:
    PlayerPings totals; // El acumulador
};