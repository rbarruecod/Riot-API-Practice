# Versión mínima y definición del proyecto
cmake_minimum_required(VERSION 3.10)
project(transport_analyzer LANGUAGES CXX)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Buscar las librerías necesarias
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

# Crear el ejecutable a partir de los ficheros fuente
add_executable(transport_analyzer
    main.cpp
    RiotAPI.cpp
    StatsAnalyzer.cpp
)

# Enlazar las librerías con nuestro ejecutable
# El target Crow::Crow debería gestionar sus includes automáticamente
target_link_libraries(transport_analyzer PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
    Crow::Crow
    asio::asio
)