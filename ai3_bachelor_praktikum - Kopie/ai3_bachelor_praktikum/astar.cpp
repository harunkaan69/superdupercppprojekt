#include "astar.h"


astar::Node::Node(int pX, int pY, Node* pParent)
{
	parent = pParent;
	x = pX;
	y = pY;
	g = h = 0;
	isObstacle = false;
	visited = false;
}

float astar::Node::getScore()
{
	return g + h;
}

float astar::heuristic(int pX1, int pY1, int pX2, int pY2)
{
	return sqrt(pow((pX2 - pX1), 2) + pow((pY2 - pY1), 2));
}

bool astar::isObstacle(int pX1, int pY1, int pX2, int pY2)
{
	if (pX1 < 0 || pX1 > map->getWidth()  ||
		pY1 < 0 || pY1 > map->getHeight() ||
		pX2 < 0 || pX2 > map->getWidth()  ||
		pY2 < 0 || pY2 > map->getHeight() ||
		tblHabitateToTable[tblTerrainToHabitate[map->tileAt(pX1, pY1).getTerrain()]][map->tileAt(pX2, pY2).getTerrain()] == -1) {
		return true;
	}

	return false;
}

astar::Node* astar::findNode(std::set<Node*>& pList, int pX, int pY)
{
	for (auto node : pList) {
		if ((node->x == pX) && (node->y == pY)) {
			return node;
		}
	}

	return nullptr;
}


std::vector<std::pair<int, int>> astar::getPath(const std::shared_ptr<creature>& pCreature, tile* pStart, const tile& pTile)
{
	if (!isObstacle(pTile.getX(), pTile.getY(), pStart->getX(), pStart->getY())) {
		Node* currentNode = nullptr;
		std::set<Node*> openList, closedList;
#
		// add starting node to the openlist
		if (pStart != nullptr) {
			openList.insert(new Node(pStart->getX(), pStart->getY()));
		} else if (pCreature != nullptr) {
			openList.insert(new Node(pCreature->getX(), pCreature->getY()));
		} else { return {}; }

		// do whole openlist is not empty
		while (!openList.empty()) {
			currentNode = *openList.begin();

			// ckeck for the note with the smalles f-value and set it to the current node
			for (auto node : openList) {
				if (node->getScore() <= currentNode->getScore()) {
					currentNode = node;
				}
			}

			// if our current node is the target node, exit
			if ((currentNode->x == pTile.getX()) && (currentNode->y == pTile.getY())) {
				break;
			}

			// remove current node from openlist and add it to the closed list
			closedList.insert(currentNode);
			openList.erase(std::find(openList.begin(), openList.end(), currentNode));

			// loop through all 4 directions and check the nodes
			for (int i = 0; i < directions.size(); i++) {
				int newX = currentNode->x + directions[i].first;
				int newY = currentNode->y + directions[i].second;

				// if the current node is an obstacle or is already in the closedlist, skip it
				if (isObstacle(currentNode->x, currentNode->y, newX, newY) || findNode(closedList, newX, newY)) {
					continue;
				}

				float totalCost = currentNode->g + heuristic(currentNode->x, currentNode->y, newX, newY);

				Node* successor = findNode(openList, newX, newY);

				if (successor == nullptr) {
					successor = new Node(newX, newY, currentNode);
					successor->g = totalCost;
					successor->h = heuristic(successor->x, successor->y, pTile.getX(), pTile.getY());
					openList.insert(successor);
				} else if (totalCost < successor->g) {
					successor->parent = currentNode;
					successor->g = totalCost;
				}
			}
		}

		std::vector<std::pair<int, int>> posVec;

		while (currentNode != nullptr) {
			posVec.push_back(std::pair<int, int>(currentNode->x, currentNode->y));
			currentNode = currentNode->parent;
		}

		return posVec;
	}

	return {};
}
