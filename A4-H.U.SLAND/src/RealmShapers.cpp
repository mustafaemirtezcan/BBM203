#include "RealmShapers.h"
#include <cmath>
#include <algorithm>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    for (RealmShaper* shaper : realmShapers)
    {
        delete shaper;
    }
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    for (RealmShaper* shaper : shapers)
    {
        insert(shaper);
    }
}

int ShaperTree::getSize()
{
    if (realmShapers.size() >= 0)
    {
        return realmShapers.size();
    }
    return 0;
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;
    if ( index >= 0 && index < realmShapers.size() )
    {
        isValid = true;
    }
    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    int index = findIndex(shaper);
    if (index != -1)
    {
        delete realmShapers[index];
        realmShapers.erase(realmShapers.begin() + index);
        return index;
    }
    return -1;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    for (int i = 0; i < realmShapers.size(); ++i) {
        if (realmShapers[i] == shaper)
        {
            return i;
        }
    }
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    int index= findIndex(shaper);
    if (index != -1)
    {
        return (int)log2(index + 1);
    }
    return -1;
}

int ShaperTree::getDepth()
{
    if (realmShapers.size() > 0)
    {
        return (int)log2(realmShapers.size());
    }
    return 0;
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    int challengerIndex = findIndex(challenger);
    if (challengerIndex == -1) {
        return *challenger;
    }

    RealmShaper *parent = getParent(challenger);
    if (parent== nullptr) {
        return *challenger;
    }

    std::string challengerName = challenger->getName();
    std::string parentName = parent->getName();

    if (result) {
        challenger->gainHonour();
        parent->loseHonour();
        replace(challenger, parent);

        std::cout << "[Duel] " << challengerName << " won the duel" << std::endl;
        std::cout << "[Honour] New honour points: " << challengerName << "-" << challenger->getHonour() << " " << parentName << "-" << parent->getHonour() << std::endl;

        if (parent->getHonour() <= 0) {
            std::cout << "[Duel] " << parentName << " lost all honour, delete" << std::endl;
            remove(parent);
        }
        return *challenger;
    } else {

        parent->gainHonour();
        challenger->loseHonour();

        std::cout << "[Duel] " << challengerName << " lost the duel" << std::endl;
        std::cout << "[Honour] New honour points: " << challengerName << " - " << challenger->getHonour() << " " << parentName << " - " << parent->getHonour() << std::endl;

        if (challenger->getHonour() <= 0) {
            std::cout << "[Duel] " << challengerName << " lost all honour, delete" << std::endl;
            remove(challenger);
        }

        return *parent;
    }
}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;
    int index = findIndex(shaper);
    if (isValidIndex(index)) {
        int parentIndex = (index - 1) / 2;
        if (isValidIndex(parentIndex)) {
            parent = realmShapers[parentIndex];
        }
    }
    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    int lowIndex = findIndex(player_low);
    int highIndex = findIndex(player_high);

    if (lowIndex != -1 && highIndex != -1) {
        realmShapers[lowIndex] = player_high;
        realmShapers[highIndex] = player_low;
    }
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;
    for (RealmShaper* realm_shaper : realmShapers)
    {
        if (*realm_shaper == shaper)
        {
            foundShaper = realm_shaper;
            break;
        }
    }
    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;
    for (RealmShaper* realm_shaper : realmShapers)
    {
        if (realm_shaper->getName()==name)
        {
            foundShaper= realm_shaper;
            break;
        }
    }
    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    inorder(index,result);
    return result;
}

void ShaperTree::inorder(int index, std::vector<std::string> &result) {
    if (isValidIndex(index)) {
        inorder(2*index+1,result);
        result.push_back(realmShapers[index]->getName());
        inorder(2*index+2,result);
    }
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    preorder(index,result);
    return result;
}
void ShaperTree::preorder(int index, std::vector<std::string> &result) {
    if (isValidIndex(index)) {
        result.push_back(realmShapers[index]->getName());
        preorder(2*index+1,result);
        preorder(2*index+2,result);
    }
}
std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    postorder(index, result);
    return result;
}

void ShaperTree::postorder(int index, std::vector<std::string> &result) {
    if (isValidIndex(index)) {
        postorder(2*index+1,result);
        postorder(2*index+2,result);
        result.push_back(realmShapers[index]->getName());
    }
}
void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    std::vector<std::string> result = {};
    preorder(index,result);
    for (const std::string &name : result) {
        outFile << name << std::endl;
    }
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    for (RealmShaper* realmShaper : realmShapers) {
        outFile << realmShaper->getName() << std::endl;
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2*index+1;
    int right = 2*index+2;

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    breadthFirstTraversal(outFile);
    std::cout<< "[Output] " << "Shapers have been written to " << filename << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    preOrderTraversal(0, outFile);
    std::cout<< "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
}
