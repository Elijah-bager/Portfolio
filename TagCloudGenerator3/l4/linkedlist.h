/* Elijah Baugher */

int deleteSome(void *p2head,bool (*mustGo)(void *data, void *helper), void *helper, void (*disposal)(void*data), int text);
bool insert(void *p2head, void *data, bool(*goesInFrontOf)(void *data1, void *data2),int text);
void iterate(void *head, void(*doThis)(void *data));
void sort(void *hptr, bool(*cf)( void *data1, void *data2));
