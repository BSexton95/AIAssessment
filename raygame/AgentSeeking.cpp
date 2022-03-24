#include "AgentSeeking.h"
#include "MoveComponent.h"
#include "SteeringComponent.h"

AgentSeeking::AgentSeeking(float x, float y, float maxSpeed, float maxForce, Actor* target) 
	: Agent(x, y, "Agent1", maxSpeed, maxForce)
{
	m_target = target;
}

void AgentSeeking::start()
{
	Agent::start();

	m_moveComponent = addComponent<MoveComponent>();
	m_moveComponent->setMaxSpeed(300);
	m_moveComponent->setUpdateFacing(true);
}

void AgentSeeking::update(float deltaTime)
{
	Actor::update(deltaTime);
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
	getMoveComponent()->setVelocity(getMoveComponent()->getVelocity() + m_force * deltaTime);
}

void AgentSeeking::draw()
{
	Agent::draw();
}

void AgentSeeking::onAddComponent(Component* component)
{
	SteeringComponent* steeringComponent = dynamic_cast<SteeringComponent*>(component);
	if (steeringComponent)
		m_steeringComponents.addItem(steeringComponent);
}

void AgentSeeking::onCollision(Actor* other)
{
}
