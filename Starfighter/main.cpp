//
//  main.cpp
//  Starfighter
//
//  Created by Patrick Montag on 10/20/16.
//  Copyright © 2016 Patrick Montag. All rights reserved.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h> // game controller api
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <time.h> // time
#include <stdlib.h> // srand and rand
#include <vector> // vector
#include <deque>
#include <list>

// headers
#include "AssetCache.h"
#include "Player.h"
#include "Bullet.h"
#include "Missile.h"
#include "Asteroid.h"
#include "Animation.h"
#include "Point.h"
#include "Starfield.h"
#include "Text.h"
#include "HUD.h"
#include "Map.h"
//#include "MusicPlayer.h"

// logical resolution
const int LOGICAL_WIDTH = 320;
const int LOGICAL_HEIGHT = 280;
const SDL_Rect gameBounds = { 0, 0, LOGICAL_WIDTH, LOGICAL_HEIGHT };

// window scale factor
const int SCREEN_SCALE_FACTOR = 3;

// screen dimension constants
const int SCREEN_WIDTH = (gameBounds.w * SCREEN_SCALE_FACTOR);
const int SCREEN_HEIGHT = (gameBounds.h * SCREEN_SCALE_FACTOR);

int FRAME_TIME = 0;

uint asteroidCounter;
int asteroidsDestroyed = 0;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

// box collision detector
bool checkCollision(SDL_Rect a, SDL_Rect b);

//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

char scoreBuf[100];

using namespace std;

/* Music_Player
// TODO: organize music player lifted from Shay Green's sample code
static Music_Player* player;
static bool paused;

void handle_error( const char* error )
{
    if ( error ) {
        fprintf(stderr, "Error: %s", error);
    }
}

static void start_track( int track, const char* path )
{
    paused = false;
    handle_error( player->start_track( track - 1 ) );

    // update window title with track info

    long seconds = player->track_info().length / 1000;
    const char* game = player->track_info().game;
    if ( !*game )
    {
        // extract filename
        game = strrchr( path, '\\' ); // DOS
        if ( !game )
            game = strrchr( path, '/' ); // UNIX
        if ( !game )
            game = path;
        else
            game++; // skip path separator
    }

    char title [512];
    printf( title, "%s: %d/%d %s (%ld:%02ld)\n",
            game, track, player->track_count(), player->track_info().song,
            seconds / 60, seconds % 60 );
}
*/

bool init() {
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        // set texture filtering to linear
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        
        SDL_GameController *controller = nullptr;
        
        for(int i = 0; i < SDL_NumJoysticks(); i++) {
            if(SDL_IsGameController(i)) {
                controller = SDL_GameControllerOpen(i);
                break;
            }
        }
        //if (controller == NULL) {
        //    printf("open failed: %s\n", SDL_GetError());
        //    return false;
        //}
        
        // connect controller
        if (controller) {
            printf("found a valid controller, named: %s\n", SDL_GameControllerName(controller));
        }

        // init audio
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 256) < 0)
            printf("Mix_OpenAudio error: %s\n", SDL_GetError());
        Mix_AllocateChannels(32);

        // load support for the MOD sample/music formats
        int flags = MIX_INIT_MODPLUG | MIX_INIT_MP3;
        int inited = Mix_Init(flags);
        if((inited & flags) != flags) {
            printf("Mix_Init error: %s\n", Mix_GetError());
        }
        // print music decoders available
        int i,max=Mix_GetNumMusicDecoders();
        for(i=0; i<max; ++i)
            printf("Music decoder %d is for %s", i, Mix_GetMusicDecoder(i));
        Mix_Music *music;
        music = Mix_LoadMUS("lifefrce.mp3");
        if(!music) {
            printf("Mix_LoadMUS error: %s\n", Mix_GetError());
        } else {
            // play music forever
            if(Mix_PlayMusic(music, -1) == -1) {
                printf("Mix_PlayMusic error: %s\n", Mix_GetError());
            }
        }

        //Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            // Create vsynced renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( renderer == NULL) {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else {
                SDL_RenderSetLogicalSize(renderer, gameBounds.w, gameBounds.h);

                // initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

// MusicPlayer NES junk - Requires SDL_OpenAudio (Not Mix_OpenAudio)
//    player = new Music_Player;
//    if ( !player )
//        handle_error( "Out of memory" );
//    handle_error( player->init() );
//
//    const char* path = "lifefrce.nsf";
//    handle_error( player->load_file(path) );
//    start_track( 1, path );

    return success;
}

void close() {
    //Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    
    //Quit SDL subsystems
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    IMG_Init(IMG_INIT_PNG);
    
    // Start up SDL and create window
    if(!init()) {
        printf("Failed to initialize!\n");
    } else {
        
    }
    // free resources and close SDL
    close();
    return 0;
}
