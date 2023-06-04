template<class T, class Compare>
struct pairing_heap {
  struct node {
    T val;
    node* head;
    node* next;
    node(const T& v) : val(v), head(nullptr), next(nullptr) {  }
  };
  int sz;
  node* root;
  Compare comp;
  pairing_heap() : sz(0), root(nullptr) {  }
  ///
  node* merge(node* x, node* y) {
    if(!y) return x;
    if(!x) return y;
    if(!comp(x->val, y->val)) swap(x,y);
    y->next = x->head;
    x->head = y;
    return x;
  }///
  node* mergeList(node * x) {
    node* nn = nullptr;
    while(x) {
      node* a = x;
      node* b = nullptr;
      x = x->next;
      a->next = nullptr;
      if(x) {
        b = x;
        x = x->next;
        b->next = nullptr;
      }
      a = merge(a, b);
      a->next = nn;
      nn = a;
    }
    while(nn) {
      node* j = nn;
      nn = nn->next;
      x = merge(j,x);
    }
    return x;
  }///
  
  T top() {
    return root->val;
  }///

  void pop() {
    --sz;
    node* te = root;
    root = mergeList(root->head);
    delete te, te = nullptr;
  }///
  
  void push(const T& x) {
    ++sz;
    root = merge(new node(x), root);
  }///
  
  int size() {
    return sz;
  }///
  
  void meld(pairing_heap<T,Compare>& h) {
    root = merge(root, h.root);
    h.root = nullptr;
    h.sz = 0;
  }///
};
