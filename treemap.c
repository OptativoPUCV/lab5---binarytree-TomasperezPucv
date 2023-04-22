#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *new = malloc(sizeof(TreeMap));
    new->current = NULL;
    new->root = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
   if(tree -> root == NULL){
     TreeNode *nodonuevo =createTreeNode(key,value);
     tree -> root = nodonuevo;
     tree -> current = tree -> root;
     
     return;
   }
   if(searchTreeMap(tree,key)!= NULL) return;
   
     TreeNode *newnode = createTreeNode(key, value);
     newnode -> parent = tree ->current;
     if(tree->lower_than(tree -> current ->pair ->key,key) ==1){
       tree ->current->right -> right= newnode;
       newnode -> parent = tree ->current -> right;
       tree -> current = tree ->current->right -> right;
     }else{ 
      
      tree -> current ->left ->left = newnode;
      newnode -> parent = tree ->current -> left;
      tree -> current = tree -> current ->left ->left;
     }
   
}

TreeNode * minimum(TreeNode * x){
  while(x!=NULL){
    if(x ->left == NULL) return x;
    x= x->left;
  }
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if(searchTreeMap(tree,node -> pair ->key)!= NULL){
      TreeNode *aux;
      //caso 1(sin ningun hijo :()
      if((tree -> current -> left == NULL)&&(tree -> current -> right == NULL))
      {
        aux = tree -> current;
        tree -> current = tree -> current -> parent;
        tree -> current -> left = NULL;
        tree -> current -> right = NULL;
        aux -> parent = NULL;
      }
      //caso 2(con 1 hijo)
      if((tree -> current -> left == NULL)||(tree -> current -> right == NULL)){
        aux = tree -> current;
        if((tree -> current -> left != NULL)){
          tree -> current = tree -> current -> left;
          tree -> current -> parent = aux -> parent;
          if(aux -> parent -> left == NULL){
            //derecha
            aux -> parent -> right = tree -> current;
            
          }else{
            aux -> parent -> right = tree -> current;
            
          }
          aux ->left =NULL;
          aux -> right = NULL;
        }
        if((tree -> current -> right != NULL)){
          tree -> current = tree -> current -> right;
          tree -> current -> parent = aux -> parent;
          if(aux -> parent -> left == NULL){
            //derecha
            aux -> parent -> right = tree -> current;
            
          }else{
            aux -> parent -> right = tree -> current;
            
          }
          aux ->left =NULL;
          aux -> right = NULL;
        }
      }
        
      //caso 3(con 2 hijos)
      if((tree -> current -> left != NULL)&&(tree -> current -> right != NULL)){
        aux = minimum(tree -> current -> right);
        tree -> current -> pair = aux -> pair;
        aux -> parent ->right = NULL;
        
      }
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}



// 0 derecha y 1 izquierda
Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode *aux = tree ->root;
  tree -> current = tree ->root;
  //if(aux == NULL) return NULL;
  while(aux != NULL){
    if(is_equal(tree, aux ->pair->key, key)== 1){
      tree ->current = aux;
      return aux -> pair;
    } 
    
    if((tree ->lower_than(aux-> pair ->key, key))==0){
      //
      aux  = aux ->left;
    }else{
      //
      aux = aux -> right;
    }
    
  }
    //tree ->current = aux;
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode *Ub_node ;
  Ub_node =tree -> current; 
  tree -> current = tree -> root;
  
  
  while(tree -> current !=NULL){
    if(is_equal(tree, Ub_node -> pair -> key, key)){
      return Ub_node -> pair;
    }

    if(tree -> lower_than(Ub_node -> pair -> key, key)){
      Ub_node =tree -> current -> right;
      tree -> current = Ub_node;
    }else{
      if (tree -> lower_than (key, Ub_node -> pair -> key)){
        Ub_node = tree -> current ->left;
        tree -> current = Ub_node;
        
      }
    }
  }
  Ub_node = tree -> current;
  return NULL;
  
  
  
  
  
  
  
  
  
  /*Pair *x = searchTreeMap(tree,key);
  if(x !=NULL) return x;
  else{
    
    while(tree -> current != NULL){
      if(tree -> lower_than(tree -> current -> pair -> key, key)) return tree -> current -> pair;
      x= nextTreeMap(tree);
    }
    
    
     //TreeNode *aux = tree ->root;
      
    
    while(tree -> current != NULL){
      
      x= nextTreeMap(tree);
      if(is_equal(tree, x-> key,key )) return x;
    }
  }*/
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
   TreeNode *nodo = minimum(tree -> root);
   Pair *x = nodo ->pair;
    return x;
}

Pair * nextTreeMap(TreeMap * tree) {
  TreeNode *next = tree -> current;
  //caso 1
  if(tree -> current -> right != NULL){
    next = minimum(tree -> current -> right);
    tree -> current = next;
    return next -> pair;
  }else{
    //caso 2
    
    while((tree -> current -> parent!= NULL)){
      if((tree ->lower_than(tree -> current -> pair -> key,tree -> current ->parent -> pair -> key))){

        tree -> current = tree -> current -> parent;
  
        return  tree -> current -> pair;
      }
      tree -> current = tree -> current -> parent;
      
    }

    return NULL;
  }
  
}


    
  
    
    
    
    
   /*next = next -> parent;
    while(next  != NULL){
      next = next -> parent;
      if((tree ->lower_than(tree -> current -> pair -> key,tree -> current -> parent -> pair -> key))==0){
        return next  ->pair;
      
      }
    
    
    } */
  
  
 
  
    

    