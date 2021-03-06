//
//  GameGrid.cpp
//  Created by Matt Kaufmann on 02/04/14.
//

#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridRoom.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Utilities/MathUtilities.h"

#ifdef DEBUG
#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#endif

GameGrid::GameGrid(unsigned int spanX, unsigned int spanZ) {
	this->init(spanX, spanZ);
}

GameGrid::~GameGrid() {
	this->destroy();
}

void GameGrid::init(unsigned int spanX, unsigned int spanZ) {
	// Initialize the grid
	this->gridWidth  = spanX;
	this->gridHeight = spanZ;

	this->gridCells = new GridCell**[this->gridWidth];
	for (unsigned int i = 0; i < this->gridWidth; ++i) {
		this->gridCells[i] = new GridCell*[this->gridHeight];
		for (unsigned int j = 0; j < this->gridHeight; ++j) {
			glm::vec3 center;
			center.x = i * CELL_SIZE;
			center.y = 0;
			center.z = j * -CELL_SIZE;
			glm::vec3 origin = center - HALF_CELL;

			this->gridCells[i][j] = new GridCell(i, 0, j, origin, center);
		}
	}

	// Initialize the bitmaps
	this->passableBits = new unsigned char**[NUM_ELEVATIONS];
	this->visibleBits  = new unsigned char**[NUM_ELEVATIONS];

	// By default, all cells are passable at level 0 and visible at all levels
	for (int i = 0; i < NUM_ELEVATIONS; ++i) {
		this->passableBits[i] = new unsigned char*[this->gridWidth];
		this->visibleBits[i]  = new unsigned char*[this->gridWidth];
		for (unsigned int j = 0; j < this->gridWidth; ++j) {
			int numBytes = (this->gridHeight + 7) / 8;
			this->passableBits[i][j] = new unsigned char[numBytes];
			this->visibleBits[i][j] = new unsigned char[numBytes];
			for (int k = 0; k < numBytes; ++k) {
				if (i == 0) {
					this->passableBits[i][j][k] = 0xFF;
				}
				else {
					this->passableBits[i][j][k] = 0x00;
				}
				this->visibleBits[i][j][k]  = 0xFF;
			}
		}
	}
}

void GameGrid::destroy() {
	// Clear the list of grid zones
	this->gridZones.clear();

	// Free the allocated memory for the grid rooms
	for (auto iter = this->gridRooms.begin(); iter != this->gridRooms.end(); ++iter) {
		delete *iter;
	}
	this->gridRooms.clear();

	// Free the allocated memory for the grid itself
	if (this->gridCells != nullptr) {
		for (unsigned int i = 0; i < this->gridWidth; ++i) {
			for (unsigned int j = 0; j < this->gridHeight; ++j) {
				delete this->gridCells[i][j];
			}
			delete this->gridCells[i];
			delete this->passableBits[i];
			delete this->visibleBits[i];
		}
		delete this->gridCells;
		this->gridCells = nullptr;
		delete this->passableBits;
		this->passableBits = nullptr;
		delete this->visibleBits;
		this->visibleBits = nullptr;
	}
	this->gridWidth = 0;
	this->gridHeight = 0;
}

int GameGrid::GetElevationFromWorldY(float worldY) {
	return (int)floor((worldY / 2.0f) + 0.5f);
}

GridCell* GameGrid::GetCell(int x, int z) {
	if (isWithinGrid(x * CELL_SIZE, z * CELL_SIZE)) { return this->gridCells[x][z]; }
	return nullptr;
}

GridCell* GameGrid::GetCell(glm::vec3 loc) {
	int gX = (int)((loc.x / CELL_SIZE) + 0.5f);
	int gZ = (int)((-loc.z / CELL_SIZE) + 0.5f);
	return GetCell(gX, gZ);
}

void GameGrid::GetNeighborCells(std::list<GridCell*>& outNbrs, GridCell* cell, int /*range= 1*/) {
	if (this->isWithinGrid(cell->x - 1, cell->z)) {
		outNbrs.push_back(this->gridCells[cell->x - 1][cell->z]);
	}
	if (this->isWithinGrid(cell->x + 1, cell->z)) {
		outNbrs.push_back(this->gridCells[cell->x + 1][cell->z]);
	}
	if (this->isWithinGrid(cell->x, cell->z - 1)) {
		outNbrs.push_back(this->gridCells[cell->x][cell->z - 1]);
	}
	if (this->isWithinGrid(cell->x, cell->z + 1)) {
		outNbrs.push_back(this->gridCells[cell->x][cell->z + 1]);
	}
}

void GameGrid::AddGridRoom(int west, int south, int width, int height) {
	GridRoom* room = new GridRoom(west, south, width, height);
	this->gridRooms.push_back(room);
}

GridRoom* GameGrid::GetRoom(int x, int z) {
	GridCell* cell = GetCell(x, z);
	return GetRoom(cell);
}

GridRoom* GameGrid::GetRoom(glm::vec3 loc) {
	GridCell* cell = GetCell(loc);
	return GetRoom(cell);
}

GridRoom* GameGrid::GetRoom(GridCell* cell) {
	if (cell != nullptr) {
		for (unsigned int i = 0; i < this->gridRooms.size(); ++i) {
			if (this->gridRooms[i]->IsWithinRoom(cell->x, cell->z)) {
				return this->gridRooms[i];
			}
		}
	}
	return nullptr;
}

glm::vec3 GameGrid::GetRoomCenter(int x, int z) {
	GridCell* cell = GetCell(x, z);
	return GetRoomCenter(cell);
}

glm::vec3 GameGrid::GetRoomCenter(GridCell* cell) {
	glm::vec3 center;
	GridRoom* room = GetRoom(cell);
	if (room != nullptr) {
		center.x = (room->westBound + room->eastBound) * CELL_SIZE * 0.5f;
		center.y = 0.0f;
		center.z = (room->southBound + room->northBound) * CELL_SIZE * -0.5f;
		return center;
	}
	return ZERO_VEC3;
}

void GameGrid::AddGridZone(ObjectIdType zoneId) {
	for (auto iter = this->gridZones.begin(); iter != this->gridZones.end(); ++iter) {
		if (*iter == zoneId) {
			// This zone is already in our list
			return;
		}
	}

	// Add the id to the list
	this->gridZones.push_back(zoneId);
}

GridZone* GameGrid::GetZone(int x, int z) {
	GridCell* cell = GetCell(x, z);
	return GetZone(cell);
}

GridZone* GameGrid::GetZone(glm::vec3 loc) {
	GridCell* cell = GetCell(loc);
	return GetZone(cell);
}

GridZone* GameGrid::GetZone(GridCell* cell) {
	if (cell != nullptr) {
		// If the cell is in multiple zones, only the first one is returned.
		for (auto iter = this->gridZones.begin(); iter != this->gridZones.end(); ++iter) {
			GameObject* zoneObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
			GridZone* zone = zoneObj->GetComponent<GridZone>();

			if (zone->IsCellWithinZone(cell)) {
				return zone;
			}
		}
	}
	return nullptr;
}

void GameGrid::TouchedCells(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outTouched) {
	int spanX = goalCell->x - startCell->x;
	int spanZ = goalCell->z - startCell->z;

	int xDirection;
	float fracX, incrX;
	if (spanX == 0) {
		xDirection = 0;
		fracX = 100.0f;
		incrX = 100.0f;
	}
	else {
		if (spanX > 0)  { xDirection =  1; }
		else            { xDirection = -1; }
		fracX = 0.0f;
		incrX = (float)xDirection / spanX;
	}

	int zDirection;
	float fracZ, incrZ;
	if (spanZ == 0) {
		zDirection = 0;
		fracZ = 100.0f;
		incrZ = 100.0f;
	}
	else {
		if (spanZ > 0)  { zDirection =  1; }
		else            { zDirection = -1; }
		fracZ = 0.0f;
		incrZ = (float)zDirection / spanZ;
	}

	int xIndex = startCell->x;
	int zIndex = startCell->z;

	while ((fracX < 1.0f) || (fracZ < 1.0f)) {
		outTouched.push_back(this->gridCells[xIndex + xDirection][zIndex]);
		outTouched.push_back(this->gridCells[xIndex][zIndex + zDirection]);

		float diff = (fracZ + incrZ) - (fracX + incrX);
		const float ERROR_MARGIN = 0.0001f;

		// Find the next cell
		if (diff > ERROR_MARGIN) {
			xIndex += xDirection;
			fracX += incrX;
		}
		else if (diff < -ERROR_MARGIN) {
			zIndex += zDirection;
			fracZ += incrZ;
		}
		else {
			outTouched.push_back(this->gridCells[xIndex + xDirection][zIndex + zDirection]);
			xIndex += xDirection;
			zIndex += zDirection;
			fracX  += incrX;
			fracZ  += incrZ;
		}
	}
}

bool GameGrid::Passable(GridCell* startCell, GridCell* goalCell) {
	if (this->IsCellPassableAtElevation(goalCell->x, goalCell->z, startCell->y)) { return true; }
	return false;
}

bool GameGrid::HasLineOfSight(GridCell* sourceCell, GridCell* targetCell) {
	// Make sure both cells exist
	if ((sourceCell != nullptr) && (targetCell != nullptr)) {
		// Check the cells along the path
		std::list<GridCell*> touchedCells;
		this->TouchedCells(sourceCell, targetCell, touchedCells);

		for (auto iter = touchedCells.begin(); iter != touchedCells.end(); ++iter) {
			if ((*iter)->y > sourceCell->y) {
				return false;
			}
			if (!this->IsCellVisibleAtElevation(targetCell->x, targetCell->z, sourceCell->y)) {
				return false;
			}
		}

		return true;
	}
	return false;
}

bool GameGrid::HasLineOfSight(int sourceCellX, int sourceCellZ, int targetCellX, int targetCellZ) {
	GridCell* sourceCell = this->GetCell(sourceCellX, sourceCellZ);
	GridCell* targetCell = this->GetCell(targetCellX, targetCellZ);
	return this->HasLineOfSight(sourceCell, targetCell);
}

int GameGrid::GetCellGroundLevel(int gridX, int gridZ) {
	GridCell* cell = this->GetCell(gridX, gridZ);
	if (cell != nullptr) {
		return cell->y;
	}
	return -1;
}

bool GameGrid::IsCellPassableAtElevation(int gridX, int gridZ, unsigned int elevation) {
	if (elevation < NUM_ELEVATIONS) {
		if (this->isWithinGrid(gridX, gridZ)) {
			return IS_BIT_SET_IN_BYTE_ARRAY(gridZ, this->passableBits[elevation][gridX]);
		}
	}
	return false;
}

bool GameGrid::IsCellVisibleAtElevation(int gridX, int gridZ, unsigned int elevation) {
	if (elevation < NUM_ELEVATIONS) {
		if (this->isWithinGrid(gridX, gridZ)) {
			return IS_BIT_SET_IN_BYTE_ARRAY(gridZ, this->visibleBits[elevation][gridX]);
		}
	}
	return false;
}

void GameGrid::SetCellGroundLevel(int gridX, int gridZ, unsigned int elevation) {
	ASSERT(elevation < NUM_ELEVATIONS, "Elevation level %u is outside grid bounds", elevation);
	if (elevation < NUM_ELEVATIONS) {
		GridCell* cell = this->GetCell(gridX, gridZ);

		ASSERT(cell != nullptr, "Cell does not exist at position (%d, %d)", gridX, gridZ);
		if (cell != nullptr) {
			int diff = elevation - cell->y;
			if (diff > 0) {
				for (int i = NUM_ELEVATIONS - 1; i >= 0; --i) {
					int copyFrom = i - diff;
					bool isPassable, isVisible;
					if (copyFrom >= 0) {
						isPassable = this->IsCellPassableAtElevation(gridX, gridZ, copyFrom);
						isVisible  = this->IsCellVisibleAtElevation(gridX, gridZ, copyFrom);
					}
					else {
						isPassable = false;
						isVisible = false;
					}
					this->SetCellPassableAtElevation(gridX, gridZ, i, isPassable);
					this->SetCellVisibleAtElevation(gridX, gridZ, i, isVisible);
				}
			}
			else if (diff < 0) {
				for (int i = 0; i < NUM_ELEVATIONS; ++i) {
					int copyFrom = i - diff;
					bool isPassable, isVisible;
					if (copyFrom < NUM_ELEVATIONS) {
						isPassable = this->IsCellPassableAtElevation(gridX, gridZ, copyFrom);
						isVisible  = this->IsCellVisibleAtElevation(gridX, gridZ, copyFrom);
					}
					else {
						isPassable = false;
						isVisible = true;
					}
					this->SetCellPassableAtElevation(gridX, gridZ, i, isPassable);
					this->SetCellVisibleAtElevation(gridX, gridZ, i, isVisible);
				}
			}
			cell->y = elevation;
			cell->origin.y = elevation;
			cell->center.y = elevation;
		}
	}
}

void GameGrid::SetCellPassableAtElevation(int gridX, int gridZ, unsigned int elevation, bool isPassable) {
	ASSERT(elevation < NUM_ELEVATIONS, "Elevation level %u is outside grid bounds", elevation);
	if (elevation < NUM_ELEVATIONS) {
		ASSERT(this->isWithinGrid(gridX, gridZ), "Cell does not exist at position (%d, %d)", gridX, gridZ);
		if (this->isWithinGrid(gridX, gridZ)) {
			if (isPassable) {
				SET_BIT_IN_BYTE_ARRAY(gridZ, this->passableBits[elevation][gridX]);
			}
			else {
				UNSET_BIT_IN_BYTE_ARRAY(gridZ, this->passableBits[elevation][gridX]);
			}
		}
	}
}

void GameGrid::SetCellVisibleAtElevation(int gridX, int gridZ, unsigned int elevation, bool isVisible) {
	ASSERT(elevation < NUM_ELEVATIONS, "Elevation level %u is outside grid bounds", elevation);
	if (elevation < NUM_ELEVATIONS) {
		ASSERT(this->isWithinGrid(gridX, gridZ), "Cell does not exist at position (%d, %d)", gridX, gridZ);
		if (this->isWithinGrid(gridX, gridZ)) {
			if (isVisible) {
				SET_BIT_IN_BYTE_ARRAY(gridZ, this->visibleBits[elevation][gridX]);
			}
			else {
				UNSET_BIT_IN_BYTE_ARRAY(gridZ, this->visibleBits[elevation][gridX]);
			}
		}
	}
}

bool GameGrid::isWithinGrid(int x, int z) {
	return (x >= 0) && (x < (int)this->gridWidth) && (z >= 0) && (z < (int)this->gridHeight);
}

bool GameGrid::isWithinGrid(glm::vec3 loc) {
	int gX = (int)((loc.x / CELL_SIZE) + 0.5f);
	int gZ = (int)((-loc.z / CELL_SIZE) + 0.5f);
	return isWithinGrid(gX, gZ);
}

#ifdef DEBUG_GRID
void GameGrid::debugDrawGrid() {
	glm::vec3 start, end;

	start.x = -CELL_SIZE / 2;
	start.y = 0.0f;
	start.z = CELL_SIZE / 2;
	end.x = -CELL_SIZE / 2;
	end.y = 0.0f;
	end.z = (0.5f - this->gridHeight) * CELL_SIZE;
	unsigned int i = 0;
	do {
		DebugDraw::DrawLine(start, end);
		++i;
		start.x += CELL_SIZE;
		end.x += CELL_SIZE;
	} while (i <= this->gridWidth);

	start.x = -CELL_SIZE / 2;
	start.y = 0.0f;
	start.z = CELL_SIZE / 2;
	end.x = (this->gridWidth - 0.5f) * CELL_SIZE;
	end.y = 0.0f;
	end.z = CELL_SIZE / 2;
	unsigned int j = 0;
	do {
		DebugDraw::DrawLine(start, end);
		++j;
		start.z -= CELL_SIZE;
		end.z -= CELL_SIZE;
	} while (j <= this->gridHeight);
}
#endif
