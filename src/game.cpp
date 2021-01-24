#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      snake2(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)-1),
      random_h(0, static_cast<int>(grid_height)-1) {
  
}

void Game::Init()
{
  for (int i = 0; i < numOfFood; ++i) {
     foods.emplace_back();
  }
  for (int i = 0; i <= foods.size(); ++i) {
      PlaceFood(i);
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

  bool startup = true;
  bool twoPlayers = false;
  while (startup) {
    controller.HandleMenuInput(startup, running, twoPlayers);
    renderer.RenderMenu(twoPlayers);
    SDL_Delay(target_frame_duration);
  }

  if (twoPlayers) {
    numOfFood = 2;
    snake.SetStartPosition(Snake::StartPosition::right);
    snake2.SetStartPosition(Snake::StartPosition::left);
  } else {
    numOfFood = 1;
  }

  Init();
  
  
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake, snake2, twoPlayers);
    Update();
    renderer.Render(snake, snake2, twoPlayers, foods);

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

void Game::PlaceFood(int foodIndex) {

  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    // Also check the food location is different from others
    std::cout << x << "," << y << std::endl;
    if (!snake.SnakeCell(x, y) && !snake2.SnakeCell(x,y) && InFoodPosition(x,y)== -1) {
      foods[foodIndex].x = x;
      foods[foodIndex].y = y;
      break;
    }
  }

}

void Game::Update() {
  if (!snake.alive || !snake2.alive) return;

  snake.Update();
  snake2.Update();

  SnakeBumpCheck(snake, snake2);

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  int new_x2 = static_cast<int>(snake2.head_x);
  int new_y2 = static_cast<int>(snake2.head_y);

  // Check if there's food over here
  auto foodIndexFound = InFoodPosition(new_x, new_y);
  if (foodIndexFound != -1)
  {
    score++;
    PlaceFood(foodIndexFound);
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

    // Check if there's food over here
  auto foodIndexFound2 = InFoodPosition(new_x2, new_y2);
  if (foodIndexFound2 != -1)
  {
    score++;
    PlaceFood(foodIndexFound2);
    // Grow snake and increase speed.
    snake2.GrowBody();
    snake2.speed += 0.02;
  }

  
}

int Game::InFoodPosition(int x, int y) {
  for (int i = 0; i < foods.size(); ++i) {
    auto currentFood = foods[i];
    if (currentFood.x == -1 || currentFood.y == -1) {
      continue;
    }
    if (currentFood.x == x && currentFood.y == y) {
      return i;
    }
  }
  return -1;
}

void Game::SnakeBumpCheck(Snake &snake1, Snake &snake2) {
  // Check if the snake has died.
  SDL_Point snake1_head = { static_cast<int>(snake1.head_x),  static_cast<int>(snake1.head_y)};
  SDL_Point snake2_head = { static_cast<int>(snake2.head_x),  static_cast<int>(snake2.head_y)};

  if (snake1_head.x == snake2_head.x && snake1_head.y == snake2_head.y) {
      snake1.alive = false;
      snake2.alive = false;
  }

  for (auto const &item : snake1.body) {
    if (snake2_head.x == item.x && snake2_head.y == item.y) {
      std::cout << "dead" << std::endl;
      snake1.alive = false;
      snake2.alive = false;
    }
  }
  for (auto const &item : snake2.body) {
    if (snake1_head.x == item.x && snake1_head.y == item.y) {
      SDL_Point current_head = { static_cast<int>(snake2.head_x),  static_cast<int>(snake2.head_y)};
      std::cout << "dead" << std::endl;
      snake1.alive = false;
      snake2.alive = false;
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size + snake2.size; }