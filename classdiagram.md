# CapTale Class Diagram Specification

---

## 1. Core System Classes

```
CapTaleSystem
Cap
Lobby
Room
MessageManager
Message
CustomCity
PongCity
CarCity
ATMCity
EarnCity
EnergyCity
SpaceShooter
Timer
CityScene (conceptual only)
```

---

## 2. Inheritance Structure

### City Modules

```
CityScene <|-- CustomCity
CityScene <|-- PongCity
CityScene <|-- CarCity
CityScene <|-- ATMCity
CityScene <|-- EarnCity
CityScene <|-- EnergyCity
CityScene <|-- SpaceShooter
```

### Paddle Hierarchy

```
Paddle <|-- AiPaddle
Paddle <|-- player1Paddle
Paddle <|-- player2Paddle
```

### Sprite System

```
Sprite <|-- Player (shooter)
Sprite <|-- Laser
Sprite <|-- Meteor
Sprite <|-- PoweredUp
Sprite <|-- ExplosionAnimation (optional)
```

### PowerUps

```
PoweredUp <|-- TripleShot
PoweredUp <|-- Shield
```

---

## 3. CapTaleSystem Attributes

* state
* player
* messages
* customCity
* lobby
* pongCity
* carCity
* atmCity
* spaceShooter
* earnCity
* energyCity
* enterPong

---

## 4. Cap Class

* pos
* speed
* direction
* size
* tokens
* cash
* energy
* gameOver

---

## 5. Lobby & Room

### Lobby

* player
* rooms[21]
* currentState
* messages
* energyWarning

### Room

* rec
* gameState

---

## 6. Messaging System

### MessageManager

* messages
* numMessages
* font

### Message

* text
* size
* message_timer
* visible

---

## 7. CustomCity

* capsList[4]
* selectedIndex
* background

---

## 8. PongCity

### PongCity

* ball
* player
* ai
* player1
* player2
* pongState
* enemyState
* isPaused

### Ball

* x, y, radius
* speedX, speedY

### Paddle

* x, y
* width, height
* speed

---

## 9. CarCity

### CarCity

* car
* bg
* obs[100]
* coins[200]
* pups[20]
* score
* gameOver
* paused

### Car

* position
* speed
* frozen

### Obstacle

* rect, isBig, active

### Coin

* rect, active

### PowerUp

* rect, active, type

### Background

* scrollY

---

## 10. ATMCity

* player
* messages
* atmState

---

## 11. EarnCity

* player
* questions
* currentQuestion
* numQuestions
* currentState
* streak

### Question

* question
* options[4]
* correctOption
* cashReward
* penalty

---

## 12. EnergyCity

* player
* items
* basket
* spawnTimer

### Item

* position
* speed
* active
* type

### Basket

* position
* speed

---

## 13. SpaceShooter

* player
* lasers
* meteors
* explosions
* powerUps
* selectedGameMode
* selected
* gameOver
* returnToLobby
* score
* meteorTimer
* powerUpTimer
* overTimer

### Sprite Base

* texture
* pos
* speed
* direction
* size
* discard
* collisionRadius

### Player (Shooter)

* tripleShot
* shield
* tripleShotTimer
* shieldTimer

### Laser

* rotation

### Meteor

* rotation

### ExplosionAnimation

* index
* discard

### PoweredUp

* player
* activated
* discardTimer

---

## 14. Timer

* duration
* startTime
* active
* repeat
* func

---

## 15. Relationships (Multiplicities)

### System Core

* CapTaleSystem 1—1 Cap
* CapTaleSystem 1—1 Lobby
* CapTaleSystem 1—1 MessageManager
* CapTaleSystem 1—1 each City module

### Lobby

* Lobby 1—1 Cap
* Lobby 1—1 MessageManager
* Lobby 1—21 Room

### Messages

* MessageManager 1—0..* Message
* Message 1—1 Timer

### Cities

* ATMCity 1—1 Cap, MessageManager
* EarnCity 1—1 Cap, 0..* Question
* EnergyCity 1—1 Cap, 9 Item, 1 Basket

### PongCity

* 1 Ball
* 1 Paddle
* 1 AiPaddle
* 1 player1Paddle
* 1 player2Paddle

### CarCity

* 1 Car
* 1 Background
* 0..100 Obstacle
* 0..200 Coin
* 0..20 PowerUp

### SpaceShooter

* 1 Player
* 0..* Laser
* 0..* Meteor
* 0..* ExplosionAnimation
* 0..* PoweredUp
* 3 Timer

---

## Implemented Mermaid Class Diagrams

### Diagram 1 — Core Orchestration

```mermaid
classDiagram
	class CityScene {
		<<abstract>>
		+update()
		+draw()
	}

	note for CityScene "Conceptual only — not in C++ source"

	class CapTaleState {
		<<enumeration>>
		LOBBY
		PONG_CITY
		CAR_CITY
		ATM_CITY
		EARN_CITY
		ENERGY_CITY
		SPACE_SHOOTER
		CUSTOM_CITY
		GAME_MANUAL
	}

	class RoomState {
		<<enumeration>>
		EMPTY
		PONG
		CAR
		ATM
		EARN
		ENERGY
		SPACESHOOTER
		CUSTOM
	}

	class CapTaleSystem {
		-state
		-player
		-messages
		-customCity
		-lobby
		-pongCity
		-carCity
		-atmCity
		-spaceShooter
		-earnCity
		-energyCity
		-enterPong
		+loadData()
		+saveData()
		+runGame()
		+update()
		+draw()
	}

	class Cap {
		-pos
		-speed
		-direction
		-size
		-tokens
		-cash
		-energy
		-gameOver
		+update()
		+draw()
	}

	class Lobby {
		-player
		-rooms[21]
		-currentState
		-messages
		-energyWarning
		+update()
		+draw()
		+checkContains()
	}

	class Room {
		-rec
		-gameState
		+drawRoom()
	}

	class MessageManager {
		-messages
		-numMessages
		-font
		+addMessage()
		+removeMessage()
		+update()
		+draw()
	}

	class Message {
		-text
		-size
		-message_timer
		-visible
		+update()
		+draw()
	}

	class Timer {
		-duration
		-startTime
		-active
		-repeat
		-func
		+update()
	}

	class CustomCity {
		-capsList[4]
		-selectedIndex
		-background
		+draw()
	}

	class PongCity {
		-ball
		-player
		-ai
		-player1
		-player2
		-pongState
		-enemyState
		-isPaused
		+update()
		+draw()
	}

	class CarCity {
		-car
		-bg
		-obs[100]
		-coins[200]
		-pups[20]
		-score
		-gameOver
		-paused
		+update()
		+draw()
	}

	class ATMCity {
		-player
		-messages
		-atmState
		+update()
		+draw()
	}

	class EarnCity {
		-player
		-questions
		-currentQuestion
		-numQuestions
		-currentState
		-streak
		+update()
		+draw()
	}

	class EnergyCity {
		-player
		-items
		-basket
		-spawnTimer
		+update()
		+draw()
	}

	class SpaceShooter {
		-player
		-lasers
		-meteors
		-powerUps
		-score
		+update()
		+draw()
	}

	CityScene <|-- CustomCity
	CityScene <|-- PongCity
	CityScene <|-- CarCity
	CityScene <|-- ATMCity
	CityScene <|-- EarnCity
	CityScene <|-- EnergyCity
	CityScene <|-- SpaceShooter

	CapTaleSystem "1" *-- "1" Cap
	CapTaleSystem "1" *-- "1" MessageManager
	CapTaleSystem "1" *-- "1" Lobby
	CapTaleSystem "1" *-- "1" CustomCity
	CapTaleSystem "1" *-- "1" PongCity
	CapTaleSystem "1" *-- "1" CarCity
	CapTaleSystem "1" *-- "1" ATMCity
	CapTaleSystem "1" *-- "1" EarnCity
	CapTaleSystem "1" *-- "1" EnergyCity
	CapTaleSystem "1" *-- "1" SpaceShooter

	Lobby "1" *-- "21" Room
	Lobby "1" --> "1" Cap : <<use>>
	Lobby "1" --> "1" MessageManager : <<use>>

	MessageManager "1" *-- "0..*" Message
	Message "1" --> "1" Timer

	CapTaleSystem --> CapTaleState
	Room --> RoomState
```

This layer captures top-level orchestration, and the most important structural decision is modeling `CapTaleSystem` as the owner of all shared runtime modules.

### Diagram 2 — City Internals

```mermaid
classDiagram
	class CustomCity {
		-capsList[4]
		-selectedIndex
		-background
		+draw()
		+nextCap()
		+prevCap()
	}

	class PongState {
		<<enumeration>>
		GAME_ENEMY
		GAME_MENU
		GAME_PLAYING
		GAME_WIN
		GAME_LOSE
	}

	class PongCity {
		-ball
		-player
		-ai
		-player1
		-player2
		-pongState
		-enemyState
		-isPaused
		+update()
		+draw()
	}

	class Ball {
		-x
		-y
		-radius
		-speedX
		-speedY
		+update()
		+draw()
	}

	class Paddle {
		-x
		-y
		-width
		-height
		-speed
		+update()
		+draw()
	}

	class AiPaddle {
		+update()
	}

	class Player1Paddle {
		+update()
	}

	class Player2Paddle {
		+update()
	}

	class CarCity {
		-car
		-bg
		-obs[100]
		-coins[200]
		-pups[20]
		-score
		-gameOver
		-paused
		+update()
		+draw()
	}

	class Car {
		-position
		-speed
		-frozen
		+update()
		+draw()
	}

	class Obstacle {
		-rect
		-isBig
		-active
		+update()
		+draw()
	}

	class Coin {
		-rect
		-active
		+update()
		+draw()
	}

	class CarPowerUp {
		-rect
		-active
		-type
		+update()
		+draw()
	}

	class Background {
		-scrollY
		+update()
		+draw()
	}

	class ATMState {
		<<enumeration>>
		MACHINE
		WITHDRAW
		BALANCE
	}

	class ATMCity {
		-player
		-messages
		-atmState
		+update()
		+draw()
	}

	class EarnCity {
		-player
		-questions
		-currentQuestion
		-numQuestions
		-currentState
		-streak
		+update()
		+draw()
	}

	class Question {
		-question
		-options[4]
		-correctOption
		-cashReward
		-penalty
		+draw()
	}

	class QuestionsCsv {
		<<datafile>>
		-questions.csv
	}

	class EnergyCity {
		-player
		-items
		-basket
		-spawnTimer
		+update()
		+draw()
	}

	class Item {
		-position
		-speed
		-active
		-type
		+update()
		+draw()
	}

	class Basket {
		-position
		-speed
		+update()
		+draw()
	}

	class Cap {
		-tokens
		-cash
		-energy
		+addCash()
		+removeTokens()
		+increaseEnergy()
	}

	class MessageManager {
		-messages
		-numMessages
		-font
		+addMessage()
	}

	PongCity "1" *-- "1" Ball
	PongCity "1" *-- "1" Paddle
	PongCity "1" *-- "1" AiPaddle
	PongCity "1" *-- "1" Player1Paddle
	PongCity "1" *-- "1" Player2Paddle
	PongCity --> PongState

	Paddle <|-- AiPaddle
	Paddle <|-- Player1Paddle
	Paddle <|-- Player2Paddle

	CarCity "1" *-- "1" Car
	CarCity "1" *-- "1" Background
	CarCity "1" o-- "0..100" Obstacle
	CarCity "1" o-- "0..200" Coin
	CarCity "1" o-- "0..20" CarPowerUp

	ATMCity --> ATMState
	ATMCity --> Cap
	ATMCity --> MessageManager

	EarnCity "1" --> "1" Cap
	EarnCity "1" *-- "0..*" Question
	EarnCity ..> QuestionsCsv : <<datafile>> questions.csv

	EnergyCity "1" --> "1" Cap
	EnergyCity "1" *-- "9" Item
	EnergyCity "1" *-- "1" Basket
```

This layer captures per-city gameplay structure, and the most important structural decision is separating bounded city compositions (Pong/Car/Energy/Earn/ATM) with exact multiplicities.

### Diagram 3 — Sprite & Shooter Subsystem

```mermaid
classDiagram
	class SpaceShooter {
		-player
		-lasers
		-meteors
		-explosions
		-powerUps
		-selectedGameMode
		-selected
		-gameOver
		-returnToLobby
		-score
		-meteorTimer
		-powerUpTimer
		-overTimer
		+update()
		+draw()
	}

	class Sprite {
		<<abstract>>
		-texture
		-pos
		-speed
		-direction
		-size
		-discard
		-collisionRadius
		+update()
		+draw()
	}

	class Player {
		-tripleShot
		-shield
		-tripleShotTimer
		-shieldTimer
		+update()
		+draw()
	}

	class Laser {
		-rotation
		+update()
		+draw()
	}

	class Meteor {
		-rotation
		+update()
		+draw()
	}

	class ExplosionAnimation {
		-index
		-discard
		+update()
		+draw()
	}

	class PoweredUp {
		-player
		-activated
		-discardTimer
		+applyPower()
		+update()
	}

	class TripleShot {
		+applyPower()
	}

	class Shield {
		+applyPower()
	}

	class Timer {
		-duration
		-startTime
		-active
		-repeat
		-func
		+update()
	}

	Sprite <|-- Player
	Sprite <|-- Laser
	Sprite <|-- Meteor
	Sprite <|-- ExplosionAnimation
	Sprite <|-- PoweredUp

	PoweredUp <|-- TripleShot
	PoweredUp <|-- Shield

	SpaceShooter "1" *-- "1" Player
	SpaceShooter "1" *-- "0..*" Laser
	SpaceShooter "1" *-- "0..*" Meteor
	SpaceShooter "1" *-- "0..*" ExplosionAnimation
	SpaceShooter "1" *-- "0..*" PoweredUp

	SpaceShooter "1" --> "1" Timer : meteorTimer
	SpaceShooter "1" --> "1" Timer : powerUpTimer
	SpaceShooter "1" --> "1" Timer : overTimer

	Player "1" --> "1" Timer : tripleShotTimer
	Player "1" --> "1" Timer : shieldTimer
	PoweredUp "1" --> "1" Timer : discardTimer

	PoweredUp --> Player : <<callback>>
```

This layer captures shooter-runtime entity composition and polymorphism, and the most important structural decision is isolating the `Sprite` inheritance chain from top-level city orchestration.
