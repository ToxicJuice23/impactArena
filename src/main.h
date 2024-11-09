#ifndef MAIN_H
#define MAIN_H
#include <errno.h>

#ifdef WIN32
#include <io.h>
#define access _access
#else
#include <unistd.h>
#endif

#include <math.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <fcntl.h>
#define PI 3.141592653589
#define MAX_VELOCITY 2
#define CLOCK_DIVISOR (1.f / CLOCKS_PER_SEC)

typedef struct {
    float x, y;
} pos_t;

typedef struct {
    float   x, // exact starting x position of timeframe (Δx when Δtx = 0)
            y, // exact starting y position of timeframe (Δy when Δty = 0)
            mass, // in kg (used for friction and radius of ball)
            v_x, // in pixels/s
            v_y, // in pixels/s
            t0x, // time in seconds since thread birth at start or at last x collision
            t0y, // time in seconds since thread birth at start or at last x collision
            dtx, // Δt since last collision on x-axis or since start
            dty, // Δt since last collision with floor or ceiling
            v0_x, // initial speed on x at instant 0
            v0_y, // initial speed on y at instant 0
            G, // gravity constant
            Fx, // the external force that is applied on x
            Fy; // the external force applied on y
    int n_seg;
    SDL_FPoint* base_circle;
    pos_t p; // current position
    u_short window_size_x, window_size_y;
} ball_t;

typedef struct {
    char* name;
    void* var_ptr;
} pair_t;

typedef struct {ball_t* b; uint8_t done; uint8_t reset;} data_t;

float calculate_velocity_collision( float v);
void set_base_circle( ball_t* ball);
void drawCircle( SDL_Renderer* renderer, ball_t* ball,  float x,  float y,  int n);
float get_time( float start);
void set_pos( ball_t* ball);
int handle_collision( ball_t* ball,  u_short size_x,  u_short size_y, u_short* counter);
void setup_ball( ball_t* ball, float v0x, float v0y);
int computing_thread( void* d);
void fatalf(const char* fmt, ...);
int change_force(ball_t* ball, const char axis, const float new_force);

#endif // BALL_H
