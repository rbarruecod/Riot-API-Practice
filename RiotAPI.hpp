#pragma once

#include <string>
#include <optional> // Para devolver valores que pueden no existir

#include <vector>
#include "ChampMastery.hpp"

#include <map> // <-- Incluir para std::map

class RiotAPI {
public:
    // Constructor: Se llama al crear un objeto de la clase.
    // Usamos explicit para evitar conversiones implícitas.
    explicit RiotAPI(const std::string& api_key);

    // Método para obtener el PUUID.
    // Devuelve un std::optional<std::string> que contendrá el PUUID si tiene éxito,
    // o estará vacío si falla.
    std::optional<std::string> getPlayerPUUID(const std::string& gameName, const std::string& tagLine, const std::string &api_region);

    // Nuevo método para obtener las maestrías 
    std::vector<ChampMastery> getChampionMasteries(const std::string& puuid, const std::string &region);

    // Devuelve el nombre del campeón a partir de su ID
    std::string getChampionNameById(long long championId);

    // Devuelve los identificadores de las ultimas 20 partidas del invocador (puuid) introducido por parametro
    std::vector <std::string> getSummonerMatchHistory(const std::string& puuid, const std::string &api_region);

private:
    // Miembros privados de la clase
    std::string api_key_; // Guardamos la clave de API aquí

    ChampMastery mastery_data_; //Info de maestria por campeon

    // Un mapa para relacionar ID de campeón -> Nombre de campeón
    std::map<long long, std::string> champion_data_; 
    // Un método privado para cargar los datos del Data Dragon que se incluirá en el constructor
    void loadChampionData();

};