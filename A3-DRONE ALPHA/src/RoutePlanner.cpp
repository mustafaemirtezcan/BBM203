#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include <limits>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    map.loadDistanceData(distance_data);
    map.resetVisited();
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if(file.is_open()){
      char one_char;
      std::string province_number;
      while(file.get(one_char)){
        if(one_char == '('){
          province_number.clear();
          while(file.get(one_char)&&one_char != ')'){
              province_number+=one_char;
            }
            if (numPriorityProvinces < MAX_PRIORITY_PROVINCES) {
                priorityProvinces[numPriorityProvinces] = std::stoi(province_number);
                numPriorityProvinces++;
            }
      }
    }
        file.close();
        if (numPriorityProvinces==0) {
            file.open(filename);
            while(file>>province_number){
                if (numPriorityProvinces < MAX_PRIORITY_PROVINCES) {
                    priorityProvinces[numPriorityProvinces] = std::stoi(province_number);
                    numPriorityProvinces++;
                }
            }
            file.close();
        }

    }
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
  std::ifstream file(filename);
    if(file.is_open()){
        char one_char;
        std::string province_number;
        while(file.get(one_char)){
        if(one_char == '('){
            province_number.clear();
            while(file.get(one_char)&&one_char != ')'){
                province_number+=one_char;
        }
            if (numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {
                weatherRestrictedProvinces[numWeatherRestrictedProvinces] = std::stoi(province_number);
                numWeatherRestrictedProvinces++;
            }
        }
    }
        file.close();
        if (numWeatherRestrictedProvinces==0) {
            file.open(filename);
            while(file>>province_number){
                if (numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {
                    weatherRestrictedProvinces[numWeatherRestrictedProvinces] = std::stoi(province_number);
                    numWeatherRestrictedProvinces++;
                }
            }
            file.close();
        }
    }
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for(int i = 0; i < numPriorityProvinces; i++){
        if(priorityProvinces[i] == province){
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for(int i = 0; i < numWeatherRestrictedProvinces; i++){
        if(weatherRestrictedProvinces[i] == province){
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
  stack.push(startingCity);
  route.push_back(startingCity);
  map.markAsVisited(startingCity);
  while(isExplorationComplete() == false){
      exploreFromProvince(stack.peek());
      backtrack();
  }
    displayResults();
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    enqueueNeighbors(province);
  int dequeued_number;
    while((!queue.isEmpty())&&(dequeued_number = queue.dequeue())!=-1) {
        if(!isWeatherRestricted(dequeued_number)){
            if(map.isWithinRange(province, dequeued_number, maxDistance) && !map.isVisited(dequeued_number)){
                totalDistanceCovered+=map.getDistance(province,dequeued_number);
                stack.push(dequeued_number);
                route.push_back(dequeued_number);
                map.markAsVisited(dequeued_number);
                exploreFromProvince(dequeued_number);
            }
        }
        else {
            std::cout << "Province " << cities[dequeued_number] << " is weather-restricted. Skipping." << std::endl;
        }
    }

}

void RoutePlanner::enqueueNeighbors(int province) {
    for(int i = 0; i < 81; i++){
        if (map.isWithinRange(province, i, maxDistance)) {
            if (isPriorityProvince(i)) {
                queue.enqueuePriority(i);
            } else {
                queue.enqueue(i);
            }
        }
    }
}

void RoutePlanner::backtrack() {
    if(stack.isEmpty()){
  	  return;
  }
    totalDistanceCovered+=map.getDistance(stack.pop(),stack.peek());
}

bool RoutePlanner::isExplorationComplete() const {
    if(stack.isEmpty()&&queue.isEmpty()){
      return true;
    }
    return false;
}

void RoutePlanner::displayResults() const {
    std::cout<<"----------------------------"<<std::endl;
    std::cout << "Journey Completed!" << std::endl;
    std::cout<<"----------------------------"<<std::endl;
    std::cout << "Total Number of Provinces Visited: " << route.size() << std::endl;
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km" << std::endl;
    std::cout << "Route Taken: "<<std::endl;
    for(int i = 0; i < route.size(); i++){
      std::cout<<cities[route[i]]<<" -> ";
    }
    std::cout<<"End"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Priority Provinces Status:"<<std::endl;
    int count=0;
    for(int i = 0; i < numPriorityProvinces; i++){
      if(map.isVisited(priorityProvinces[i])){
        std::cout<<"- "<<cities[priorityProvinces[i]]<<" (Visited)"<<std::endl;
          count++;
      }
      else{
        std::cout<<"- "<<cities[priorityProvinces[i]]<<" (Not Visited)"<<std::endl;
      }
    }
    std::cout<<std::endl;
    std::cout<<"Total Priority Provinces Visited: "<<count<<" out of "<<numPriorityProvinces<<std::endl;
    if(count == numPriorityProvinces){
      std::cout<<"Success: All priority provinces were visited."<<std::endl;
    }
    else{
      std::cout<<"Warning: Not all priority provinces were visited."<<std::endl;
    }
    std::cout<<std::endl;
}


