#include <GameEngineFramework/Engine/EngineSystems.h>

#include <GameEngineFramework/Plugins/ChunkSpawner/ChunkManager.h>
#include <GameEngineFramework/Plugins/WeatherSystem/WeatherSystem.h>
#include <GameEngineFramework/Plugins/ParticleSystem/ParticleSystem.h>


// UI
extern GameObject* menuPanelObject;
extern GameObject* versionTextObject;
extern Button* loadWorldButton;
extern Button* saveWorldButton;
extern Button* clearWorldButton;
extern Button* quitButton;

void MainMenuEnable(void);
void MainMenuDisable(void);

void ButtonLoadWorld(Button* currentButton);
void ButtonSaveWorld(Button* currentButton);
void ButtonClearWorld(Button* currentButton);
void ButtonQuitApplication(Button* currentButton);


// List worlds
void FuncList(std::vector<std::string> args);

// Save world
void FuncSave(std::vector<std::string> args);

// Load world
void FuncLoad(std::vector<std::string> args);

// Remove a world
void FuncRemove(std::vector<std::string> args);

// Clear world
void FuncClear(std::vector<std::string> args);

// Set the world seed
void FuncSeed(std::vector<std::string> args);

// Summon an actor
void FuncSummon(std::vector<std::string> args);

// Time control
void FuncTime(std::vector<std::string> args);

// Weather control
void FuncWeather(std::vector<std::string> args);

// Event callbacks
void EventLostFocus(void);

