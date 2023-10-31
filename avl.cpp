/*
 * C++ Avl Tree implementation
 * Operations: 
 *  - Insert    O(log N)
 *  - Erase     O(log N)
 *  - Contains  O(log N)
 *  - Find      O(log N)
 *  - Prec      O(log N)
 *  - Succ      O(log N)
*/

#include <iostream>
#include <optional>
#include <string>
using namespace std;

template<typename K,typename X>
class AVLTree{
private:

    struct Node{
        K key;
        X val;
        Node *left;
        Node *right;
        Node *par;
        int height;

        Node(K key,X val,Node *par): key(key),val(val),left(NULL),right(NULL),par(par),height(1){}
    } *root;

public:

    AVLTree(): root(NULL){}
    ~AVLTree(){
        free(root);
    }

    void insert(const K key,const X val){
        if(root==NULL)
            root=new Node(key,val,NULL);
        else
            insert(key,val,root);
    }

    void erase(const K key){
        erase(key,root);
    }

    bool contains(const K key){
        return contains(key,root);
    }

    optional<pair<K,X>> find(const K key){
        return find(key,root);
    }

    optional<pair<K,X>> prec(const K key){
        return prec(key,root);
    }

    optional<pair<K,X>> succ(const K key){
        return succ(key,root);
    }

    void print(){
        printInOrder(root);
    }

private:

    int getHeight(Node *node){
        return node==NULL?0:node->height;
    }

    int getBalanceFactor(Node *node){
        return node==NULL?0:getHeight(node->right)-getHeight(node->left);
    }

    void update(Node *node){
        if(node==NULL)
            return;
        node->height=1+max(getHeight(node->left),getHeight(node->right));
    }

    void rotateLeft(Node *node){
        Node *par=node->par;
        Node *right=node->right;

        node->right=right->left;
        if(node->right!=NULL)
            node->right->par=node;

        right->left=node;
        node->par=right;

        right->par=par;
        if(par==NULL)
            root=right;
        else if(par->left==node)
            par->left=right;
        else
            par->right=right;

        update(node);
        update(right);
        update(par);
    }

    void rotateRight(Node *node){
        Node *par=node->par;
        Node *left=node->left;

        node->left=left->right;
        if(node->left!=NULL)
            node->left->par=node;

        left->right=node;
        node->par=left;

        left->par=par;
        if(par==NULL)
            root=left;
        else if(par->left==node)
            par->left=left;
        else
            par->right=left;

        update(node);
        update(left);
        update(par);
    }

    void balance(Node *node){
        update(node);

        if(getBalanceFactor(node)==-2){
            if(getBalanceFactor(node->left)!=1)
                rotateRight(node);
            else{
                rotateLeft(node->left);
                rotateRight(node);
            }
        }else if(getBalanceFactor(node)==2){
            if(getBalanceFactor(node->right)!=-1)
                rotateLeft(node);
            else{
                rotateRight(node->right);
                rotateLeft(node);
            }
        }
    }

    void insert(const K key,const X val,Node *node){
        if(key==node->key)
            node->val=val;
        else if(key<node->key){
            if(node->left==NULL)
                node->left=new Node(key,val,node);
            else
                insert(key,val,node->left);
        }else{
            if(node->right==NULL)
                node->right=new Node(key,val,node);
            else
                insert(key,val,node->right);
        }

        balance(node);
    }

    void erase(const K key,Node *node){
        if(node==NULL)
            return;

        if(node->key==key){
            if(node->left==NULL && node->right==NULL){
                if(node->par==NULL)
                    root=NULL;
                else if(node->par->left==node)
                    node->par->left=NULL;
                else
                    node->par->right=NULL;
                delete node;
                return;
            }else if(node->left==NULL){
                Node *right=node->right;

                node->key=right->key;
                node->val=right->val;
                node->left=right->left;
                node->right=right->right;

                if(node->right!=NULL)
                    node->right->par=node;
                if(node->left!=NULL)
                    node->left->par=node;

                delete right;
            }else if(node->right==NULL){
                Node *left=node->left;

                node->key=left->key;
                node->val=left->val;
                node->left=left->left;
                node->right=left->right;

                if(node->right!=NULL)
                    node->right->par=node;
                if(node->left!=NULL)
                    node->left->par=node;

                delete left;
            }else if(node->right->left==NULL){
                Node *right=node->right;

                node->key=right->key;
                node->val=right->val;
                node->right=right->right;

                if(node->right!=NULL)
                    node->right->par=node;

                delete right;
            }else
                eraseUtility(key,node->right,node);
        }else if(node->key>key)
            erase(key,node->left);
        else
            erase(key,node->right);

        balance(node);
    }

    void eraseUtility(const K key,Node *node,Node *toErase){
        if(node->left!=NULL)
            eraseUtility(key,node->left,toErase);
        else{
            toErase->key=node->key;
            toErase->val=node->val;

            if(node->right==NULL){
                if(node->par==NULL)
                    root=NULL;
                else if(node->par->right==node)
                    node->par->right=NULL;
                else
                    node->par->left=NULL;
                delete node;
                return;
            }else{
                Node *right=node->right;

                node->key=right->key;
                node->val=right->val;
                node->left=right->left;
                node->right=right->right;

                if(node->right!=NULL)
                    node->right->par=node;
                if(node->left!=NULL)
                    node->left->par=node;

                delete right;
            }
        }
        balance(node);
    }

    bool contains(const K key,Node *node){
        if(node==NULL)
            return false;
        if(node->key>key)
            return contains(key,node->left);
        if(node->key<key)
            return contains(key,node->right);
        return true;
    }

    optional<pair<K,X>> find(const K key,Node *node){
        if(node==NULL)
            return {};
        if(node->key>key)
            return find(key,node->left);
        if(node->key<key)
            return find(key,node->right);
        return {{node->key,node->val}};
    }

    optional<pair<K,X>> prec(const K key,Node *node){
        if(node==NULL)
            return {};
        
        if(node->key==key){
            return {{node->key,node->val}};
        }else if(node->key<key){
            optional<pair<K,X>> temp=prec(key,node->right);
            if(!temp)
                return {{node->key,node->val}};

            return temp;
        }
        return prec(key,node->left);
    }

    optional<pair<K,X>> succ(const K key,Node *node){
        if(node==NULL)
            return {};
        
        if(node->key==key){
            return {{node->key,node->val}};
        }else if(node->key>key){
            optional<pair<K,X>> temp=succ(key,node->left);
            if(!temp)
                return {{node->key,node->val}};

            return temp;
        }
        return succ(key,node->right);
    }

    void free(Node *node){
        if(node==NULL)
            return;
        free(node->right);
        free(node->left);
        delete node;
    }

    void printInOrder(Node *node){
        if(node==NULL)
            return;
        
        printInOrder(node->left);
        cout<<node->val<<' ';
        printInOrder(node->right);
    }
};