#ifndef IGNIS_RENDERER_HARDWARE_WINDOW_H
#define IGNIS_RENDERER_HARDWARE_WINDOW_H

#include "ignis/containers/strview.h"

typedef struct SDL_Window SDL_Window;

typedef enum rhi_window_flag {
    R_WINDOW_FLAG_INVALID = 0,

    R_WINDOW_FLAG_FULLSCREEN_BTI = 1 << 0,
    R_WINDOW_FLAG_WINDOWED_BIT   = 1 << 1,
    R_WINDOW_FLAG_BORDERLESS_BIT = 1 << 2,
    R_WINDOW_FLAG_RESIABLE_BTI   = 1 << 3,
} rhi_window_flag_t;

typedef struct rhi_window_handle {
    SDL_Window* window;
    void*       backend;
    i32         opts;
} rhi_window_handle_t;

rhi_window_handle_t rhi_window_create(const strview_t title, i32 width,
                                      i32 height, i32 opts);
void                rhi_window_destroy(rhi_window_handle_t* win);
void                rhi_window_swap_buffers(rhi_window_handle_t* win);

#endif
