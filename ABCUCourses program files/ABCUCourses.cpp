//============================================================================
// Name        : ABCUCourses.cpp
// Author      : James Furman
// Version     : 1.0
// Description : Loads and prints course information for ABC University
//============================================================================

#include <iostream>
#include <time.h>
#include "ParseCsv.hpp"

using namespace csv;



using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold course information
struct Course {
    string courseNum; // unique identifier
    string title;
    vector<string> prereqs;
    Course() {
        courseNum = "";
        title = "";
    }
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course course) :
        Node() {
        this->course = course;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void AddNode(Node* node, Course course);
    void DestructPostOrder(Node* node);
    void InOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void DisplayCourse(Course course);
    void SearchAndPrint(string searchNum);
    void PrintAll();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNum);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

/**
 * Recursively iterate through tree post-order and delete each node
 * Called by destructor
 */
void BinarySearchTree::DestructPostOrder(Node* node) {
    if (node != nullptr) {
        DestructPostOrder(node->left);
        DestructPostOrder(node->right);
        delete node;
    }
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    DestructPostOrder(this->root);
}

/**
 * Insert a course
 *
 * @param course struct to insert
 */
void BinarySearchTree::Insert(Course course) {
    // if tree is empty, make new node its root
    if (this->root == nullptr) {
        this->root = new Node(course);
    }

    // if tree is not empty, call AddNode function
    else {
        AddNode(this->root, course);
    }
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::AddNode(Node* node, Course course) {
    if (course.courseNum.compare(node->course.courseNum) == 0) {
        cout << course.courseNum << " already exists." << endl;
    }
    else if (course.courseNum.compare(node->course.courseNum) < 0) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            AddNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            AddNode(node->right, course);
        }
    }
}

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void BinarySearchTree::DisplayCourse(Course course) {
    // if trying to display an empty course struct, skip
    if (course.courseNum.empty()) {
        return;
    }

    // display course number and title, which must be presents
    cout << course.courseNum << ": " << course.title;

    // if there is one prerequisite, display it
    if (course.prereqs.size() == 1) {
        cout << " | Prerequisite: " << course.prereqs.at(0);
    }

    // if there are multiple prerequisites, display them
    else if (course.prereqs.size() > 1) {
        cout << " | Prerequisites: ";
        for (int i = 0; i < course.prereqs.size() - 1; ++i) {
            cout << course.prereqs.at(i) << ", ";
        }
        cout << course.prereqs.at(course.prereqs.size() - 1);
    }

    // end console line
    cout << endl;
}

/**
 * Search for a course
 *
 * @param number of course to search for
 */
Course BinarySearchTree::Search(string courseNum) {
    // start search at root
    Node* currNode = this->root;

    while (currNode != nullptr) {
        if (courseNum.compare(currNode->course.courseNum) == 0) {
            return currNode->course;
        }

        // if search ID may be left of current node, move left
        if (courseNum.compare(currNode->course.courseNum) < 0) {
            currNode = currNode->left;
        }
        // if search ID may be right of current node, move right
        else {
            currNode = currNode->right;
        }
    }

    // default to returning empty course if match not found
    Course course;
    return course;
}

void BinarySearchTree::SearchAndPrint(string searchNum) {
    Course course;
    course = this->Search(searchNum);
    if (course.title.empty()) {
        cout << "Could not find course " << searchNum << endl;
    }
    else {
        this->DisplayCourse(course);
    }
}

/**
 * Identical to BST::InOrder; more readable
 */
void BinarySearchTree::PrintAll() {
    this->InOrder(this->root);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    this->InOrder(this->root);
}

/**
 * Print information of all courses in tree, ordered 
 * alphanumerically by course numbers
 *
 * @param node to print from recursively
 */
void BinarySearchTree::InOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    this->InOrder(node->left);
    this->DisplayCourse(node->course);
    this->InOrder(node->right);
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* searchTree) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            // Create a data structure and add to the collection of courses
            Course course;
            //cout << file[i][0] << endl;
            //cout << file[i][1] << endl;
            course.courseNum = file[i][0];
            course.title = file[i][1];
            if (file[i].size() > 2) {
                vector<string> prereqsList;
                for (unsigned int j = 2; j < file[i].size(); ++j) {
                    prereqsList.push_back(file[i][j]);
                }
                course.prereqs = prereqsList;
            }
            
            //searchTree->DisplayCourse(course);

            // insert this course in the tree
            searchTree->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int getMenuInput() {
    string userIn;
    bool numIn = false;

    while (!numIn) {
        numIn = true;
        cin >> userIn;
        if (userIn.size() > 1) {
            numIn = false;
            cout << "Please enter one digit: ";
            continue;
        }
        if (!isdigit(userIn[0])) {
            numIn = false;
            cout << "Please enter one digit: ";
            continue;
        }
    }

    return stoi(userIn);
}

/**
 * Validate that a string has the format "ABCD123"
 */
bool valCourseNum(string courseNum) {
    if (courseNum.length() != 7) { return false; }
    for (int i = 0; i < 4; ++i) {
        if (!isalpha(courseNum[i])) { return false; }
    }
    for (int i = 4; i < 7; ++i) {
        if (!isdigit(courseNum[i])) { return false; }
    }
    return true;
}

/**
 * Get and validate user input of a 7-char course number
 */
string getCourseInput() {
    string userIn;
    bool valCourse = false;

    cout << "Enter a 7-character course ID (e.g., \"MATH201\"): ";
    
    while (!valCourse) {
        valCourse = true;
        cin >> userIn;
        if (!valCourseNum(userIn)) {
            cout << "Please enter a valid course number formatted \"ABCD123\": ";
            continue;
        }
    }

    string upperIn;
    for (int i = 0; i < userIn.size(); ++i) {
        upperIn += toupper(userIn[i]);
    }

    return upperIn;
}

/**
 * The one and only main() method
 */
int main() {

    // process command line arguments
    string csvPath, courseNum;
    
    // Define a binary search tree to hold all courses
    BinarySearchTree* searchTree;
    searchTree = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        choice = getMenuInput();

        switch (choice) {

        case 1:
            cout << "Enter the csv filename: ";
            cin >> csvPath;
            cin.clear();
            
            // Complete the method call to load the courses
            loadCourses(csvPath, searchTree);
            break;

        case 2:
            searchTree->InOrder();
            break;

        case 3:
            courseNum = getCourseInput();
            course = searchTree->Search(courseNum);

            if (!course.courseNum.empty()) {
                searchTree->DisplayCourse(course);
            }
            else {
                cout << "Course number " << courseNum << " not found." << endl;
            }
            break;
        }
    }
    cout << "Exiting." << endl;

    return 0;
}
