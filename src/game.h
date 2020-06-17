#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include <string>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, bool two_player_mode);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  std::string GetScore() const;
  std::string GetSize() const;

 private:
  std::vector<Snake> snakes;
  std::vector<SDL_Point> foods;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  std::size_t grid_width;
  std::size_t grid_height;
  bool two_player_mode;

  void PlaceFood();
  void PlaceFood(std::size_t count);
  void Update();
  SDL_Point FindSpot();
  void NewGame();
};

#endif