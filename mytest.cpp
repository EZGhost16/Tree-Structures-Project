// UMBC - CMSC 341 - Fall 2024 - Proj2
// Name: Eden Zhang
// Section: 5
// Professor: Sanorita Dey
// E-Mail: ezhang1@umbc.edu
// Date: 10/12/2024
// Description: 
//      Tester file for swarm.cpp
//  */
#include "swarm.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = std::mt19937(10);// 10 is the fixed seed value
        m_unidist = std::uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester{
    public:
    // Test Cases
    bool testBSTInsertNormal();
    bool testBSTInsertDuplicate();  // Tries inserting duplicate value, tree should not be affected
    bool testBSTRemoveNormal();
    bool testBSTRemoveLast();  // Remove last node
    bool testBSTRemoveEmpty();  // Remove empty
    bool testBSTRemoveError();  // Attempt to remove an id not present in tree
    bool testAVLInsertNormal();  // Tests Insert function AVL Tree with 300 random inserts
    bool testAVLInsertDuplicate();  // Tries inserting duplicate value, tree should not be affected
    bool testAVLRemoveNormal();  // Tests Remove function AVL with 300 inserts and 150 removals
    bool testAVLRemoveLast();
    bool testAVLRemoveEmpty();  // Remove empty
    bool testAVLRemoveError();  // Attempt to remove an id not present in tree
    bool testSplayInsertNormal();  // Test Splay and BST properties are retained
    bool testSplayRemove();  // Test that tree is not modified
    bool testAssignOpNormal(TREETYPE);  // Copies for each tree type
    bool testAssignOpEmpty();  // Test empty = empty
    bool testChangeToAVL();  // Changes Tree type to AVL and checks rebalance
    bool testChangeToSplay();  // Tests the tree follows the Splay property
    bool testChangeToNONE();  // Tree is cleared out

    // Helper functions
    Swarm generateBSTTree();  // Helper func that generates a BST Tree with nodes
    Robot* searchTree(Robot* bot, int id);  // Helper func that searches Tree for target node
    bool followsBST(Robot* bot);  // Checks that the tree follows BST rules
    bool followsBSTInOrder(Robot* rootBot, int &prev);  // Helper function for follows BST
    void listTree(Robot* root, vector<int>& ids);  // Traverses through tree, returns vector of all nodes
    bool isBalanced(Robot* root);  // Checks if the whole tree is balanced
    int treeHeight(Robot* bot);  // Calculates the total height of the whole tree
    bool verifyHeights(Robot*);  // Verifies all the heights in the tree are correct
    int findHeight(Robot*);  // Finds the height of the current node relative to the whole tree
    int treeSize(Robot*);  // Counts up the total number of nodes in the tree
};

int main() {
    // Tester Object setup
    Tester tester;

    cout << "Testing BST Insert Normal Case:" << endl;
    if (tester.testBSTInsertNormal())
        cout << "\tBST Insert Normal Case test passed." << endl;
    else
        cout << "\tBST Insert Normal Case test failed." << endl;
    cout << endl;

    cout << "Testing BST Remove Normal Case:" << endl;
    if (tester.testBSTRemoveNormal())
        cout << "\tBST Remove Normal Case test passed." << endl;
    else
        cout << "\tBST Remove Normal Case test failed." << endl;
    cout << endl;

    cout << "Testing BST Remove Last Node Edge Case:" << endl;
    if (tester.testBSTRemoveLast())
        cout << "\tBST Remove Last Node Edge Case test passed." << endl;
    else
        cout << "\tBST Remove Last Node Edge Case test failed." << endl;
    cout << endl;

    cout << "Testing BST Remove from Empty Tree Edge Case:" << endl;
    if (tester.testBSTRemoveEmpty())
        cout << "\tBST Remove from Empty Tree Edge Case test passed." << endl;
    else
        cout << "\tBST Remove from Empty Tree Edge Case test failed." << endl;
    cout << endl;

    cout << "Testing BST Remove Error Case:" << endl;
    if (tester.testBSTRemoveError())
        cout << "\tBST Remove Error Case test passed." << endl;
    else
        cout << "\tBST Remove Error Case test failed." << endl;
    cout << endl;

    cout << "Testing AVL Insert Normal Case:" << endl;
    if (tester.testAVLInsertNormal())
        cout << "\tAVL Insert Normal Case test passed." << endl;
    else
        cout << "\tAVL Insert Normal Case test failed." << endl;
    cout << endl;

    cout << "Testing AVL Remove Normal Case:" << endl;
    if (tester.testAVLRemoveNormal())
        cout << "\tAVL Remove Normal Case test passed." << endl;
    else
        cout << "\tAVL Remove Normal Case test failed." << endl;
    cout << endl;

    cout << "Testing AVL Remove Last Node Edge Case:" << endl;
    if (tester.testAVLRemoveLast())
        cout << "\tAVL Remove Last Node Edge Case test passed." << endl;
    else
        cout << "\tAVL Remove Last Node Edge Case test failed." << endl;
    cout << endl;

    cout << "Testing AVL Remove from Empty Tree Edge Case:" << endl;
    if (tester.testAVLRemoveEmpty())
        cout << "\tAVL Remove from Empty Tree Edge Case test passed." << endl;
    else
        cout << "\tAVL Remove from Empty Tree Edge Case test failed." << endl;
    cout << endl;

    cout << "Testing AVL Remove Error Case:" << endl;
    if (tester.testAVLRemoveError())
        cout << "\tAVL Remove Error Case test passed." << endl;
    else
        cout << "\tAVL Remove Error Case test failed." << endl;
    cout << endl;

    cout << "Testing SPLAY Insert Normal Case:" << endl;
    if (tester.testSplayInsertNormal())
        cout << "\tSPLAY Insert Normal Case test passed." << endl;
    else
        cout << "\tSPLAY Insert Normal Case test failed." << endl;
    cout << endl;

    cout << "Testing SPLAY Remove Case:" << endl;
    if (tester.testSplayRemove())
        cout << "\tSplay Remove Case test passed." << endl;
    else
        cout << "\tSplay Remove Case test failed." << endl;
    cout << endl;

    cout << "Testing Assignment Operator Normal (BST) Case:" << endl;
    if (tester.testAssignOpNormal(BST))
        cout << "\tAssignment Operator Normal (BST) Case test passed." << endl;
    else
        cout << "\tAssignment Operator Normal (BST) Case test failed." << endl;
    cout << endl;

    cout << "Testing Assignment Operator Normal (AVL) Case:" << endl;
    if (tester.testAssignOpNormal(AVL))
        cout << "\tAssignment Operator Normal (AVL) Case test passed." << endl;
    else
        cout << "\tAssignment Operator Normal (AVL) Case test failed." << endl;
    cout << endl;
    
    cout << "Testing Assignment Operator Normal (SPLAY) Case:" << endl;
    if (tester.testAssignOpNormal(SPLAY))
        cout << "\tAssignment Operator Normal (SPLAY) Case test passed." << endl;
    else
        cout << "\tAssignment Operator Normal (SPLAY) Case test failed." << endl;
    cout << endl;

    cout << "Testing Set Type to AVL Case:" << endl;
    if (tester.testChangeToAVL())
        cout << "\tSet Type to AVL Case test passed." << endl;
    else
        cout << "\tSet Type to AVL Case test failed." << endl;
    cout << endl;

    cout << "Testing Set Type to SPLAY Case:" << endl;
    if (tester.testChangeToSplay())
        cout << "\tSet Type to SPLAY Case test passed." << endl;
    else
        cout << "\tSet Type to SPLAY Case test failed." << endl;
    cout << endl;

    cout << "Testing Set Type to NONE Case:" << endl;
    if (tester.testChangeToNONE())
        cout << "\tSet Type to NONE Case test passed." << endl;
    else
        cout << "\tSet Type to NONE Case test failed." << endl;
    cout << endl;
    
    return 0;
}

bool Tester::testBSTInsertNormal() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 300;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Swarm Tree Object Generated
    Swarm team(BST);
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);

        // Check that the new robot was inserted
        if (searchTree(team.m_root, ID) == nullptr)
            return false;
    }

    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checking that the tree retained its BST property
    return followsBST(team.m_root);
}

bool Tester::testBSTInsertDuplicate() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 10;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Swarm Tree Object Generated
    Swarm team(BST);
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);

        // Check that the new robot was inserted
        if (searchTree(team.m_root, ID) == nullptr)
            return false;
    }
    // Track original size of tree before duplicate insertion attempt
    int size = treeSize(team.m_root);

    // Attempt to insert known duplicate node (the last inserted node)
    Robot robot(ID, roboType);
    team.insert(robot);

    // Checks the value is still in the tree
    if (searchTree(team.m_root, ID) == nullptr)
        return false;

    // Check that duplicate node did not affect the tree (i.e. not inserted)
    if (treeSize(team.m_root) != size)
        return false;

    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checking that the tree retained its BST property
    return followsBST(team.m_root);
}

bool Tester::testBSTRemoveNormal() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 300;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Swarm Tree Object Generated
    Swarm team(BST);
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Vector used to store the inserted ids
    vector<int> insertedIDs;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);
        
        // Record the inserted
        insertedIDs.push_back(ID);
    }
    
    // New Random object for shuffling
    Random shuffler(insertedIDs[0], insertedIDs[TEAMSIZE - 1]);

    // Shuffle the vector for when randomly removing an ID
    shuffler.getShuffle(insertedIDs);

    // Number of removals to do
    const int REMOVALS = 150;

    // Removals
    for (int i = 0; i < REMOVALS; i++) {
        // Removing a random robot from the IDs present in the tree
        team.remove(insertedIDs[i]);

        // Check that the new robot was removed
        if (searchTree(team.m_root, insertedIDs[i]) != nullptr)
            return false;
    }
    
    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checking that the tree retained its BST property
    return followsBST(team.m_root);
}

bool Tester::testBSTRemoveLast() {
    // Estabish the swarm object
    Swarm team(BST);

    // Construct with 1 last node
    Robot lastBot(10000);
    team.insert(10000);

    // Remove the last node test
    team.remove(10000);

    // Checks
    // Tree should simply be empty
    if (team.m_root != nullptr)
        return false;
    return true;
}

bool Tester::testBSTRemoveEmpty() {
    // Estabish the swarm object
    Swarm team(BST);

    // Tree is empty, tries to remove from it
    team.remove(10000);

    // Nothing should happen and the tree remains empty
    if (team.m_root != nullptr)
        return false;
    return true;
}

bool Tester::testBSTRemoveError() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 10;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Swarm Tree Object Generated
    Swarm team(BST);
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Vector used to store the inserted ids
    vector<int> insertedIDs;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);
        
        // Record the inserted
        insertedIDs.push_back(ID);
    }
    
    // Randomly pick an ID not present in the tree
    int tempID = 0;
    bool inTree = false;
    do {
        tempID = idGen.getRandNum();
        inTree = false;
        // If id is not found in tree, continue
        for (int i = 0; i < int(insertedIDs.size()); i++) {
            if (tempID == insertedIDs[i])
                inTree = true;
        }
    } while (inTree == true);

    // The tree should not change, thus to verify need to track before and after
    vector<int> before;
    vector<int> after;
    listTree(team.m_root, before);

    // Removing a random robot with id not in tree
    team.remove(tempID);
    listTree(team.m_root, after);

    // Vectors should be the same
    if (before != after)
        return false;
    
    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checking that the tree retained its BST property
    return followsBST(team.m_root);
}

bool Tester::testAVLInsertNormal() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 300;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Swarm Tree Object Generated
    Swarm team(AVL);
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);

        // Check that the new robot was inserted
        if (searchTree(team.m_root, ID) == nullptr)
            return false;
    }

    // Checks the Tree follows BST property
    if (!followsBST(team.m_root))
        return false;
    
    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checks Tree is Height Balanced for AVL property
    return isBalanced(team.m_root);
}

bool Tester::testAVLInsertDuplicate() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 10;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Swarm Tree Object Generated
    Swarm team(AVL);
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);

        // Check that the new robot was inserted
        if (searchTree(team.m_root, ID) == nullptr)
            return false;
    }
    // Track original size of tree before duplicate insertion attempt
    int size = treeSize(team.m_root);

    // Attempt to insert known duplicate node (the last inserted node)
    Robot robot(ID, roboType);
    team.insert(robot);

    // Checks the value is still in the tree
    if (searchTree(team.m_root, ID) == nullptr)
        return false;

    // Check that duplicate node did not affect the tree (i.e. not inserted)
    if (treeSize(team.m_root) != size)
        return false;

    // Checks the Tree follows BST property
    if (!followsBST(team.m_root))
        return false;
    
    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checks Tree is Height Balanced for AVL property
    return isBalanced(team.m_root);
}

bool Tester::testAVLRemoveNormal() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 300;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Swarm Tree Object Generated
    Swarm team(AVL);
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Vector used to store the inserted ids
    vector<int> insertedIDs;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);
        
        // Record the inserted
        insertedIDs.push_back(ID);
    }
    
    // New Random object for shuffling
    Random shuffler(insertedIDs[0], insertedIDs[TEAMSIZE - 1]);

    // Shuffle the vector for when randomly removing an ID
    shuffler.getShuffle(insertedIDs);

    // Number of removals to do
    const int REMOVALS = 150;

    // Removals
    for (int i = 0; i < REMOVALS; i++) {
        // Removing a random robot from the IDs present in the tree
        team.remove(insertedIDs[i]);

        // Check that the new robot was removed
        if (searchTree(team.m_root, insertedIDs[i]) != nullptr)
            return false;
    }

    // Checks the Tree follows BST property
    if (!followsBST(team.m_root))
        return false;

    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checks Tree is Height Balanced for AVL property
    return isBalanced(team.m_root);
}

bool Tester::testAVLRemoveLast() {
    // Estabish the swarm object
    Swarm team(AVL);

    // Construct with 1 last node
    Robot lastBot(10000);
    team.insert(10000);

    // Remove the last node test
    team.remove(10000);

    // Checks
    // Tree should simply be empty
    if (team.m_root != nullptr)
        return false;
    return true;
}

bool Tester::testAVLRemoveEmpty() {
    // Estabish the swarm object
    Swarm team(AVL);

    // Tree is empty, tries to remove from it
    team.remove(10000);

    // Nothing should happen and the tree remains empty
    if (team.m_root != nullptr)
        return false;
    return true;
}

bool Tester::testAVLRemoveError() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 10;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Swarm Tree Object Generated
    Swarm team(AVL);
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Vector used to store the inserted ids
    vector<int> insertedIDs;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);
        
        // Record the inserted
        insertedIDs.push_back(ID);
    }
    
    // Randomly pick an ID not present in the tree
    int tempID = 0;
    bool inTree = false;
    do {
        tempID = idGen.getRandNum();
        inTree = false;
        // If id is not found in tree, continue
        for (int i = 0; i < int(insertedIDs.size()); i++) {
            if (tempID == insertedIDs[i])
                inTree = true;
        }
    } while (inTree == true);

    // The tree should not change, thus to verify need to track before and after
    vector<int> before;
    vector<int> after;
    listTree(team.m_root, before);

    // Removing a random robot with id not in tree
    team.remove(tempID);
    listTree(team.m_root, after);

    // Vectors should be the same
    if (before != after)
        return false;

    // Checks the Tree follows BST property
    if (!followsBST(team.m_root))
        return false;

    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checks Tree is Height Balanced for AVL property
    return isBalanced(team.m_root);
}

bool Tester::testSplayInsertNormal() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 300;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Swarm Tree Object Generated
    Swarm team(SPLAY);
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);

        // Check that the new robot was inserted
        if (searchTree(team.m_root, ID) == nullptr)
            return false;
        
        // Splay check, inserted node is root of tree
        if (team.m_root->getID() != ID)
            return false;
    }

    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checking that the tree retained its BST property
    return followsBST(team.m_root);
}

bool Tester::testSplayRemove() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 10;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Swarm Tree Object Generated
    Swarm team(SPLAY);
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);

        // Splay check, inserted node is root of tree
        if (team.m_root->getID() != ID)
            return false;
    }

    // The tree should not change, thus to verify need to track before and after
    vector<int> before;
    vector<int> after;
    listTree(team.m_root, before);

    // Removing a random robot with id not in tree
    team.remove(ID);
    listTree(team.m_root, after);

    // Vectors should be the same (no change)
    if (before != after)
        return false;

    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checks the Tree follows BST property
    return followsBST(team.m_root);
}

bool Tester::testAssignOpNormal(TREETYPE type) {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 300;

    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Variables needed to construct Trees
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Tree Object Construction of given Type
    Swarm bTeam(type);

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        bTeam.insert(robot);
    }

    // Copy Tree
    Swarm cTeam(type);

    // The tree has existing nodes
    Robot a(10005);
    Robot b(10000);
    Robot c(20000);
    cTeam.insert(a);
    cTeam.insert(b);
    cTeam.insert(c);

    // Assignment Op test
    cTeam = bTeam;

    // Tracking and counting every node in both trees
    vector<int> cList;
    vector<int> bList;
    listTree(cTeam.m_root, cList);
    listTree(bTeam.m_root, bList);

    // Verifies that nodes in each tree are in the same spots
    if (cList != bList)
        return false;
    
    // Checking sizes are the same
    if (treeSize(cTeam.m_root) != treeSize(bTeam.m_root))
        return false;
    
    // Check memory addresses are different
    if (&cTeam == &bTeam)
        return false;

    // Test on empty tree
    Swarm eTeam;
    eTeam = bTeam;

    // Tracking and counting every node in both trees
    vector<int> eList;
    listTree(eTeam.m_root, eList);

    // Comparing
    if (eList != bList)
        return false;
    
    // Checking sizes are the same
    if (treeSize(eTeam.m_root) != treeSize(bTeam.m_root))
        return false;
    
    // Check memory addresses are different
    if (&eTeam == &bTeam)
        return false;
    
    // All above checks passed
    return true;
}

bool Tester::testAssignOpEmpty() {
    // Creating an empty tree
    Swarm xTree;

    // Empty tree for copy
    Swarm yTree;

    // Attempting error case: Assigning an empty tree to an empty tree
    yTree = xTree;

    // Both trees should be empty
    if (xTree.m_root != nullptr)
        return false;
    if (yTree.m_root != nullptr)
        return false;
    return true;
}

bool Tester::testChangeToAVL() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 50;

    // Test tree starts off as BST
    Swarm team(BST); 
    
    // +++++++ Insertions +++++++
    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Variables needed to construct Trees
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);
    }

    // Checks the Tree follows BST property
    if (!followsBST(team.m_root))
        return false;
    
    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // =====TYPE CHANGING=======
    // Change to AVL tree
    team.setType(AVL);

    // Verifies tree type changed
    if (team.getType() != AVL)
        return false;

    // Running AVL property checks
    // Checks the Tree follows BST property
    if (!followsBST(team.m_root))
        return false;
    
    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Checks Tree is Height Balanced for AVL property
    return isBalanced(team.m_root);
}

bool Tester::testChangeToSplay() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 50;

    // Test tree starts off as BST
    Swarm team(BST); 
    
    // +++++++ Insertions +++++++
    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Variables needed to construct Trees
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);
    }

    // Checks the Tree follows BST property
    if (!followsBST(team.m_root))
        return false;
    
    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // =====TYPE CHANGING=======
    // Change to Splay
    team.setType(SPLAY);

    // Verifies tree type changed
    if (team.getType() != SPLAY)
        return false;
    
    // Insert new nodes to test Splay property
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);

        // After splaying, new insert should be root
        if (team.m_root->getID() != ID)
            return false;
    }

    // Checks the Tree follows BST property
    if (!followsBST(team.m_root))
        return false;
    
    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // Makes it through checks above
    return true;
}

bool Tester::testChangeToNONE() {
    // Number of nodes to construct tree with
    const int TEAMSIZE = 50;

    // Test tree starts off as BST
    Swarm team(BST);
    
    // +++++++ Insertions +++++++
    // Random Object set up
    // Randomly insert many values into tree to test
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);

    // Variables needed to construct Trees
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    // Loop to insert robots
    for (int i = 0; i < TEAMSIZE; i++) {
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID, roboType);
        team.insert(robot);
    }

    // Checks the Tree follows BST property
    if (!followsBST(team.m_root))
        return false;
    
    // Checks all the Heights are correct
    if (!verifyHeights(team.m_root))
        return false;

    // =====TYPE CHANGING=======
    // Change to None
    team.setType(NONE);

    // Verifies tree type changed
    if (team.getType() != NONE)
        return false;
    
    // Tree should be empty
    if (team.m_root != nullptr)
        return false;
    return true;
}

// searchTree
// Searches through tree for target Robot id
// Used to check existence of node in the tree
Robot* Tester::searchTree(Robot* bot, int id) {
    // Base Case: Bot found or not
    if (bot == nullptr || bot->getID() == id)
        return bot;

    // Traversal
    if (bot->getID() < id)
        return searchTree(bot->getRight(), id);
    return searchTree(bot->getLeft(), id);
}

// followsBST
// General Function that checks the current tree follows BST rules
bool Tester::followsBST(Robot* bot) {
    // Set Up
    int prev = MINID - 1;
    return followsBSTInOrder(bot, prev);
}

// followsBSTInOrder
// Helper function that recursively traverses through the tree In Order
// Checks that the ids are in correct places
bool Tester::followsBSTInOrder(Robot* rootBot, int &prev) {
    // Base Case
    if (!rootBot)
        return true;

    // Check left subtree
    if (!followsBSTInOrder(rootBot->getLeft(), prev))
        return false;

    // Check current node
    if (prev >= rootBot->getID())
        return false;
    
    // Update prev value to curr robot id
    prev = rootBot->getID();
    
    // Check right subtree
    return followsBSTInOrder(rootBot->getRight(), prev);
}

// listTree
// Helper function that goes through the tree and 
// fills a vector with all the ids in tree
void Tester::listTree(Robot* root, vector<int>& ids) {
    // Base Case:
    if (root == nullptr)
        return;
    
    // In order traversal
    listTree(root->getLeft(), ids);

    // Storing robot ids into the vector
    ids.push_back(root->getID());

    // Right side
    listTree(root->getRight(), ids);
}

// isBalanced
// Goes through tree and checks if height balanced or not
bool Tester::isBalanced(Robot* root) {
    // Set variables for height of both left and right subtrees
    int leftHeight, rightHeight;

    // Base Case: Tree is empty
    if (root == nullptr)
        return true;
    
    // Getting heights
    leftHeight = treeHeight(root->getLeft());
    rightHeight = treeHeight(root->getRight());

    // Check Balance Factors
    if (abs(leftHeight - rightHeight) <= 1 && isBalanced(root->getLeft()) && isBalanced(root->getRight()))
        return true;
    // Otherwise, the Tree is not balanced
    return false;
}

// treeHeight
// Helper function for isBalanced
// Returns total height of the current tree
int Tester::treeHeight(Robot* bot) {
    // Base Case:
    if (bot == nullptr)
        return -1;
    // Height of the tree
    return 1 + max(treeHeight(bot->getLeft()), treeHeight(bot->getRight()));
}

// verifyHeights
// Goes through each node and checks that its height is correct
bool Tester::verifyHeights(Robot* bot) {
    // Base Case
    if (bot == nullptr)
        return true;

    // Establishes the correct height to compare
    int correctHeight = 1 + max(findHeight(bot->getLeft()), findHeight(bot->getRight()));

    // Compares and verifies
    if (bot->getHeight() != correctHeight)
        return false;

    // Traverse through all subtrees
    return verifyHeights(bot->getLeft()) && verifyHeights(bot->getRight());
}

// findHeight
// Helper func returns height of given robot node
// Additional check for leaf nodes edge case
int Tester::findHeight(Robot* robot) {
    // Check if the parent node is a leaf
    if (robot == nullptr)
        return -1;
    return robot->getHeight();
}

// treeSize
// Goes through the whole tree and counts up each node
// Returns total number of node in the tree
int Tester::treeSize(Robot* bot) {
    // Base Case:
    if (bot == nullptr)
        return 0;
    
    // Traverse tree and count each node
    return (treeSize(bot->getLeft()) + 1 + treeSize(bot->getRight()));
}
