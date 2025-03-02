#include "Map.h"

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    deleteNodes(root);
}
void Map::deleteNodes(MapNode* node)
{
    if (node == nullptr)
    {
        return;
    }

    deleteNodes(node->left);
    deleteNodes(node->right);
    delete node;
}

void Map::initializeMap(std::vector<Isle *> isles)
{
    for (Isle *isle : isles)
    {
        insert(isle);
        rebalance_count=0;
    }
    populateWithItems();
}

MapNode *Map::rotateRight(MapNode *current)
{
    if (current == nullptr || current->left == nullptr)
    {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    if (current == nullptr || current->right == nullptr)
    {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    if (!(node == nullptr))
    {
        return node->height;
    }
    return 0;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    if (node == nullptr)
    {
        return new MapNode(isle);
    }

    if (*isle < *(node->isle))
    {
        node->left = insert(node->left, isle);
    }
    else if (*isle > *(node->isle))
    {
        node->right = insert(node->right, isle);
    }
    else
    {
        return node;
    }

    node->height =  1+std::max(height(node->left), height(node->right));

    int balance = height(node->left) - height(node->right);

    if (balance > 1 && *isle < *(node->left->isle))
    {
        node= rotateRight(node);
        rebalance_count++;
    }

    else if (balance < -1 && *isle > *(node->right->isle))
    {
        node= rotateLeft(node);
        rebalance_count++;
    }

    else if (balance > 1 && *isle > *(node->left->isle))
    {
        node->left = rotateLeft(node->left);
        node= rotateRight(node);
        rebalance_count++;
    }

    else if (balance < -1 && *isle < *(node->right->isle))
    {

        node->right = rotateRight(node->right);
        node= rotateLeft(node);
        rebalance_count++;
    }

    return node;
}

void Map::insert(Isle *isle)
{
    root = insert((root), isle);
    if (rebalance_count%3==0&& rebalance_count!=0){
        populateWithItems();
        dropItemBFS();
        rebalance_count=0;
    }
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    if (node == nullptr)
    {
        return node;
    }

    if (*isle < *(node->isle))
    {
        node->left = remove(node->left, isle);
    }
    else if (*isle > *(node->isle))
    {
        node->right = remove(node->right, isle);
    }
    else
    {
        if (node->left == nullptr || node->right == nullptr)
        {
            MapNode* temp = node->left ? node->left : node->right;
            if (temp == nullptr)
            {
                temp = node;
                node = nullptr;
            }
            else
            {
                *node = *temp;
            }
        }
        else
        {
            MapNode *temp = maxNode(node->left);
            node->isle = temp->isle;
            node->left = remove(node->left, temp->isle);
        }
    }

    if (node == nullptr)
    {
        return node;
    }

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int balance = height(node->left) - height(node->right);

    if (balance > 1 && height(node->left->left) >= height(node->left->right))
    {
        node= rotateRight(node);
        rebalance_count++;
    }

    else if (balance > 1 && height(node->left->left) < height(node->left->right))
    {
        node->left = rotateLeft(node->left);
        node= rotateRight(node);
        rebalance_count++;
    }

    else if (balance < -1 && height(node->right->right) >= height(node->right->left))
    {
        node= rotateLeft(node);
        rebalance_count++;
    }

    else if (balance < -1 && height(node->right->right) < height(node->right->left))
    {
        node->right = rotateRight(node->right);
        node= rotateLeft(node);
        rebalance_count++;
    }
    return node;
}

MapNode *Map::maxNode(MapNode *node)
{
    MapNode *current = node;

    while (current->right != nullptr)
    {
        current = current->right;
    }

    return current;
}

void Map::remove(Isle *isle)
{
    root = remove((root), isle);
    if (rebalance_count%3==0&& rebalance_count!=0){
        populateWithItems();
        dropItemBFS();
        rebalance_count=0;
    }
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    if (current == nullptr)
    {
        return;
    }

    count++;
    if (count % 5 == 0)
    {
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
        current->isle->setItem(EINSTEINIUM);
    }

    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    if (current == nullptr)
    {
        return;
    }

    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);

    count++;
    if (count % 3 == 0)
    {
        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
        current->isle->setItem(GOLDIUM);
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    std::queue<MapNode *> nodes;
    nodes.push(node);

    while (!nodes.empty())
    {
        MapNode *current = nodes.front();
        nodes.pop();

        if (current->isle->getItem() == EMPTY)
        {
            return current;
        }

        if (current->left != nullptr)
        {
            nodes.push(current->left);
        }

        if (current->right != nullptr)
        {
            nodes.push(current->right);
        }
    }

    return nullptr;
}

void Map::dropItemBFS()
{
        MapNode *targetNode = findFirstEmptyIsle(root);
        if (targetNode != nullptr)
        {
            targetNode->isle->setItem(AMAZONITE);
            std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
        }
        else
        {
            std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
        }
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{

    if (!(node == nullptr))
    {
        int depth = 0;
        MapNode *current = root;

        while (current != nullptr)
        {
            if (current == node)
            {
                return depth;
            }
            else if (*node->isle < *(current->isle))
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
            depth++;
        }
    }
    return -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    MapNode *node = findNode(*isle);
    if (node != nullptr)
    {
        return getDepth(node);
    }
    return -1;
}

int Map::getDepth()
{
    if (root != nullptr)
    {
        return height(root)-1;
    }
    return -1;
}

void Map::populateWithItems()
{
    int postOrderCount = 0;
    postOrderItemDrop(root, postOrderCount);

    int preOrderCount = 0;
    preOrderItemDrop(root, preOrderCount);
}

Isle *Map::findIsle(Isle isle)
{
    MapNode *node = findNode(isle);
    if (node != nullptr)
    {
        return node->isle;
    }
    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    MapNode *current = root;

    while (current != nullptr)
    {
        if (current->isle->getName() == name)
        {
            return current->isle;
        }
        else if (current->isle->getName() < name)
        {
            current = current->right;
        }
        else
        {
            current = current->left;
        }
    }
    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    MapNode *current = root;

    while (current != nullptr)
    {
        if (*current->isle == isle)
        {
            return current;
        }
        else if (*current->isle < isle)
        {
            current = current->right;
        }
        else
        {
            current = current->left;
        }
    }
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    MapNode *current = root;

    while (current != nullptr)
    {
        if (current->isle->getName() == name)
        {
            return current;
        }
        else if (current->isle->getName() < name)
        {
            current = current->right;
        }
        else
        {
            current = current->left;
        }
    }
    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename)
{
    std::ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        return;
    }

    std::queue<MapNode *> isles;
    isles.push(root);

    while (!isles.empty())
    {
        int levelSize = isles.size();
        bool levelEmpty = true;
        std::vector<std::string> levelIsles;
        for (int i = 0; i < levelSize; ++i)
        {
            MapNode *current = isles.front();
            isles.pop();

            if (current != nullptr)
            {
                levelIsles.push_back(current->isle->getName());
                isles.push(current->left);
                isles.push(current->right);
                levelEmpty = false;
            }
            else
            {
                levelIsles.push_back("NULL");
            }
        }
        if (!levelEmpty) {
            for ( const std::string &isle : levelIsles)
            {
                outputFile << isle << " ";
            }
            outputFile << std::endl;
        }
    }
    outputFile.close();
}

void Map::writeIslesToFile(const std::string &filename)
{
    std::ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        return;
    }

    std::vector<Isle*> isles;
    inOrderTraversal(root, isles);

    for (Isle* isle : isles)
    {
        outputFile << isle->getName() << std::endl;
    }

    outputFile.close();
    std::cout << "[Output] Isles have been written to " << filename << " in alphabetical order." << std::endl;
}
void Map::inOrderTraversal(MapNode* node, std::vector<Isle*>& isles)
{
    if (node == nullptr)
    {
        return;
    }

    inOrderTraversal(node->left, isles);
    isles.push_back(node->isle);
    inOrderTraversal(node->right, isles);
}