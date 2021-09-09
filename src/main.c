/*******************************************************************************************
*
*   raylib - Standard Game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "screens/screens.h"    // NOTE: Defines global variable: currentScreen
#include <stdio.h>      // Required for fopen()
#include <string.h>

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------



int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------

    applicationName = "CELLULAR AUTOMATA";
    currentScreen = LOGO;   // NOTE: currentScreen is defined in screens.h as a global variable
    prevScreen = LOGO;

    style_file = "./cellular_automata.rgs";
    settings_file = "./cellular_automata.settings";

    
    if(!FileExists(settings_file)){
        SaveFileText(settings_file, SETTINGS_DEFAULT);
    }
    LoadSettings(settings_file, &useropts);

    InitWindow(useropts.screenWidth, useropts.screenHeight, applicationName);
    SetWindowSize(useropts.screenWidth, useropts.screenHeight);

    if(useropts.fullscreen && !IsWindowFullscreen()) ToggleFullscreen();
    
    UpdateGuiSizing();
    TransitionScreens(prevScreen, currentScreen);
    

    SetTargetFPS(60);

    //----------------------------------------------------------

    SetExitKey(0);
    bool exitApplication = false;

    // Main game loop
    while (!exitApplication)    // Detect window close button or ESC key
    {       
        // Update
        //----------------------------------------------------------------------------------

        exitApplication = WindowShouldClose();

        switch(currentScreen) 
        {
            case LOGO: 
            {
                UpdateLogoScreen();
                
                if (FinishLogoScreen())
                {
                    prevScreen = LOGO;
                    currentScreen = TITLE;
                    TransitionScreens(prevScreen, currentScreen);
                }
            } break;
            case TITLE: 
            {
                UpdateTitleScreen();
                
                // NOTE: FinishTitleScreen() return an int defining the screen to jump to
                if (FinishTitleScreen() == 1)
                {
                    prevScreen = TITLE;
                    currentScreen = GAMEPLAY;
                    TransitionScreens(prevScreen, currentScreen);
                }
                else if (FinishTitleScreen() == 2)
                {
                    prevScreen = TITLE;
                    currentScreen = OPTIONS;
                    TransitionScreens(prevScreen, currentScreen);
                }
                else if (FinishTitleScreen() == 3){
                    exitApplication = true;
                }
            } break;
            case OPTIONS:
            {
                UpdateOptionsScreen();
                
                if (FinishOptionsScreen() && (prevScreen == TITLE))
                {
                    prevScreen = OPTIONS;
                    currentScreen = TITLE;
                    UpdateGuiSizing();
                    TransitionScreens(prevScreen, currentScreen);
                }
                else if (FinishOptionsScreen() && (prevScreen == GAMEPLAY))
                {
                    prevScreen = OPTIONS;
                    currentScreen = GAMEPLAY;
                    UpdateGuiSizing();
                    UnloadOptionsScreen();
                }
            } break;
            case GAMEPLAY:
            { 
                UpdateGameplayScreen();
                
                if (FinishGameplayScreen() == 1)
                {
                    prevScreen = GAMEPLAY;
                    currentScreen = TITLE;
                    TransitionScreens(prevScreen, currentScreen);
                }
                else if(FinishGameplayScreen() == 2)
                {
                    prevScreen = GAMEPLAY;
                    currentScreen = OPTIONS;
                    UnloadGameplayScreen();
                    InitOptionsScreen();
                }
                
            } break;
            case ENDING: 
            {
                UpdateEndingScreen();
                
                if (FinishEndingScreen())
                {
                    prevScreen = ENDING;
                    currentScreen = TITLE;
                    TransitionScreens(prevScreen, currentScreen);
                } 
            } break;
            default: break;
        }
        
        //----------------------------------------------------------------------------------
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
            ClearBackground(RAYWHITE);
            
            switch(currentScreen) 
            {
                case LOGO: DrawLogoScreen(); break;
                case TITLE: DrawTitleScreen(); break;
                case OPTIONS: DrawOptionsScreen(); break;
                case GAMEPLAY: DrawGameplayScreen(); break;
                case ENDING: DrawEndingScreen(); break;
                default: break;
            }
        
            //DrawFPS(10, 10);
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    
    // TODO: Unload all global loaded data (i.e. fonts) here!
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}





