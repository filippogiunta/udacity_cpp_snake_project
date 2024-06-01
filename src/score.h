#ifndef SCORE_H
#define SCORE_H

#include <string>

class Score {
public:
    Score();
    Score(const Score& other); // Copy constructor
    Score& operator=(const Score& other); // Copy assignment operator
    Score(Score&& other) noexcept; // Move constructor
    Score& operator=(Score&& other) noexcept; // Move assignment operator
    ~Score(); // Destructor

    int getScore() const;
    void increaseScore(int increment);
    void SaveToFile(const std::string& filename, const std::string& user_name);

private:
    int score;
};

#endif // SCORE_H