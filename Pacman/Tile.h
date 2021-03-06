#pragma once

#include "S2D/S2D.h"
#include <vector>

using namespace S2D;
using namespace std;

class Tile
{
public:
	Tile();
	~Tile();
	bool wall = false;
	bool visited = false;
	float globalGoal;
	float localGoal;
	int posInArray;
	Rect* sourceRect;
	Vector2* position;
	vector<Tile*> nodeNeighbours;
	Tile* parent;
};
