#include "bst.h"

#include <iomanip>
#include <queue>
#include <stack>
#include <vector>

// methods of BST

BST::BST(std::initializer_list<int> il) {
    root = new Node(0);
    for (auto i : il)
        add_node(i);
}

static void copyBST(BST::Node *src, BST::Node *&dst) {
    if (src) {
        dst = new BST::Node(src->value);
        copyBST(src->left, dst->left);
        copyBST(src->right, dst->right);
    }
}

BST::BST(const BST &bst) {
    root = new Node(bst.root->value);
    if (bst.get_root())
        copyBST(bst.get_root(), get_root());
}

BST &BST::operator=(const BST &bst) {
    auto new_bst = new BST(bst);
    this->~BST();
    root = new_bst->root;
    return *this;
}

BST::BST(BST &&bst) {
    root = new Node(bst.root->value);
    root->left = bst.root->left;
    bst.root->left = nullptr;
}

BST &BST::operator=(BST &&bst) {
    if (&bst == this)
        return *this;
    this->~BST();
    root = new Node(bst.root->value);
    root->left = bst.root->left;
    bst.root->left = nullptr;
    return *this;
}

BST::Node *&BST::get_root() const { return root->left; }

void BST::bfs(std::function<void(Node *&node)> func) {
    if (!get_root())
        return;
    std::queue<Node *> que;
    que.push(get_root());
    while (!que.empty()) {
        auto cur = que.front();
        que.pop();
        func(cur);
        if (cur->left)
            que.push(cur->left);
        if (cur->right)
            que.push(cur->right);
    }
}

size_t BST::length() const { return root->value; }

bool BST::add_node(int value) const {
    Node *r = get_root(), *pre;
    if (!r) {
        root->left = new Node(value);
        root->value++;
        return true;
    }
    while (r) {
        pre = r;
        if (value == r->value)
            return false;
        if (value < r->value)
            r = r->left;
        else if (value > r->value)
            r = r->right;
    }
    if (value == pre->value)
        return false;
    if (value < pre->value)
        pre->left = new Node(value);
    else if (value > pre->value)
        pre->right = new Node(value);
    root->value++;
    return true;
}

BST::Node **BST::find_node(int value) {
    Node *r = get_root();
    if (!r)
        return nullptr;
    Node *pre = root;
    while (r) {
        if (value == r->value)
            break;
        pre = r;
        if (value > r->value)
            r = r->right;
        else
            r = r->left;
    }
    if (r) {
        if (pre->left && value == pre->left->value)
            return &(pre->left);
        if (pre->right && value == pre->right->value)
            return &(pre->right);
    }
    return nullptr;
}

BST::Node **BST::find_parrent(int value) {
    Node *r = get_root();
    if (!r)
        return nullptr;
    Node **pre = &root; // pre stores address of r's parrent
    bool found = false;
    while (r) {
        if (value == r->value) {
            found = true;
            break;
        }
        pre = &r;
        if (value < r->value)
            r = r->left;
        else
            r = r->right;
    }
    return found ? pre : nullptr;
}

BST::Node **BST::find_successor(int value) {
    Node *r = get_root();
    if (!r)
        return nullptr;
    std::vector<std::pair<int, Node **>> inorder_res;
    std::stack<Node *> stk;
    while (r || !stk.empty()) {
        while (r) {
            stk.push(r);
            r = r->left;
        }
        if (!stk.empty()) {
            r = stk.top();
            stk.pop();
            inorder_res.push_back(std::make_pair(r->value, &r));
            r = r->right;
        }
    }
    for (auto i = 0; i < inorder_res.size(); i++) {
        if (inorder_res[i].first == value) {
            if (i == inorder_res.size())
                return nullptr;
            else
                return inorder_res[i + 1].second;
        }
    }
    return nullptr;
}

static BST::Node **find_max_node_addr(BST::Node *root, bool parent = false) {
    BST::Node *pre;
    while (root && root->right) {
        pre = root;
        root = root->right;
    }
    return (parent) ? &pre : &root;
}

bool BST::delete_node(int value) {
    Node *r = get_root();
    if (!r)
        return false;
    Node **p_node = find_node(value);
    if (p_node == nullptr)
        return false;
    Node *node = *p_node;
    if (!node->left && !node->right) {
        delete node;
        *p_node = nullptr;
    } else if (!node->left) {
        *p_node = node->right;
        delete node;
    } else if (!node->right) {
        *p_node = node->left;
        delete node;
    } else {
        Node *max_node = *(find_max_node_addr(node->left));
        Node **parent_max_node = find_max_node_addr(node->left, true);
        std::swap(max_node->value, node->value);
        (*parent_max_node)->right = nullptr;
        delete max_node;
    }
    root->value--;
    return true;
}

BST::~BST() {
    std::vector<Node *> nodes;
    bfs([&nodes](BST::Node *&node) { nodes.push_back(node); });
    for (auto &node : nodes)
        delete node;
    delete root;
}

// methods of Node

BST::Node::Node(int value, Node *left, Node *right)
    : value(value), left(left), right(right) {}

BST::Node::Node(const Node &node) : Node(node.value, node.left, node.right) {}

bool operator>(const BST::Node &node, int x) { return node.value > x; }

bool operator>=(const BST::Node &node, int x) { return node.value >= x; }

bool operator<(const BST::Node &node, int x) { return node.value < x; }

bool operator<=(const BST::Node &node, int x) { return node.value <= x; }

bool operator==(const BST::Node &node, int x) { return node.value == x; }

bool operator<(int x, const BST::Node &node) { return x < node.value; }

bool operator<=(int x, const BST::Node &node) { return x <= node.value; }

bool operator>(int x, const BST::Node &node) { return x > node.value; }

bool operator>=(int x, const BST::Node &node) { return x >= node.value; }

bool operator==(int x, const BST::Node &node) { return x == node.value; }

std::ostream &operator<<(std::ostream &os, const BST::Node &node) {
    os << std::hex << &os << std::dec << "\t=> value:" << node.value
       << "\tleft:" << std::left << std::setw(20) << std::hex << node.left
       << "\tright:" << node.right << std::dec;
    return os;
}

std::ostream &operator<<(std::ostream &os, const BST &bst) {
    std::cout << std::string(80, '*') << std::endl;
    std::queue<BST::Node *> que;
    que.push(bst.get_root());
    while (!que.empty()) {
        auto node = que.front();
        que.pop();
        std::cout << *node << std::endl;
        if (node->left)
            que.push(node->left);
        if (node->right)
            que.push(node->right);
    }
    std::cout << std::string(80, '*') << std::endl;
    return os;
}

BST &operator++(BST &bst) {
    bst.bfs([](BST::Node *&node) { ++node->value; });
    return bst;
}

BST operator++(BST &bst, int) {
    BST b(bst);
    ++bst;
    return b;
}