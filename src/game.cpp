#include "game.h"
#include <iostream>
#include "SDL.h"
#include <score.h>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

SDL_Point bad_food;
bool is_bad_food_active = false;
std::thread badFoodThread;
std::mutex mutex;
std::condition_variable conditon_var;


Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(new Snake(grid_width, grid_height)),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
  PlaceBadFood();
}


void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *snake.get());
    Update();
    renderer.Render(*snake.get(), food, bad_food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

std::shared_ptr<Snake> Game::GetSnake() const { return snake; }

void Game::UpdateStartSpeed()
{
  snake->SetStartingSpeed();
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlaceBadFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // bad food.
    if (!snake->SnakeCell(x, y)) {
      bad_food.x = x;
      bad_food.y = y;
      return;
    }
  }
}


void Game::Update() {
  if (!snake->alive) return;

  snake->Update();

  int new_x = static_cast<int>(snake->head_x);
  int new_y = static_cast<int>(snake->head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score.increaseScore(1);
    PlaceFood();
    // Grow snake and increase speed.
    snake->GrowBody();
    snake->speed += 0.02;
  }

  // Check if there's bad food over here
  if (bad_food.x == new_x && bad_food.y == new_y) {
    if (!is_bad_food_active) { // Check if bad food is already active
      is_bad_food_active = true;
      badFoodThread = std::thread(&Game::BadFoodTimer, this);
      badFoodThread.detach();
    }
    // Shrink snake and reposition the bad food after it's eaten
    // Notice that the snake will shorten as bad food is eaten and the bad food will reappear in a new location
    // If the snake eats bad food when only the head is left, the snake dies
    snake->ShrinkBody();
    PlaceBadFood();
  }
}

void Game::BadFoodTimer() {
  // Define the duration for which the bad food is active
  const int badSeconds = 5;

  // Get the current time
  auto startTime = std::chrono::high_resolution_clock::now();

  // Lock the mutex
  std::unique_lock<std::mutex> lock(mutex);

  // While the bad food is active
  while (is_bad_food_active) {
    // Unlock the mutex
    lock.unlock();

    // Get the current time
    auto current_Time = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time in seconds
    auto elapsed_Seconds = std::chrono::duration_cast<std::chrono::seconds>(current_Time - startTime).count();

    // If the elapsed time is greater than or equal to the bad food duration
    if (elapsed_Seconds >= badSeconds) {
      // Set the bad food as inactive
      is_bad_food_active = false;

      // Move the bad food off the grid
      bad_food.x = 1;
      bad_food.y = 1;

      // Break the loop
      break;
    }

    // Lock the mutex
    lock.lock();

    // Wait for 800 milliseconds or until the condition_variable is notified
    conditon_var.wait_for(lock, std::chrono::milliseconds(800));
  }
}

int Game::GetScore() const { return score.getScore(); }
int Game::GetSize() const { return snake->size; }
