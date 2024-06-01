#include "score.h"
#include <fstream>

Score::Score() : score(0) {}

// Copy constructor
Score::Score(const Score& other) : score(other.score) {}

// Copy assignment operator
Score& Score::operator=(const Score& other) {
    if (this != &other) {
        score = other.score;
    }
    return *this;
}

// Move constructor
Score::Score(Score&& other) noexcept : score(other.score) {
    other.score = 0;
}

// Move assignment operator
Score& Score::operator=(Score&& other) noexcept {
    if (this != &other) {
        score = other.score;
        other.score = 0;
    }
    return *this;
}

// Destructor
Score::~Score() {}

int Score::getScore() const { 
    return score; 
}

void Score::increaseScore(int increment) { 
    score += increment; 
}

void Score::SaveToFile(const std::string& filename, const std::string& user_name) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << "Username: " << user_name << ", Score: " << score << "\n";
        file.close();
    }
}