#include "NodeGraph.h"
#include <raylib.h>
#include <xlocinfo>

DynamicArray<NodeGraph::Node*> reconstructPath(NodeGraph::Node* start, NodeGraph::Node* end)
{
	DynamicArray<NodeGraph::Node*> path;
	NodeGraph::Node* currentNode = end;

	while (currentNode != start->previous)
	{
		currentNode->color = 0xFFFF00FF;
		path.insert(currentNode, 0);
		currentNode = currentNode->previous;
	}

	return path;
}

float diagonalDistance(NodeGraph::Node* node, NodeGraph::Node* goal, float cardinalCost, float diagonalCost)
{
	float displacementX = abs(node->position.x - goal->position.x);
	float displacementY = abs(node->position.y - goal->position.y);

	return cardinalCost * (displacementX + displacementY) + (diagonalCost - 2 * cardinalCost) * fmin(displacementX, displacementY);
}

void sortFScore(DynamicArray<NodeGraph::Node*>& nodes)
{
	NodeGraph::Node* key = nullptr;
	int j = 0;

	for (int i = 1; i < nodes.getLength(); i++) {
		key = nodes[i];
		j = i - 1;
		while (j >= 0 && nodes[j]->fScore > key->fScore) {
			nodes[j + 1] = nodes[j];
			j--;
		}

		nodes[j + 1] = key;
	}
}



void NodeGraph::sortGScore(DynamicArray<NodeGraph::Node*>& nodes)
{
	NodeGraph::Node* key = nullptr;
	int j = 0;

	for (int i = 1; i < nodes.getLength(); i++)
	{
		key = nodes[i];
		j = i - 1;

		while (j >= 0 && nodes[j]->gScore > key->gScore)
		{
			nodes[j + 1] = nodes[j];
			j--;
		}

		nodes[j + 1] = key;
	}
}

DynamicArray<NodeGraph::Node*> NodeGraph::findPath(Node* start, Node* goal)
{
	resetGraphScore(start);

	//If there is not a starting node...
	if (!start)
		//...return an empty node graph
		return DynamicArray<NodeGraph::Node*>();
	
	//Create an open list and closed list
	//Open list will hold nodes that need to be looked at
	//Closed list will hold the nodes that have been looked at
	DynamicArray<NodeGraph::Node*> openList;
	DynamicArray<NodeGraph::Node*> closedList;
	//Set the current node to be the start node
	Node* currentNode = start;
	
	//Add the starting node to the open list
	openList.addItem(start);

	//The starting node will be green
	start->color = 0x00FF00FF; 

	//Loops while the open list contains nodes
	while (openList.getLength() > 0)
	{
		//If the current node is the goal node...
		if (currentNode == goal)
		{
			//Reconstruct the path for the ghost to follow
			return reconstructPath(start, goal);
		}
		
		//Sort the open list to lowest f score to higher f sScore
		sortFScore(openList);
		//Set the current node to be the first node in the open list
		currentNode = openList[0];
		//remove the current node from the open list
		openList.remove(currentNode);
	
		//Loops through the currend nodes neighbors
		for (int i = 0; i < currentNode->edges.getLength(); i++)
		{
			//if the current not is not in the closed list...
			if (!closedList.contains(currentNode))
			{
				//If the current nodes neighboring node is walkable...
				if (!currentNode->edges[i].target->walkable)
					//...continue on
					continue;

				//The current nodes neighbors are red
				currentNode->edges[i].target->color = 0xFF0000FF;

				//If the current nodes neighbors g score is 0 or if it's greater than the current nodes gscore + the cost of the edge...
				if (currentNode->edges[i].target->gScore == 0 || currentNode->edges[i].target->gScore > currentNode->gScore + currentNode->edges[i].cost)
				{
					//...Calculate the neighbors g score
					currentNode->edges[i].target->gScore = currentNode->gScore + currentNode->edges[i].cost;
					//Calculate the h score by using the manhattan distance algorithm
					currentNode->edges[i].target->hScore = getManhattanDistance(currentNode->edges[i].target, goal);
					//Calculate the f score
					currentNode->edges[i].target->fScore = currentNode->edges[i].target->gScore + currentNode->edges[i].target->hScore;
					//Set the neighbors parent node to be the current node
					currentNode->edges[i].target->previous = currentNode;
				}

				//If the current nodes neighbor is not in the open list...
				if (!openList.contains(currentNode->edges[i].target))
				{
					//...add it to the open list to be looked at
					openList.addItem(currentNode->edges[i].target);

				}
			}
		}
		//After looking at the current node add it to the closed list
		closedList.addItem(currentNode);
		
	}

	//Return a path reconstructed for the ghost to follow
	return reconstructPath(start, goal);
	
}

float NodeGraph::getManhattanDistance(Node* start, Node* end)
{
	return abs(start->position.x - end->position.x)
		+ abs(start->position.y - end->position.y);
}


void NodeGraph::drawGraph(Node* start)
{
	DynamicArray<Node*> drawnList = DynamicArray<Node*>();
	drawConnectedNodes(start, drawnList);
}

void NodeGraph::drawNode(Node* node, float size)
{
	static char buffer[10];
	sprintf_s(buffer, "%.0f", node->gScore);

	//Draw the circle for the outline
	DrawCircle((int)node->position.x, (int)node->position.y, size + 1, GetColor(node->color));
	//Draw the inner circle
	DrawCircle((int)node->position.x, (int)node->position.y, size, GetColor(node->color));
	//Draw the text
	DrawText(buffer, (int)node->position.x, (int)node->position.y, .8f, BLACK);
}

void NodeGraph::drawConnectedNodes(Node* node, DynamicArray<Node*>& drawnList)
{
	drawnList.addItem(node);
	if (node->walkable)
		drawNode(node, 8);

	for (int i = 0; i < node->edges.getLength(); i++)
	{
		Edge e = node->edges[i];
		////Draw the Edge
		//DrawLine((int)node->position.x, (int)node->position.y, (int)e.target->position.x, (int)e.target->position.y, WHITE);
		////Draw the cost
		//MathLibrary::Vector2 costPos = { (node->position.x + e.target->position.x) / 2, (node->position.y + e.target->position.y) / 2 };
		//static char buffer[10];
		//sprintf_s(buffer, "%.0f", e.cost);
		//DrawText(buffer, (int)costPos.x, (int)costPos.y, 16, RAYWHITE);
		//Draw the target node
		if (!drawnList.contains(e.target)) {
			drawConnectedNodes(e.target, drawnList);
		}
	}
}

void NodeGraph::resetGraphScore(Node * start)
{
	DynamicArray<Node*> resetList = DynamicArray<Node*>();
	resetConnectedNodes(start, resetList);
}

void NodeGraph::resetConnectedNodes(Node* node, DynamicArray<Node*>& resetList)
{
	resetList.addItem(node);

	for (int i = 0; i < node->edges.getLength(); i++)
	{
		node->edges[i].target->gScore = 0;
		node->edges[i].target->hScore = 0;
		node->edges[i].target->fScore = 0;
		node->edges[i].target->color = 0xFFFFFFFF;

		//Draw the target node
		if (!resetList.contains(node->edges[i].target)) {
			resetConnectedNodes(node->edges[i].target, resetList);
		}
	}
}