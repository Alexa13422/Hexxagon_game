

#ifndef HEX_SCOREBOARD_H
#define HEX_SCOREBOARD_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

struct ScoreboardEntry {
    int scoreRuby{};
    int scorePearl{};
};
/**
 * @class ScoreBoard
 * @brief Manages the scoreboard and updates it with new scores.
 */
class ScoreBoard {
public:
    /**
 * @brief Updates the scoreboard with the given scores.
 * @param filename The name of the file to read the scoreboard from and write the updated scoreboard to.
 * @param rubyScore The score for the Ruby game.
 * @param pearlScore The score for the Pearl game.
 *
 * This function reads the existing scoreboard from the file with the specified filename,
 * adds the given scores to the scoreboard, sorts it in descending order based on the Ruby scores,
 * and keeps only the top 5 entries. The updated scoreboard is then written back to the file,
 * and the current top 5 entries are displayed on the console.
 */
    static void updateScoreboard(const std::string& filename, int rubyScore, int pearlScore) {
        std::vector<ScoreboardEntry> scoreboard;

        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                ScoreboardEntry entry;
                std::istringstream iss(line);
                if (iss >> entry.scoreRuby >> entry.scorePearl) {
                    scoreboard.push_back(entry);
                }
            }
            file.close();
        }

        scoreboard.push_back({rubyScore, pearlScore});

        std::sort(scoreboard.begin(), scoreboard.end(), [](const ScoreboardEntry& a, const ScoreboardEntry& b) {
            return a.scoreRuby > b.scoreRuby;
        });

        if (scoreboard.size() > 5) {
            scoreboard.resize(5);
        }

        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const auto& entry : scoreboard) {
                outFile << entry.scoreRuby<<" "<<entry.scorePearl << std::endl;
            }
            outFile.close();

            std::cout << "Scoreboard:\n";
            for (const auto& entry : scoreboard) {
                std::cout <<"Ruby: "<< entry.scoreRuby<< " " <<"Pearl: "<< entry.scorePearl << std::endl;
            }
        }
        else {
            std::cout << "Unable to open file for writing." << std::endl;
        }
    }
};


#endif //HEX_SCOREBOARD_H
