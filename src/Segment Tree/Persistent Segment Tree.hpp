struct persistent_segment_tree {
  using T = pair<int, int>;
  struct node {
    using Ptr = node*;
    T data;
    Ptr left;
    Ptr right;
    
    node(T data): data(data), left(), right() {}
    node(T data, Ptr left, Ptr right)
      : data(data), left(left), right(right) {}
  };///

  using Ptr = node*;

  static T ope(const T& a, const T& b) { return std::min(a, b); }
  static T ide() { return {(int)(1e18), (int)(1e18)}; }

  Ptr root;
  int N;///

  static Ptr build(int l, int r, const vector<T>& init) {
    if(l + 1 >= r) return new node(init[l]);
    else {
      Ptr le = build(l , (l + r) / 2, init);
      Ptr ri = build((l + r) / 2, r, init);
      T da = ope(le->data, ri->data);
      return new node(da, le, ri);
    }
  }///

  static Ptr update(Ptr node, int i, T val, int l, int r) {
    if(i == l && i + 1 == r) return new struct node(val);
    Ptr left = nullptr;
    Ptr right = nullptr;
    if(l <= i && i < ((l + r) >> 1)) {
      left = update(node->left, i, val, l, (l + r) >> 1);
      right = node->right;
    }
    else {
      left = node->left;
      right = update(node->right, i, val, (l + r) >> 1, r);
    }
    return new struct node(ope(left->data, right->data), left, right);
  }///

  static T sum(Ptr node, int a, int b, int l, int r) {
    if(b <= l || r <= a) return ide();
    else if(a <= l && r <= b) return node->data;
    else return ope(
        sum(node->left, a, b, l, (l + r) >> 1),
        sum(node->right, a, b, (l + r) >> 1, r)
        );
  }///

  persistent_segment_tree(const vector<T>& init)
    : root(build(0, init.size(), init)), N(init.size()) {}
  persistent_segment_tree(Ptr root, int N): root(root), N(N) {}
  persistent_segment_tree update(int i, T x) const {
    return persistent_segment_tree(update(root, i, x, 0, N), N);
  }
  T sum(int l, int r) { return sum(root, l, r, 0, N); }
}; ///
