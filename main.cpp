#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <limits> // Para obtener el valor mínimo de long long
#include <ctime>  // For converting timestamp to date
#include <crow.h>                  
#include <crow/middlewares/cors.h> 
#include <cpr/util.h> // Para urlDecode

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
    // 1. Crear instancia de la API (carga el Data Dragon)
    RiotAPI api(RIOT_API_KEY);

    // 2. Crear la aplicación del servidor Crow
    crow::App<crow::CORSHandler> app;

    // 3. Configurar CORS para permitir peticiones de WordPress
    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global().methods("GET"_method);
    // Para más seguridad, podrías restringir el origen a tu dominio de WordPress:
    // .origin("https://tu-sitio-wordpress.com"); 

    std::cout << "Servidor C++ iniciando..." << std::endl;

    // --- RUTA PRINCIPAL DE LA API ---
    // Escuchará en: /api/recap/aram/{gameName}/{tagLine}
    CROW_ROUTE(app, "/api/recap/aram/<string>/<string>")
    ([&api](const std::string& encodedGameName, const std::string& encodedTagLine) {
        
        // Decodificar los parámetros de la URL
        std::string gameName = cpr::util::urlDecode(encodedGameName).c_str();
        std::string tagLine = cpr::util::urlDecode(encodedTagLine).c_str();
        
        std::cout << "Peticion de RECAP recibida para: " << gameName << "#" << tagLine << std::endl;

        // --- 1. Obtener PUUID ---
        std::optional<std::string> puuid_opt = api.getPlayerPUUID(gameName, tagLine);
        if (!puuid_opt) {
            std::cerr << "  Error: Jugador no encontrado." << std::endl;
            return crow::response(404, nlohmann::json{{"error", "Jugador no encontrado"}}.dump());
        }
        std::string puuid = *puuid_opt;
        std::cout << "  > PUUID encontrado: " << puuid << std::endl;

        // --- 2. Obtener IDs de partidas ARAM ---
        MatchHistoryFilters filters;
        filters.count = 20; // Analizamos las últimas 20 partidas (puedes ajustar esto)
        filters.queue = 450; // ARAM
        std::vector<std::string> match_ids = api.getSummonerMatchHistory(puuid, "europe",filters);

        if (match_ids.empty()) {
            std::cerr << "  Error: No se encontraron partidas de ARAM." << std::endl;
            return crow::response(404, nlohmann::json{{"error", "No se encontraron partidas de ARAM"}}.dump());
        }
        std::cout << "  > Encontrados " << match_ids.size() << " IDs de partidas." << std::endl;

        // --- 3. Analizar cada partida ---
        StatsAnalyzer analyzer;
        int processedGames = 0;
        std::cout << "  > Iniciando análisis de partidas..." << std::endl;
        
        for (const std::string& matchId : match_ids) {
            std::optional<MatchSummary> summary = api.getMatchSummary(matchId, puuid);
            if (summary) {
                analyzer.analyzeMatch(*summary); // Acumular datos
                processedGames++;
            }
            // NOTA: Si procesas muchas partidas, aquí deberías añadir un delay
            // para no exceder el Rate Limit de la API de Riot.
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        std::cout << "  > Análisis completado. " << processedGames << " partidas procesadas." << std::endl;

        // --- 4. Generar el informe JSON y devolverlo ---
        nlohmann::json report = analyzer.generateReportJson();
        report["totalGamesProcessed"] = processedGames;
        report["player"]["gameName"] = gameName;
        report["player"]["tagLine"] = tagLine;

        return crow::response(200, report.dump());
    });

    // Iniciar el servidor en el puerto 18080
    std::cout << "Servidor C++ de Stats iniciado. Escuchando en http://localhost:18080" << std::endl;
    app.port(18080).multithreaded().run();
    
    return 0;

    /*
    std::optional<std::string> puuid_optional = api.getPlayerPUUID("Plank Sparröw", "EUW");
    if (!puuid_optional) {  return 1; }
    std::string puuid = *puuid_optional;

    MatchHistoryFilters filters;
    filters.count = 40; 
    filters.queue = 450;
    std::vector<std::string> match_ids = api.getSummonerMatchHistory(puuid, "europe",filters);

    if (match_ids.empty()) {  return 1; }

    // --- LÓGICA DE ANÁLISIS LIMPIA ---
    StatsAnalyzer analyzer; // 1. Creamos el analizador
    int processedGames = 0;

    for (const std::string& matchId : match_ids) {
        std::optional<MatchSummary> summary = api.getMatchSummary(matchId, puuid);
        if (summary) {
            analyzer.analyzeMatch(*summary); // 2. Acumulamos datos
            processedGames++;
        }
    }

    // --- INFORME FINAL ---
    std::cout << "\n--- Resultado del Análisis de " << processedGames << " partidas ---" << std::endl;
    if (processedGames > 0) {
        analyzer.printFinalReport(); // 3. Imprimimos el informe

        // --- 2. VERIFICACIÓN DEL JSON  ---
        // Obtenemos el objeto JSON
        nlohmann::json reportJson = analyzer.generateReportJson();
        
        
        //  Método .dump() convierte el objeto JSON de C++ en un std::string. 
        //  Al pasarle el número 4, formatea el string con una indentación de 4 espacios.
        
        std::cout << "\n--- JSON CRUDO PARA EL FRONTEND ---" << std::endl;
        std::cout << reportJson.dump(4) << std::endl;
    }

    return 0;*/
}
