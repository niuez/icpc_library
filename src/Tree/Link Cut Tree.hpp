namespace lctree {

  struct R {
    int a;
    R(): a(0) {}
    R(int a): a(a) {}
  };///
  struct V {
    int a;
    V(): a(0) {}
    V(int a): a(a) {}
  };///
  inline V compress(const V& a, const V& b) { return V(a.a + b.a); }
  inline V rake_merge(const V& a, const R& b) { return V(a.a + b.a); }
  inline V reverse(const V& a) { return a; }
  inline void rake_plus(R& a, const V& b) { a.a += b.a; }
  inline void rake_minus(R& a, const V& b) { a.a -= b.a; }

  struct node;
  extern struct node n[505050];
  extern int ni;

  struct node {
    int c[3];
    V v; V f; R r;
    bool rev;
    node(): rev(false) { c[0] = c[1] = c[2] = 0; }
    node& operator[](int d) { return n[c[d]]; }
  };///

  inline int new_node(V v) { n[ni].v = v; n[ni].f = v; return ni++; }
  inline void reverse(int i) {
    n[i].v = reverse(n[i].v);
    n[i].f = reverse(n[i].f);
    n[i].rev ^= true;
  }///
  inline void push(int i) {
    if(n[i].rev) {
      swap(n[i].c[0], n[i].c[1]);
      if(n[i].c[0]) reverse(n[i].c[0]);
      if(n[i].c[1]) reverse(n[i].c[1]);
      n[i].rev = false;
    }
  }///
  inline void fix(int i) {
    push(i);
    n[i].f = compress(compress(n[i][0].f, n[i].v), rake_merge(n[i][1].f, n[i].r));
  }///

  inline int child_dir(int i) {
    if(n[i].c[2]) {
      if(n[i][2].c[0] == i) { return 0; }
      else if(n[i][2].c[1] == i) { return 1; }
    }
    return 3;
  }///

  inline void rotate(int x, int dir) {
    int p = n[x].c[2];
    int x_dir = child_dir(x);
    int y = n[x].c[dir ^ 1];

    n[n[y][dir].c[2] = x].c[dir ^ 1] = n[y].c[dir];
    n[n[x].c[2] = y].c[dir] = x;
    n[y].c[2] = p;
    if(x_dir < 2) n[p].c[x_dir] = y;
    fix(n[x].c[dir ^ 1]);
    fix(x);
  }///

  void splay(int i) {
    push(i);
    int i_dir;
    int j_dir;
    while(child_dir(i) < 2) {
      int j = n[i].c[2];
      if(child_dir(j) < 2) {
        int k = n[j].c[2];
        push(k), push(j), push(i);
        i_dir = child_dir(i);
        j_dir = child_dir(j);
        if(i_dir == j_dir) rotate(k, j_dir ^ 1), rotate(j, i_dir ^ 1);
        else rotate(j, i_dir ^ 1), rotate(k, j_dir ^ 1);
      }
      else push(j), push(i), rotate(j, child_dir(i) ^ 1);
    }
    fix(i);
  }///

  int expose(int i) {
    int right = 0;
    int ii = i;
    while(i) {
      splay(i);
      rake_minus(n[i].r, n[right].f);
      rake_plus(n[i].r, n[i][1].f);
      n[i].c[1] = right;
      fix(i);
      right = i;
      i = n[i].c[2];
    }
    splay(ii);
    return ii;
  }///

  void link(int i, int j) {
    if(!i || !j) return;
    expose(i);
    expose(j);
    n[n[j].c[2] = i].c[1] = j;
    fix(i);
  }///

  void cut(int i) {
    if(!i) return;
    expose(i);
    int p = n[i].c[0];
    n[i].c[0] = n[p].c[2] = 0;
    fix(i);
  }///

  void evert(int i) {
    if(!i) return;
    expose(i);
    reverse(i);
    push(i);
  }///

  node n[505050];
  int ni = 1;

  int all_tree(int i) {
    expose(i);
    return n[i].f.a;
  }///
}
