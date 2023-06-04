template<class T, T (*ide)(), T (*ope)(T, T)>
struct segment_tree {
  int n;
  vector<T> node;

  segment_tree() {}
  segment_tree(const vector<T>& init) {
    n = 1;
    while(n < init.size()) n *= 2;
    node.resize(2 * n, ide());
    for(int i = 0;i < init.size();i++) node[i + n] = init[i];
    for(int i = n; i --> 1;) node[i] = ope(node[i * 2], node[i * 2 + 1]);
  }///
 
  void update(int i, T x) {
    i += n;
    node[i] = x;
    while(i > 1) {
      i >>= 1;
      node[i] = ope(node[i * 2], node[i * 2 + 1]);
    }
  }

  T sum(int l, int r) const {
    T lx = ide();
    T rx = ide();
    l += n;
    r += n;
    while(l < r) {
      if(l & 1) { lx = ope(lx, node[l++]); }
      if(r & 1) { rx = ope(node[--r], rx); }
      l >>= 1;
      r >>= 1;
    }
    return ope(lx, rx);
  }

  const T& at(int i) const {
    return node[i + n];
  }///

  template<class F>
  int subtree_down_first(int i, T lx, F isok) const {
    while(i < n) {
      T next = ope(lx, node[i * 2]);
      if(isok(next)) i = i * 2;
      else {
        lx = next;
        i = i * 2 + 1;
      }
    }
    return i - n + 1;
  }

  template<class F>
  int find_first(int l, F isok) const {
    if(isok(ide())) {
      return l;
    }
    if(l == 0) {
      if(isok(node[1])) return subtree_down_first(1, ide(), isok);
      return -1;
    }
    T lx = ide();
    int r = n << 1;
    l += n;
    while(l < r) {
      if(l & 1) {
        T next = ope(lx, node[l]);
        if(isok(next)) return subtree_down_first(l, lx, isok);
        lx = next;
        l++;
      }
      l >>= 1;
      r >>= 1;
    }
    return -1;
  }///
};
