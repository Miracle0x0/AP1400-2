#ifndef BST_H
#define BST_H

#include <functional>
#include <iostream>

/*
 * BST Class
 */
class BST {
  public:
    class Node; // nested class

    BST(std::initializer_list<int>);
    BST(const BST &bst);            // copy constructor
    BST &operator=(const BST &bst); // copy assignment
    BST(BST &&bst);                 // move constructor
    BST &operator=(BST &&bst);      // move assignment

    /*
     * get_root
     *
     * todo:
     * 		Returns the private member variable root.
     */
    Node *&get_root() const;

    /*
     * bfs
     *
     * todo:
     * 		The user should be able to use this argument and apply any functions
     * on the nodes of the tree i.e. call the object func on every node found by
     * BFS algorithm.
     */
    void bfs(std::function<void(Node *&node)> func);

    /*
     * length
     *
     * todo:
     * 		Returns the number of nodes exist in bst.
     */
    size_t length() const;

    /*
     * add_node
     *
     * todo:
     * 		Adds a new node to BST in the proper place.
     *
     * note:
     * 		The user should not be able to add a value that already exists in
     * the tree.
     */
    bool add_node(int value) const;

    /*
     * find_node
     *
     * todo:
     * 		Finds the node in tree with the specified value and returns a
     * pointer to the pointer of the node.
     *
     * note:
     * 		Do not use bfs algorithm to find the node, instead use the property
     * of binary search trees: that is why we use them in the first place.
     */
    Node **find_node(int value);

    /*
     * find_parrent
     *
     * todo:
     * 		Finds the parrent node of a node with specified value and returns a
     * pointer to the pointer of that parrent.
     *
     * note:
     * 		Do not use bfs algorithm to find the node.
     */
    Node **find_parrent(int value);

    /*
     * find_successor
     *
     * todo:
     * 		Finds a successor of a node with specified value.
     *
     * note:
     * 		Here 'successor' means the next value of the given value in an
     * inorder-list.
     */
    Node **find_successor(int value);

    /*
     * delete_node
     *
     * todo:
     * 		Deletes a node with the specified value. If there is no such node
     * with that value return false.
     */
    bool delete_node(int value);

    /*
     * Destructor
     */
    ~BST();

  private:
    Node *root = nullptr;
};

/*
 * Node Class
 */
class BST::Node {
  public:
    Node(int value = 0, Node *left = nullptr, Node *right = nullptr);
    Node(const Node &node);

    int value;
    Node *left;
    Node *right;
};

bool operator>(const BST::Node &node, int x);
bool operator>=(const BST::Node &node, int x);
bool operator<(const BST::Node &node, int x);
bool operator<=(const BST::Node &node, int x);
bool operator==(const BST::Node &node, int x);

bool operator<(int x, const BST::Node &node);
bool operator<=(int x, const BST::Node &node);
bool operator>(int x, const BST::Node &node);
bool operator>=(int x, const BST::Node &node);
bool operator==(int x, const BST::Node &node);

std::ostream &operator<<(std::ostream &os, const BST::Node &node);

std::ostream &operator<<(std::ostream &os, const BST &bst);

BST &operator++(BST &bst);
BST operator++(BST &bst, int);

#endif // BST_H