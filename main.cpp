#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <limits> // Para obtener el valor mínimo de long long
#include <ctime>  // For converting timestamp to date

#include "config.hpp"
#include "RiotAPI.hpp"
#include "ChampMastery.hpp" // Aunque no la usemos aquí, la dejamos por si acaso
#include "MatchData.hpp"
#include "MatchSummary.hpp"


// Helper function to convert Unix timestamp (milliseconds) to readable date
std::string formatTimestamp(long long msTimestamp)
{
    if (msTimestamp <= 0)
        return "N/A";
    auto seconds = msTimestamp / 1000;
    std::time_t time = seconds;
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    return std::string(buffer);
}


int main()
{
    // 1. Crear instancia de la API
    RiotAPI api(RIOT_API_KEY);

    // 2. Obtener PUUID
    std::optional<std::string> puuid_optional = api.getPlayerPUUID("CösmicSeaBass", "4224");

    if (!puuid_optional)
    {
        std::cerr << "Error: No se pudo obtener el PUUID. Saliendo." << std::endl;
        return 1;
    }
    std::string puuid = *puuid_optional;

    // 3. Obtener IDs de partidas ARAM
    MatchHistoryFilters aram_filters;
    // TODO: Ajustar estos parametros para que recupere todos los games de la season

    aram_filters.count = 50;
    aram_filters.queue = 450; // ID de ARAM
    std::vector<std::string> match_ids = api.getSummonerMatchHistory(puuid, "europe", aram_filters);

    if (match_ids.empty())
    {
        std::cerr << "No se encontraron partidas de ARAM recientes." << std::endl;
        return 1;
    }
    std::cout << "Se encontraron " << match_ids.size() << " IDs de partidas." << std::endl;

    // Variables to track longest and shortest games
    MatchSummary longestGame;
    MatchSummary shortestGame;
    longestGame.gameDuration = 0;                                      // Start with 0 duration
    shortestGame.gameDuration = std::numeric_limits<long long>::max(); // Start with max duration

    // Contadores de estadísticas de la partida
    int surrenderCount = 0;
    int winCount = 0;
    int lossCount = 0;
    int processedGames = 0;

    // Contadores de mapa
    int map_howling_abyss_count = 0;
    int map_koshin_crossing_count = 0;
    int map_butchers_bridge_count = 0;
    int map_other_count = 0;

    //Para albergar los pings
    PlayerPings pings;
    StatsAnalyzer pingAnalyzer;

    for (const std::string &matchId : match_ids)
    { // TODO: significado de &
        std::optional<MatchSummary> summary_opt = api.getMatchSummary(matchId, puuid);

        if (summary_opt)
        {
            processedGames++;
            MatchSummary currentSummary = *summary_opt;
            pingAnalyzer.accumulatePings(currentSummary);

            // Longest game
            if (currentSummary.gameDuration > longestGame.gameDuration)
            {
                longestGame = currentSummary;
            }

            // Shortest game
            if (currentSummary.gameDuration < shortestGame.gameDuration && !currentSummary.playerSurrendered)
            {
                shortestGame = currentSummary;
            }

            if (currentSummary.playerSurrendered && !currentSummary.playerWin)
            {
                surrenderCount++;
            }
            if (currentSummary.playerWin)
            {
                winCount++;
            }
            else
            {
                lossCount++;
            }

            // --- Logica de Conteo de Mapas --- (Koeshin y Carnicero como Mutators de Howling Abyss)
            if (currentSummary.mapId == 12)
            {
                bool isVariant = false;
                for (const std::string &mutator : currentSummary.gameModeMutators)
                {
                    if (mutator == "mapskin_map12_bloom")
                    {
                        map_koshin_crossing_count++;
                        isVariant = true;
                        break;
                    }
                    else if (mutator == "mapskin_ha_bilgewater")
                    {
                        map_butchers_bridge_count++;
                        isVariant = true;
                        break;
                    }
                }
                if (!isVariant)
                {
                    // Si no tiene mutador conocido, es el Abismo estándar
                    map_howling_abyss_count++;
                }
            }
        }
        // TODO: Optional: Add delay for rate limiting if needed
    }


    // Display results
    std::cout << "\n--- Resultado del Análisis ---" << std::endl;
    std::cout << "Partidas jugadas: " << match_ids.size() << std::endl;
    std::cout << "W/R: " << ((winCount * 100.000) / match_ids.size()) << "% --- VICTORIAS: " << winCount << " | DERROTAS: " << lossCount << std::endl;
    if (longestGame.gameDuration > 0)
    {
        std::cout << "Partida más larga:" << std::endl;
        std::cout << "  Duración: " << longestGame.gameDuration / 60 << "m " << longestGame.gameDuration % 60 << "s" << std::endl;
        std::cout << "  Fecha: " << formatTimestamp(longestGame.gameEndTimestamp) << std::endl;
        std::cout << "  Campeón: " << longestGame.playerChampionName << std::endl;
        std::cout << "  Resultado: " << (longestGame.playerWin ? " [Victoria]" : " [Derrota]") << std::endl;
    }
    else
    {
        std::cout << "No se encontraron datos de partidas largas." << std::endl;
    }

    if (shortestGame.gameDuration != std::numeric_limits<long long>::max())
    {
        std::cout << "\nPartida más corta:" << std::endl;
        std::cout << "  Duración: " << shortestGame.gameDuration / 60 << "m " << shortestGame.gameDuration % 60 << "s" << std::endl;
        std::cout << "  Fecha: " << formatTimestamp(shortestGame.gameEndTimestamp) << std::endl;
        std::cout << "  Campeón: " << shortestGame.playerChampionName << std::endl;
        std::cout << "  Resultado: " << (shortestGame.playerWin ? " [Victoria]" : " [Derrota]") << std::endl;
    }
    else
    {
        std::cout << "\nNo se encontraron datos de partidas cortas válidas." << std::endl;
    }
    if (surrenderCount != 0)
    {
        std::cout << "\nPartidas perdidas por rendicion: " << surrenderCount << std::endl;
        std::cout << "Porcentaje de rendicion: " << (surrenderCount * 100.00) / match_ids.size() << " %" << std::endl;
    }
    else
    {
        std::cout << "\nEres inquebrantable, de tus " << lossCount << " derrotas, NINGUNA ha sido por FF!!" << std::endl;
    }

    // --- MOSTRAR RESULTADOS DE MAPAS ---
    std::cout << "\nDistribución de Mapas:" << std::endl;
    std::cout << "  Abismo de los Lamentos: " << map_howling_abyss_count << " (" << ((map_howling_abyss_count *100.00) / match_ids.size()) << "%)"<< std::endl;

    std::cout << "  Cruce de Koeshin: " << map_koshin_crossing_count << " (" << ((map_koshin_crossing_count *100.00) / match_ids.size()) << "%)"<< std::endl;

    std::cout << "  Puente del Carnicero: " << map_butchers_bridge_count << " (" << ((map_butchers_bridge_count *100.00) / match_ids.size()) << "%)"<< std::endl;
    if (map_other_count > 0)
    {
        std::cout << "  Otros mapas: " << map_other_count << std::endl;
    }

    if (processedGames > 0) {
        pingAnalyzer.printTotalPingStatistics();
    }
    return 0;
}
