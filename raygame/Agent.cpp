#include "Agent.h"
#include "MoveComponent.h"
#include "SteeringComponent.h"
#include "Engine.h"
#include "Wall.h"
#include "MazeScene.h"
#include "Transform2D.h"
#include "AABBCollider.h"

float maxSpeedRef = 0;
Agent::Agent(float x, float y, const char* name, float maxSpeed, float maxForce) : Actor(x, y, name)
{
	setMaxForce(maxForce);
	maxSpeedRef = maxSpeed;
}

void Agent::start()
{
	Actor::start();

	m_moveComponent = addComponent<MoveComponent>();
	m_moveComponent->setMaxSpeed(maxSpeedRef);
	m_moveComponent->setUpdateFacing(true);

	//Set the agents collider
	setCollider(new AABBCollider(Maze::TILE_SIZE, Maze::TILE_SIZE, this));
}

void Agent::fixedUpdate(float deltaTime)
{
	Actor::fixedUpdate(deltaTime);
	//Get all force being applied from steering behaviours
	for (int i = 0; i < m_steeringComponents.getLength(); i++)
	{
		m_force = m_force + m_steeringComponents[i]->calculateForce();
	}

	//Clamp force if it exceeds the maximum scale
	if (m_force.getMagnitude() > getMaxForce())
	{
		m_force = m_force.getNormalized() * getMaxForce();
	}

	//Apply force to velocity
	applyForce(m_force);
}

void Agent::onAddComponent(Component* component)
{
	SteeringComponent* steeringComponent = dynamic_cast<SteeringComponent*>(component);
	if (steeringComponent)
		m_steeringComponents.addItem(steeringComponent);
}

void Agent::onCollision(Actor* other)
{
	//If the agent collides with a wall it will not pass through
	if (Wall* wall = dynamic_cast<Wall*>(other))
	{
		MathLibrary::Vector2 halfTile = { Maze::TILE_SIZE / 2.0f, Maze::TILE_SIZE / 2.0f };
		MathLibrary::Vector2 position = getTransform()->getWorldPosition();
		position = position + halfTile;
		MathLibrary::Vector2 tilePosition = {
			roundf(position.x / Maze::TILE_SIZE) * Maze::TILE_SIZE,
			roundf(position.y / Maze::TILE_SIZE) * Maze::TILE_SIZE
		};
		tilePosition = tilePosition - halfTile;
		//getTransform()->setWorldPostion(getTransform()->getWorldPosition() - getMoveComponent()->getVelocity().getNormalized() * -.05f);
		applyForce(getCollider()->getCollisionNormal() * -1 * getMoveComponent()->getVelocity().getMagnitude());
	}

	//If agent collides with player...
	if (other->getName() == "Player")
	{
		if (getName() == "Agent1")
			//...player respawns
			other->getTransform()->setWorldPostion({ 350, 700 });
		else if (getName() == "Agent2")
			Engine::CloseApplication();
	}
}

void Agent::applyForce(MathLibrary::Vector2 force)
{
	m_moveComponent->setVelocity((m_moveComponent->getVelocity() + force));
}
