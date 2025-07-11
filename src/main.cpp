#include <iostream>
#include <string_view>
#include <cstdlib>
#include <cmath>
#include <cstddef>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

namespace
{

#ifndef _MIN_SCREEN_HEIGHT
  #define _MIN_SCREEN_HEIGHT 1000
#elif _MIN_SCREEN_HEIGHT < 0
  #error "_MIN_SCREEN_HEIGHT must be nonnegative value"
#endif

#ifndef _MIN_SCREEN_WIDTH
  #define _MIN_SCREEN_WIDTH 1400
#elif _MIN_SCREEN_WIDTH < 0
  #error "_MIN_SCREEN_WIDTH must be nonnegative value"
#endif

#ifndef _BASE_FPS
  #define _BASE_FPS 60
#elif _BASE_FPS < 0
  #error "_BASE_FPS must be nonnegative value"
#endif

const int screenHeight{GetScreenHeight()};
const int screenWidth{GetScreenWidth()};
constexpr int minScreenHeight{_MIN_SCREEN_HEIGHT};
constexpr int minScreenWidth{_MIN_SCREEN_WIDTH};
constexpr int BASE_FPS{_BASE_FPS};

constexpr int BUTTON_WIDTH{400};
constexpr int BUTTON_HEIGHT{100};

constexpr std::string_view windowTitle{"Infinite-Runner"};
constexpr std::string_view canyonTexturePath{"resources/textures/Canyon/layers/canyon.png"};
constexpr std::string_view cloudsTexturePath{"resources/textures/Canyon/layers/clouds.png"};
constexpr std::string_view farMountainsTexturePath{"resources/textures/Canyon/layers/far-mountains.png"};
constexpr std::string_view frontTexturePath{"resources/textures/Canyon/layers/front.png"};
constexpr std::string_view skyTexturePath{"resources/textures/Canyon/layers/sky.png"};

constexpr std::string_view blurShaderPath{"resources/shaders/blur.fs"};

constexpr Vector2 ZERO_VECTOR2{0.0f, 0.0f};
constexpr float NO_ROTATION{0.0f};
constexpr float ORIGIN_X_POSITION{0.0f};
constexpr float ORIGIN_Y_POSITION{0.0f};
constexpr char* DEFAULT_VERTEX_SHADER{nullptr};

constexpr Rectangle BUTTON_BASE_BOX{
    ORIGIN_X_POSITION,
    ORIGIN_Y_POSITION,
    BUTTON_WIDTH,
    BUTTON_HEIGHT
};

#ifndef _GLSL_VERSION
  #define _GLSL_VERSION 330
#elif _GLSL_VERSION != 330 && _GLSL_VERSION != 100
  #error "_GLSL_VERSION must be {330, 100}"
#endif

constexpr int GLSL_VERSION{_GLSL_VERSION};

enum GameState : int
{
  UNKNOWN,
  MAIN_MENU,
  SETTINGS,
  GAME
};

}

auto main() -> int
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(BASE_FPS);
  InitWindow(screenWidth, screenHeight, windowTitle.data());
  SetWindowMinSize(minScreenWidth, minScreenHeight);

  Texture2D canyonTexture{LoadTexture(canyonTexturePath.data())};
  Texture2D cloudsTexture{LoadTexture(cloudsTexturePath.data())};
  Texture2D farMountainsTexture{LoadTexture(farMountainsTexturePath.data())};
  Texture2D frontTexture{LoadTexture(frontTexturePath.data())};
  Texture2D skyTexture{LoadTexture(skyTexturePath.data())};

  Shader blurShader{LoadShader(DEFAULT_VERTEX_SHADER, TextFormat(blurShaderPath.data(), GLSL_VERSION))};

  float scrollingBackgroundSky{0.0f};
  constexpr float backgroundSpeed{1.0f};

  GameState currentState{GameState::MAIN_MENU};

  while (WindowShouldClose() == false)
  {
    int currentScreenWidth{GetScreenWidth()};
    int currentScreenHeight{GetScreenHeight()};
    float currentButtonXposition{static_cast<float>(currentScreenWidth >> 1) - (BUTTON_WIDTH >> 1)};
    float currentButtonYposition{static_cast<float>(currentScreenHeight >> 1)};

    if (currentState == GameState::MAIN_MENU || currentState == GameState::SETTINGS)
    {
      scrollingBackgroundSky -= backgroundSpeed * GetFrameTime();

      if (islessequal(scrollingBackgroundSky, cloudsTexture.width * -10.0f))
      {
        scrollingBackgroundSky = 0.0f;
      }

      BeginDrawing();
      ClearBackground(RAYWHITE);
      Rectangle sourceSkyRectangle{
          ORIGIN_X_POSITION,
          ORIGIN_Y_POSITION,
          static_cast<float>(skyTexture.width),
          static_cast<float>(skyTexture.height)
      };
      Rectangle targetSkyRectangle{
          ORIGIN_X_POSITION,
          ORIGIN_Y_POSITION,
          static_cast<float>(skyTexture.width << 7),
          static_cast<float>(skyTexture.height << 3)
      };
      DrawTexturePro(
          skyTexture,
          sourceSkyRectangle,
          targetSkyRectangle,
          ZERO_VECTOR2,
          NO_ROTATION,
          WHITE
      );
      Rectangle sourceCloudsRectangle{
          -scrollingBackgroundSky,
          ORIGIN_Y_POSITION,
          static_cast<float>(cloudsTexture.width),
          static_cast<float>(cloudsTexture.height)
      };
      Rectangle targetCloudsRectangle{
          ORIGIN_X_POSITION,
          ORIGIN_Y_POSITION,
          static_cast<float>(cloudsTexture.width) * 10.0f + scrollingBackgroundSky,
          static_cast<float>(cloudsTexture.height << 3)
      };
      DrawTexturePro(
          cloudsTexture,
          sourceCloudsRectangle,
          targetCloudsRectangle,
          ZERO_VECTOR2,
          NO_ROTATION,
          WHITE
      );
      targetCloudsRectangle.x = targetCloudsRectangle.width;
      DrawTexturePro(
          cloudsTexture,
          sourceCloudsRectangle,
          targetCloudsRectangle,
          ZERO_VECTOR2,
          NO_ROTATION,
          WHITE
      );
      Rectangle sourceFarMountainsRectangle{
          ORIGIN_X_POSITION,
          ORIGIN_Y_POSITION,
          static_cast<float>(farMountainsTexture.width),
          static_cast<float>(farMountainsTexture.height)
      };
      Rectangle targetFarMountainsRectangle{
          ORIGIN_X_POSITION,
          ORIGIN_Y_POSITION,
          static_cast<float>(farMountainsTexture.width) * 10.0f,
          static_cast<float>(farMountainsTexture.height << 3)
      };
      DrawTexturePro(
          farMountainsTexture,
          sourceFarMountainsRectangle,
          targetFarMountainsRectangle,
          ZERO_VECTOR2,
          NO_ROTATION,
          WHITE
      );
      targetFarMountainsRectangle.x = 2080.0f;
      DrawTexturePro(
          farMountainsTexture,
          sourceFarMountainsRectangle,
          targetFarMountainsRectangle,
          ZERO_VECTOR2,
          NO_ROTATION,
          WHITE
      );
      Rectangle sourceCanyonRectangle{
          ORIGIN_X_POSITION,
          ORIGIN_Y_POSITION,
          static_cast<float>(canyonTexture.width),
          static_cast<float>(canyonTexture.height)
      };
      Rectangle targetCanyonRectangle{
          ORIGIN_X_POSITION,
          ORIGIN_Y_POSITION,
          static_cast<float>(canyonTexture.width) * 10.0f,
          static_cast<float>(canyonTexture.height << 3)
      };
      DrawTexturePro(
          canyonTexture,
          sourceCanyonRectangle,
          targetCanyonRectangle,
          ZERO_VECTOR2,
          NO_ROTATION,
          WHITE
      );
      Rectangle sourceFrontRectangle{
          ORIGIN_X_POSITION,
          ORIGIN_Y_POSITION,
          static_cast<float>(frontTexture.width),
          static_cast<float>(frontTexture.height)
      };
      Rectangle targetFrontRectangle{
          ORIGIN_X_POSITION,
          ORIGIN_Y_POSITION,
          static_cast<float>(frontTexture.width) * 10.0f,
          static_cast<float>(frontTexture.height << 3)
      };
      DrawTexturePro(
          frontTexture,
          sourceCanyonRectangle,
          targetCanyonRectangle,
          ZERO_VECTOR2,
          NO_ROTATION,
          WHITE
      );

      if (currentState == GameState::MAIN_MENU)
      {
        if (GuiButton({currentButtonXposition, 100.0f, BUTTON_WIDTH, BUTTON_HEIGHT}, "Start Game"))
        {
          currentState = GameState::GAME;
        }
        else if (GuiButton({currentButtonXposition, 300.0f, BUTTON_WIDTH, BUTTON_HEIGHT}, "Settings"))
        {
          currentState = GameState::SETTINGS;
        }
        else if (GuiButton({currentButtonXposition, 500.0f, BUTTON_WIDTH, BUTTON_HEIGHT}, "Exit"))
        {
          EndDrawing();
          CloseWindow();
          exit(EXIT_SUCCESS);
        }
      }
      else
      {
        // TODO : Complete blur effect for settings. Add settings to this runner game.
        RenderTexture2D target{
            static_cast<unsigned int>(currentScreenWidth),
            static_cast<unsigned int>(currentScreenHeight)
        };
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);
        EndTextureMode();
        BeginShaderMode(blurShader);
        Rectangle sourceRectangle{
            ORIGIN_X_POSITION,
            ORIGIN_Y_POSITION,
            static_cast<float>(target.texture.width),
            static_cast<float>(-target.texture.height)
        };
        DrawTextureRec(
            target.texture,
            sourceRectangle,
            ZERO_VECTOR2,
            WHITE
        );
        EndShaderMode();
        /*DrawRectangle(
            ORIGIN_X_POSITION,
            ORIGIN_Y_POSITION,
            currentScreenWidth,
            currentScreenHeight,
            {255, 255, 255, 100}
        );*/
        UnloadRenderTexture(target);
        if (GuiButton({currentButtonXposition, 100.0f, BUTTON_WIDTH, BUTTON_HEIGHT}, "Back"))
        {
          currentState = GameState::MAIN_MENU;
        }
      }

      EndDrawing();
    }
    else if (currentState == GameState::GAME)
    {
      BeginDrawing();
      ClearBackground(RAYWHITE);
      if (GuiButton({currentButtonXposition, 100.0f, BUTTON_WIDTH, BUTTON_HEIGHT}, "Back"))
      {
        currentState = GameState::MAIN_MENU;
      }
      EndDrawing();
    }
    else
    {
      std::cerr << "Unknown game state. Aborting...\n";
      exit(EXIT_FAILURE);
    }
  }

  UnloadTexture(canyonTexture);
  UnloadTexture(cloudsTexture);
  UnloadTexture(farMountainsTexture);
  UnloadTexture(frontTexture);
  UnloadTexture(skyTexture);

  UnloadShader(blurShader);

  CloseWindow();
  return 0;
}