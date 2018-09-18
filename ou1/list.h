
typedef struct node {
  struct node* prev;
  void* value;
  struct node* next;
}node;

typedef struct user_info {
  unsigned int uid;
  char* uname;
}user_info;

node *linkedList_create(void);

void linkedList_free(node *rootNode);

void linkedList_appendUserInfo(node *rootNode, user_info *value);

void linkedList_append(node *rootNode, void *value);

void linkedList_print(node *rootNode);

void linkedList_swapNodes(node *a, node *b);

void linkedList_sort(node *start);