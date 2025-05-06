#pragma once
#include <glm/glm.hpp>

#include <Bengine/SpriteBatch.h>

class Projectile
{

public:
	Projectile(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);

	void draw(Bengine::SpriteBatch& spriteBatch);
	bool update(); //returns true when we are out of life

private:

	int _lifeTime;
	float _speed;

	glm::vec2 _direction;

	glm::vec2 _position;



};

