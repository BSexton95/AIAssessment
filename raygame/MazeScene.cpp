#include "MazeScene.h"
#include "Player.h"
#include "Wall.h"
#include "Ghost.h"
#include "Transform2D.h"
#include "SpriteComponent.h"
#include "WanderComponent.h"
#include "SeekComponent.h"
#include "FleeComponent.h"
#include "Agent.h"
#include "StateMachineComponent.h"

Maze::TileKey _ = Maze::TileKey::OPEN;
Maze::TileKey w = Maze::TileKey::WALL;
Maze::TileKey s = Maze::TileKey::MUD;
Maze::TileKey p = Maze::TileKey::PLAYER;
Maze::TileKey g = Maze::TileKey::GHOST;
Maze::TileKey a1 = Maze::TileKey::AGENTSEEKING;
Maze::TileKey a2 = Maze::TileKey::AGENTFLEEING;

Maze::Maze()
{
	//Create the player
	


	TileKey map[Maze::HEIGHT][Maze::WIDTH] = {
		{ w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, w, _, w, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, w, _, w, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, w, g, w, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, w, w, w, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, a2, _, _, w, w, w, w, _, _, _, _, _, _, w, w, w, w, _, _, _, _, a1, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, w, w, _, _, _, _, w, w, w, w, w, _, _, _, _, w, w, _, _, _, _, _, w },
		{ w, _, _, _, _, w, w, _, _, _, _, w, w, w, w, w, _, _, _, _, w, w, _, _, _, _, _, w },
		{ w, _, _, _, _, w, w, _, _, _, _, _, _, _, _, _, _, _, _, _, w, w, _, _, _, _, _, w },
		{ w, _, _, _, _, w, w, _, _, _, _, _, _, _, _, _, _, _, _, _, w, w, _, _, _, _, _, w },
		{ w, _, _, _, _, w, w, _, _, _, _, w, w, w, w, w, _, _, _, _, w, w, _, _, _, _, _, w },
		{ w, _, _, _, _, w, w, _, _, _, _, w, w, w, w, w, _, _, _, _, w, w, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, w, w, w, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, w, p, w, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, w, _, w, _, _, _, _, _, _, _, _, w, w, w, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w, w, w, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w, w, w, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, w },
		{ w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w },
	};


	m_player = new Player(0, 0, "Player", 2000, 50);
	//Generate the map
	generate(map);
}

Maze::~Maze()
{
	delete m_player;
	for (int y = 0; y < m_size.y; y++) {
		for (int x = 0; x < m_size.x; x++) {
			delete m_grid[x][y].actor;
			delete m_grid[x][y].node;
		}
	}
}

void Maze::draw()
{
	NodeGraph::drawGraph(m_grid[0][0].node);
	Scene::draw();
}

Maze::Tile Maze::getTile(MathLibrary::Vector2 position)
{
	int x = (int)(position.x / TILE_SIZE);
	int y = (int)(position.y / TILE_SIZE);
	if (x >= 0 && x < Maze::WIDTH && y >= 0 && y < Maze::HEIGHT)
		return m_grid[x][y];
	else
		return m_grid[0][0];
}

MathLibrary::Vector2 Maze::getPosition(Tile tile)
{
	return MathLibrary::Vector2{ tile.x * TILE_SIZE + (TILE_SIZE / 2.0f), tile.y * TILE_SIZE + (TILE_SIZE / 2.0f) };
}

Maze::Tile Maze::createTile(int x, int y, TileKey key)
{
	// Create a new tile at the given location
	Tile tile{ x, y };
	tile.node = new NodeGraph::Node();
	MathLibrary::Vector2 position = getPosition(tile);
	// Set the cost and actor of each tile
	switch (key) {
	case TileKey::OPEN:
		tile.cost = 1.0f;
		break;
	case TileKey::WALL:
		tile.cost = 100.0f;
		tile.actor = new Wall(position.x, position.y);
		tile.node->walkable = false;
		addActor(tile.actor);
		break;
	case TileKey::PLAYER:
		tile.cost = 1.0f;
		m_player->getTransform()->setWorldPostion(position);
		tile.actor = m_player;
		addActor(tile.actor);
		break;
	case TileKey::GHOST: {
		tile.cost = 1.0f;
		Ghost* ghost = new Ghost(position.x, position.y, 100, 300, 0xFF6666FF, this);
		ghost->setTarget(m_player);
		tile.actor = ghost;
		addActor(tile.actor);
	}
		break;
	case TileKey::AGENTSEEKING: {
		//Tile cost is set to 1
		tile.cost = 1.0f;
		//Create a new agent
		Agent* agent1 = new Agent(position.x, position.y, "Agent1", 10, 100);

		//Add a sprite component to the new agent
		agent1->addComponent(new SpriteComponent("Images/enemy.png"));
		//Set the agents scale
		agent1->getTransform()->setScale({ 25, 25 });

		//Add a wander component to the new agent
		WanderComponent* wanderComponent = new WanderComponent(1000, 20, 200);
		agent1->addComponent(wanderComponent);

		//Add a seek component to the new agent
		SeekComponent* seekComponent = new SeekComponent();
		//Set the steering force for the seek component to be 50 and set the target to be the player
		seekComponent->setSteeringForce(50);
		seekComponent->setTarget(m_player);
		agent1->addComponent(seekComponent);


		//Add a flee component to the agent
		FleeComponent* fleeComponent = new FleeComponent();
		fleeComponent->setSteeringForce(0);
		fleeComponent->setTarget(m_player);
		agent1->addComponent(fleeComponent);

		//Add the state machine component to the new agent
		agent1->addComponent<StateMachineComponent>();

		//Add the agent to the scene
		tile.actor = agent1;
		addActor(agent1); 
	}
		break;
	case TileKey::AGENTFLEEING: {
		//Set cost of tile to 1
		tile.cost = 1.0f;
		//Create a new agent
		Agent* agent2 = new Agent(position.x, position.y, "Agent2", 100, 100);

		//Add a sprite component to the new agent
		agent2->addComponent(new SpriteComponent("Images/enemy.png"));
		//Set the agents scale
		agent2->getTransform()->setScale({ 25, 25 });

		//Add a wander component to the new agent
		WanderComponent* wanderComponent = new WanderComponent(1000, 20, 200);
		agent2->addComponent(wanderComponent);

		//Add a flee component to the agent
		FleeComponent* fleeComponent = new FleeComponent();
		fleeComponent->setSteeringForce(50);
		fleeComponent->setTarget(m_player);
		agent2->addComponent(fleeComponent);

		//Add a seek component to the new agent
		SeekComponent* seekComponent = new SeekComponent();
		//Set the steering force for the seek component to be 50 and set the target to be the player
		seekComponent->setSteeringForce(0);
		seekComponent->setTarget(m_player);
		agent2->addComponent(seekComponent);

		//Add the state machine component to the new agent
		agent2->addComponent<StateMachineComponent>();

		tile.actor = agent2;
		addActor(agent2);
	}
		break;
	}
	return tile;
}

void Maze::generate(TileKey map[Maze::HEIGHT][Maze::WIDTH])
{
	for (int y = 0; y < m_size.y; y++) {
		for (int x = 0; x < m_size.x; x++) {
			// Create the tile
			Tile currentTile = createTile(x, y, map[y][x]);
			// Add node to graph
			currentTile.node->position = getPosition(currentTile);
			if (x > 0) { // west connection
				Tile other = m_grid[x - 1][y];
				currentTile.node->edges.addItem(NodeGraph::Edge{ other.node, other.cost });
				other.node->edges.addItem(NodeGraph::Edge{ currentTile.node, currentTile.cost });
			}
			if (y > 0) { // north connection
				Tile other = m_grid[x][y - 1];
				currentTile.node->edges.addItem(NodeGraph::Edge{ other.node, other.cost });
				other.node->edges.addItem(NodeGraph::Edge{ currentTile.node, currentTile.cost });
			}
			// Set the tile on the grid
			m_grid[x][y] = currentTile;
		}
	}
}