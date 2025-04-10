//Elijah Baugher
typedef struct Node{
 struct Node *next;
 void *data; 
 }Node;
//system libraries rarely have bugs, do them first
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//constants

// do C code headers last, this file's header dead last.
#include "debug.h"
#include "altmem.h"
#include "libll.h"
#include "memory.h"
// my own header file is included dead last.  IT is MANDATORY!
#include "linkedlist.h"



//free node from list
static void free_node(Node *new_node)
{
    static int freed= 0;
    if(new_node !=NULL)
    {
        if(DEBUG)
	{
	    printf("DEBUG: freeing memory at %p\n", new_node);
	    fflush(stdout);
	}
    alternative_free(new_node);
    freed++;
    if(TEXT) printf("DIAGNOSTIC %d nodes freed\n", freed);

    }
     else
    {
        if(TEXT)printf( "ERROR: linkedlist: Failed to free a Node\n");
    }

}

//allocated new node for list
static Node *allocate_node()
{
    static int count=0;
    long size = sizeof(Node);
    Node *new_node= allocate_thing(size);
    if(new_node!=NULL){
        count ++;
        if(TEXT) {
            printf("DIAGNOSTIC allocated %ld bytes at %p, %d nodes allocated\n " ,size ,new_node,count);
        }
    }
     else
    {
        free_node(new_node);
        return NULL;
        if(TEXT)printf( "ERROR: linkedlist: Failed to malloc a Node\n", size);
    }
    return new_node;

}

//Sort function for list based off comparison function
void sort(void *hptr, bool(*cf)( void *data1, void *data2))
{   
    Node *p2head=hptr;
    bool worked;
    
    do{
        worked=false;
        Node *ptr= p2head;
    while(ptr->next!=NULL)
    {
        
        if(cf(ptr->next->data,ptr->data))
        {
            worked=true;
            Node *temp = ptr->data;
            ptr->data=ptr->next->data;
            ptr->next->data=temp;
        }
        ptr=ptr->next;
       
    }
    
    }
    while(worked);

}

//Delete node implimentation
int deleteSome(void *p2head,bool (*mustGo)(void *data, void *helper), void *helper, void (*disposal)(void*data), int text)
{
    Node **p2change =p2head;
    Node *holder;
    static int deletions=0;
    while(*p2change!=NULL)
    {
        if(mustGo((*p2change)->data,helper))
        {
            deletions++;
            holder=*p2change;
           *p2change=holder->next;
           disposal(holder->data);
           holder=NULL;
            
        }
        else
        {
            p2change=&((**p2change).next);
        }
    }
    return deletions;
}



//Insert node implimentation 
bool insert(void *p2head, void *data, bool(*goesInFrontOf)(void *data1, void *data2),int text) 
        {
            Node *new_node=allocate_node();
            if(new_node!=NULL){
            new_node->data=data;
            
            Node **p2p2head=p2head;
            while( *p2p2head !=NULL && goesInFrontOf((**p2p2head).data,new_node->data))
            {
                p2p2head=&((**p2p2head).next);
            }
            new_node->next=*p2p2head;
            *p2p2head=new_node;
            return true;
            }
        
        else 
        {
            return false;
        }

        }
        

        //p1_iterate function
    void iterate(void *head, void(*doThis)(void *data))
    {
        Node *p2head=head;
        
        while(p2head!=NULL)
        {
            doThis(p2head->data);
            p2head=p2head->next;
        }
    }




