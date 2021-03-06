#ifndef GAME_H
#define GAME_H

#include <vector>
#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Init();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;


 private:
  Snake snake;
  Snake snake2;
  std::vector<SDL_Point> foods;

  //Snake snake2;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int numOfFood{0};
  void PlaceFood(int foodIndex);
  void Update();
  int InFoodPosition(int x, int y);
  void SnakeBumpCheck(Snake &snake1, Snake &snake2);
};

#endif