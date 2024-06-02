#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "score.h"
#include <memory>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  std::shared_ptr<Snake> GetSnake() const;
  void UpdateStartSpeed();
  Score score;
 private:

  std::shared_ptr<Snake> snake;
  SDL_Point food;
  SDL_Point bad_food;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  void PlaceFood();
  void PlaceBadFood();
  void BadFoodTimer();
  void Update();
};

#endif