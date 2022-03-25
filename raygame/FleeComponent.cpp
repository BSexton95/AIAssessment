#include "FleeComponent.h"
#include "Actor.h"
#include "Transform2D.h"
#include "Agent.h"
#include "MoveComponent.h"

MathLibrary::Vector2 FleeComponent::calculateForce()
{
	if (!getTarget())
		return { 0, 0 };

	//Calculate the distance between the agent and the player to get a vector to the player
	MathLibrary::Vector2 directionToTarget = getOwner()->getTransform()->getWorldPosition() 
		- getTarget()->getTransform()->getWorldPosition();

	//Calculate a desired velocity by normalizing the vector that was found and scaling it by a steering force
	MathLibrary::Vector2 desiredVelocity = directionToTarget.getNormalized() * getSteeringForce();

	//By subtracting the desired velocity by the agents velocity the seek force is found
	MathLibrary::Vector2 seekForce = desiredVelocity - getAgent()->getMoveComponent()->getVelocity();

	//Return the seek force found
	return seekForce;
}
