#ifndef BST_TREE_H
#define BST_TREE_H
#include "AVL_Tree.h"

template<typename Key, typename Info>
class BST_Tree : public Tree<Key, Info> {
    typename Tree<Key, Info>::Node* insert(typename Tree<Key, Info>::Node* root, const Key& key, const Info& info)
    {
        // Create a new Node containing
        // the new element
        typename Tree<Key, Info>::Node* newnode = new typename Tree<Key, Info>::Node(key, info);

        // Pointer to start traversing from root and
        // traverses downward path to search
        // where the new node to be inserted
        typename Tree<Key, Info>::Node* x = root;

        // Pointer y maintains the trailing
        // pointer of x
        typename Tree<Key, Info>::Node* y = NULL;

        while (x != NULL) {
            y = x;
            if (key < x->m_key)
                x = x->left;
            else
                x = x->right;
        }

        // If the root is NULL i.e the tree is empty
        // The new node is the root node
        if (y == NULL)
            y = newnode;

        // If the new key is less then the leaf node key
        // Assign the new node to be its left child
        else if (key < y->m_key)
            y->left = newnode;

        // else assign the new node its right child
        else
            y->right = newnode;

        // Returns the pointer where the
        // new node is inserted
        return y;
    }
public:
    /**
    * Functionallity: Inserts the node to the AVL tree.
    * Approche: I am using recursive insert. There is possible throw when key is already in the tree, however, public method outputs true/false in that case.
    * param[in] key : Key of the element that is going to be inserted.
    * param[in] info : Info of the element that is going to be inserted.
    */
    bool insertTree(const Key& key, const Info& info);
};

template<typename Key, typename Info>
inline bool BST_Tree<Key, Info>::insertTree(const Key& key, const Info& info)
{
    if(!Tree<Key, Info>::head)
        Tree<Key, Info>::head = insert(Tree<Key, Info>::head, key, info);
    else {
    	insert(Tree<Key, Info>::head, key, info);
    }
    return true;
}

#endif
