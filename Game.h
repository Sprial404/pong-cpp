#ifndef PONG_GAME_H
#define PONG_GAME_H

#include <SDL.h>

namespace pong {

const int thickness = 15;
const float paddle_height = 100.0f;

struct Vec2 {
    float x;
    float y;
};

class Game {
  public:
    Game();

    // Initialize the game.
    bool initialize();

    // Runs the game loop until the game is over.
    void run_loop();

    // Shutdown the game.
    void shutdown();

  private:
    void process_input();

    void update();

    void render();

  private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    Vec2 m_paddle_pos;
    int  m_paddle_dir;

    Vec2 m_ball_pos;
    Vec2 m_ball_vel;

    bool m_running;

    uint32_t m_ticks_count;
};

} // namespace pong

#endif