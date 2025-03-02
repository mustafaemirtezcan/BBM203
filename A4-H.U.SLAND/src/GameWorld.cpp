#include "GameWorld.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;
    if (realmShaper == nullptr) {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return hasAccess;
    }
    int player_depth=shaperTree.getDepth(realmShaper);
    if (player_depth == -1) {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return hasAccess;
    }
    int shaper_tree_height=shaperTree.getDepth();
    int total_map_depth=mapTree.getDepth();
    int minMapDepthAccess=mapTree.calculateMinMapDepthAccess( player_depth, shaper_tree_height, total_map_depth);
    int isle_depth=mapTree.getIsleDepth(isle);
    hasAccess = (isle_depth >= minMapDepthAccess);
    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    if (!hasAccess(realmShaper, isle)){
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << isle->getName() <<"."<<std::endl;
        return;
    }
    if (realmShaper->currentIsle!=nullptr&&realmShaper->currentIsle!=isle){
        realmShaper->currentIsle->decreaseShaperCount();
    }
    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    realmShaper->collectItem(isle->getItem());
    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    if (realmShaper->currentIsle!=isle) {
        if(isle->increaseShaperCount()) {
            std::cout << "[Owercrowding] " << isle->getName() << "self-destructed, it will be removed from the map" << std::endl;
            mapTree.remove(isle);
        }
    }
    realmShaper->currentIsle=isle;
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    std::string shaperName = shaper->getName();
    int shaperEnergyLevel = shaper->getEnergyLevel();
    if (shaper->hasEnoughEnergy()) {
        Isle* newIsle = new Isle(isleName);
        shaper->loseEnergy();
        std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
        mapTree.insert( newIsle);
        std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    }
    else {
        std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;
    }
}

void GameWorld::displayGameState()
{   //mapTree.displayMap();
    //shaperTree.displayTree();
}


void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);
    std::string skipLine;
    std::getline(accessFile, skipLine);
    std::getline(duelFile, skipLine);
    std::string accessLine;
    int duelCount = 0;
    while (std::getline(accessFile, accessLine)) {
        duelCount++;
        std::istringstream iss(accessLine);
        std::string shaperName;
        std::string isleName;
        iss >> shaperName >> isleName;
        RealmShaper* shaper = shaperTree.findPlayer(shaperName);
        Isle* isle = mapTree.findIsle(isleName);
        if (isle == nullptr) {
            craft(shaper, isleName);
        } else {
            exploreArea(shaper, isle);
        }
        if (duelCount == 5) {
            std::string duelLine;
            if (std::getline(duelFile, duelLine)) {
                std::istringstream duelIss(duelLine);
                std::string challenger;
                std::string resultLog;
                duelIss >> challenger >> resultLog;
                RealmShaper* challengerShaper = shaperTree.findPlayer(challenger);
                bool result = (resultLog == "1");
                if (challengerShaper != nullptr) {
                    shaperTree.duel(challengerShaper, result);
                }
            }
            duelCount = 0;
        }
        displayGameState();
    }
    std::string duelLine;
    while (std::getline(duelFile, duelLine)) {
        std::istringstream iss(duelLine);
        std::string challenger;
        std::string resultLog;
        iss >> challenger >> resultLog;
        RealmShaper* challengerShaper = shaperTree.findPlayer(challenger);
        bool result = (resultLog == "1");
        if ( challengerShaper != nullptr) {
            shaperTree.duel(challengerShaper, result);
        }
        displayGameState();
    }
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}