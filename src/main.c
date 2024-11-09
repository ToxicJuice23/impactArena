#include "main.h"

// 200 pixels ~= 1 meter (used for physics)

int main(const int argc, char** argv) {
    // argv[1] will be the server address not a file

    // setup window and renderer
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Impact Arena Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    int *wx = malloc(sizeof(int)), *wy = malloc(sizeof(int));
    SDL_GetWindowSize(window, wx, wy);
    const u_short window_size_x = *wx;
    const u_short window_size_y = *wy;
    free(wx); free(wy);


    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        fatalf("Failed to create renderer.\n");
        return -1;
    }
    // allocate memory for variables
    data_t* data = malloc(sizeof(data_t));

    ball_t ball;

    ball.v0_x = 0;
    ball.v0_y = 0;
    ball.window_size_x = window_size_x;
    ball.window_size_y = window_size_y;
    ball.G = 200.f * -9.8f; // 200 px/m * -9.8 m/s^2
    ball.Fx = 0;
    ball.Fy = 0;
    // don't kill the thread every restart lol
    data->b = &ball; data->done = 0; data->reset = 0;
    SDL_CreateThread(computing_thread, "ball computing", data);
restart:
    SDL_Event e; int quit = 0;
    while( quit == 0 ) {
        while (SDL_PollEvent( &e )) {
            if( e.type == SDL_QUIT ) {
                quit = 1;
                break;
            }
            if ( e.type == SDL_KEYUP ) {
                const SDL_Keycode k = e.key.keysym.sym;
                if ( k == SDLK_ESCAPE ) {
                    data->reset = 1;
                    goto restart;
                }
                if (k == SDLK_q) {
                    goto done;
                }
                if (k == SDLK_DOWN || k == SDLK_UP) {
                    const int res = change_force(&ball, 'y', 0);
                    if (res != 0) {
                        fatalf("change_force() failed\n");
                    }
                }
                if (k == SDLK_RIGHT || k == SDLK_LEFT) {
                    const int res = change_force(&ball, 'x', 0);
                    if (res != 0) {
                        fatalf("change_force() failed\n");
                    }
                }
            }
            if (e.type == SDL_KEYDOWN) {
                const float force = 43200;
                const SDL_Keycode k = e.key.keysym.sym;
                if (k == SDLK_UP && ball.Fy != force) {
                    const int res = change_force(&ball, 'y', force);
                    if (res != 0) {
                        fatalf("change_force() failed\n");
                    }
                } else if (k == SDLK_DOWN && ball.Fy != 0-force) {
                    const int res = change_force(&ball, 'y', 0-force);
                    if (res != 0) {
                        fatalf("change_force() failed\n");
                    }
                }
                if (k == SDLK_RIGHT && ball.Fx != force) {
                    const int res = change_force(&ball, 'x', force);
                    if (res != 0) {
                        fatalf("change_force() failed\n");
                    }
                } else if (k == SDLK_LEFT && ball.Fx != 0-force) {
                    const int res = change_force(&ball, 'x', 0-force);
                    if (res != 0) {
                        fatalf("change_force() failed\n");
                    }
                }
            }
        }

        if (SDL_RenderClear(renderer)) {
            fatalf("Failed to clear render: %s\n", SDL_GetError());
        }
        // draw frame
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, &ball, ball.x + ball.p.x, (float)ball.window_size_y - (ball.y+ball.p.y), ball.n_seg);
        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    // clean up
    done:
    data->done = 1;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(data);
    return 0;
}

int computing_thread(void* d) {
    data_t* data = (data_t*)d;
    if (data == NULL) {
        fatalf("Corrupted data ptr\n");
        return 1;
    }

    ball_t* ball = data->b;

    reset:
    setup_ball(ball, ball->v0_x, ball->v0_y);

    SDL_Event e;
    u_short counter = 0;
    u_short collision_counter = 0;
    while (!data->done) {
        // every time counter loops back, we poll to see if quit
        if (counter == 0) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    free(d);
                    fatalf("SDL quit.\n");
                    return 1;
                }
            }
        }

        // actual computing:
        ball->dtx = get_time(ball->t0x);
        ball->dty = get_time(ball->t0y);
        handle_collision(ball, ball->window_size_x, ball->window_size_y, &collision_counter);
        set_pos(ball);
        // done
        if (counter == UINT16_MAX) counter = 0;
        else counter++;

        if (collision_counter > 70) {
            goto reset;
        }
        if (data->reset) {
            data->reset = 0;
            goto reset;
        }
        SDL_Delay(10);
    }
    return 0;
}