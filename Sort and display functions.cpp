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
