# Software Design Diagram Inputs for CapTale

## 1. Use Case Inputs

Supported user actions:

- Move the player avatar in the lobby and in movement-based mini-games.
- Select a cap appearance in Custom City.
- Enter a lobby room/city when standing inside a valid room.
- Press `L` to return to the lobby from most city scenes.
- Press `Enter` to confirm entry into a room.
- Open the game manual from the lobby menu icon.
- Play Pong as either AI opponent mode or human-vs-human mode.
- Pause and resume Pong gameplay.
- Restart or replay Pong after win/lose states.
- Play Car City by moving left and right.
- Restart Car City after game over.
- Play ATM City by checking balance or converting cash to tokens.
- Play Earn City by answering quiz questions or skipping a question.
- Play Energy City by moving the basket to collect falling items.
- Play Space Shooter by moving, shooting, selecting modes, and returning to the lobby.
- Save and restore the player profile between sessions.

## 2. Class Diagram Inputs

### Major classes

- `CapTaleSystem`
- `Cap`
- `Lobby`
- `Room`
- `Message`
- `MessageManager`
- `CustomCity`
- `PongCity`
- `Ball`
- `Paddle`
- `AiPaddle`
- `player1Paddle`
- `player2Paddle`
- `CarCity`
- `Car`
- `Obstacle`
- `Coin`
- `PowerUp`
- `Background`
- `ATMCity`
- `EarnCity`
- `Question`
- `EnergyCity`
- `Item`
- `Basket`
- `SpaceShooter`
- `Sprite`
- `Player`
- `Laser`
- `Meteor`
- `ExplosionAnimation`
- `PoweredUp`
- `TripleShot`
- `Shield`
- `Timer`

### Suggested inheritance and grouping

- `PongCity`, `CarCity`, `ATMCity`, `EarnCity`, `EnergyCity`, `SpaceShooter`, and `CustomCity` can be treated as scene modules under a common conceptual `CityScene` or `GameScene` base in a design diagram, even though the code does not define such a base class.
- `AiPaddle`, `player1Paddle`, and `player2Paddle` inherit from `Paddle`.
- `Player`, `Laser`, `Meteor`, `ExplosionAnimation`, and `PoweredUp` inherit from `Sprite`.
- `TripleShot` and `Shield` inherit from `PoweredUp`.
- `MessageManager` aggregates `Message` objects.
- `Lobby` aggregates `Room` objects.
- `SpaceShooter` composes `Player`, `Laser`, `Meteor`, `ExplosionAnimation`, and `PoweredUp` objects.
- `CarCity` composes `Car`, `Background`, `Obstacle`, `Coin`, and `PowerUp` objects.
- `EnergyCity` composes `Item` and `Basket` objects.

### Associations, aggregations, dependencies

- `CapTaleSystem` depends on every scene module and owns the top-level application flow.
- `CapTaleSystem` aggregates one shared `Cap` object and one `MessageManager`.
- `Lobby` depends on `Cap` for position, tokens, and energy.
- `Lobby` depends on `MessageManager` for warnings and blocked-entry feedback.
- `ATMCity` depends on `Cap` and `MessageManager`.
- `EarnCity` depends on `Cap` and quiz data loaded from CSV.
- `EnergyCity` depends on `Cap`.
- `PongCity` depends on its ball and paddle objects and on global score/level variables in the current implementation.
- `SpaceShooter` depends on `Timer`, raylib audio, and the sprite hierarchy.
- `Message` depends on `Timer` for expiry.

## 3. Sequence Diagram Inputs

### Player entering a city

1. Player moves the cap in the lobby.
2. `Cap` updates its position and energy drain logic.
3. `Lobby` checks whether the cap rectangle is fully inside a room.
4. If the room is valid and the player presses `Enter`, `Lobby` checks token requirements.
5. If enough tokens exist, the required tokens are removed from `Cap`.
6. `Lobby` returns the room’s state to `CapTaleSystem`.
7. `CapTaleSystem` switches to the corresponding city scene.
8. The selected city begins processing its own update/draw loop.

### Player playing a game and updating tokens/energy

1. Player enters a city scene from the lobby.
2. The active city runs its own update logic each frame.
3. Mini-game input is processed according to that scene’s rules.
4. The scene may modify `Cap` resources directly.
5. Examples of resource updates:
   - ATM City converts cash into tokens.
   - Earn City adds or removes cash based on answers.
   - Energy City increases energy on item collection.
   - Lobby movement decreases energy over time.
6. The UI redraws resource values such as tokens, cash, energy, or score.
7. If a scene signals completion or return, control goes back to the lobby.

## 4. Deployment Inputs

Decision: a deployment diagram is needed.

Reason:

- CapTale is not just source code structure; it has runtime packaging concerns (executable + runtime DLLs + local assets + save file I/O).
- The system has critical physical/runtime dependencies worth modeling: OS, Raylib runtime, filesystem artifacts, and player I/O path.

Deployment scope (project-specific):

- One local node only: `Windows Desktop PC <<device>>`.
- Inside that node: `Windows OS + Filesystem <<executionEnvironment>>`.
- Deployed software artifacts/components:
   - `CapTale.exe <<artifact>>` (main process).
   - `Raylib + C++ runtime DLLs <<library>>` (graphics/audio/input backend dependency).
   - `Assets bundle <<artifact>>` (textures, fonts, audio, CSV).
   - `characterData.txt <<artifact>>` (persistent profile store).

Critical communication associations (label with protocol/stereotype):

- `Player -> CapTale.exe`: `<<keyboard/mouse input + display/audio output>>`.
- `CapTale.exe -> Raylib runtime`: `<<in-process API calls>>`.
- `CapTale.exe -> Assets bundle`: `<<filesystem read>>`.
- `CapTale.exe -> characterData.txt`: `<<filesystem read/write>>`.
- `CapTale.exe -> Windows OS`: `<<Win32/OpenGL/audio driver services>>`.

What to intentionally exclude:

- No backend server node.
- No database server node.
- No network protocol links (HTTP/TCP/etc.), because the repository shows fully local execution.

## 5. 4+1 View Inputs

### Logical View

- `CapTaleSystem` is the top-level controller.
- `Cap` is the shared player state.
- `Lobby` is the hub that routes to scene modules.
- Each city module handles one gameplay domain.
- `MessageManager` provides shared notification support.
- `Timer` supports time-based behavior in messaging and Space Shooter.

### Development View

- `main.cpp` contains the application shell and state machine.
- `headers/cap.h` contains the shared player object.
- `headers/lobby.h` contains lobby logic and room detection.
- `headers/message.h` contains the message subsystem.
- `headers/customCity.h` contains cap selection.
- `headers/pongCity.h` contains Pong gameplay.
- `headers/carCity.h` contains Car City gameplay.
- `headers/atmCity.h` contains ATM logic.
- `headers/earningCity.h` contains Earn City quiz logic.
- `headers/energyCity.h` contains Energy City logic.
- `headers/spaceShooter.h` and `headers/sprites.h` contain Space Shooter and its entity hierarchy.
- `headers/custom_timer.h` contains the reusable timer utility.
- `headers/settings.h` and `headers/spaceShooterSettings.h` define constants.

### Process View

- The system runs a single frame-based game loop.
- Each frame performs input handling, state updates, and rendering.
- `CapTaleSystem` dispatches behavior based on the current scene state.
- Scene modules may run their own internal update logic, timers, and collision checks.
- Raylib handles real-time keyboard, mouse, drawing, and audio operations.

### Physical View

- Execution environment is a Windows desktop machine.
- The game runs as a native C++ executable.
- Raylib provides the graphics, audio, and windowing layer.
- Assets are loaded from the local filesystem relative to the project folder.
- Save data is read from and written to `characterData.txt` in the project root.

### Scenarios

- Entering a city from the lobby.
- Playing Pong, Car City, Space Shooter, Earn City, ATM City, or Energy City.
- Managing tokens through room entry and ATM conversion.
- Managing energy through movement drain and Energy City item collection.
- Saving progress on exit and restoring progress on the next run.