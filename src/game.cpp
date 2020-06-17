#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, bool two_player_mode) :
    engine(dev()),
    random_w(0, static_cast<int>(grid_width) - 1),
    random_h(0, static_cast<int>(grid_height) - 1),
    grid_width(grid_width),
    grid_height(grid_height),
    two_player_mode(two_player_mode) {
  NewGame();
}

void Game::NewGame() {
  snakes.clear();
  foods.clear();
  snakes.emplace_back(Snake(grid_width, grid_height, Snake::Direction::kUp));
  PlaceFood();
  if (two_player_mode) {
    snakes.emplace_back(Snake(grid_width, grid_height, Snake::Direction::kDown));
    // Surprise eating party for two snakes if run with a little bit of luck
    if (random_w(engine) > 0.8 * grid_width)
      PlaceFood(9);
    else
      PlaceFood();
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  bool on_pause = false;
  bool start_new_game = false;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, on_pause, start_new_game, snakes);
    if (!on_pause)
      Update();
    if (start_new_game) {
      NewGame();
      start_new_game = false;
    }
    renderer.Render(snakes, foods);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(snakes, frame_count);
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

SDL_Point Game::FindSpot() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    bool DUPLICATE = false;
    for (Snake &snake : snakes) {
      if (snake.SnakeCell(x, y)) {
        DUPLICATE = true;
        break;
      }
    }
    // Make sure each food is unique
    if (!DUPLICATE) {
      for (auto &food : foods) {
        if (food.x == x && food.y == y) {
          DUPLICATE = true;
          break;
        }
      }
    }

    if (!DUPLICATE) {
      return SDL_Point{x, y};
    }
  }
}

void Game::PlaceFood() {
  foods.emplace_back(FindSpot());
}

void Game::PlaceFood(std::size_t count) {
  for (std::size_t i = 0; i < count; ++i)
    PlaceFood();
}

void Game::Update() {
  // No update if any snake died
  for (Snake &snake : snakes)
    if (!snake.alive) return;

  // Loop over the snake vector to check if a snake has eaten any food
  for (Snake &snake : snakes) {
    snake.Update();
    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);

    // Make a copy of foods for safe iteration in case foods gets modified inside the loop
    auto foods_copy = foods;
    // Check if there's food over here
    for (int offset = 0; offset < foods_copy.size(); ++offset) {
      auto food = foods_copy.at(offset);
      if (food.x == new_x && food.y == new_y) {
        snake.score++;
        foods.erase(foods.begin() + offset);
        PlaceFood();
        // Grow snake and increase speed.
        snake.GrowBody();
        snake.speed += 0.02;
      }
    }
  }

  // Check if a snake caught the other (the one touch the other with its head loses the round
  if (snakes.size() == 2) {
    for (std::size_t i = 0; i < 2; ++i) {
      Snake &snake = snakes.at(i);
      Snake &other = snakes.at(i == 0? 1: 0);
      if (snake.CaughtSnake(other)) {
        snake.score += other.size;
        snake.Devour(other);
        other.Rebirth(std::move(FindSpot()));
        break;
      }
    }
  }
}

std::string Game::GetScore() const {
  std::string player_1{" Player 1: " + std::to_string(snakes.at(0).score)};
  if (snakes.size() == 2) {
    std::string player_2{"; Player 2: " + std::to_string(snakes.at(1).score)};
    return player_1 + player_2;
  }
  return player_1;
}
std::string Game::GetSize() const {
  std::string player_1{" Player 1: " + std::to_string(snakes.at(0).size)};
  if (snakes.size() == 2) {
    std::string player_2{"; Player 2: " + std::to_string(snakes.at(1).size)};
    return player_1 + player_2;
  }
  return player_1;
}