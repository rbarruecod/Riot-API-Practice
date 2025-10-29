#pragma once

#include <string>
#include <vector>

// Structure for individual participant stats we care about
struct ParticipantStats {
    long long totalDamageDealtToChampions = 0;
    // Add other stats later if needed (kills, deaths, assists, cs, etc.)
};

// Structure for a participant in the match
struct Participant {
    std::string puuid;
    std::string championName;
    ParticipantStats stats;
};

// Structure for the overall match details
struct MatchDetails {
    std::string matchId;
    std::vector<Participant> participants;
    // Add other general match info later if needed (gameDuration, win/loss, etc.)
};