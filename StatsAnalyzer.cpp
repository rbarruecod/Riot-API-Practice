#include "StatsAnalyzer.hpp"
#include <iostream> // Necesario para std::cout

// El constructor inicializa 'totals' (ya lo hace por defecto, pero ser explícito es bueno)
StatsAnalyzer::StatsAnalyzer() : totals() {}

// Implementación del método que ACUMULA
void StatsAnalyzer::accumulatePings(const MatchSummary& match) {
    // Añade los pings de esta partida a la variable 'totals' de la clase
    totals.allInPings += match.pings.allInPings;
    totals.assistMePings += match.pings.assistMePings;
    totals.commandPings += match.pings.commandPings;
    totals.enemyMissingPings += match.pings.enemyMissingPings;
    totals.enemyVisionPings += match.pings.enemyVisionPings;
    totals.holdPings += match.pings.holdPings;
    totals.getBackPings += match.pings.getBackPings;
    totals.needVisionPings += match.pings.needVisionPings;
    totals.onMyWayPings += match.pings.onMyWayPings;
    totals.pushPings += match.pings.pushPings;
    totals.visionClearedPings += match.pings.visionClearedPings;
}

// Implementación del método que IMPRIME
void StatsAnalyzer::printTotalPingStatistics() const {
    // Imprime la variable 'totals' de la clase
    std::cout << "\nEstadísticas de Alertas (Pings Totales):" << std::endl;
    std::cout << "  'All-In' (Ataque total): " << totals.allInPings << std::endl;
    std::cout << "  'Asistidme': " << totals.assistMePings << std::endl;
    std::cout << "  'Comando' (Rueda de pings): " << totals.commandPings << std::endl;
    std::cout << "  'Enemigo desaparecido': " << totals.enemyMissingPings << std::endl;
    std::cout << "  'Enemigo tiene visión': " << totals.enemyVisionPings << std::endl;
    std::cout << "  'Mantener posición': " << totals.holdPings << std::endl;
    std::cout << "  'Retirada': " << totals.getBackPings << std::endl;
    std::cout << "  'Necesita visión': " << totals.needVisionPings << std::endl;
    std::cout << "  'Voy de camino': " << totals.onMyWayPings << std::endl;
    std::cout << "  'Empujar línea': " << totals.pushPings << std::endl;
    std::cout << "  'Visión eliminada': " << totals.visionClearedPings << std::endl;
}