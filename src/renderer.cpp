#include "renderer.h"
#include <iostream>
#include <string>



Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::RenderMenu(bool twoPlayers)
{
  SDL_Rect block;
  block.w = screen_width / 2;
  block.h = screen_height / 8;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  TTF_Init(); 
  TTF_Font* Arial = TTF_OpenFont("/Library/Fonts/Arial Unicode.ttf", 30); //this opens a font style and sets a size
  if (Arial == NULL) {
    std::cout << "No font" << std::endl;
    std::cout << TTF_GetError() << std::endl;
  }
  SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
  SDL_Surface* onePlayerSurfaceMessage = TTF_RenderText_Solid(Arial, "One Player", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
  SDL_Texture* onePlayerMessage = SDL_CreateTextureFromSurface(sdl_renderer, onePlayerSurfaceMessage); //now you can convert it into a texture
  SDL_Surface* twoPlayerSurfaceMessage = TTF_RenderText_Solid(Arial, "Two Players", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
  SDL_Texture* twoPlayerMessage = SDL_CreateTextureFromSurface(sdl_renderer, twoPlayerSurfaceMessage); //now you can convert it into a texture


  // Render Menu: One Player
  RGBA one_player_color = twoPlayers ? not_selected_color : selected_color;
  SDL_SetRenderDrawColor(sdl_renderer, one_player_color.R, one_player_color.G, one_player_color.B, one_player_color.A);
  block.x = screen_width / 4;
  block.y = screen_height / 4;

  SDL_RenderFillRect(sdl_renderer, &block);
  SDL_RenderCopy(sdl_renderer, onePlayerMessage, NULL, &block); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

  // //Don't forget to free your surface and texture
  SDL_FreeSurface(onePlayerSurfaceMessage);
  SDL_DestroyTexture(onePlayerMessage);


  // Render Menu: Two Player
  RGBA two_players_color = twoPlayers ?  selected_color : not_selected_color;
  SDL_SetRenderDrawColor(sdl_renderer, two_players_color.R, two_players_color.G, two_players_color.B, two_players_color.A);
  block.x = screen_width / 4;
  block.y = screen_height / 4 * 2;
  SDL_RenderFillRect(sdl_renderer, &block);
  SDL_RenderCopy(sdl_renderer, twoPlayerMessage, NULL, &block); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
  // //Don't forget to free your surface and texture
  SDL_FreeSurface(twoPlayerSurfaceMessage);
  SDL_DestroyTexture(twoPlayerMessage);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::Render(Snake const snake, Snake const snake2, bool twoPlayers, std::vector<SDL_Point> const &foods) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  for(auto food : foods)
  {
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  if (twoPlayers) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (SDL_Point const &point : snake2.body) {
      block.x = point.x * block.w;
      block.y = point.y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }
  }


  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  if (twoPlayers) {
    block.x = static_cast<int>(snake2.head_x) * block.w;
    block.y = static_cast<int>(snake2.head_y) * block.h;
    if (snake2.alive) {
      SDL_SetRenderDrawColor(sdl_renderer, 0x7A, 0x0, 0xCC, 0xFF);
    } else {
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    }
    SDL_RenderFillRect(sdl_renderer, &block);
  }


  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
