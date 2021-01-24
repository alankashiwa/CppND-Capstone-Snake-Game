#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleMenuInput(bool &startup,bool &running, bool &twoPlayers) const {
  SDL_Event e;
    while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      startup = false;
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_DOWN:
          twoPlayers = !twoPlayers;
          break;
        case SDLK_RETURN:
          // Decide Mode and out
          startup = false;
          break;
      }
    }
  }
}
void Controller::HandleInput(bool &running, Snake &snake, Snake &snake2, bool twoPlayers) const {
  SDL_Event e;
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
        case SDLK_w:
          if (twoPlayers) {
            ChangeDirection(snake2, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          }
          break;
        case SDLK_s:
          if (twoPlayers) {
            ChangeDirection(snake2, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          }
          break;
        case SDLK_a:
          if (twoPlayers) {
            ChangeDirection(snake2, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          }
          break;
        case SDLK_d:
          if (twoPlayers) {
            ChangeDirection(snake2, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          }
          break;
      }
    }
  }
}