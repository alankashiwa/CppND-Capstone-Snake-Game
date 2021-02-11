#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "SDL_ttf.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void RenderMenu(bool twoPlayers);
  void Render(Snake const snake, Snake const snake2, bool twoPlayers, std::vector<SDL_Point> const &foods);
  void UpdateWindowTitle(int score, int fps);
  struct RGBA
  {
    Uint8 R;
    Uint8 G;
    Uint8 B;
    Uint8 A;
  };



 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  RGBA selected_color = {0xFF, 0xCC, 0x00, 0xFF};
  RGBA not_selected_color = {0xAA, 0xAA, 0xAA, 0xFF};
};

#endif