# Versión mínima y definición del proyecto
cmake_minimum_required(VERSION 3.10)
project(transport_analyzer LANGUAGES CXX)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Buscar y enlazar la librería cpr (forma correcta y explícita)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED) 

# Crear el ejecutable
add_executable(transport_analyzer main.cpp RiotAPI.cpp)

# Enlazar cpr con nuestro ejecutable (en minúsculas)
target_link_libraries(transport_analyzer PRIVATE cpr::cpr)