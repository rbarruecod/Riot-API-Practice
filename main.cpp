
#include <iostream>
#include <string>
#include "config.hpp"  // Para nuestra clave de API
#include "RiotAPI.hpp" // Para nuestra nueva clase

struct summonerInfo
{
    std::optional<std::string> gameName;
    std::optional<std::string> tagLine;
    std::optional<std::string> region;
};

int main() {
    // 1. Creamos una instancia de nuestra clase, pasándole la clave de API
    RiotAPI api(RIOT_API_KEY);

    summonerInfo summoner;
    summoner.gameName = "Plank Sparröw";
    summoner.tagLine = "EUW";
    summoner.region = "europe";

    // 2. Llamamos al método para obtener el PUUID
    std::optional<std::string> puuid_optional = api.getPlayerPUUID(*summoner.gameName, *summoner.tagLine, *summoner.region);

    if (puuid_optional) {
        // Si el opcional contiene un valor, accedemos a él con *
        std::string puuid = *puuid_optional;
        std::cout << "¡PUUID encontrado con exito!: " << puuid << std::endl;

        // --- MENÚ INTERACTIVO PARA FILTROS ---
        std::cout << "--- Opciones de Busqueda de Partidas ---" << std::endl;
        std::cout << "(Deja en blanco y pulsa Enter para usar el valor por defecto)" << std::endl;

        MatchHistoryFilters filters; // Creamos el objeto para guardar los filtros
        std::string input;

        // Pedir el número de partidas
        std::cout << "Numero de partidas a buscar (1-100, defecto: 20): ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            filters.count = std::stoi(input);
        }

        // Pedir el tipo de cola (Ranked SoloQ es 420)
        std::cout << "ID de la cola (ej: 420 para Ranked Solo/Duo, defecto: todas): ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            filters.queue = std::stoi(input);
        }

        std::cout << "\nBuscando historial de partidas con los filtros seleccionados..." << std::endl;
        
        // Llamamos al nuevo método con los filtros
        std::vector<std::string> match_ids = api.getSummonerMatchHistory(puuid, "europe", filters);

        if (!match_ids.empty()) {
            std::cout << "IDs de las ultimas " << match_ids.size() << " partidas recibidos." << std::endl;
            std::cout << "ID de la partida mas reciente: " << match_ids[0] << std::endl;
        } else {
            std::cerr << "No se pudo obtener el historial de partidas con esos filtros." << std::endl;
        }

    } else {
        std::cerr << "No se pudo obtener el PUUID del jugador." << std::endl;
    }



    return 0;
}