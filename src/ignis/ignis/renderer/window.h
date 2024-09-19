#ifndef IGNIS_RENDERER_WINDOW_H
#define IGNIS_RENDERER_WINDOW_H

typedef struct SDL_Window SDL_Window;

typedef struct rndr_window {
  SDL_Window* window;
  void*       backend;
} rndr_window;

#endif
