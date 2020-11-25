#pragma once

#include "S2D/S2D.h"
#include <vector>

using namespace S2D;
using namespace std;

class Node
{
public:
	bool wall = false;
	bool visited = false;
	float globalGoal;
	float localGoal;
	Vector2* position;
	Rect* sourceRect;
	vector<Node*> nodeNeighbours;
	Node* parent;
	void solve_Astar(Node* nodes[]);
};
