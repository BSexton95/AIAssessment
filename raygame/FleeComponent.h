#pragma once
#include "SteeringComponent.h"

class FleeComponent :
    public SteeringComponent
{
public:

	/// <summary>
	/// Calcuates the forces for the flee component
	/// </summary>
	/// <returns></returns>
	MathLibrary::Vector2 calculateForce() override;
};

