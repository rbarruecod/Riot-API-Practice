#include <iostream>
#include <string>
#include "config.hpp"  // Para nuestra clave de API
#include "RiotAPI.hpp" // Para nuestra nueva clase

int main() {
    // 1. Creamos una instancia de nuestra clase, pasándole la clave de API
    RiotAPI api(RIOT_API_KEY);

    // 2. Llamamos al método para obtener el PUUID
    std::optional<std::string> puuid_optional = api.getPlayerPUUID("Plank Sparröw", "EUW", "europe");

    // 3. Comprobamos si la operación tuvo éxito - ELIMINAR
    if (puuid_optional) {
        // Si el opcional contiene un valor, accedemos a él con *
        std::string puuid = *puuid_optional;
        std::cout << "¡PUUID encontrado con exito!: " << puuid << std::endl;

        std::cout << "Obteniendo el historial de partidas del incovador: " << puuid << std::endl;
        std::vector <std::string> matchIdHistory = api.getSummonerMatchHistory(puuid, "europe");

        for (auto id: matchIdHistory){
            std::cout << "Registro recuperado: " << id << std::endl;
        }
    } else {
        std::cerr << "No se pudo obtener el PUUID del jugador." << std::endl;
    }

    /*
    std::vector<ChampMastery> maestria = api.getChampionMasteries(*puuid_optional, "euw1");
    for (ChampMastery aux: maestria){
        if (aux.championPoints > 100000){
            std::cout << "Tienes mas de 100k con:" << aux.championName << " Concretamente tienes: " << aux.championPoints << " puntos" << std::endl; 
        }    
    }
    */


    return 0;
}