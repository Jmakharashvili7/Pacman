#include "Tile.h"
#include "Pacman.h"

Tile::Tile()
{
	wall = false;
	parent = nullptr;
	visited = false;
}

Tile::~Tile()
{
	delete sourceRect;
	delete position;
	delete parent;
}
