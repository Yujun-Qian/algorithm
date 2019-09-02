#include <iostream>
#include <queue>
#include <memory>
#include <assert.h>
#include <random>

using namespace std;

enum Color {
    RED = true,
    BLACK = false
};

ostream & operator << (ostream &out, Color color)
{
    if (color == RED) {
        out << "r  ";
    } else {
        out << "b  ";
    }

    return out;
}

struct Node;
typedef shared_ptr<Node> NodePtr;

struct Node {
private:
    int key;
    // don't use val for simplicity
    //int val;
    NodePtr left;
    NodePtr right;
    bool color;

    int height;
    int blackHeight;
private:
    Node() {};

    friend class Tree;
    friend bool isRed(NodePtr node);
public:
    Node(int key_, bool color_) : key(key_), color(color_) {
        left = nullptr;
        right = nullptr;
        height = 0;
        blackHeight = 0;
    }

    Node(const Node& other) {
        key = other.key;
        left = other.left;
        right = other.right;
        color = other.color;
        height = other.height;
        blackHeight = other.blackHeight;
    }
};

bool isRed(NodePtr node) {
    if (node == nullptr)
        return false;

    return node->color == RED;
}

class Tree {
private:
    NodePtr root;
    NodePtr insert(NodePtr node, int key);
    NodePtr rotateLeft(NodePtr node) {
        NodePtr temp = node->right;

        node->right = temp->left;
        temp->left = node;


        temp->color = node->color;
        node->color = RED;

        return temp;
    }

    NodePtr rotateRight(NodePtr node) {
        NodePtr temp = node->left;

        node->left = temp->right;
        temp->right = node;

        temp->color = node->color;
        node->color = RED;

        return temp;
    }

    NodePtr flipColors(NodePtr node) {
        if (node->color == BLACK) {
            // Do we need to check the right child here?
            if (node->left != nullptr) {
                node->left->color = BLACK;
                node->right->color = BLACK;
            }
            node->color = RED;
        } else {
            if (node->left != nullptr) {
                node->left->color = RED;
                node->right->color = RED;
            }
            node->color = BLACK;
        }
        return node;
    }

    NodePtr deleteMin(NodePtr node, NodePtr& deletedNode);
    NodePtr deleteMax(NodePtr node, NodePtr& deletedNode);
    NodePtr deleteKey(NodePtr node, int key);
public:
    Tree();
    Tree(const Tree& other);
    void insert(int key);
    bool bfs();
    void deleteMin();
    void deleteMax();
    void deleteKey(int key);
    bool empty() {
        return root == nullptr;
    }
};

Tree::Tree() {

}

Tree::Tree(const Tree &other) {
    std::queue<NodePtr> queue;
    std::queue<NodePtr> otherQueue;

    root.reset(new Node(*(other.root)));
    if (root == nullptr) {
        return;
    }

    queue.push(root);
    otherQueue.push(other.root);

    while (!otherQueue.empty()) {
        NodePtr & otherNode = otherQueue.front();
        otherQueue.pop();

        NodePtr & node = queue.front();
        queue.pop();

        if (otherNode->left) {
            NodePtr newNode(new Node(*(otherNode->left)));
            node->left = newNode;
            otherQueue.push(otherNode->left);
            queue.push(newNode);
        }
        if (otherNode->right) {
            NodePtr newNode(new Node(*(otherNode->right)));
            node->right = newNode;
            otherQueue.push(otherNode->right);
            queue.push(newNode);
        }
    }
}

void Tree::insert(int key) {
    root = insert(root, key);
    root->color = BLACK;
}

void Tree::deleteMin() {
    NodePtr deletedNode = nullptr;
    root = deleteMin(root, deletedNode);
    if (deletedNode != nullptr)
        cout << "deleted key is: " << deletedNode->key << endl;
    if (root != nullptr)
        root->color = BLACK;
}

void Tree::deleteMax() {
    NodePtr deletedNode = nullptr;
    root = deleteMax(root, deletedNode);
    if (deletedNode != nullptr)
        cout << "deleted key is: " << deletedNode->key << endl;
    if (root != nullptr)
        root->color = BLACK;
}

void Tree::deleteKey(int key) {
    root = deleteKey(root, key);
    if (root != nullptr)
        root->color = BLACK;
}

NodePtr Tree::deleteMin(NodePtr node, NodePtr& deletedNode) {
    if (node->left == nullptr) {
        assert(node->right == nullptr);
        deletedNode = node;
        return nullptr;
    }

    // don't need to adjust if either the left child or the left child of the left child is red
    if (!isRed(node->left) && !isRed(node->left->left)) {
        assert(isRed(node) || node == root);
        node->color = BLACK;
        node->right->color = RED;
        node->left->color = RED;
        if (isRed(node->right->left)) {
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
        }
    }

    // either the left child or the left child of the left child has to be red
    assert (isRed(node->left->left) || isRed(node->left));

    node->left = deleteMin(node->left, deletedNode);

    if (isRed(node->left) && isRed(node->right)) {
        node = flipColors(node);
    }

    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        node = flipColors(node);
    }

    return node;
}

NodePtr Tree::deleteMax(NodePtr node, NodePtr& deletedNode) {
    if (node == nullptr) {
        return node;
    }

    if (node->right == nullptr) {
        assert(node->left == nullptr || isRed(node->left));
        deletedNode = node;
        if (node->left != nullptr) {
            node->left->color = BLACK;
        }
        return node->left;
    }

    // don't need to adjust if the right child of root is already a 3-node
    if (!isRed(node->right) && !isRed(node->right->left)) {
        assert(isRed(node->left) || isRed(node) || node == root);
        if (isRed(node->left)) {
            node = rotateRight(node);
        } else {
            node->color = BLACK;
            node->right->color = RED;
            node->left->color = RED;
        }
    }

    // node has to be either a 3 node or a 4 node
    // or a node with a black right node and a red left of right node
    assert ((node->right != nullptr && !isRed(node->right) && isRed(node->right->left)) || isRed(node->right));

    node->right = deleteMax(node->right, deletedNode);

    if (isRed(node->left) && isRed(node->right)) {
        node = flipColors(node);
    }
    
    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        node = flipColors(node);
    }

    return node;
}

NodePtr Tree::deleteKey(NodePtr node, int key) {
    if (node == nullptr) {
        return node;
    }

    if (key < node->key) {
        if (node->left == nullptr) {
            return node;
        }

        if (!isRed(node->left) && !isRed(node->left->left)) {
            assert(isRed(node) || node == root);
            node->color = BLACK;
            node->right->color = RED;
            node->left->color = RED;
            if (isRed(node->right->left)) {
                node->right = rotateRight(node->right);
                node = rotateLeft(node);
            }
        }
        node->left = deleteKey(node->left, key);
    } else if (key > node->key) {
        if (node->right == nullptr) {
            return node;
        }

        if (!isRed(node->right) && !isRed(node->right->left)) {
            assert(isRed(node->left) || isRed(node) || node == root);
            if (isRed(node->left)) {
                node = rotateRight(node);
            } else {
                node->color = BLACK;
                node->right->color = RED;
                node->left->color = RED;
            }
        }
        node->right = deleteKey(node->right, key);
    } else {
        if (node->right == nullptr) {
            // the key to be deleted is at the bottom
            assert(node->left == nullptr || isRed(node->left));
            if (isRed(node->left))
                node->left->color = node->color;
            return node->left;
        }

        NodePtr deletedNode = nullptr;
        if (!isRed(node->right) && !isRed(node->right->left)) {
            assert(isRed(node->left) || isRed(node) || node == root);
            if (isRed(node->left)) {
                node = rotateRight(node);
                node->right = deleteKey(node->right, key);
            } else {
                node->color = BLACK;
                node->right->color = RED;
                node->left->color = RED;
                node->right = deleteMin(node->right, deletedNode);
                node->key = deletedNode->key;
            }
        } else {
            node->right = deleteMin(node->right, deletedNode);
            node->key = deletedNode->key;
        }
    }

    if (isRed(node->left) && isRed(node->right)) {
        node = flipColors(node);
    }

    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        node = flipColors(node);
    }

    return node;
}

NodePtr Tree::insert(NodePtr node, int key) {
    if (node == nullptr) {
        return NodePtr(new Node(key, RED));
    }

    if (key > node->key) {
        node->right = insert(node->right, key);
    } else if (key < node->key) {
        node->left = insert(node->left, key);
    } else {
        // no-op as there is no value
    }

    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        node = flipColors(node);
    }

    return node;
}

bool Tree::bfs() {
    queue<NodePtr> queue;
    bool valid = true;

    if (root == nullptr) {
        cout << "empty tree" << endl;
        return true;
    }

    root->height = 0;
    root->blackHeight = 0;
    queue.push(root);

    int maxBlackHeight = -1;
    while (!queue.empty()) {
        NodePtr & node = queue.front();
        queue.pop();

        if (node && node->left) {
            node->left->height = node->height + 1;
            node->left->blackHeight = node->blackHeight;
            if (!isRed(node->left)) {
                node->left->blackHeight++;
            }
        }

        if (node && node->right) {
            node->right->height = node->height + 1;
            node->right->blackHeight = node->blackHeight;
            if (!isRed(node->right)) {
                node->right->blackHeight++;
            } else {
                valid = false;
            }
        }

        if (!node->right || !node->left) {
            if (maxBlackHeight == -1) {
                maxBlackHeight = node->blackHeight;
            }

            if (node->blackHeight != maxBlackHeight) {
                valid = false;
            }
        }

        cout << node->key << "\t";
        if (node->color == RED) {
            cout << "red,\t";
        }
        else {
            cout << "black,\t";
        }

        if (node->left != nullptr) {
            cout << "left: " << (Color)node->left->color << node->left->key << ",\t";
            queue.push(node->left);
        }
        if (node->right != nullptr) {
            cout << "right: " << (Color)node->right->color << node->right->key << ",\t";
            queue.push(node->right);
        }

        cout << endl;
    }

    cout << endl;
    return valid;
}


int main() {
    Tree tree;
    for (int i = 1; i <= 15; i++) {
        tree.insert(i);
        assert(tree.bfs());
    }

    static default_random_engine e(time(NULL));
    static uniform_int_distribution<unsigned > u(0, 100);

    for (int i = 0; i < 100000; i++) {
        int r = u(e);
        int key = u(e);

        if (r % 2 == 0) {
            cout << "delete: " << key << endl;
            tree.deleteKey(key);
        } else if (r % 2 == 1) {
            cout << "insert: " << key % 15 + 1 << endl;
            tree.insert(key % 15 + 1);
        }

        assert(tree.bfs());

        if (i % 16 == 0) {
            Tree temp(tree);
            assert(temp.bfs());

            while(!temp.empty()) {
                temp.deleteMin();
                assert(temp.bfs());
            }
        } else if (i % 16 == 8) {
            Tree temp(tree);
            assert(temp.bfs());

            while(!temp.empty()) {
                temp.deleteMax();
                assert(temp.bfs());
            }
        }
    }

    assert(tree.bfs());
}







