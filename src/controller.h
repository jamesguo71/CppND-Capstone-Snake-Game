#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, bool &on_pause, bool &start_new_game, std::vector<Snake> &snake) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif