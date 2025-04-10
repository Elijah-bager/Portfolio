//Elijah Baugher

//system libraries rarely have bugs, do them first
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//constants

// do C code headers last, this file's header dead last.
#include "debug.h"
#include "libll.h"
#include "node_struct.h"

// my own header file is included dead last.  IT is MANDATORY!
#include "p1.h"
bool delete_s(void *data, void *helper)
{
    char *yes=data;
    return yes[0]=='s' || yes[0]=='S';

}

 bool compare( void *first, void *second)
{
    char *first_char= first;
    char *second_char= second;
    bool comparison = first_char[0]<second_char[0];
    return comparison;

}


 //Allocated memory
void *allocate_thing(long size)
{
    static int count = 0;
    void *thing = malloc(size); //alt malloc and free
    if(thing)
    {
        count++;
        if(TEXT)
        {
            printf("DIAGNOSTIC allocated %ld bytes at %p, %d things allocated\n", size, thing, count);
        }
    }
    else
    {
        if(TEXT)printf( "ERROR: failed to allocate %ld bytes\n", size);
    }
    return thing;
}

//free memory
void free_thing(void *thing)
{
    static int count = 0;
    if(thing)
    {
        if(DEBUG)
	{
	    printf("DEBUG: freeing memory at %p\n", thing);
	    fflush(stdout);
	}
         free(thing);
         count++;
        if(TEXT) printf("DIAGNOSTIC %d things freed\n", count);       
    }
    else
    {
        if(TEXT)printf( "ERROR: attempt to free NULL\n");
    }
}

//free node from head
void free_node(Node *new_node)
{
    static int freed= 0;
    if(new_node !=NULL)
    {
        if(DEBUG)
	{
	    printf("DEBUG: freeing memory at %p\n", new_node);
	    fflush(stdout);
	}
    free(new_node);
    freed++;
    if(TEXT) printf("DIAGNOSTIC %d nodes freed\n", freed);

    }
     else
    {
        if(TEXT)printf( "ERROR: attempt to free NULL\n");
    }

}

//allocated new node for list
Node *allocate_node()
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
        if(TEXT)printf( "ERROR: failed to allocate %ld bytes\n", size);
    }
    return new_node;

}

void p1_sort(void *hptr, ComparisonFunction cf)
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


int p1_delete_some(void *p2head, CriteriaFunction mustGo, void *helper,ActionFunction disposal, int text)
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




bool p1_insert(void *p2head, void *data, ComparisonFunction goesInFrontOf,int text) 
        {
            Node *new_node=allocate_node();
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

//Prints a string for p1_iterate function
void print_string(void *first)
{
    char *str=first;
    printf("%s\n",str);

}
    //p1_iterate function
    void p1_iterate(void *head, ActionFunction doThis)
    {
        Node *p2head=head;
        
        while(p2head!=NULL)
        {
            doThis(p2head->data);
            p2head=p2head->next;
        }
    }



        int main()
 {

    bool worked;
    int deleted;
   
    Node *node_one=allocate_node();
     void *head=node_one;
     node_one->data="Sarisota";
    
     Node *node_two=allocate_node();
     node_two->data="Destin";
     node_one->next=node_two;
     
      Node *node_three=allocate_node();
      node_three->data="Saint_Petes";
      node_two->next=node_three;
     
       Node *node_four=allocate_node();
       node_four->data="Panama";
       node_three->next=node_four;

        printf("Before:\n");
        iterate(head,print_string);
         p1_sort(head,compare);
         printf("After:\n");
         iterate(head,print_string);

   


//     worked=p1_insert(&head,node_one->data,compare,1);

//     if(worked) 
//     {
//         p1_iterate(head,print_string);
//     }
//     else 
//     {
//         printf("INSTERT_1: FAIL");
//     }

//     worked=p1_insert(&head,node_two->data,compare,1);

//     if(worked) 
//     {
//         p1_iterate(head,print_string);
//     }
//     else 
//     {
//         printf("INSTERT_1: FAIL");
//     }
//     worked=p1_insert(&head,node_three->data,compare,1);

//     if(worked) 
//     {
//         p1_iterate(head,print_string);
//     }
//     else 
//     {
//         printf("INSTERT_1: FAIL");
//     }
//     worked=p1_insert(&head,node_four->data,compare,1);

//     if(worked) 
//     {
//         p1_iterate(head,print_string);
//     }
//     else 
//     {
//         printf("INSTERT_1: FAIL");
//     }
    
// int x;
//   x= deleted=p1_delete_some(&head,delete_s,NULL,free_thing,1);
//   printf("Number of things deleted: %d",x);

       }