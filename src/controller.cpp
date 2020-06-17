#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, bool &on_pause, bool &start_new_game, std::vector<Snake> &snakes) const {
  SDL_Event e;
  Snake &snake = snakes.at(0);
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;

        case SDLK_SPACE:
          on_pause = !on_pause;
          break;

        case SDLK_n:
          start_new_game = true;
          break;
      }
      if (snakes.size() == 2) {
        Snake &snake2 = snakes.at(1);
        switch (e.key.keysym.sym) {
          case SDLK_w:
            ChangeDirection(snake2, Snake::Direction::kUp,
                            Snake::Direction::kDown);
            break;

          case SDLK_s:
            ChangeDirection(snake2, Snake::Direction::kDown,
                            Snake::Direction::kUp);
            break;

          case SDLK_a:
            ChangeDirection(snake2, Snake::Direction::kLeft,
                            Snake::Direction::kRight);
            break;

          case SDLK_d:
            ChangeDirection(snake2, Snake::Direction::kRight,
                            Snake::Direction::kLeft);
            break;
        }

      }
    }
  }
}