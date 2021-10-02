#include "Game.h"

namespace pong {

template<typename T>
inline const T clamp(const T& value, const T& min, const T& max) {
    return value < min ? min : (value > max ? max : value);
}

template<typename T>
inline const T abs(const T& x) {
    return (x < 0) ? -x : x;
}

Game::Game() {
    m_window   = nullptr;
    m_renderer = nullptr;

    m_running = true;

    m_ticks_count = 0;

    m_paddle_pos = {10.0f, 768.0f / 2.0f};
    m_paddle_dir = 0;

    m_ball_pos = {
        1024.0f / 2.0f,
        768.0f / 2.0f,
    };
    m_ball_vel = {
        -200.0f,
        235.0f,
    };
}

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    m_window = SDL_CreateWindow(
        "Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, 0);
    if (!m_window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    m_renderer = SDL_CreateRenderer(
        m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Game::run_loop() {
    while (m_running) {
        process_input();
        update();
        render();
    }
}

void Game::shutdown() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Game::process_input() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                m_running = false;
            } break;
        }
    }

    const uint8_t* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_ESCAPE]) {
        m_running = false;
    }

    m_paddle_dir = 0;
    if (state[SDL_SCANCODE_W]) {
        m_paddle_dir -= 1;
    }
    if (state[SDL_SCANCODE_S]) {
        m_paddle_dir += 1;
    }
}

void Game::update() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticks_count + 16)) {}

    uint32_t now   = SDL_GetTicks();
    float    delta = (now - m_ticks_count) / 1000.0f;
    m_ticks_count  = now;

    if (delta > 0.05f) {
        delta = 0.05f;
    }

    if (m_paddle_dir != 0) {
        m_paddle_pos.y += m_paddle_dir * 300.0f * delta;
        m_paddle_pos.y = clamp(
            m_paddle_pos.y, (paddle_height / 2.0f + thickness),
            768.0f - paddle_height / 2.0f - thickness);
    }

    m_ball_pos.x += m_ball_vel.x * delta;
    m_ball_pos.y += m_ball_vel.y * delta;

    // Bounce of paddle, end if ball offscreen or bounce of right wall.
    float diff = abs(m_paddle_pos.y - m_ball_pos.y);
    if (diff <= paddle_height / 2.0f && m_ball_pos.x <= 25.0f &&
        m_ball_pos.x >= 20.0f && m_ball_vel.x < 0.0f) {
        m_ball_vel.x *= -1.0f;
    } else if (m_ball_pos.x <= 0.0f) {
        m_running = false;
    } else if (m_ball_pos.x >= (1024.0f - thickness) && m_ball_vel.x > 0.0f) {
        m_ball_vel.x *= -1.0f;
    }

    // Bounce ball off top wall or bottom wall.
    if (m_ball_pos.y <= thickness && m_ball_vel.y < 0.0f) {
        m_ball_vel.y *= -1;
    } else if (m_ball_pos.y >= (768 - thickness) && m_ball_vel.y > 0.0f) {
        m_ball_vel.y *= -1;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(m_renderer, 0, 9, 255, 255);
    SDL_RenderClear(m_renderer);

    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

    // Draw top wall
    SDL_Rect wall{
        0,
        0,
        1024,
        thickness,
    };
    SDL_RenderFillRect(m_renderer, &wall);

    // Draw bottom wall
    wall.y = 768 - thickness;
    SDL_RenderFillRect(m_renderer, &wall);

    // Draw right wall
    wall.x = 1024 - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 1024;
    SDL_RenderFillRect(m_renderer, &wall);

    SDL_Rect ball{
        static_cast<int>(m_ball_pos.x - thickness / 2),
        static_cast<int>(m_ball_pos.y - thickness / 2),
        thickness,
        thickness,
    };
    SDL_RenderFillRect(m_renderer, &ball);

    SDL_Rect paddle{
        static_cast<int>(m_paddle_pos.x),
        static_cast<int>(m_paddle_pos.y - paddle_height / 2),
        thickness,
        paddle_height,
    };
    SDL_RenderFillRect(m_renderer, &paddle);

    SDL_RenderPresent(m_renderer);
}

} // namespace pong