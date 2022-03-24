#pragma once
#include "Agent.h"
#include "DynamicArray.h"
#include <Vector2.h>

class SteeringComponent;
class MoveComponent;

class Maze;

class AgentSeeking :
    public Agent
{
public:
    /// <param name="x">The x-position</param>
    /// <param name="y">The y-position</param>
    /// <param name="speed">The mazimum movement speed</param>
    /// <param name="maxForce">The max force applyed to the agent</param>
    AgentSeeking(float x, float y, float maxSpeed, float maxForce, Actor* target);
    ~AgentSeeking() {};

    void start() override;
    void update(float deltaTime) override;
    void draw() override;

    void onAddComponent(Component* component) override;
    void onCollision(Actor* other) override;

private:
    DynamicArray<SteeringComponent*> m_steeringComponents;
    MoveComponent* m_moveComponent;
    Actor* m_target = nullptr;
    MathLibrary::Vector2 m_force;
};

