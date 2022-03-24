#include "StateMachineComponent.h"
#include "Actor.h"
#include "SeekComponent.h"
#include "WanderComponent.h"
#include "FleeComponent.h"
#include "Transform2D.h"

void StateMachineComponent::start()
{
	Component::start();

	m_fleeComponent = getOwner()->getComponent<FleeComponent>();
	m_fleeForce = m_fleeComponent->getSteeringForce();

	m_seekComponent = getOwner()->getComponent<SeekComponent>();
	m_seekForce = m_seekComponent->getSteeringForce();

	m_wanderComponent = getOwner()->getComponent<WanderComponent>();
	m_wanderForce = m_wanderComponent->getSteeringForce();

	m_currentState = WANDER;
}

void StateMachineComponent::update(float deltaTime)
{
	Component::update(deltaTime);

	MathLibrary::Vector2 targetPos = m_seekComponent->getTarget()->getTransform()->getWorldPosition();
	MathLibrary::Vector2 ownerPos = getOwner()->getTransform()->getWorldPosition();
	float distanceFromTarget = (targetPos - ownerPos).getMagnitude();

	bool targetInRange = distanceFromTarget <= m_seekRange;

	switch (m_currentState)
	{
	case FLEE:
		m_fleeComponent->setSteeringForce(m_fleeForce);
		m_seekComponent->setSteeringForce(0);
		m_wanderComponent->setSteeringForce(0);

		if (!targetInRange)
			setCurrentState(WANDER);

		break;
	case WANDER:
		m_fleeComponent->setSteeringForce(0);
		m_seekComponent->setSteeringForce(0);
		m_wanderComponent->setSteeringForce(m_wanderForce);

		//If target is in range...
		if (targetInRange)
		{
			//If agent attached to this state machine is agent 1...
			if (getOwner()->getName() == "Agent1")
				//Set current state to seek
				setCurrentState(SEEK);
			//If agent attached to this state machine is agent 2...
			else if (getOwner()->getName() == "Agent2")
				//Set current state to flee
				setCurrentState(FLEE);
		}
		
		break;
	case SEEK:
		m_fleeComponent->setSteeringForce(0);
		m_seekComponent->setSteeringForce(m_seekForce);
		m_wanderComponent->setSteeringForce(0);

		if (!targetInRange)
			setCurrentState(WANDER);

		break;
	}
}
