#include "MainGame.h"

#include <iostream>
#include <string>

#include <Bengine/Errors.h>
#include <Bengine/ResourceManager.h>


MainGame::MainGame() :  _screenWidth(1024), _screenHeight(768), _mouseHeight(0), _gameState(GameState::PLAY), _time(0.0f), _maxFPS(100)
{
	_camera.init(_screenWidth, _screenHeight);
}
MainGame::~MainGame()
{
}


void MainGame::run()
{
	initSystems();



	gameLoop();
}


void MainGame::initSystems()
{
	Bengine::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, Bengine::NONE);
	
	initShaders();

	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);

}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPositon");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();

		float startTicks = SDL_GetTicks(); // frame time measuring

		processInput();
		_time += 0.01;

		_camera.update();

		//update all projectiles
		for (int i = 0; i < _projectiles.size();)
		{
			if (_projectiles[i].update() == true)
			{
				_projectiles[i] = _projectiles.back();
				_projectiles.pop_back();
			}
			else i++; //only increment if we didn't delete a projectile
		}

		drawGame();

		_fps = _fpsLimiter.end();

		//print every 10 frames
		static int frameCount = 0;
		frameCount++;
		if (frameCount % 10000 == 0)
		{
			std::cout << _fps << std::endl;
			frameCount = 0;
		}

	}

}
void MainGame::processInput()
{
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	//I offloaded the processing of events to the input manager class
	if (_inputManager.setInputEvents() == 1) _gameState = GameState::EXIT;
	
	if (_inputManager.isKeyPressed(SDLK_w)) _camera.setPosition(_camera.getPosition() + glm::vec2(0.0, CAMERA_SPEED));
	if (_inputManager.isKeyPressed(SDLK_s)) _camera.setPosition(_camera.getPosition() + glm::vec2(0.0, -CAMERA_SPEED));
	if (_inputManager.isKeyPressed(SDLK_a)) _camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0));
	if (_inputManager.isKeyPressed(SDLK_d)) _camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0));
	if (_inputManager.isKeyPressed(SDLK_q)) _camera.setScale(_camera.getScale() + SCALE_SPEED);
	if (_inputManager.isKeyPressed(SDLK_e)) _camera.setScale(_camera.getScale() - SCALE_SPEED);
	if (_inputManager.isKeyPressed(SDLK_ESCAPE)) _gameState = GameState::EXIT;
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		
		glm::vec2 playerPosition(0.0f);

		glm::vec2 direction = mouseCoords - playerPosition;

		direction = glm::normalize(direction);

		_projectiles.emplace_back(playerPosition , direction, 20.0f, 20);
	}

}

void MainGame::drawGame()
{

	_colorProgram.preClear(); //clear the buffers

	_colorProgram.use(); //enable the current program

	_colorProgram.addTextureUniform("mySampler"); //set the texture uniform variable
	
	_colorProgram.addCameraUniformVariable("P", _camera.getCameraMatrix());//set the camera matrix

	

	_spriteBatch.begin(); //begin the spritebatch

	glm::vec4 pos(0.0f, 0.0f, 100.0f, 100.0f);

	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/JimmyJump/PNG/CharacterRight_Standing.png");


	_spriteBatch.draw(pos, texture.id, 0.0f); //draw the main character


	for (int i = 0; i < _projectiles.size(); i++)
	{
		_projectiles[i].draw(_spriteBatch); //draw any projectiles
	}



	_spriteBatch.end(); //end the spritebatch
	_spriteBatch.renderBatch(); //render the sprites to the screen

	_colorProgram.unuse(); //disable the current color program and unbind the texture
	
	_window.swapBuffer();
}

