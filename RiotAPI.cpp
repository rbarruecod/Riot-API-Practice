#include "RiotAPI.hpp" 

#include <iostream>
#include <cpr/cpr.h>
#include <cpr/util.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Implementación del constructor
RiotAPI::RiotAPI(const std::string &api_key) : api_key_(api_key)
{
    // El cuerpo puede estar vacío, la inicialización de la api se hace en la cabecera
    // Esta "inicialiación" es más eficiente que hacerlo dentro del constructor
    // api_key_ = api_key;        Esto es ASIGNACIÓN y menos eficiente
    loadChampionData(); // <- metodo de carga del mapa (id-nombre)
}

// Implementación del método que busca el PUUID y lo devuelve si lo encuentra
// PRE: &gameName -> nombre de Invocador | &tagLine -> tag de invocador (EUW) | &api_region -> "europe"
// POST: devuelve el PUUID asociado al invocador
std::optional<std::string> RiotAPI::getPlayerPUUID(const std::string &gameName, const std::string &tagLine)
{

    std::string encodedGameName = cpr::util::urlEncode(gameName).c_str();
    std::string encodedTagLine = cpr::util::urlEncode(tagLine).c_str();
    //std::string encodedApiRegion = cpr::util::urlEncode(api_region).c_str();

    cpr::Url url = cpr::Url{
        "https://europe.api.riotgames.com/riot/account/v1/accounts/by-riot-id/" + encodedGameName + "/" + encodedTagLine};

    /*  1. ENVIO DE API POR URL - Desaconsejado
        cpr::Parameters params = cpr::Parameters{
            {"api_key", api_key_} // Usamos la clave guardada en la clase
        };

        cpr::Response r = cpr::Get(url, params);
    */
    //  2. ENVIO DE API MEDIANTE HEADER
    cpr::Header headers = cpr::Header{
        {"X-Riot-Token", api_key_} // Usamos el nombre de header que espera Riot
    };

    std::cout << "Pidiendo datos del Riot ID: " << gameName << "#" << tagLine << std::endl;

    // 3. Modificamos la llamada Get para que use 'headers'
    cpr::Response r = cpr::Get(url, headers);

    if (r.status_code == 200)
    {
        try
        {
            json data = json::parse(r.text);
            std::string puuid = data["puuid"];
            return puuid; // Devolvemos el PUUID encontrado
        }
        catch (const json::exception &e)
        {
            std::cerr << "Error al parsear el JSON: " << e.what() << std::endl;
            return std::nullopt; // Devolvemos un opcional vacío si el JSON es inválido
        }
    }
    else
    {
        std::cerr << "Error en la peticion a la API. Codigo: " << r.status_code << std::endl;
        std::cerr << "Respuesta: " << r.text << std::endl;
        return std::nullopt; // Devolvemos un opcional vacío si la petición falla
    }
}

// Implementación del método que devuelve la lista de maestrias por campeon
// PRE: &puuid -> idInvocador | &gameName -> nombre de Invocador | &tagLine -> tag de invocador (EUW) | &api_region -> "europe"
// POST: devuelve la lista de maestrias asociada a cada campeón del invocador
std::vector<ChampMastery> RiotAPI::getChampionMasteries(const std::string &puuid, const std::string &region)
{

    std::vector<ChampMastery> masteries; // Elemento vacio
    std::string encodedApiRegion = cpr::util::urlEncode(region).c_str();

    cpr::Url url = cpr::Url{
        "https://" + encodedApiRegion + ".api.riotgames.com/lol/champion-mastery/v4/champion-masteries/by-puuid/" + puuid};

    //  2. ENVIO DE API MEDIANTE HEADER
    cpr::Header headers = cpr::Header{
        {"X-Riot-Token", api_key_} // Usamos el nombre de header que espera Riot
    };

    cpr::Response r = cpr::Get(url, headers);

    if (r.status_code == 200)
    {
        try
        {
            json data = json::parse(r.text);

            // Iteramos sobre cada objeto en el array JSON 'data'
            for (const auto &item : data)
            {
                // 1. Creamos una instancia vacía de nuestro struct
                ChampMastery mastery;

                // 2. "Desempaquetamos" los datos del JSON (item) a nuestro struct (mastery)
                mastery.championID = item["championId"];
                mastery.championLevel = item["championLevel"];
                mastery.championPoints = item["championPoints"];
                mastery.lastPlayTime = item["lastPlayTime"];
                //para este elemento, el campo no existe, por tanto se asigna un valor por defecto si no encuentra el campo:
                item.value("chestGranted", false); //en lugar de fallar, devuelve false
                mastery.championPointsUntilNextLevel = item["championPointsUntilNextLevel"];
                mastery.championPointsSinceLastLevel = item["championPointsSinceLastLevel"];

                mastery.championName = getChampionNameById(mastery.championID);    //Buscamos en el mapa el nombre del campeon

                // 3. Añadimos el struct ya rellenado a nuestro vector
                masteries.push_back(mastery);
            }
        }
        catch (const json::exception &e)
        {
            std::cerr << "Error al parsear el JSON: " << e.what() << std::endl;
            return {}; // Devolvemos un vector vacío si el JSON es inválido
        }
    }
    else
    {
        std::cerr << "Error en la peticion a la API. Codigo: " << r.status_code << std::endl;
        std::cerr << "Respuesta: " << r.text << std::endl;
        return {}; // Devolvemos un vector vacío si la petición falla
    }

    return masteries; // Devolvemos el vector con las maestrias
}

// Carga en el mapa la información de nombre e id de los campeones 
void RiotAPI::loadChampionData() {
    // Primero, necesitamos saber cuál es la última versión del juego.
    cpr::Url version_url = cpr::Url{"https://ddragon.leagueoflegends.com/api/versions.json"};
    cpr::Response r_version = cpr::Get(version_url);
    if (r_version.status_code != 200) 
        return; // Fallo al cargar

    json version_data = json::parse(r_version.text);
    std::string latest_version = version_data[0]; // La primera es la más reciente

    // Ahora, descargamos el fichero de campeones de la última versión
    cpr::Url champions_url = cpr::Url{"https://ddragon.leagueoflegends.com/cdn/" + latest_version + "/data/en_US/champion.json"};
    cpr::Response r_champions = cpr::Get(champions_url);
    if (r_champions.status_code != 200) return; // Fallo al cargar

    json champions_json = json::parse(r_champions.text);

    //std::cout << "Cargando datos de campeones de la version " << latest_version << "..." << std::endl;

    // Iteramos sobre el JSON para rellenar nuestro mapa
    for (const auto& champion : champions_json["data"].items()) {
        long long champion_key = std::stoll(champion.value()["key"].get<std::string>());
        std::string champion_name = champion.value()["name"];
        champion_data_[champion_key] = champion_name;
    }
    //std::cout << "Datos de " << champion_data_.size() << " campeones cargados." << std::endl;
}
std::string RiotAPI::getChampionNameById(long long championId) {
    auto it = champion_data_.find(championId);
    if (it != champion_data_.end()) {
        return it->second; // Devolvemos el nombre encontrado
    }
    return "Unknown Champion"; // Devolvemos un valor por defecto si no se encuentra
}

// Devuelve los identificadores de las ultimas 20 partidas del invocador (puuid) introducido por parametro
std::vector <std::string> RiotAPI::getSummonerMatchHistory(const std::string& puuid, const std::string &api_region){
    std::vector <std::string> matchHistoryId;

        std::string encodedApiRegion = cpr::util::urlEncode(api_region).c_str();

    cpr::Url url = cpr::Url{
        "https://" + encodedApiRegion + ".api.riotgames.com/lol/match/v5/matches/by-puuid/" + puuid + "/ids" };

    //  2. ENVIO DE API MEDIANTE HEADER
    cpr::Header headers = cpr::Header{
        {"X-Riot-Token", api_key_} // Usamos el nombre de header que espera Riot
    };

    cpr::Response r = cpr::Get(url, headers);
    if (r.status_code == 200)
    {
        try
        {
            json data = json::parse(r.text);

            for (const auto &item : data)
            {
                matchHistoryId.push_back(item);
            }
        }
        catch (const json::exception &e)
        {
            std::cerr << "Error al parsear el JSON: " << e.what() << std::endl;
            return {}; // Devolvemos un vector vacío si el JSON es inválido
        }
    }
    else
    {
        std::cerr << "Error en la peticion a la API. Codigo: " << r.status_code << std::endl;
        std::cerr << "Respuesta: " << r.text << std::endl;
        return {}; // Devolvemos un vector vacío si la petición falla
    }

    return matchHistoryId; // Devolvemos el vector con los Id's de las partidas

}
// MÉTODO SOBRECARGADO CON FILTROS
std::vector<std::string> RiotAPI::getSummonerMatchHistory(const std::string& puuid,const std::string &api_region, const MatchHistoryFilters& filters) {
    std::vector<std::string> matchHistoryIds;

    cpr::Url url = cpr::Url{
        "https://" + api_region + ".api.riotgames.com/lol/match/v5/matches/by-puuid/" + puuid + "/ids"
    };

    cpr::Header headers = cpr::Header{
        {"X-Riot-Token", api_key_}
    };

    // --- 2. Creamos los PARÁMETROS para los filtros ---
    cpr::Parameters params = cpr::Parameters{};     // Empezamos con parámetros vacíos

    // Añadimos los filtros solo si tienen un valor
    if (filters.count) {
        params.Add({"count", std::to_string(*filters.count)});
    }
    if (filters.start) {
        params.Add({"start", std::to_string(*filters.start)});
    }
    if (filters.queue) {
        params.Add({"queue", std::to_string(*filters.queue)});
    }
    if (filters.type) {
        params.Add({"type", *filters.type});
    }

    cpr::Response r = cpr::Get(url, headers, params);

    if (r.status_code == 200) {
        try {
            json data = json::parse(r.text);
            for (const auto& item : data) {
                matchHistoryIds.push_back(item);
            }
        } catch (const json::exception& e) {
            std::cerr << "Error al parsear el JSON de historial: " << e.what() << std::endl;
            return {};
        }
    } else {
        std::cerr << "Error en la peticion de historial. Codigo: " << r.status_code << std::endl;
        std::cerr << "Respuesta: " << r.text << std::endl;
        return {};
    }

    return matchHistoryIds;
}


// --- METODOS DE PARTIDA ---
std::optional<MatchDetails> RiotAPI::getMatchDetails(const std::string& matchId) {
    // Match API uses regional servers (europe, americas, asia)
    std::string api_region = "europe"; 

    cpr::Url url = cpr::Url{
        "https://" + api_region + ".api.riotgames.com/lol/match/v5/matches/" + matchId
    };

    cpr::Header headers = cpr::Header{
        {"X-Riot-Token", api_key_}
    };

    cpr::Response r = cpr::Get(url, headers);

    if (r.status_code == 200) {
        try {
            json data = json::parse(r.text);
            MatchDetails details;
            details.matchId = matchId;

            // Navigate through the complex JSON structure
            if (data.contains("info") && data["info"].contains("participants")) {
                for (const auto& participantJson : data["info"]["participants"]) {
                    Participant p;
                    p.puuid = participantJson.value("puuid", "");
                    p.championName = participantJson.value("championName", "Unknown");
                    
                    // Extract stats
                    p.stats.totalDamageDealtToChampions = participantJson.value("totalDamageDealtToChampions", 0LL);
                    // Add extraction for other stats here if needed

                    details.participants.push_back(p);
                }
            } else {
                 std::cerr << "Error: JSON response for match " << matchId << " missing 'info' or 'participants'." << std::endl;
                return std::nullopt;
            }
            return details; // Return the populated struct

        } catch (const json::exception& e) {
            std::cerr << "Error parsing JSON for match " << matchId << ": " << e.what() << std::endl;
            return std::nullopt;
        }
    } else {
        std::cerr << "Error fetching details for match " << matchId << ". Code: " << r.status_code << std::endl;
        std::cerr << "Response: " << r.text << std::endl;
        return std::nullopt;
    }
}

std::optional<MatchSummary> RiotAPI::getMatchSummary(const std::string& matchId, const std::string& playerPuuid) {
    std::string api_region = "europe"; // Match API uses regional servers

    cpr::Url url = cpr::Url{
        "https://" + api_region + ".api.riotgames.com/lol/match/v5/matches/" + matchId
    };
    cpr::Header headers = cpr::Header{{"X-Riot-Token", api_key_}};
    cpr::Response r = cpr::Get(url, headers);

    if (r.status_code == 200) {
        try {
            json data = json::parse(r.text);
            MatchSummary summary;           
            summary.matchId = matchId;

            if (data.contains("info")) {
                const auto& info = data["info"];
                summary.gameDuration = info.value("gameDuration", 0LL);
                summary.gameEndTimestamp = info.value("gameEndTimestamp", 0LL);
                summary.mapId = info.value ("mapId", -1);
                
                if (info.contains("gameModeMutators")) {
                    // .get<T>() convierte el array de JSON a un vector de C++
                    summary.gameModeMutators = info["gameModeMutators"].get<std::vector<std::string>>();
                }

                // Find the player's champion name
                if (info.contains("participants")) {
                    for (const auto& participantJson : info["participants"]) {
                        if (participantJson.value("puuid", "") == playerPuuid) {
                            summary.playerChampionName = participantJson.value("championName", "Unknown");
                            summary.playerSurrendered = participantJson.value("gameEndedInSurrender", false);
                            summary.playerWin = participantJson.value("win", false);

                            summary.pings.allInPings = participantJson.value("allInPings", -1);
                            summary.pings.assistMePings = participantJson.value("assistMePings", -1);
                            summary.pings.commandPings = participantJson.value("commandPings", -1);
                            summary.pings.enemyMissingPings = participantJson.value("enemyMissingPings", -1);
                            summary.pings.enemyVisionPings = participantJson.value("enemyVisionPings", -1);
                            summary.pings.getBackPings = participantJson.value("getBackPings", -1);
                            summary.pings.holdPings = participantJson.value("holdPings", -1);
                            summary.pings.needVisionPings = participantJson.value("needVisionPings", -1);
                            summary.pings.onMyWayPings = participantJson.value("onMyWayPings", -1);
                            summary.pings.pushPings = participantJson.value("pushPings", -1);
                            summary.pings.visionClearedPings = participantJson.value("visionClearedPings", -1);

                            break; // Found the players
                        }
                    }
                }
                return summary; // Return the populated summary
            } else {
                 std::cerr << "Error: JSON response for match " << matchId << " missing 'info'." << std::endl;
                 return std::nullopt;
            }
        } catch (const json::exception& e) {
            std::cerr << "Error parsing JSON summary for match " << matchId << ": " << e.what() << std::endl;
            return std::nullopt;
        }
    } else {
        std::cerr << "Error fetching summary for match " << matchId << ". Code: " << r.status_code << std::endl;
        return std::nullopt; // Return empty optional on API error
    }
}







//TODO: VOLCAR EL JSON POR COMPLETO DE PARTIDA (BORRAR)
std::optional<std::string> RiotAPI::getRawMatchJson(const std::string& matchId) {
    std::string api_region = "europe"; // Match API uses regional servers

    cpr::Url url = cpr::Url{
        "https://" + api_region + ".api.riotgames.com/lol/match/v5/matches/" + matchId
    };
    cpr::Header headers = cpr::Header{{"X-Riot-Token", api_key_}};
    cpr::Response r = cpr::Get(url, headers);

    if (r.status_code == 200) {
        return r.text; // <-- Devuelve el JSON crudo como un string
    } else {
        std::cerr << "Error fetching raw JSON for match " << matchId << ". Code: " << r.status_code << std::endl;
        return std::nullopt; // Devuelve vacío si falla
    }
}
