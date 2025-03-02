#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
        for (int j = 0; j < MAX_SIZE; j++) {
            distanceMatrix[i][j] = -1;
        }
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    if (!file.is_open()) {
        std::cout << "Error: File could not be opened. " << filename << std::endl;
        return;
    }
    int i = 0;
    while (std::getline(file, line)) {
        std::stringstream province(line);
        std::string distance;
        int j = 0;
        while (std::getline(province, distance, ',')) {
            distanceMatrix[i][j] = (i == j) ? 0 : std::stoi(distance);
            j++;
        }
        i++;
    }
    file.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    if(provinceA>=0 && provinceB>=0 && provinceA<MAX_SIZE && provinceB<MAX_SIZE){
        if(distanceMatrix[provinceA][provinceB]!=-1 && distanceMatrix[provinceA][provinceB]<=maxDistance){
            return true;
        }
    }
    return false; 
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    if (province >= 0 && province < MAX_SIZE) {
        visited[province] = true;
    }
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
  if(province>=0 && province<MAX_SIZE){
        if(visited[province] == true){
            return true;
        }
    }
    return false;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
  int count=0;
  for(int i=0;i<MAX_SIZE;i++){
        if(visited[i]){
        count++;    }
    }
    if(count>0){
        return count;
    }
    return 0;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
  if(provinceA>=0 && provinceB>=0 && provinceA<MAX_SIZE && provinceB<MAX_SIZE){
  if(distanceMatrix[provinceA][provinceB]!=-1){
        return distanceMatrix[provinceA][provinceB];
    }}
    return 0;
}