int returnNumber(int);

typedef struct node {
  struct node* prev;
  void* value;
  struct node* next;
}

typedef struct user_info {
  unsigned int uid;
  char* uname;
};
