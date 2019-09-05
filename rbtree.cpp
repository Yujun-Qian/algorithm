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

    int count;
    int ownCount;
    int height;
    int blackHeight;
private:
    Node() {};

    friend class Tree;
    friend bool isRed(NodePtr& node);
public:
    Node(int key_, bool color_) : key(key_), color(color_) {
        left = nullptr;
        right = nullptr;
        height = 0;
        blackHeight = 0;
        ownCount = 1;
        count = 1;
    }

    Node(const Node& other) {
        key = other.key;
        left = other.left;
        right = other.right;
        color = other.color;
        height = other.height;
        blackHeight = other.blackHeight;
        count = other.count;
        ownCount = other.ownCount;
    }
};

bool isRed(NodePtr& node) {
    if (node == nullptr)
        return false;

    return node->color == RED;
}

class Tree {
private:
    NodePtr root;
    NodePtr insert(NodePtr& node, int key);
    NodePtr rotateLeft(NodePtr& node) {
        NodePtr temp = std::move(node->right);

        node->right = std::move(temp->left);
        temp->left = std::move(node);

        temp->color = temp->left->color;
        temp->left->color = RED;

        adjustCount(temp->left);
        adjustCount(temp);

        return temp;
    }

    void adjustCount(NodePtr& node) {
        int count = node->ownCount;

        if (node->left) {
            count += node->left->count;
        }
        if (node->right) {
            count += node->right->count;
        }

        node->count = count;
    }

    NodePtr rotateRight(NodePtr& node) {
        NodePtr temp = std::move(node->left);

        node->left = std::move(temp->right);
        temp->right = std::move(node);

        temp->color = temp->right->color;
        temp->right->color = RED;

        adjustCount(temp->right);
        adjustCount(temp);

        return temp;
    }

    void flipColors(NodePtr& node) {
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
    }

    NodePtr deleteMin(NodePtr& node, NodePtr& deletedNode);
    NodePtr deleteMax(NodePtr& node, NodePtr& deletedNode);
    NodePtr deleteKey(NodePtr& node, int key);
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

    int count(NodePtr& node) {
        return node->count;
    }

    int lower_bound(int key) {
        NodePtr node = root;
        int totalCount = root ? root->count : 0;
        int ret = 0;

        while (node) {
            if (key < node->key) {
                node = node->left;
            } else if (key > node->key) {
                ret += node->ownCount;
                if (node->left)
                    ret += node->left->count;
                node = node->right;
            } else {
                if (node->left)
                    ret += node->left->count;
                break;
            }
        }

        return totalCount - ret;
    }

    int upper_bound(int key) {
        NodePtr node = root;
        int totalCount = root ? root->count : 0;
        int ret = 0;

        while (node) {
            if (key < node->key) {
                node = node->left;
            } else if (key > node->key) {
                ret += node->ownCount;
                if (node->left)
                    ret += node->left->count;
                node = node->right;
            } else {
                ret += node->ownCount;
                if (node->left)
                    ret += node->left->count;
                break;
            }
        }

        return totalCount - ret;
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
    if (!isRed(root->left))
        root->color = RED;
    root = deleteMin(root, deletedNode);
    if (deletedNode != nullptr)
        cout << "deleted key is: " << deletedNode->key << endl;
    if (root != nullptr)
        root->color = BLACK;
}

void Tree::deleteMax() {
    NodePtr deletedNode = nullptr;
    if (!isRed(root->left))
        root->color = RED;
    root = deleteMax(root, deletedNode);
    if (deletedNode != nullptr)
        cout << "deleted key is: " << deletedNode->key << endl;
    if (root != nullptr)
        root->color = BLACK;
}

void Tree::deleteKey(int key) {
    if (!isRed(root->left))
        root->color = RED;
    root = deleteKey(root, key);
    if (root != nullptr)
        root->color = BLACK;
}

NodePtr Tree::deleteMin(NodePtr& node, NodePtr& deletedNode) {
    // the point of adjust is to ensure that in the recursive process
    // either node or node->left is red
    assert(isRed(node->left) || isRed(node) || node == root);

    if (node->left == nullptr) {
        assert(node->right == nullptr);
        deletedNode = node;
        return nullptr;
    }

    // don't need to adjust if either the left child or the left child of the left child is red
    if (!isRed(node->left) && !isRed(node->left->left)) {
        assert(isRed(node) || node == root);
        flipColors(node);
        if (isRed(node->right->left)) {
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
        }
    }

    // either the left child or the left child of the left child has to be red
    assert (isRed(node->left->left) || isRed(node->left));

    node->left = deleteMin(node->left, deletedNode);
    adjustCount(node);

    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }

    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }

    return node;
}

NodePtr Tree::deleteMax(NodePtr& node, NodePtr& deletedNode) {
    // the point of adjust is to ensure that in the recursive process
    // either node or node->left is red
    assert(isRed(node->left) || isRed(node) || node == root);

    if (node->right == nullptr) {
        assert(node->left == nullptr || isRed(node->left));
        deletedNode = node;
        if (node->left != nullptr) {
            node->left->color = BLACK;
        }
        return node->left;
    }

    // don't need to adjust if either the right child or the left child of the right child is red
    if (!isRed(node->right) && !isRed(node->right->left)) {
        if (isRed(node->left)) {
            node = rotateRight(node);
        } else {
            flipColors(node);
        }
    }

    // either the right child or the left child of the right child has to be red
    assert (isRed(node->right->left) || isRed(node->right));

    node->right = deleteMax(node->right, deletedNode);
    adjustCount(node);

    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }
    
    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }

    return node;
}

NodePtr Tree::deleteKey(NodePtr& node, int key) {
    // the point of adjust is to ensure that in the recursive process
    // either node or node->left is red
    assert(isRed(node->left) || isRed(node) || node == root);

    if (key < node->key) {
        if (node->left == nullptr) {
            return node;
        }

        if (!isRed(node->left) && !isRed(node->left->left)) {
            assert(isRed(node) || node == root);
            flipColors(node);
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
            if (isRed(node->left)) {
                node = rotateRight(node);
            } else {
                flipColors(node);
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

    adjustCount(node);

    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }

    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }

    return node;
}

NodePtr Tree::insert(NodePtr& node, int key) {
    if (node == nullptr) {
        return NodePtr(new Node(key, RED));
    }

    if (key > node->key) {
        node->right = insert(node->right, key);
    } else if (key < node->key) {
        node->left = insert(node->left, key);
    } else {
        node->ownCount++;
    }

    adjustCount(node);

    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
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
        cout << "(" << node->count << ")\t";
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

    cout << tree.lower_bound(7) << endl;
    cout << tree.upper_bound(7) << endl;

    cout << tree.lower_bound(15) << endl;
    cout << tree.upper_bound(15) << endl;

    cout << tree.lower_bound(0) << endl;
    cout << tree.upper_bound(0) << endl;
}







