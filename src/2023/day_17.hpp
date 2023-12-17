#ifndef AOC_2023_16
#define AOC_2023_16

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

/*using namespace std;

// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;

// A structure to hold the necessary parameters
struct cell {
	// Row and Column index of its parent
	// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	int parent_i, parent_j;
	std::pair<int, int> direction;
	int amount_of_times_in_direction = 0;
	// f = g + h
	double f, g, h;
};

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(const auto& grid, int row, int col)
{
	// Returns true if row number and column number
	// is in range
	return (row >= 0) && (row < grid.size()) && (col >= 0)
		   && (col < grid[0].size());
}

// A Utility Function to check whether the given cell is
// blocked or not
bool isUnBlocked(const auto& grid, int row, int col)
{
	// Returns true if the cell is not blocked else false
	return (true);
}

// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}

// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(const auto& grid, int row, int col, int same_direction, Pair dest)
{
	// Return using the distance formula
	size_t val = 0;

	size_t n_col = col;

	for (; n_col < dest.second; ++n_col) {
		if (same_direction == 3) {
			if (row < grid.size() - 1) ++row;
			same_direction = 0;
		}

		val += grid[row][n_col];
		same_direction++;
	}

	for (size_t n_row = row; n_row < dest.first; ++n_row) {
		val += grid[n_row][n_col];
	}
	return val;
}

// A Utility Function to trace the path from the source
// to destination
void tracePath(const auto& grid, const std::vector<std::vector<cell>>& cellDetails, Pair dest)
{
	size_t res = 0;
	printf("\nThe Path is ");
	int row = dest.first;
	int col = dest.second;

	//res += grid[row][col];

	stack<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row
			 && cellDetails[row][col].parent_j == col)) {
		Path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(make_pair(row, col));
	while (!Path.empty()) {
		pair<int, int> p = Path.top();
		Path.pop();
		printf("-> (%d,%d) ", p.first, p.second);

		res += grid[p.first][p.second];
	}

	std::cout << std::endl;
	std::cout << res << std::endl;

	return;
}

// A Function to find the shortest path between
// a given source cell to a destination cell according
// to A* Search Algorithm
void aStarSearch(const auto& grid, Pair src, Pair dest)
{
	// If the source is out of range
	if (isValid(grid, src.first, src.second) == false) {
		printf("Source is invalid\n");
		return;
	}

	// If the destination is out of range
	if (isValid(grid, dest.first, dest.second) == false) {
		printf("Destination is invalid\n");
		return;
	}

	// Either the source or the destination is blocked
	if (isUnBlocked(grid, src.first, src.second) == false
		|| isUnBlocked(grid, dest.first, dest.second)
			   == false) {
		printf("Source or the destination is blocked\n");
		return;
	}

	// If the destination cell is the same as source cell
	if (isDestination(src.first, src.second, dest)
		== true) {
		printf("We are already at the destination\n");
		return;
	}

	// Create a closed list and initialise it to false which
	// means that no cell has been included yet This closed
	// list is implemented as a boolean 2D array
	std::vector<std::vector<int>> closedList(grid.size(), std::vector<int>(grid[0].size(), 0));

	// Declare a 2D array of structure to hold the details
	// of that cell
	std::vector<std::vector<cell>> cellDetails(grid.size(), std::vector<cell>(grid[0].size()));

	int i, j;

	for (i = 0; i < cellDetails.size(); i++) {
		for (j = 0; j < cellDetails[0].size(); j++) {
			cellDetails[i][j].f = 9999;
			cellDetails[i][j].g = 9999;
			cellDetails[i][j].h = 9999;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	// Initialising the parameters of the starting node
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	set<pPair> openList;

	// Put the starting cell on the open list and set its
	// 'f' as 0
	openList.insert(make_pair(0.0, make_pair(i, j)));

	// We set this boolean value as false as initially
	// the destination is not reached.
	bool foundDest = false;

	while (!openList.empty()) {
		pPair p = *openList.begin();

		// Remove this vertex from the open list
		openList.erase(openList.begin());

		// Add this vertex to the closed list
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		// To store the 'g', 'h' and 'f' of the 8 successors
		double gNew, hNew, fNew;

		int amount_of_times_in_this_direction = 0;
		std::vector<std::pair<int, int>> offsets = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

		for (auto offset : offsets) {
			// Only process this cell if this is a valid one
			auto new_i = i + offset.first;
			auto new_j = j + offset.second;
			auto amount_of_times_in_same_direction = 0;
			if (i != 0 || j != 0) {
				auto old_direction = cellDetails[i][j].direction;
				amount_of_times_in_same_direction = old_direction == offset ? cellDetails[i][j].amount_of_times_in_direction + 1 : 0;
			}
			if (isValid(grid, new_i, new_j) == true) {
				// If the destination cell is the same as the
				// current successor
				if (isDestination(new_i, new_j, dest) == true) {
					// Set the Parent of the destination cell
					cellDetails[new_i][new_j].parent_i = i;
					cellDetails[new_i][new_j].parent_j = j;
					printf("The destination cell is found\n");
					tracePath(grid, cellDetails, dest);
					foundDest = true;
					return;
				}
				// If the successor is already on the closed
				// list or if it is blocked, then ignore it.
				// Else do the following
				else if (closedList[new_i][new_j] == false
						 && isUnBlocked(grid, new_i, new_j)
								== true && amount_of_times_in_same_direction < 3) {
					gNew = cellDetails[i][j].g + grid[i][j];
					hNew = calculateHValue(grid, new_i, new_j, amount_of_times_in_same_direction, dest);
					fNew = gNew + hNew;

					// If it isn’t on the open list, add it to
					// the open list. Make the current square
					// the parent of this square. Record the
					// f, g, and h costs of the square cell
					//                OR
					// If it is on the open list already, check
					// to see if this path to that square is
					// better, using 'f' cost as the measure.
					if (cellDetails[new_i][new_j].f > fNew) {
						openList.insert(make_pair(fNew, make_pair(new_i, new_j)));

						// Update the details of this cell
						cellDetails[new_i][new_j].f = fNew;
						cellDetails[new_i][new_j].g = gNew;
						cellDetails[new_i][new_j].h = hNew;
						cellDetails[new_i][new_j].parent_i = i;
						cellDetails[new_i][new_j].parent_j = j;
						cellDetails[new_i][new_j].direction = offset;
						cellDetails[new_i][new_j].amount_of_times_in_direction = amount_of_times_in_same_direction;
						std::cout << new_i << " " << new_j << " from: " << i << " " << j << " ";
						std::cout << amount_of_times_in_same_direction << std::endl;
					}
				}
			}
		}
	}

	// When the destination cell is not found and the open
	// list is empty, then we conclude that we failed to
	// reach the destination cell. This may happen when the
	// there is no way to destination cell (due to
	// blockages)
	if (foundDest == false)
		printf("Failed to find the Destination Cell\n");

	return;
}*/

struct next_point {
	int64_t y, x;
	std::pair<int, int> direction;
	uint8_t times_in_current_direction;

	bool operator<(const next_point& ref) const {
		return y < ref.y ? true : y == ref.y && x < ref.x ? true : false;
	}
};

void update_neighbors(const auto &mat, auto &neighbors, auto &visited, const next_point& current) {
	std::vector<std::pair<int, int>> possible_directions = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
	neighbors.clear();

	for (auto& d : possible_directions) {
		if ((d.first != -current.direction.first || d.second != -current.direction.second) && (current.times_in_current_direction < 3 || d != current.direction)) {
			if (current.y + d.first < 0 || current.y + d.first >= mat.size()) continue;
			if (current.x + d.second < 0 || current.x + d.second >= mat[0].size()) continue;
			neighbors.push_back({ .y = current.y + d.first,.x = current.x + d.second,.direction = d,.times_in_current_direction = (d == current.direction ? current.times_in_current_direction + 1 : 0) });
		}
	}
}

void update_point_value(auto &mat, const auto &point, size_t curr_i, size_t curr_j) {
	auto new_potential_point_weight = mat[curr_i][curr_j].second + mat[point.y][point.x].first;
	if (new_potential_point_weight < mat[point.y][point.x].second) {
		mat[point.y][point.x].second = new_potential_point_weight;
	}
}

std::size_t shortest_path_value(auto &points) {
	std::vector<next_point> neighbors;
	std::set<next_point> next_to_visit;
	std::set<std::pair<std::pair<int, int>, std::pair<size_t, size_t>>> visited;
	points[0][0].second = 0;
	next_point current = { .y = 0,.x = 0,.direction = {0, 0},.times_in_current_direction = 0 };
	next_to_visit.insert(current);
	while (!next_to_visit.empty()) {
		std::cout << current.y << " " << current.x << " " << (int)current.times_in_current_direction << std::endl;
		visited.insert({ current.direction, {current.y, current.x} });
		update_neighbors(points, neighbors, visited, current);
		for (auto &point : neighbors) {
			if (visited.find({ point.direction, {point.y, point.x} }) != visited.end()) continue;
			update_point_value(points, point, current.y, current.x);
			next_to_visit.insert(point);
		}

		next_to_visit.erase(current);

		auto curr_i = current.y;
		auto curr_j = current.x;

		if (curr_i == points.size() - 1 && curr_j == points[0].size() - 1) break;

		size_t min_distance = -1;
		for (auto& next_point : next_to_visit) {
			if (points[next_point.y][next_point.x].second < min_distance) {
				min_distance = points[next_point.y][next_point.x].second;
				current = next_point;
			}
		}
	}
	return points.at(points.size() - 1).at(points.at(points.size() - 1).size() - 1).second;
}

int first_part_2023_17() {
	std::vector<std::vector<std::pair<int, size_t>>> grid;
	std::for_each(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>(), [&grid](std::string str) {
		grid.emplace_back();
		for (char c : str) {
			grid.back().emplace_back(c - '0', -1);
		}
		});

	std::cout << shortest_path_value(grid) << std::endl;

	/*// Source is the left-most bottom-most corner
	Pair src = make_pair(0, 0);

	// Destination is the left-most top-most corner
	Pair dest = make_pair(grid.size() - 1, grid[0].size() - 1);

	aStarSearch(grid, src, dest);*/
    return EXIT_SUCCESS;
}

int second_part_2023_16() {
    
    return EXIT_SUCCESS;
}

#endif