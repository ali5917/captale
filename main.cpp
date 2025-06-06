#include <iostream>
#include "C:\raylib\raylib\src\raylib.h"
#include <fstream>
#include "headers\customCity.h"
#include "headers\lobby.h"
#include "headers\pongCity.h"
#include "headers\carCity.h"
#include "headers\atmCity.h"
#include "headers\spaceShooter.h"
#include "headers\earningCity.h"
#include "headers\energyCity.h"
using namespace std;

class FileException : public exception {
    private:
        string fileName;
    public:
        FileException(string f) :fileName(f) {}
        const char* what() const noexcept override {
            string message = "FileException: Couldn't open file " + fileName;
            return message.c_str();
        }
};

class DataNotFoundException : public exception {
    private:
        string missingData;
    public:
        DataNotFoundException(string m) : missingData(m) {}
        const char* what() const noexcept override {
            string message = "DataNotFoundException: Complete data not found: " + missingData;
            return message.c_str();
        }
};

class CapTaleSystem {
    public:
        enum CapTaleState {
            LOBBY,
            CUSTOM_CITY, 
            PONG_CITY,
            ATM_CITY,
            CAR_CITY,
            SPACE_CITY,
            EARN_CITY,
            ENERGY_CITY,
            GAME_MANUAL,
            GAME_OVER
        };
    
    private:
        CapTaleState state;
        Cap* player;
        Font messagesFont;
        MessageManager messages;
        CustomCity customCity;
        PongCity pongCity;
        CarCity carCity;
        ATMCity atmCity;
        Lobby lobby;
        SpaceShooter spaceShooter;
        EarnCity earnCity;
        EnergyCity energyCity;
        Texture2D gameOver;
        Texture2D gameManual;
        bool enterPong;

    public:
        CapTaleSystem (Texture2D bgTex) : player(new Cap()), messagesFont(LoadFontEx("assets/fonts/Montserrat-SemiBold.ttf", TOKEN_FONT_SIZE, NULL, 0)), messages(messagesFont), atmCity(player, &messages), lobby(player, &messages), earnCity(player), energyCity(player), enterPong(false) {
            gameOver = LoadTexture("assets/gameOver/game-over.png");
            gameManual = LoadTexture("assets/gameManual/game-manual.png");
            bool success = loadData();
            if(success) {
                state = LOBBY;
            } else {
                state = CUSTOM_CITY;
            }
        }

        bool loadData() {
            ifstream inFile;
            bool loaded = false;
            try {
                inFile.open("characterData.txt", ios::in);
                if(!inFile) {
                    throw FileException("characterData.txt");
                }
                Vector2 pos;
                string line;
                // Pos X
                if (!getline(inFile, line)) throw DataNotFoundException("Missing pos.x");
                pos.x = stof(line);
                // Pos Y
                if (!getline(inFile, line)) throw DataNotFoundException("Missing pos.y");
                pos.y = stof(line);
                // Cash
                if (!getline(inFile, line)) throw DataNotFoundException("Missing cash");
                int cash = stoi(line);
                // Tokens
                if (!getline(inFile, line)) throw DataNotFoundException("Missing tokens");
                int tokens = stoi(line);
                // Energy
                if (!getline(inFile, line)) throw DataNotFoundException("Missing energy");
                int energy = stoi(line);
                if (energy == 0) energy = 20;
                // selectedCap
                if (!getline(inFile, line)) throw DataNotFoundException("Missing selectedCap");
                int selectedCap = stoi(line);
                Texture lobbyCapTex = LoadTexture(("assets/lobby/" + to_string(selectedCap) + ".png").c_str());
                player->setTexture(lobbyCapTex);
                player->setSize({(float)lobbyCapTex.width, (float)lobbyCapTex.height});
                player->setPosition(pos);
                player->addCash(cash - INITIAL_CASH);
                player->addTokens(tokens);
                player->increaseEnergy(energy - player->getEnergy());
                loaded = true;
            } catch (FileException& obj) {
                cout << obj.what();
            } catch (DataNotFoundException& obj) {
                cout << obj.what();
            }
            inFile.close();
            return loaded;
        }

        void saveData() {
            ofstream writeFile;
            try {
                writeFile.open("characterData.txt", ios::out);
                if(!writeFile) {
                    throw FileException("characterData.txt");
                }
                // if(!player->getGameOver()) {
                    writeFile << player->getPos().x << endl;
                    writeFile << player->getPos().y << endl;
                    writeFile << player->getCash() << endl;
                    writeFile << player->getTokens() << endl;
                    writeFile << player->getEnergy() << endl;
                    writeFile << customCity.getSelectedCap() << endl;
                // }

            } catch (FileException& obj) {
                cout << obj.what();
                cout << "Couldn't save data :(" << endl;
            }
            writeFile.close();
        }

        ~CapTaleSystem () {
            saveData();
            UnloadTexture(gameOver);
            UnloadTexture(gameManual);
            CloseWindow();
        }

        void runGame () {
            while (WindowShouldClose() == false) {
                update();
                draw();
            }
        }

        void update() {
            if (state == CUSTOM_CITY) {
                if (IsKeyPressed(KEY_RIGHT)) customCity.nextCap();
                if (IsKeyPressed(KEY_LEFT)) customCity.prevCap();
                if (IsKeyPressed(KEY_ENTER)) {
                    Texture lobbyCapTex = LoadTexture(("assets/lobby/" + to_string(customCity.getSelectedCap()) + ".png").c_str());
                    player->setTexture(lobbyCapTex);
                    player->setPosition({WINDOW_WIDTH/2.0f - lobbyCapTex.width/2.0f, WINDOW_HEIGHT - lobbyCapTex.height - 20.0f});
                    player->setSize({(float)lobbyCapTex.width, (float)lobbyCapTex.height});
                    state = LOBBY;
                }
            } else if (state == LOBBY) {
                state = (CapTaleState)lobby.update();

                if (player->getGameOver()) {
                    state = GAME_OVER;
                }

            } else if (state == PONG_CITY) {
                if (IsKeyPressed(KEY_L)) {
                    state = LOBBY;
                    enterPong = false;
                }

                if (!enterPong) {
                    enterPong = true;
                    pongCity.isPaused = false;
                    pongCity.pongState = PongCity::GAME_ENEMY;
                    
                }
                pongCity.update();         

            } else if (state == CAR_CITY) {
                if (IsKeyPressed(KEY_L)) {
                    state = LOBBY;
                }
                float d = GetFrameTime();
                carCity.update(d);
            } else if (state == ATM_CITY) {
                if (IsKeyPressed(KEY_L)) {
                    state = LOBBY;
                }
                atmCity.update();
               
            } else if (state == SPACE_CITY) {
                if (IsKeyPressed(KEY_L)) {
                    spaceShooter.restart();
                    state = LOBBY;
                }
                bool val = spaceShooter.update();
                if(val) {
                    spaceShooter.restart();
                    state = LOBBY;
                }
            } else if(state == EARN_CITY) {
                if(IsKeyPressed(KEY_L)) {
                    earnCity.reset();
                    state = LOBBY;
                }
                earnCity.update();
            } else if (state == ENERGY_CITY) {
                if (IsKeyPressed(KEY_L)) {
                    state = LOBBY;
                }
                energyCity.update();
            } else if (state == GAME_MANUAL) {
                if (IsKeyPressed(KEY_L)) {
                    state = LOBBY;
                }
                
            }
            messages.update();
        }

        void draw () {
            BeginDrawing();
            ClearBackground(BLACK);
           
            if (state == CUSTOM_CITY) {
                customCity.draw();
            } else if (state == LOBBY) {
                lobby.draw();
            } else if (state == PONG_CITY) {
                pongCity.draw();
            } else if (state == CAR_CITY) {
                carCity.draw();
            } else if (state == ATM_CITY) {
                atmCity.draw();
            } else if (state == SPACE_CITY) {
                spaceShooter.draw();
            } else if (state == EARN_CITY) {
                earnCity.draw();
            } else if (state == ENERGY_CITY) {
                energyCity.draw();
            } else if (state == GAME_OVER) {
                DrawTexture(gameOver, 0, 0, WHITE);
            } else if (state == GAME_MANUAL) {
                DrawTexture(gameManual, 0, 0, WHITE);
            }
            messages.draw();
            EndDrawing();
        }    
};



int main () {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CapTale");
    SetTargetFPS(60);

    Texture2D capTex = LoadTexture("assets/cap.png");
    Texture2D bgTex = LoadTexture("assets/lobby/bg1.png");

    CapTaleSystem game(bgTex);
    game.runGame();

    UnloadTexture(capTex);
    UnloadTexture(bgTex);
    return 0;
}