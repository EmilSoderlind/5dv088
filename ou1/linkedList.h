
typedef struct node {
  struct node* prev;
  void* value;
  struct node* next;
}node;

typedef struct user_info {
  unsigned int uid;
  char* uname;
}user_info;
