// UMBC - CMSC 341 - Fall 2024 - Proj2
// Name: Eden Zhang
// Section: 5
// Professor: Sanorita Dey
// E-Mail: ezhang1@umbc.edu
// Date: 10/12/2024
// Description: 
//      Header file for swarm.cpp
//  */
#ifndef SWARM_H
#define SWARM_H
#include <iostream>
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
class Swarm;
enum STATE {ALIVE, DEAD};   // possible states for a robot
enum ROBOTTYPE {BIRD, DRONE, REPTILE, SUB, QUADRUPED};
enum TREETYPE {NONE, BST, AVL, SPLAY};
const int MINID = 10000;    // min robot ID
const int MAXID = 99999;    // max robot ID
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0
#define DEFAULT_TYPE DRONE
#define DEFAULT_STATE ALIVE

class Robot{
    public:
    friend class Swarm;
    friend class Grader;
    friend class Tester;
    Robot(int id, ROBOTTYPE type = DEFAULT_TYPE, STATE state = DEFAULT_STATE)
        :m_id(id),m_type(type), m_state(state) {
            m_left = nullptr;
            m_right = nullptr;
            m_height = DEFAULT_HEIGHT;
        }
    Robot(){
        m_id = DEFAULT_ID;
        m_type = DEFAULT_TYPE;
        m_state = DEFAULT_STATE;
        m_left = nullptr;
        m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }
    int getID() const {return m_id;}
    STATE getState() const {return m_state;}
    string getStateStr() const {
        string text = "";
        switch (m_state)
        {
        case DEAD:text = "DEAD";break;
        case ALIVE:text = "ALIVE";break;
        default:text = "UNKOWN";break;
        }
        return text;
    }
    ROBOTTYPE getType() const {return m_type;}
    string getTypeStr() const {
        string text = "";
        switch (m_type)
        {
        case BIRD:text = "BIRD";break;
        case DRONE:text = "DRONE";break;
        case REPTILE:text = "REPTILE";break;
        case SUB:text = "SUB";break;
        case QUADRUPED:text = "QUADRUPED";break;
        default:text = "UNKNOWN";break;
        }
        return text
        ;
    }
    int getHeight() const {return m_height;}
    Robot* getLeft() const {return m_left;}
    Robot* getRight() const {return m_right;}
    void setID(const int id){m_id=id;}
    void setState(STATE state){m_state=state;}
    void setType(ROBOTTYPE type){m_type=type;}
    void setHeight(int height){m_height=height;}
    void setLeft(Robot* left){m_left=left;}
    void setRight(Robot* right){m_right=right;}
    private:
    int m_id;
    ROBOTTYPE m_type;
    STATE m_state;
    Robot* m_left;  //the pointer to the left child in the BST
    Robot* m_right; //the pointer to the right child in the BST
    int m_height;   //the height of this node in the BST
};
class Swarm{
    public:
    friend class Grader;
    friend class Tester;
    Swarm();
    Swarm(TREETYPE type);
    ~Swarm();
    const Swarm & operator=(const Swarm & rhs);
    void clear();
    TREETYPE getType() const;
    void setType(TREETYPE type);
    void insert(const Robot& robot);
    void remove(int id);
    void dumpTree() const;
    private:
    Robot* m_root;  // the root of the BST
    TREETYPE m_type;// the type of tree

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************
    Robot* insertRobotBST(Robot* robot, int id, ROBOTTYPE type, STATE state);  // Recursive, inserts the robot into tree
    Robot* insertRobotAVL(Robot* robot, int id, ROBOTTYPE type, STATE state);
    Robot* insertRobotSplay(Robot* robot, int id, ROBOTTYPE type, STATE state);
    int findHeight(Robot* robot);  // Get Height with nullptr check
    Robot* rightRotation(Robot* bot);  // Helpers for ASL balancing
    Robot* leftRotation(Robot* bot);
    int getBalanceFactor(Robot* bot);  // Finds the balance factor of node
    Robot* removeRobotBST(Robot* root, int id);  // Removes robot from tree
    Robot* removeRobotAVL(Robot* root, int id);
    Robot* getSuccessor(Robot* bot);  // Helper, finds robot with the smallest id
    Robot* splaying(Robot* root, int id);  // Helper for splay trees
    Robot* avlRebalance(Robot* bot, Robot* newBot);  // Change Type AVL Helper
    Robot* recursiveCopy(Robot* rhs);  // Helper for = operator to copy
    void deleteRobots(Robot* robot);  // Helper for clear(), recursively deletes tree nodes
    int treeSize(Robot* bot);  // Total number of nodes in the tree
    // Goes through curr tree and puts robots in an array
    void collectRobots(Robot* bot, Robot* botsArr[], int& i); 

    void dump(Robot* aBot) const;//helper for recursive traversal
};
#endif
