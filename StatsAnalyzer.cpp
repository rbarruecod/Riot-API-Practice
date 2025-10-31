#include "StatsAnalyzer.hpp"
#include <iostream> //
#include <iomanip>  // std::setprecision

StatsAnalyzer::StatsAnalyzer()
    : pingTotals_(),
      totalQuadraKills_(0),
      totalTripleKills_(0),
      totalDoubleKills_(0),
      largestKillingSpree_(0),
      maxKillsMatch_(),
      maxDeathsMatch_(),
      zeroDeathMatches_(),
      surrenderCount_(0),
      winCount_(0),
      lossCount_(0),
      map_howling_abyss_count_(0),
      map_koshin_crossing_count_(0),
      map_butchers_bridge_count_(0),
      map_other_count_(0)
{
    // Inicializamos las duraciones para la comparativa
    longestGame_.gameDuration = 0;
    shortestGame_.gameDuration = std::numeric_limits<long long>::max();
}

// Implementación del método que ACUMULA
void StatsAnalyzer::analyzeMatch(const MatchSummary &match)
{
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
    if (match.pentaKills > 0)
    {
        for (int i = 0; i < match.pentaKills; ++i)
        {
            pentakills_.push_back({match.playerChampionName, match.matchId});
        }
    }

    // --- 3. Comprobar Récords (Máximos) ---

    // Comprobar si es la partida con más kills
    if (match.kills > maxKillsMatch_.kills)
    {
        maxKillsMatch_.kills = match.kills;
        maxKillsMatch_.championName = match.playerChampionName;
        maxKillsMatch_.matchId = match.matchId;
    }

    // Comprobar si es la mayor racha de kills
    if (match.largestKillingSpree > largestKillingSpree_)
    {
        largestKillingSpree_ = match.largestKillingSpree;
    }

    // Comprobar si es la partida con más muertes
    if (match.deaths > maxDeathsMatch_.kills)
    { // Usamos el campo 'kills' de la struct
        maxDeathsMatch_.kills = match.deaths;
        maxDeathsMatch_.championName = match.playerChampionName;
        maxDeathsMatch_.matchId = match.matchId;
    }

    // Comprobar si fue una partida sin muertes
    if (match.deaths == 0)
    {
        zeroDeathMatches_.push_back(match.matchId);
    }

    // TODO: Comprobacion de partida más larga, mas corta y surrenders
    //  Longest game
    if (match.gameDuration > longestGame_.gameDuration)
    {
        longestGame_ = match;
    }

    // Shortest game
    if (match.gameDuration < shortestGame_.gameDuration && !match.playerSurrendered)
    {
        shortestGame_ = match;
    }

    if (match.playerSurrendered && !match.playerWin)
    {
        surrenderCount_++;
    }
    if (match.playerWin)
    {
        winCount_++;
    }
    else
    {
        lossCount_++;
    }

    // --- Logica de Conteo de Mapas --- (Koeshin y Carnicero como Mutators de Howling Abyss)
    if (match.mapId == 12)
    {
        bool isVariant = false;
        for (const std::string &mutator : match.gameModeMutators)
        {
            if (mutator == "mapskin_map12_bloom")
            {
                map_koshin_crossing_count_++;
                isVariant = true;
                break;
            }
            else if (mutator == "mapskin_ha_bilgewater")
            {
                map_butchers_bridge_count_++;
                isVariant = true;
                break;
            }
        }
        if (!isVariant)
        {
            // Si no tiene mutador conocido, es el Abismo estándar
            map_howling_abyss_count_++;
        }
    }
}

// Implementación del método que IMPRIME
void StatsAnalyzer::printFinalReport() const
{


    // --- Imprimir Estadísticas de Kills ---
    std::cout << "\nEstadísticas de Kills (Totales):" << std::endl;
    std::cout << "  Pentakills: " << pentakills_.size() << std::endl;
    for (const auto &penta : pentakills_)
    {
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
    for (const std::string &matchId : zeroDeathMatches_)
    {
        std::cout << "    > Partida: " << matchId << std::endl;
    }

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
}

nlohmann::json StatsAnalyzer::generateReportJson() const
{
    nlohmann::json report; // El objeto JSON principal que devolveremos

    // --- 1. Añadir Estadísticas de Partidas Largas/Cortas ---
    report["longestGame"]["matchId"] = longestGame_.matchId;
    report["longestGame"]["durationSec"] = longestGame_.gameDuration;
    report["longestGame"]["champion"] = longestGame_.playerChampionName;

    report["shortestGame"]["matchId"] = shortestGame_.matchId;
    report["shortestGame"]["durationSec"] = shortestGame_.gameDuration;
    report["shortestGame"]["champion"] = shortestGame_.playerChampionName;

    // --- 2. Añadir Estadísticas de Win/Loss ---
    report["generalStats"]["wins"] = winCount_;
    report["generalStats"]["losses"] = lossCount_;
    if (winCount_ + lossCount_ > 0)
    {
        report["generalStats"]["winrate"] = (static_cast<double>(winCount_) / (winCount_ + lossCount_)) * 100.0;
    }
    else
    {
        report["generalStats"]["winrate"] = 0;
    }
    report["generalStats"]["surrenders"] = surrenderCount_;

    // --- 3. Añadir Distribución de Mapas ---
    report["mapDistribution"]["howlingAbyss"] = map_howling_abyss_count_;
    report["mapDistribution"]["koshinCrossing"] = map_koshin_crossing_count_;
    report["mapDistribution"]["butchersBridge"] = map_butchers_bridge_count_;

    // --- 4. Añadir Pings ---
    nlohmann::json pingsJson; // Usamos un sub-objeto para los pings
    pingsJson["allIn"] = pingTotals_.allInPings;
    pingsJson["assistMe"] = pingTotals_.assistMePings;
    pingsJson["command"] = pingTotals_.commandPings;
    pingsJson["enemyMissing"] = pingTotals_.enemyMissingPings;
    pingsJson["enemyVision"] = pingTotals_.enemyVisionPings;
    pingsJson["getBack"] = pingTotals_.getBackPings;
    pingsJson["hold"] = pingTotals_.holdPings;
    pingsJson["needVision"] = pingTotals_.needVisionPings;
    pingsJson["onMyWay"] = pingTotals_.onMyWayPings;
    pingsJson["push"] = pingTotals_.pushPings;
    pingsJson["visionCleared"] = pingTotals_.visionClearedPings;
    report["pings"] = pingsJson;

    // --- 5. Añadir Multi-Kills y Récords ---
    report["killStats"]["totalDoubleKills"] = totalDoubleKills_;
    report["killStats"]["totalTripleKills"] = totalTripleKills_;
    report["killStats"]["totalQuadraKills"] = totalQuadraKills_;
    report["killStats"]["totalPentaKills"] = pentakills_.size();

    // Creamos un array JSON para los pentakills
    nlohmann::json pentaEvents = nlohmann::json::array();
    for (const auto &penta : pentakills_)
    {
        pentaEvents.push_back({{"champion", penta.championName},
                               {"matchId", penta.matchId}});
    }
    report["killStats"]["pentaKillEvents"] = pentaEvents;

    report["killStats"]["largestKillingSpree"] = largestKillingSpree_;
    report["killStats"]["maxKillsMatch"] = {
        {"champion", maxKillsMatch_.championName},
        {"kills", maxKillsMatch_.kills},
        {"matchId", maxKillsMatch_.matchId}};
    report["killStats"]["maxDeathsMatch"] = {
        {"champion", maxDeathsMatch_.championName},
        {"deaths", maxDeathsMatch_.kills}, // Recuerda que aquí reutilizamos el campo 'kills'
        {"matchId", maxDeathsMatch_.matchId}};

    return report;
}
