/* Elijah Baugher */

Node *allocate_node();
void *allocate_thing(long size);
bool compare( void *first, void *second);
bool delete_s(void *data, void *helper);
void free_node(Node *new_node);
void free_thing(void *thing);
int main();
int p1_delete_some(void *p2head, CriteriaFunction mustGo, void *helper,ActionFunction disposal, int text);
bool p1_insert(void *p2head, void *data, ComparisonFunction goesInFrontOf,int text);
void p1_iterate(void *head, ActionFunction doThis);
void p1_sort(void *hptr, ComparisonFunction cf);
void print_string(void *first);
