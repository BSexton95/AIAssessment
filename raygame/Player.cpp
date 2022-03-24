#include "Player.h"
#include "InputComponent.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Transform2D.h"
#include "MazeScene.h"
#include "Wall.h"
#include "AABBCollider.h"
void Player::start()
{
	Actor::start();

	m_inputComponent = dynamic_cast<InputComponent*>(addComponent(new InputComponent()));
	m_moveComponent = dynamic_cast<MoveComponent*>(addComponent(new MoveComponent()));
	m_moveComponent->setMaxSpeed(10);
	m_spriteComponent = dynamic_cast<SpriteComponent*>(addComponent(new SpriteComponent("Images/player.png")));
	getTransform()->setScale({ Maze::TILE_SIZE, Maze::TILE_SIZE });

	setCollider(new AABBCollider(Maze::TILE_SIZE, Maze::TILE_SIZE, this));
	//Set spawn point
	//Set move speed
	//Set position clamps
}

void Player::update(float deltaTime)
{

	MathLibrary::Vector2 moveDirection = m_inputComponent->getMoveAxis();

	m_moveComponent->setVelocity(moveDirection * 200);
	Actor::update(deltaTime);
}

void Player::onCollision(Actor* other)
{
	if (Wall* wall = dynamic_cast<Wall*>(other)) {
		MathLibrary::Vector2 halfTile = { Maze::TILE_SIZE / 2.0f, Maze::TILE_SIZE / 2.0f };
		MathLibrary::Vector2 position = getTransform()->getWorldPosition();
		position = position + halfTile;
		MathLibrary::Vector2 tilePosition = {
			roundf(position.x / Maze::TILE_SIZE) * Maze::TILE_SIZE,
			roundf(position.y / Maze::TILE_SIZE) * Maze::TILE_SIZE
		};
		tilePosition = tilePosition - halfTile;
		getTransform()->setWorldPostion(getTransform()->getWorldPosition() + (m_moveComponent->getVelocity().getNormalized() * -1 * Maze::TILE_SIZE / 2.0f));

		m_moveComponent->setVelocity({ 0, 0 });
	}
}
