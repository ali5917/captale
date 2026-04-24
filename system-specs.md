# CapTale System Specifications

## 1. System Overview

CapTale is a desktop game built with Raylib. The player controls a character called Cap, moves through a lobby map, and enters different city scenes that act as mini-games or utility rooms. The system persists the player profile between sessions in `characterData.txt`.

The main gameplay loop is a Raylib frame loop inside `CapTaleSystem::runGame()`: while the window is open, the game updates the active state, then draws the active screen. The state machine routes control between the custom character selection screen, the lobby, individual city mini-games, a manual screen, and a game-over screen.

Key features include:

- Character selection and persistence across runs.
- A lobby map with interactive rooms.
- Several separate mini-games: Pong, Car City, Space Shooter, Earn City, Energy City, and ATM City.
- Resource management for cash, tokens, and energy.
- Message notifications for low energy and failed room entry.
- Win/lose/game-over presentation screens.

## 2. Actors

- Player: the human user who presses keyboard and mouse inputs to navigate the world and play mini-games.
- Cap character: the in-game avatar controlled by the player.
- Lobby rooms: interactive map zones that trigger city transitions.
- Mini-game opponents or systems: Pong AI, Car City hazards, Space Shooter enemies, Earn City questions, ATM conversion, and Energy City item collection.
- File system: stores and restores persistent player data from `characterData.txt` and quiz content from `assets/earnCity/earn_questions.csv`.

## 3. Core Modules / Components

### CapTaleSystem

Central application controller in `main.cpp`.

Responsibilities:

- Owns the global game state enum.
- Loads the player profile and selected cap.
- Holds all major scene objects.
- Dispatches update/draw calls based on the active state.
- Saves the player profile on shutdown.

Interactions:

- Owns a `Cap` instance and passes it to `Lobby`, `ATMCity`, `EarnCity`, and `EnergyCity`.
- Calls `Lobby::update()` to decide when to enter a room.
- Calls the active city object’s `update()` and `draw()` methods.

### Cap

Player avatar and shared resource container.

Responsibilities:

- Stores position, texture, size, direction, speed, cash, tokens, energy, and game-over status.
- Handles movement and screen boundary constraints.
- Tracks energy drain over movement.

Interactions:

- Used directly by the lobby and several cities.
- Exposes token, cash, and energy mutators for rooms that modify resources.
- Friend access is granted to `Lobby` and `ATMCity`.

### Lobby

Hub scene that represents the city map.

Responsibilities:

- Draws the lobby background, room grid, player avatar, token display, and energy bar.
- Detects when the player stands inside a room.
- Enforces token entry costs for some rooms.
- Shows warning messages when energy is low.
- Opens the game manual via the menu icon.

Interactions:

- Reads and updates the shared `Cap` object.
- Sends messages through `MessageManager`.
- Returns a room/state identifier to `CapTaleSystem`.

### Room

Simple rectangle wrapper used by the lobby grid.

Responsibilities:

- Stores room bounds and the room’s destination state.
- Draws an invisible hit area.

### Message and MessageManager

Notification system used for temporary in-game messages.

Responsibilities:

- `Message` stores text, size, visibility, and an auto-expiring timer.
- `MessageManager` owns a dynamic list of `Message` objects.
- Draws messages in sequence with spacing.

Interactions:

- `Lobby` and `ATMCity` push messages into the manager.
- `CapTaleSystem` updates and draws the manager every frame.

### CustomCity

Character selection scene.

Responsibilities:

- Displays four selectable cap textures.
- Lets the user cycle through available cap appearances.

Interactions:

- Selected cap index is later persisted and reloaded by `CapTaleSystem`.

### PongCity

Pong mini-game with an AI mode and a human-vs-human mode.

Responsibilities:

- Manages game mode selection, match play, pause, win/lose screens, and level themes.
- Uses shared score variables and level-dependent settings.

Interactions:

- Contains `Ball`, `Paddle`, `AiPaddle`, `player1Paddle`, and `player2Paddle` objects.
- Uses global score variables and a global level selector in the current header.

### CarCity

Endless runner style mini-game.

Responsibilities:

- Moves a car left/right while spawning obstacles, coins, and power-ups.
- Tracks score, collected coins, difficulty, pause state, and game over.
- Applies temporary effects like slow, freeze, and coin rain.

Interactions:

- Contains `Car`, `Background`, `Obstacle`, `Coin`, and `PowerUp` objects.

### ATMCity

Utility room for balance checking and cash-to-token conversion.

Responsibilities:

- Shows machine, balance, and withdraw screens.
- Displays current cash or tokens.
- Converts all cash into tokens at a fixed ratio when withdraw is used.

Interactions:

- Modifies the shared `Cap` profile directly.
- Sends conversion feedback through `MessageManager`.

### EarnCity

Quiz-based cash reward room.

Responsibilities:

- Loads questions from CSV.
- Shows a question/answer flow with correct and incorrect feedback screens.
- Applies cash reward or cash penalty based on answer choice.
- Tracks answer streaks.

Interactions:

- Mutates the shared `Cap` cash value.
- Reads question data from `assets/earnCity/earn_questions.csv`.

### EnergyCity

Resource-gathering room for energy recovery.

Responsibilities:

- Spawns falling items and a movable basket.
- Increases player energy when fruit or cap items are collected.
- Displays the player’s energy bar.

Interactions:

- Mutates the shared `Cap` energy value.

### SpaceShooter

Arcade shooter mini-game.

Responsibilities:

- Manages player movement, lasers, meteors, power-ups, explosions, score, timers, game modes, and return-to-lobby behavior.
- Selects between three modes: time survival, laser limit, and freestyle without power-ups.
- Uses audio and visual assets for the shooter scene.

Interactions:

- Uses the sprite hierarchy in `sprites.h`.
- Uses `Timer` for meteor spawning, power-up spawning, and timed game-over logic.
- Returns a boolean signal to the top-level controller when the player wants to leave the scene.

### Sprite Hierarchy and Timer

Shared runtime support for Space Shooter.

Responsibilities:

- `Sprite` is the base class for moving, discardable game objects.
- `Player`, `Laser`, `Meteor`, `ExplosionAnimation`, and `PoweredUp` extend `Sprite` behavior.
- `TripleShot` and `Shield` implement specific power-up effects.
- `Timer` provides time-based callbacks and repeating or one-shot behavior.

## 4. Class-Level Breakdown

### CapTaleSystem

Purpose: top-level game state controller.

Key attributes:

- `state`: current scene.
- `player`: shared `Cap` instance.
- `messages`: global message manager.
- `customCity`, `lobby`, `pongCity`, `carCity`, `atmCity`, `spaceShooter`, `earnCity`, `energyCity`: scene objects.
- `gameOver`, `gameManual`: textures for static screens.
- `enterPong`: one-time initialization flag for Pong City.

Key methods:

- `loadData()`: reads `characterData.txt` and restores player state.
- `saveData()`: writes the current player state to disk.
- `runGame()`: frame loop.
- `update()`: state-specific input and scene transitions.
- `draw()`: state-specific rendering.

Relationships:

- Owns all top-level scenes and shares the `Cap` object across them.

### Cap

Purpose: the persistent player profile and movement entity.

Key attributes:

- `texture`, `pos`, `speed`, `direction`, `size`.
- `tokens`, `cash`, `energy`, `gameOver`.
- `collisionRadius`, `stepsMoved`, `stepsPerDrain`, `energyPerDrain`.

Key methods:

- `input()`, `move()`, `constraint()`, `update()`, `draw()`.
- Resource mutators: `addTokens()`, `removeTokens()`, `addCash()`, `removeCash()`, `increaseEnergy()`, `decreaseEnergy()`.
- Accessors: `getCash()`, `getTokens()`, `getEnergy()`, `getGameOver()`.

Relationships:

- Used by the lobby and most city modules as the shared player state.

### Lobby

Purpose: hub map and entry gate to cities.

Key attributes:

- `background`, `menuIcon`.
- `player` pointer.
- `rooms` grid of `Room*`.
- `map` layout of room states.
- `currentState`, `roomName`, `messages`, `energyWarning`, `font`.

Key methods:

- `draw()`: renders lobby UI and room grid.
- `update()`: handles movement, entry checks, energy warning, and menu button.
- `checkContains()`: detects current room overlap.
- `CheckRectangleContainsRec()`: rectangle containment helper.

Relationships:

- Queries `Cap` position and resources.
- Sends scene transitions back to `CapTaleSystem` as enum values.

### Room

Purpose: single lobby tile.

Key attributes:

- `pos`, `size`, `rec`, `gameState`.

Key methods:

- `drawRoom()`, `getCenter()`, `getRec()`.

Relationships:

- Owned by `Lobby`.

### Message / MessageManager

Purpose: lightweight notification queue.

Key attributes:

- `Message`: text, size, visibility, timer.
- `MessageManager`: `messages` array, `numMessages`, `startPos`, `font`.

Key methods:

- `Message::update()`, `Message::draw()`, `Message::isVisible()`, `Message::getHeight()`.
- `MessageManager::addMessage()`, `removeMessage()`, `update()`, `draw()`.

Relationships:

- Shared by `Lobby` and `ATMCity` to show temporary user feedback.

### CustomCity

Purpose: cap selection screen.

Key attributes:

- `capsList[4]`, `capsCount`, `selectedIndex`, `background`.

Key methods:

- `draw()`, `getSelectedCap()`, `nextCap()`, `prevCap()`.

Relationships:

- The selected index is read by `CapTaleSystem` when starting or saving the game.

### PongCity

Purpose: pong mini-game scene.

Key attributes:

- `ball`, `player`, `ai`, `player1`, `player2`.
- `isPaused`, `pongState`, `enemyState`.
- Scene textures such as `menu`, `enemyMenu`, `humanBg`, `karachi`, `newYork`, `rome`, `win`, `lose`, `pause`, `humanPause`.
- Shared score and level globals defined at file scope.

Key methods:

- `draw()`, `update()`, `enemyUpdate()`, `unloadTextures()`.

Relationships:

- Contains the ball and paddles directly.
- Uses the current level to alter scoring targets and movement speed.

### Ball

Purpose: game ball for Pong.

Key attributes:

- Position, radius, speed components.

Key methods:

- `draw()`, `update()`, `resetBall()`, `InvertSpeedX()`, getters.

Relationships:

- Collides with paddles and updates global score variables.

### Paddle, AiPaddle, player1Paddle, player2Paddle

Purpose: Pong paddles.

Key attributes:

- Base paddle stores position, size, and speed.

Key methods:

- `draw()`, `update()`, getters, `setSpeed()`.
- `AiPaddle::update(ballY)` follows the ball vertically.

Relationships:

- Controlled by keyboard or AI depending on mode.

### CarCity

Purpose: endless runner mini-game.

Key attributes:

- `car`, `bg`, textures for obstacles and power-ups.
- Arrays of `Obstacle`, `Coin`, and `PowerUp`.
- Timers and state counters for spawning, score, difficulty, pause, and effects.

Key methods:

- `update(float d)`, `draw() const`, `Restart()`.

Relationships:

- Owns the player car and spawned hazards.
- Uses keyboard input directly for movement and restart.

### Car, Obstacle, Coin, PowerUp, Background

Purpose: supporting entities for Car City.

Key methods:

- `Car::Update()`, `Car::Draw()`, `Car::GetRect()`, `Car::Unload()`.
- `Obstacle::Spawn()`, `Update()`, `Draw()`.
- `Coin::Spawn()`, `Update()`, `Draw()`.
- `PowerUp::Spawn()`, `Update()`, `Draw()`.
- `Background::Update()`, `Draw()`, `Unload()`.

Relationships:

- `CarCity` composes all of them and drives their lifecycle.

### ATMCity

Purpose: cash/token utility room.

Key attributes:

- `player`, `messages`, `machine`, `balance`, `withdraw`, `atmState`.

Key methods:

- `draw()`, `update()`.

Relationships:

- Reads and mutates the shared `Cap` cash and token values.

### EarnCity and Question

Purpose: quiz-driven cash reward room.

Key attributes:

- `Question`: question text, four options, correct option, reward, penalty.
- `EarnCity`: `player`, `questions`, `currentQuestion`, `numQuestions`, `font`, `currentState`, scene textures, `streak`, `streakComments`.

Key methods:

- `Question` getters and `answer()`.
- `EarnCity::draw()`, `reset()`, `updateQuestion()`, `update()`.

Relationships:

- `EarnCity` loads questions from CSV and updates player cash.

### EnergyCity, Item, Basket

Purpose: energy recovery room.

Key attributes:

- `EnergyCity`: `player`, `background`, `items`, `basket`, `spawnTimer`, `currentItem`, `energyCount`, `font`.
- `Item`: position, texture, speed, active flag, type string.
- `Basket`: position, texture, speed.

Key methods:

- `Item::initialize()`, `update()`, `draw()`, `spawn()`, `isActive()`, `checkCollision()`, `getType()`.
- `Basket::update()`, `draw()`, `getRect()`.
- `EnergyCity::draw()`, `update()`.

Relationships:

- Colliding items increase the shared player energy.

### SpaceShooter

Purpose: arcade shooter scene with progress tracking and timers.

Key attributes:

- Assets, sounds, music, and font.
- `player`, `stars`, dynamic arrays for `lasers`, `meteors`, `explosions`, and `powerUps`.
- `meteorTimer`, `powerUpTimer`, `overTimer`.
- Score and mode state such as `selectedGameMode`, `selected`, `gameOver`, `timeSurvived`, `returnToLobby`.

Key methods:

- Asset loading and creation helpers: `importAssets()`, `createPowerUp()`, `shootLaser()`, `createMeteor()`, `createExplosion()`.
- Lifecycle helpers: `checkCollisions()`, `discardSprites()`, `drawScore()`, `updateGameOver()`, `restart()`.
- Main scene methods: `update()`, `draw()`, `run()`.

Relationships:

- Composes the sprite hierarchy from `sprites.h`.
- Uses `Timer` to spawn enemies and time-limited effects.

### Sprite, Player, Laser, Meteor, ExplosionAnimation, PoweredUp, TripleShot, Shield

Purpose: reusable object model for Space Shooter.

Key attributes:

- `Sprite`: texture, position, speed, direction, size, discard flag, collision radius.
- `Player`: power-up flags and timers, callback for shooting.
- `Laser`: rotation.
- `Meteor`: rotation and hit rectangle.
- `ExplosionAnimation`: frame textures and animation state.
- `PoweredUp`: player pointer, activation state, discard timer.

Key methods:

- `Sprite::move()`, `update()`, `draw()`, discard and collision helpers.
- `Player::update()`, `draw()`, power-up toggles.
- `Laser::draw()`, `getRect()`.
- `Meteor::update()`, `draw()`, `getCenter()`.
- `ExplosionAnimation::update()`, `draw()`.
- `PoweredUp::applyPower()` in derived classes.
- `TripleShot::applyPower()`, `Shield::applyPower()`.

Relationships:

- `Player` emits lasers through a callback into `SpaceShooter`.
- `TripleShot` and `Shield` modify the `Player` state.

### Timer

Purpose: time-based callback helper.

Key attributes:

- `duration`, `startTime`, `active`, `repeat`, `func`.

Key methods:

- `activate()`, `deactivate()`, `update()`, `getTimePassed()`.

Relationships:

- Used by `Message`, `SpaceShooter`, and power-up discard logic.

## 5. Architecture Style

CapTale uses a modular, scene-based, game-loop architecture. The top-level controller runs a single frame loop and switches among self-contained scenes using a finite state machine. Each city module owns its own assets and logic, while shared player resources are passed through the `Cap` object.

The design is layered at a coarse level:

- Application shell: `CapTaleSystem`.
- Shared domain objects: `Cap`, `MessageManager`, `Timer`.
- Scene modules: lobby and city mini-games.
- Scene-local entities: pong paddles, car obstacles, space shooter sprites, quiz questions, and basket items.

## 6. Game Flow

1. Program starts in `main()`.
2. Raylib window and FPS are initialized.
3. The game loads base textures and creates `CapTaleSystem`.
4. `CapTaleSystem` tries to load `characterData.txt`.
5. If data loads successfully, the game starts in the lobby; otherwise it starts in custom city for cap selection.
6. The frame loop runs until the window closes.
7. In each frame, the current state is updated and then drawn.
8. The player moves through the lobby using keyboard input.
9. Entering a room transitions to the relevant city or mini-game.
10. The player can return to the lobby from most cities with `L`.
11. If player energy reaches the minimum threshold, the `Cap` object marks game over.
12. The game over screen is shown and the final state is saved on shutdown.
13. Exiting the window triggers destructors and `characterData.txt` is written.

## 7. Key Scenarios

### Entering a city

The player moves the cap around the lobby. `Lobby::checkContains()` detects when the cap rectangle is fully inside a room. If the player presses `Enter`, `Lobby::update()` either deducts the required token cost or blocks entry with a message. On success, the room state is returned to `CapTaleSystem`, which switches to the matching city scene.

### Playing a mini-game

Each city owns its own loop and input rules. For example, Pong City switches between enemy selection, gameplay, pause, and win/lose states. Space Shooter has its own selection screen, timed enemy spawning, collision checks, score tracking, and a return-to-lobby signal. The top-level system simply forwards update/draw control to the active city.

### Managing tokens and energy

Tokens and energy are shared through the `Cap` object. Tokens are spent in the lobby to enter certain rooms, added in ATM City when cash is converted, and shown in the lobby UI. Energy decreases as the player moves around the lobby and increases in Energy City by catching items. When energy is low, the lobby emits a warning message; if energy reaches the minimum threshold, the player is flagged as game over.

## 8. Data Flow

- Startup data flow: `characterData.txt` -> `CapTaleSystem::loadData()` -> `Cap` object -> lobby and cities.
- Selection data flow: `CustomCity` selected cap index -> `CapTaleSystem` -> player texture and saved profile.
- Lobby flow: `Cap` position and resources -> `Lobby` collision checks and token gating -> state transition back to `CapTaleSystem`.
- Mini-game flow: scene-specific controllers mutate local state and sometimes the shared `Cap` resources.
- Message flow: scene logic -> `MessageManager` -> on-screen notification queue.
- Space Shooter flow: player input -> `Player` -> callback into `SpaceShooter::shootLaser()` -> dynamic sprite arrays -> collision and score updates.
- Earn City flow: CSV question file -> `EarnCity` question pool -> answer input -> `Cap` cash mutation.
- ATM flow: `Cap` cash -> ATM conversion -> `Cap` tokens and cash update.
- Energy flow: `EnergyCity` item collisions -> `Cap` energy increase.
- Shutdown flow: `Cap` state -> `CapTaleSystem::saveData()` -> `characterData.txt`.

## 9. Resource Management

### Tokens

- Stored directly on `Cap` as an integer.
- Decreased in the lobby when entering token-gated rooms.
- Increased in ATM City by converting all cash at a 50-to-1 rate.
- Displayed in the lobby as available tokens.

### Energy

- Stored directly on `Cap` as an integer.
- Initialized from saved data or defaulted to 20 when a loaded value is zero.
- Decreased by movement in `Cap::move()` after enough steps are taken.
- Increased in Energy City when items are collected.
- Displayed as a progress bar in the lobby and Energy City.
- When the value reaches the minimum threshold, the player is considered game over.

### Cash

- Stored directly on `Cap` as an integer.
- Set from save data or the initial cash constant.
- Increased by correct Earn City answers.
- Decreased by incorrect Earn City answers.
- Converted into tokens in ATM City.

### Scene-local resources

- Pong City keeps score and level variables in file scope.
- Car City tracks score, coins collected, effects, and difficulty progression.
- Space Shooter tracks score, lasers used, power-up counters, and time survived.

## 10. External Dependencies

- Raylib: rendering, textures, audio, input, collision checks, timing, and window management.
- C++ standard library: file I/O, strings, exceptions, dynamic allocation, random utilities, and functional callbacks.
- Asset files: PNG textures, OTF/TTF fonts, WAV/MP3 audio, and the Earn City CSV question bank.

Raylib is the core runtime dependency. It supplies the window, drawing, audio, keyboard and mouse input, collision helpers, and frame timing used throughout the game. The standard library provides persistence, scene data loading, and the callback/timer utility code.