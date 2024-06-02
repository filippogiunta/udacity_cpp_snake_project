#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <chrono>
#include <iomanip>
#include <sstream>

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  std::string user_name;
  do {
      std::cout << "User name: ";
      std::cin >> user_name;
  } while (user_name.empty());


  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);


  game.UpdateStartSpeed();
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";


  // Get current time
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);

  // Format time as string
  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_c), "%Y%m%d_%H%M%S");

  // Use formatted time string in filename
  std::string filename = ss.str() + "_score.txt";


  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  game.score.SaveToFile(filename, user_name);

  return 0;
}