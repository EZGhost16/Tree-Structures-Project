// UMBC - CMSC 341 - Fall 2024 - Proj2
// Name: Eden Zhang
// Section: 5
// Professor: Sanorita Dey
// E-Mail: ezhang1@umbc.edu
// Date: 10/12/2024
// Description: 
//      Function definitions for swarm.cpp
//  */
#include "swarm.h"
Swarm::Swarm(){
    // Initialize to default values
    m_root = nullptr;
    m_type = NONE;
}

Swarm::Swarm(TREETYPE type){
    // Initializes the tree
    m_root = nullptr;
    m_type = type;
}

Swarm::~Swarm(){
    clear();
}

void Swarm::clear(){
    // Running helper function that recursively deletes
    // Robot nodes in the tree
    deleteRobots(m_root);

    // Resetting values to default empty object
    m_root = nullptr;
    m_type = NONE;
}

void Swarm::insert(const Robot& robot){
    // Guards to prevent invalid ids from entering the tree //
    // List is empty
    if (m_root == nullptr) {
        // Checks that the id is valid
        if (robot.getID() >= MINID && robot.getID() <= MAXID) {
            m_root = new Robot(robot.getID(), robot.getType(), robot.getState());
        }
    } else {
        // List has existing nodes
        if (robot.getID() >= MINID && robot.getID() <= MAXID) {
            // Appropriate balancing for Tree Types
            if (m_type == BST) {
                m_root = insertRobotBST(m_root, robot.getID(), robot.getType(), robot.getState());
            } else if (m_type == AVL) {
                m_root = insertRobotAVL(m_root, robot.getID(), robot.getType(), robot.getState());
            } else if (m_type == SPLAY) {
                m_root = insertRobotSplay(m_root, robot.getID(), robot.getType(), robot.getState());
            }        
        }
    }
}

void Swarm::remove(int id){
    // Removes the target ID Robot
    if (m_type == BST) {
        m_root = removeRobotBST(m_root, id);
    } else if (m_type == AVL) {
        m_root = removeRobotAVL(m_root, id);
    }
    // If m_type is SPLAY, then tree is left unmodified
}

const Swarm & Swarm::operator=(const Swarm & rhs){
    // Only do things when operands are not already equal
    if (this != &rhs) {
        // Clearing out the current tree
        clear();

        // Check rhs isn't empty
        if (rhs.m_type != NONE) {
            // Copy variables
            m_type = rhs.m_type;

            // Traverse through the tree and create copies of each node
            m_root = recursiveCopy(rhs.m_root);
        }
    }
    return *this;
}

TREETYPE Swarm::getType() const{
    return m_type;
}

void Swarm::setType(TREETYPE type){
    // Check that the type is different, otherwise nothing changes
    if (m_type != type) {
        m_type = type;

        // Restructure the tree
        if (m_type == AVL) {
            // Check tree isn't empty
            if (m_root != nullptr) {
                // Collect all the nodes from the current tree in an array
                // 1. Get the size of the tree to establish the array
                const int NUMNODES = treeSize(m_root);
                Robot* botsArr[NUMNODES];
                int i = 0;
                collectRobots(m_root, botsArr, i);

                // Set pointer to null for security
                for (int i = 0; i < NUMNODES; i++) {
                    // Setting each node at default state with no initial connections
                    botsArr[i]->setLeft(nullptr);
                    botsArr[i]->setRight(nullptr);
                    botsArr[i]->setHeight(DEFAULT_HEIGHT);
                }

                // Rebuild Tree with AVL
                Robot* newRoot = nullptr;
                for (int i = 0; i < NUMNODES; i++) {
                    // Insert
                    newRoot = avlRebalance(newRoot, botsArr[i]);
                }

                // Setting the new AVL tree as original tree
                m_root = newRoot;
            }
        } else if (m_type == NONE) {
            clear();
        }
    }
}

void Swarm::dumpTree() const {
    dump(m_root);
}

void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}

// insertRobotBST
// Recursively goes through the path on the tree and inserts the node
// If a duplicate is found, then inserts nothing
Robot* Swarm::insertRobotBST(Robot* robot, int id, ROBOTTYPE type, STATE state) {
    // Base case, when the right empty position is found
    // Empty tree
    if (robot == nullptr)
        return new Robot(id, type, state);
    
    // If a duplicate is found
    if (robot->getID() == id)
        return robot;

    // Traverse through the tree until the correct position is found
    if (robot->getID() < id)
        robot->setRight(insertRobotBST(robot->getRight(), id, type, state));
    else
        robot->setLeft(insertRobotBST(robot->getLeft(), id, type, state));

    // Marking the Height for each Node
    robot->setHeight(1 + max(findHeight(robot->getLeft()), findHeight(robot->getRight())));

    return robot;
}

// insertRobotAVL
// Recursively goes through the path on the tree and inserts the node
// If a duplicate is found, then inserts nothing
// Additional Height Balancing
Robot* Swarm::insertRobotAVL(Robot* robot, int id, ROBOTTYPE type, STATE state) {
    // Base case, when the right empty position is found
    // Empty tree
    if (robot == nullptr)
        return new Robot(id, type, state);
    
    // If a duplicate is found
    if (robot->getID() == id)
        return robot;

    // Traverse through the tree until the correct position is found
    if (robot->getID() < id)
        robot->setRight(insertRobotAVL(robot->getRight(), id, type, state));
    else
        robot->setLeft(insertRobotAVL(robot->getLeft(), id, type, state));

    // Updating Height
    robot->setHeight(1 + max(findHeight(robot->getLeft()), findHeight(robot->getRight())));

    // Getting the Balance Factor
    int balFactor = getBalanceFactor(robot);

    // Balancing
    // 4 Different Cases if balance factor is unbalanced
    // Left Left Case
    if (balFactor > 1 && id < robot->getLeft()->getID())
        return rightRotation(robot);

    // Right Right Case
    if (balFactor < -1 && id > robot->getRight()->getID())
        return leftRotation(robot);
    
    // Left Right Case
    if (balFactor > 1 && id > robot->getLeft()->getID()) {
        robot->setLeft(leftRotation(robot->getLeft()));
        return rightRotation(robot);
    }

    // Right Left Case
    if (balFactor < -1 && id < robot->getRight()->getID()) {
        robot->setRight(rightRotation(robot->getRight()));
        return leftRotation(robot);
    }

    return robot;
} 

// insertRobotAVL
// Recursively goes through the path on the tree and inserts the node
// If a duplicate is found, then inserts nothing
// Brings recent insertion node to the root node
Robot* Swarm::insertRobotSplay(Robot* robot, int id, ROBOTTYPE type, STATE state) {
    // Base case, when the right empty position is found
    // Empty tree
    if (robot == nullptr)
        return new Robot(id, type, state);

    // Splaying
    robot = splaying(robot, id);
    
    // If a duplicate is found
    if (robot->getID() == id)
        return robot;

    // Creating new robot
    Robot* newBot = new Robot(id, type, state);

    // New robot id smaller than root, place on left
    if (robot->getID() > id) {
        newBot->setRight(robot);
        newBot->setLeft(robot->getLeft());
        robot->setLeft(nullptr);
    } else {
        // New bot id greater than root, place on right
        newBot->setLeft(robot);
        newBot->setRight(robot->getRight());
        robot->setRight(nullptr);
    }

    // Updating Heights
    robot->setHeight(1 + max(findHeight(robot->getLeft()), findHeight(robot->getRight())));
    newBot->setHeight(1 + max(findHeight(newBot->getLeft()), findHeight(newBot->getRight())));

    return newBot;
}

// findHeight
// Helper func returns height of given robot node
// Additional check for leaf nodes edge case
int Swarm::findHeight(Robot* robot) {
    // Check if the parent node is a leaf
    if (robot == nullptr)
        return -1;
    return robot->getHeight();
}

// rightRotation
// Helper for the AVL Height balancing
Robot* Swarm::rightRotation(Robot* xBot) {
    // Set up for the rotation
    Robot* yBot = xBot->getLeft();
    Robot* r2 = yBot->getRight();

    // Perform Rotation
    yBot->setRight(xBot);
    xBot->setLeft(r2);

    // Updating Heights
    xBot->setHeight(1 + max(findHeight(xBot->getLeft()), findHeight(xBot->getRight())));
    yBot->setHeight(1 + max(findHeight(yBot->getLeft()), findHeight(yBot->getRight())));

    // New Root
    return yBot;
}

// leftRotation
// Helper for the AVL Height balancing
Robot* Swarm::leftRotation(Robot* xBot) {
    // Set up for the rotation
    Robot* yBot = xBot->getRight();
    Robot* r2 = yBot->getLeft();

    // Perform Rotation
    yBot->setLeft(xBot);
    xBot->setRight(r2);

    // Updating Heights
    xBot->setHeight(1 + max(findHeight(xBot->getLeft()), findHeight(xBot->getRight())));
    yBot->setHeight(1 + max(findHeight(yBot->getLeft()), findHeight(yBot->getRight())));

    // New Root
    return yBot;
}

// getBalanceFactor
// Returns the balance factor of given node
int Swarm::getBalanceFactor(Robot* bot) {
    // Leaf node
    if (bot == nullptr)
        return 0;
    return findHeight(bot->getLeft()) - findHeight(bot->getRight());
}

// removeRobotBST
// Recurses through the tree and removes the node
Robot* Swarm::removeRobotBST(Robot* root, int id) {
    // Base Case
    if (root == nullptr)
        return root;

    // Find where the target robot is
    // Less than, left
    if (id < root->getID())
        root->setLeft(removeRobotBST(root->getLeft(), id));
    // Greater than, right
    else if (id > root->getID())
        root->setRight(removeRobotBST(root->getRight(), id));
    else {
        // The target robot has been found, proceeds to remove
        // Case 1: Robot has 0-1 Child
        if ((root->getLeft() == nullptr) || (root->getRight() == nullptr)) {
            // Ternary operator to simply assign temp with whichever child exists, based on left child
            Robot* temp = root->getLeft() ? root->getLeft() : root->getRight();

            // Case: No Child
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else // Case: 1 Child
                *root = *temp;  // Copying new root with temp
            // Remove
            delete temp;
            temp = nullptr;
        } else {
            // Case 2: 2 Children
            // Find the min ID Robot on this side
            Robot* temp = getSuccessor(root->getRight());

            // Updatet the id placement
            root->setID(temp->getID());

            // Finally delete the robot
            root->setRight(removeRobotBST(root->getRight(), temp->getID()));
        }
    }

    // Base Case 2: If only 1 Child
    if (root == nullptr)
        return root;

    // Update Height
    root->setHeight(1 + max(findHeight(root->getLeft()), findHeight(root->getRight())));

    return root;
}

// removeRobotAVL
// Recurses through the tree and removes the node
// Retain balance
Robot* Swarm::removeRobotAVL(Robot* root, int id) {
    // Base Case
    if (root == nullptr)
        return root;

    // Find where the target robot is
    // Less than, left
    if (id < root->getID())
        root->setLeft(removeRobotAVL(root->getLeft(), id));
    // Greater than, right
    else if (id > root->getID())
        root->setRight(removeRobotAVL(root->getRight(), id));
    else {
        // The target robot has been found, proceeds to remove
        // Case 1: Robot has 0-1 Child
        if ((root->getLeft() == nullptr) || (root->getRight() == nullptr)) {
            // Ternary operator to simply assign temp with whichever child exists, based on left child
            Robot* temp = root->getLeft() ? root->getLeft() : root->getRight();

            // Case: No Child
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else // Case: 1 Child
                *root = *temp;  // Copying new root with temp
            // Remove
            delete temp;
            temp = nullptr;
        } else {
            // Case 2: 2 Children
            // Find the min ID Robot on this side
            Robot* temp = getSuccessor(root->getRight());

            // Updatet the id placement
            root->setID(temp->getID());

            // Finally delete the robot
            root->setRight(removeRobotAVL(root->getRight(), temp->getID()));
        }
    }

    // Base Case 2: If only 1 Child
    if (root == nullptr)
        return root;

    // Update Height
    root->setHeight(1 + max(findHeight(root->getLeft()), findHeight(root->getRight())));
    
    // Balancing
    int balFactor = getBalanceFactor(root);

    // If unbalanced, balances according to case
    // Left Left Case
    if (balFactor > 1 && getBalanceFactor(root->getLeft()) >= 0)
        return rightRotation(root);
    
    // Left Right Case
    if (balFactor > 1 && getBalanceFactor(root->getLeft()) < 0) {
        root->setLeft(leftRotation(root->getLeft()));
        return rightRotation(root);
    }

    // Right Right Case
    if (balFactor < -1 && getBalanceFactor(root->getRight()) <= 0)
        return leftRotation(root);

    // Right Left Case
    if (balFactor < -1 && getBalanceFactor(root->getRight()) > 0) {
        root->setRight(rightRotation(root->getRight()));
        return leftRotation(root);
    }
    
    return root;
}

// getSuccessor
// Helper func, goes through the tree
// Returns the robot with the smallest id in the current subtree
Robot* Swarm::getSuccessor(Robot* bot) {
    Robot* curr = bot;

    // Loop down tree, find leftmost leaf
    while (curr->getLeft() != nullptr)
        curr = curr->getLeft();
    return curr;
}

// splaying
// Helper func for Splay Trees
// The splaying action of bring recent insertion to root
Robot* Swarm::splaying(Robot* root, int id) {
    // Base Case: Root is empty or target robot is the root
    if (root == nullptr || root->getID() == id)
        return root;

    // Search for the target bot/node
    if (root->getID() > id) {
        // Key not found in the tree
        if (root->getLeft() == nullptr)
            return root;
        
        // Zig Zig Rotation
        if (root->getLeft()->getID() > id) {
            root->getLeft()->setLeft(splaying(root->getLeft()->getLeft(), id));
            root = rightRotation(root);
        } else if (root->getLeft()->getID() < id) {
            // Zig Zag Rotation
            root->getLeft()->setRight(splaying(root->getLeft()->getRight(), id));
            if (root->getLeft()->getRight() != nullptr)
                root->setLeft(leftRotation(root->getLeft()));
        }
        
        // Second rotation needed if not at end
        if (root->getLeft() == nullptr)
            return root;
        else
            return rightRotation(root);
    } else {
        // Right SubTree
        // Base Case
        if (root->getRight() == nullptr)
            return root;

        // Zig Zag Rotation
        if (root->getRight()->getID() > id) {
            root->getRight()->setLeft(splaying(root->getRight()->getLeft(), id));
            if (root->getRight()->getLeft() != nullptr)
                root->setRight(rightRotation(root->getRight()));
        } else if (root->getRight()->getID() < id) {
            // Zag Zag Rotation
            root->getRight()->setRight(splaying(root->getRight()->getRight(), id));
            root = leftRotation(root);
        }

        // Second rotation needed if not at end
        if (root->getRight() == nullptr)
            return root;
        else
            return leftRotation(root);
    }
}

// avlRebalance
// Recurivsely reconstructs the tree with AVL insertion
Robot* Swarm::avlRebalance(Robot* bot, Robot* newBot) {
    // BST Insert
    // Base Case
    if (bot == nullptr)
        return newBot;
    
    // No Duplicates
    if (bot->getID() == newBot->getID())
        return bot;

    // Find spot
    if (bot->getID() < newBot->getID())
        bot->setRight(avlRebalance(bot->getRight(), newBot));
    else if (bot->getID() > newBot->getID())
        bot->setLeft(avlRebalance(bot->getLeft(), newBot));

    // Update Height
    bot->setHeight(1 + max(findHeight(bot->getLeft()), findHeight(bot->getRight())));

    // Getting the Balance Factor
    int balFactor = getBalanceFactor(bot);

    // Balancing
    // 4 Different Cases if balance factor is unbalanced
    // Left Left Case
    if (balFactor > 1 && newBot->getID() < bot->getLeft()->getID())
        return rightRotation(bot);

    // Right Right Case
    if (balFactor < -1 && newBot->getID() > bot->getRight()->getID())
        return leftRotation(bot);

    // Left Right Case
    if (balFactor > 1 && newBot->getID() > bot->getLeft()->getID()) {
        bot->setLeft(leftRotation(bot->getLeft()));
        return rightRotation(bot);
    }

    // Right Left Case
    if (balFactor < -1 && newBot->getID() < bot->getRight()->getID()) {
        bot->setRight(rightRotation(bot->getRight()));
        return leftRotation(bot);
    }

    return bot;
}

// recursiveCopy
// Helper for overloaded = operator
// Goes through rhs tree and deep copies each node over
Robot* Swarm::recursiveCopy(Robot* rhs) {
    // Base Case
    if (rhs == nullptr)
        return nullptr;
    
    // Creating a Copy of the current Robot
    Robot* copyBot = new Robot(rhs->getID(), rhs->getType(), rhs->getState());

    // Traverse through tree PreOrder
    copyBot->setRight(recursiveCopy(rhs->getRight()));
    copyBot->setLeft(recursiveCopy(rhs->getLeft()));

    // Returns copy object
    return copyBot;
}

// deleteRobots
// Helper for clearing out the Tree object
void Swarm::deleteRobots(Robot* robot) {
    // Base case
    if (robot == nullptr)
        return;

    // Traverse to the child nodes
    deleteRobots(robot->getLeft());
    deleteRobots(robot->getRight());

    // Deleting node
    delete robot;
    robot = nullptr;
}

// treeSize
// Goes through the whole tree and counts up each node
// Returns total number of node in the tree
// Helper for AVL Rebalance
int Swarm::treeSize(Robot* bot) {
    // Base Case:
    if (bot == nullptr)
        return 0;
    
    // Traverse tree and count each node
    return (treeSize(bot->getLeft()) + 1 + treeSize(bot->getRight()));
}

// collectRobots
// Helper for setType to AVL
// Stores all nodes in curr tree to an array
void Swarm::collectRobots(Robot* bot, Robot* botsArr[], int& i) {
    // Base Case
    if (bot == nullptr)
        return;

    // Collection
    collectRobots(bot->getLeft(), botsArr, i);
    botsArr[i++] = bot;
    collectRobots(bot->getRight(), botsArr, i);
}
