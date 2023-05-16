struct dynamic_segment_tree {
  using T = int;
  static T ide() { return 0ll; }
  static T ope(const T& a, const T& b) { return a + b; }

  struct node {
    node* left;
    node* right;
    T val;
    node(T v): val(v), left(nullptr), right(nullptr) {}
  };

  int n;
  node* root;
  dynamic_segment_tree(const int sz): root(new node(ide())) {
    n = 1;
    while(n < sz) n *= 2;
  }
  T value(node* n) {
    if(n) return n->val;
    else return ide();
  }
  void update(node* n, int i, T x, int l, int r) {
    if(l + 1 == r) {
      n->val = x;
    }
    else {
      int m = (l + r) / 2;
      if(i < m) {
        if(!n->left) {
          n->left = new node(ide());
        }
        update(n->left, i, x, l, m);
      }
      else {
        if(!n->right) {
          n->right = new node(ide());
        }
        update(n->right, i, x, m, r);
      }
      n->val = ope(value(n->left), value(n->right));
    }
  }

  T get(node* n, int a, int b, int l, int r) {
    if(!n) return ide();
    if(a <= l && r <= b) return value(n);
    else if(r <= a || b <= l) return ide();
    else return ope(get(n->left, a, b, l, (l + r) / 2), get(n->right, a, b, (l + r) / 2, r));
  }

  void update(int i, T x) {
    update(root, i, x, 0, n);
  }

  T sum(int a, int b) {
    return get(root, a, b, 0, n);
  }
};
