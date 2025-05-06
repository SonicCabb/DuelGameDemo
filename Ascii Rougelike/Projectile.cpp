#include "Projectile.h"

#include <Bengine/ResourceManager.h>

Projectile::Projectile(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime)
{
	_lifeTime = lifeTime;
	_position = pos;
	_direction = dir;
	_speed = speed;
}




void Projectile::draw(Bengine::SpriteBatch& spriteBatch)
{
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/JimmyJump/PNG/CharacterRight_Standing.png");


	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);

	spriteBatch.draw(posAndSize, texture.id, 0.0f);
}

bool Projectile::update()
{
	_position += _direction * _speed;
	_lifeTime--;

	if (_lifeTime == 0) return true;
	else return false;
}