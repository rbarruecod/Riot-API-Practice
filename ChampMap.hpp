#include <iostream>
#include <string>
#include <map>

    // --- 1. Implementación del mapa ---
    // Declaramos un mapa donde la clave es un std::string (nombre del campeón)
    // y el valor es un int (el código del campeón).
    // Usamos la inicialización de lista (con {}) para poblarlo directamente.
    std::map<std::string, int> champion_codes = {
    {"Aatrox", 266},
    {"Ahri", 103},
    {"Akali", 84},
    {"Akshan", 166},
    {"Alistar", 12},
    {"Ambessa", 799},
    {"Amumu", 32},
    {"Anivia", 34},
    {"Annie", 1},
    {"Aphelios", 523},
    {"Ashe", 22},
    {"Aurelion Sol", 136},
    {"Aurora", 893},
    {"Azir", 268},
    {"Bard", 432},
    {"Bel'Veth", 200},
    {"Blitzcrank", 53},
    {"Brand", 63},
    {"Braum", 201},
    {"Briar", 233},
    {"Caitlyn", 51},
    {"Camille", 164},
    {"Cassiopeia", 69},
    {"Cho'Gath", 31},
    {"Corki", 42},
    {"Darius", 122},
    {"Diana", 131},
    {"Dr. Mundo", 36},
    {"Draven", 119},
    {"Ekko", 245},
    {"Elise", 60},
    {"Evelynn", 28},
    {"Ezreal", 81},
    {"Fiddlesticks", 9},
    {"Fiora", 114},
    {"Fizz", 105},
    {"Galio", 3},
    {"Gangplank", 41},
    {"Garen", 86},
    {"Gnar", 150},
    {"Gragas", 79},
    {"Graves", 104},
    {"Gwen", 887},
    {"Hecarim", 120},
    {"Heimerdinger", 74},
    {"Hwei", 910},
    {"Illaoi", 420},
    {"Irelia", 39},
    {"Ivern", 427},
    {"Janna", 40},
    {"Jarvan IV", 59},
    {"Jax", 24},
    {"Jayce", 126},
    {"Jhin", 202},
    {"Jinx", 222},
    {"K'Sante", 897},
    {"Kai'Sa", 145},
    {"Kalista", 429},
    {"Karma", 43},
    {"Karthus", 30},
    {"Kassadin", 38},
    {"Katarina", 55},
    {"Kayle", 10},
    {"Kayn", 141},
    {"Kennen", 85},
    {"Kha'Zix", 121},
    {"Kindred", 203},
    {"Kled", 240},
    {"Kog'Maw", 96},
    {"LeBlanc", 7},
    {"Lee Sin", 64},
    {"Leona", 89},
    {"Lillia", 876},
    {"Lissandra", 127},
    {"Lucian", 236},
    {"Lulu", 117},
    {"Lux", 99},
    {"Malphite", 54},
    {"Malzahar", 90},
    {"Maokai", 57},
    {"Master Yi", 11},
    {"Mel", 800},
    {"Milio", 902},
    {"Miss Fortune", 21},
    {"Mordekaiser", 82},
    {"Morgana", 25},
    {"Naafiri", 950},
    {"Nami", 267},
    {"Nasus", 75},
    {"Nautilus", 111},
    {"Neeko", 518},
    {"Nidalee", 76},
    {"Nilah", 895},
    {"Nocturne", 56},
    {"Nunu & Willump", 20},
    {"Olaf", 2},
    {"Orianna", 61},
    {"Ornn", 516},
    {"Pantheon", 80},
    {"Poppy", 78},
    {"Pyke", 555},
    {"Qiyana", 246},
    {"Quinn", 133},
    {"Rakan", 497},
    {"Rammus", 33},
    {"Rek'Sai", 421},
    {"Rell", 526},
    {"Renata Glasc", 888},
    {"Renekton", 58},
    {"Rengar", 107},
    {"Riven", 92},
    {"Rumble", 68},
    {"Ryze", 13},
    {"Samira", 360},
    {"Sejuani", 113},
    {"Senna", 235},
    {"Seraphine", 147},
    {"Sett", 875},
    {"Shaco", 35},
    {"Shen", 98},
    {"Shyvana", 102},
    {"Singed", 27},
    {"Sion", 14},
    {"Sivir", 15},
    {"Skarner", 72},
    {"Smolder", 901},
    {"Sona", 37},
    {"Soraka", 16},
    {"Swain", 50},
    {"Sylas", 517},
    {"Syndra", 134},
    {"Tahm Kench", 223},
    {"Taliyah", 163},
    {"Talon", 91},
    {"Taric", 44},
    {"Teemo", 17},
    {"Thresh", 412},
    {"Tristana", 18},
    {"Trundle", 48},
    {"Tryndamere", 23},
    {"Twisted Fate", 4},
    {"Twitch", 29},
    {"Udyr", 77},
    {"Urgot", 6},
    {"Varus", 110},
    {"Vayne", 67},
    {"Veigar", 45},
    {"Vel'Koz", 161},
    {"Vex", 711},
    {"Vi", 254},
    {"Viego", 234},
    {"Viktor", 112},
    {"Vladimir", 8},
    {"Volibear", 106},
    {"Warwick", 19},
    {"Wukong", 62},
    {"Xayah", 498},
    {"Xerath", 101},
    {"Xin Zhao", 5},
    {"Yasuo", 157},
    {"Yone", 777},
    {"Yorick", 83},
    {"Yunara", 804},
    {"Yuumi", 350},
    {"Zac", 154},
    {"Zed", 238},
    {"Zeri", 221},
    {"Ziggs", 115},
    {"Zilean", 26},
    {"Zoe", 142},
    {"Zyra", 143}
};

//     // --- 2. Cómo usar el mapa ---

//     // A) Acceder a un valor directamente por su clave
//     std::cout << "El codigo de Zyra es: " << champion_codes["Zyra"] << std::endl;
//     std::cout << "El codigo de Akali es: " << champion_codes["Akali"] << std::endl;
    
//     std::cout << "\n--- Buscando campeones ---\n";

//     // B) Búsqueda segura de una clave que SÍ existe
//     std::string champion_to_find = "Alistar";
//     auto it = champion_codes.find(champion_to_find);

//     if (it != champion_codes.end()) {
//         // 'it' es un iterador que apunta al par clave-valor.
//         // it->first es la clave ("Alistar")
//         // it->second es el valor (12)
//         std::cout << "Encontrado: " << it->first << " -> ID: " << it->second << std::endl;
//     }

//     // C) Búsqueda segura de una clave que NO existe
//     champion_to_find = "Teemo"; // Teemo no está en nuestra lista abreviada
//     if (champion_codes.find(champion_to_find) == champion_codes.end()) {
//         std::cout << "El campeon '" << champion_to_find << "' no se encuentra en el mapa." << std::endl;
//     }

//     std::cout << "\n--- Iterando sobre el mapa ---\n";

//     // D) Iterar sobre todos los elementos del mapa
//     // Usamos un bucle for-each (range-based for). 'pair' será cada elemento del mapa.
//     for (const auto& pair : champion_codes) {
//         std::cout << "Campeon: " << pair.first << ", Codigo: " << pair.second << std::endl;
//     }

//     return 0;
// }