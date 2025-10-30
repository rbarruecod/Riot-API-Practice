#include "StatsAnalyzer.hpp"
#include <iostream> // 
#include <iomanip> // std::setprecision

StatsAnalyzer::StatsAnalyzer() 
    : pingTotals_(), 
      totalQuadraKills_(0),
      totalTripleKills_(0),
      totalDoubleKills_(0),
      largestKillingSpree_(0),
      maxKillsMatch_(),
      maxDeathsMatch_(),
      zeroDeathMatches_()
{}

// Implementación del método que ACUMULA
void StatsAnalyzer::analyzeMatch(const MatchSummary& match) {
    // Añade los pings de esta partida a la variable 'totals' de la clase
    pingTotals_.allInPings += match.pings.allInPings;
    pingTotals_.assistMePings += match.pings.assistMePings;
    pingTotals_.commandPings += match.pings.commandPings;
    pingTotals_.enemyMissingPings += match.pings.enemyMissingPings;
    pingTotals_.enemyVisionPings += match.pings.enemyVisionPings;
    pingTotals_.holdPings += match.pings.holdPings;
    pingTotals_.getBackPings += match.pings.getBackPings;
    pingTotals_.needVisionPings += match.pings.needVisionPings;
    pingTotals_.onMyWayPings += match.pings.onMyWayPings;
    pingTotals_.pushPings += match.pings.pushPings;
    pingTotals_.visionClearedPings += match.pings.visionClearedPings;

    // --- 2. Acumular Multi-Kills ---
    totalDoubleKills_ += match.doubleKills;
    totalTripleKills_ += match.tripleKills;
    totalQuadraKills_ += match.quadraKills;
    
    // Si hubo pentakills, guardamos los detalles
    if (match.pentaKills > 0) {
        for (int i = 0; i < match.pentaKills; ++i) {
            pentakills_.push_back({match.playerChampionName, match.matchId});
        }
    }

    // --- 3. Comprobar Récords (Máximos) ---
    
    // Comprobar si es la partida con más kills
    if (match.kills > maxKillsMatch_.kills) {
        maxKillsMatch_.kills = match.kills;
        maxKillsMatch_.championName = match.playerChampionName;
        maxKillsMatch_.matchId = match.matchId;
    }

    // Comprobar si es la mayor racha de kills
    if (match.largestKillingSpree > largestKillingSpree_) {
        largestKillingSpree_ = match.largestKillingSpree;
    }

    // Comprobar si es la partida con más muertes
    if (match.deaths > maxDeathsMatch_.kills) { // Usamos el campo 'kills' de la struct
        maxDeathsMatch_.kills = match.deaths;
        maxDeathsMatch_.championName = match.playerChampionName;
        maxDeathsMatch_.matchId = match.matchId;
    }

    // Comprobar si fue una partida sin muertes
    if (match.deaths == 0) {
        zeroDeathMatches_.push_back(match.matchId);
    }
}

// Implementación del método que IMPRIME
void StatsAnalyzer::printFinalReport() const {
    // Imprime la variable 'totals' de la clase
    std::cout << "\nEstadísticas de Alertas (Pings Totales):" << std::endl;
    std::cout << "  'All-In' (Ataque total): " << pingTotals_.allInPings << std::endl;
    std::cout << "  'Asistidme': " << pingTotals_.assistMePings << std::endl;
    std::cout << "  'Comando' (Rueda de pings): " << pingTotals_.commandPings << std::endl;
    std::cout << "  'Enemigo desaparecido': " << pingTotals_.enemyMissingPings << std::endl;
    std::cout << "  'Enemigo tiene visión': " << pingTotals_.enemyVisionPings << std::endl;
    std::cout << "  'Mantener posición': " << pingTotals_.holdPings << std::endl;
    std::cout << "  'Retirada': " << pingTotals_.getBackPings << std::endl;
    std::cout << "  'Necesita visión': " << pingTotals_.needVisionPings << std::endl;
    std::cout << "  'Voy de camino': " << pingTotals_.onMyWayPings << std::endl;
    std::cout << "  'Empujar línea': " << pingTotals_.pushPings << std::endl;
    std::cout << "  'Visión eliminada': " << pingTotals_.visionClearedPings << std::endl;

    // --- Imprimir Estadísticas de Kills ---
    std::cout << "\nEstadísticas de Kills (Totales):" << std::endl;
    std::cout << "  Pentakills: " << pentakills_.size() << std::endl;
    for (const auto& penta : pentakills_) {
        std::cout << "    > Con " << penta.championName << " (Partida: " << penta.matchId << ")" << std::endl;
    }
    std::cout << "  Quadrakills: " << totalQuadraKills_ << std::endl;
    std::cout << "  Triplekills: " << totalTripleKills_ << std::endl;
    std::cout << "  Doublekills: " << totalDoubleKills_ << std::endl;

    std::cout << "\nRécords de la Sesión:" << std::endl;
    std::cout << "  Mayor racha de kills: " << largestKillingSpree_ << std::endl;
    std::cout << "  Partida con más Kills:" << std::endl;
    std::cout << "    > Campeón: " << maxKillsMatch_.championName << std::endl;
    std::cout << "    > Kills: " << maxKillsMatch_.kills << std::endl;
    std::cout << "    > Partida: " << maxKillsMatch_.matchId << std::endl;

    // --- NUEVO: Imprimir Estadísticas de Muertes ---
    std::cout << "\nEstadísticas de Supervivencia:" << std::endl;
    std::cout << "  Partida con más Muertes:" << std::endl;
    std::cout << "    > Campeón: " << maxDeathsMatch_.championName << std::endl;
    std::cout << "    > Muertes: " << maxDeathsMatch_.kills << std::endl;
    std::cout << "    > Partida: " << maxDeathsMatch_.matchId << std::endl;

    std::cout << "\n  Partidas Perfectas (0 Muertes): " << zeroDeathMatches_.size() << std::endl;
    for (const std::string& matchId : zeroDeathMatches_) {
        std::cout << "    > Partida: " << matchId << std::endl;
    }

}

