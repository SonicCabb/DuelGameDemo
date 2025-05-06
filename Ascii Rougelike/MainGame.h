#pragma once
#include<sdl/SDL.h>
#include <GL/glew.h>
#include <vector>

#include <Bengine/Bengine.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Sprite.h>
#include <Bengine/GLTexture.h>
#include <Bengine/Window.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/InputManager.h>
#include <Bengine/Timing.h>

#include "Projectile.h"

enum class GameState {PLAY, EXIT};


class MainGame
{


public:

	MainGame();
	~MainGame();

	void run();
	

private:

	void initSystems();

	void initShaders();

	void gameLoop();
	void processInput();

	void drawGame();


	Bengine::Window _window; //pointer to our sdl window

	Bengine::Camera2D _camera;

	Bengine::SpriteBatch _spriteBatch;

	Bengine::InputManager _inputManager;

	Bengine::FpsLimiter _fpsLimiter;

	int _screenWidth;
	int _screenHeight;

	std::vector<Projectile> _projectiles;


	float _mouseHeight;

	GameState _gameState;


	Bengine::GLSLProgram _colorProgram;

	float _fps;
	float _time; //keeps track of gametime
	float _maxFPS;

};

