struct euler_tour_tree {

  struct node;
  static struct node n[2020];
  static int ni;

  struct node {
    int s, d;
    int c[3];
    int sz;
    node(): sz(1) {}
    inline node& operator[](int d) { return n[c[d]]; }
  };

  int new_edge(int s, int d) {
    int i = ni++;
    int ri = ni++;
    n[i].s = n[ri].d = s;
    n[i].d = n[ri].s = d;
    return i;
  }

  static void fix(int i) {
    n[i].sz = (n[i].s == n[i].d) ? 1 : 0;
    if(n[i].c[0]) n[i].sz += n[i][0].sz;
    if(n[i].c[1]) n[i].sz += n[i][1].sz;
  }

  static int child_dir(int i) {
    if(n[i].c[2]) {
      if(n[i][2].c[0] == i) { return 0; }
      else if(n[i][2].c[1] == i) { return 1; }
    }
    return 2;
  }

  static void rotate(int x, int dir) {
    int p = n[x].c[2];
    int x_dir = child_dir(x);
    int y = n[x].c[dir ^ 1];
    if(n[y].c[dir]) n[y][dir].c[2] = x;
    n[x].c[dir ^ 1] = n[y].c[dir];
    n[n[x].c[2] = y].c[dir] = x;
    n[y].c[2] = p;
    if(x_dir < 2) n[p].c[x_dir] = y;
    if(n[x].c[dir ^ 1]) fix(n[x].c[dir ^ 1]);
    fix(x);
  }

  static void splay(int i) {
    int i_dir;
    int j_dir;
    while((i_dir = child_dir(i)) < 2) {
      int j = n[i].c[2];
      if((j_dir = child_dir(j)) < 2) {
        int k = n[j].c[2];
        if(i_dir == j_dir) rotate(k, j_dir ^ 1), rotate(j, i_dir ^ 1);
        else rotate(j, i_dir ^ 1), rotate(k, j_dir ^ 1);
      }
      else rotate(j, i_dir ^ 1);
    }
    fix(i);
  }

  static int merge_back(int l, int r) {
    if(!l) return r;
    if(!r) return l;
    while(n[l].c[1]) l = n[l].c[1];
    splay(l);
    n[n[r].c[2] = l].c[1] = r;
    fix(l);
    return l;
  }

  static std::pair<int, int> split(int i) {
    splay(i);
    int l = n[i].c[0];
    n[i].c[0] = n[l].c[2] = 0;
    fix(i);
    return { l, i };
  }

  static void reroot(int v) {
    auto p = split(v);
    merge_back(p.second, p.first);
    splay(v);
  }

  static bool same_root(int i, int j) {
    if(i) splay(i);
    if(j) splay(j);
    while(n[i].c[2]) i = n[i].c[2];
    while(n[j].c[2]) j = n[j].c[2];
    return i == j;
  }

  int n_start;
  euler_tour_tree(int N): n_start(ni) {
    ni += N;
    for(int i = 0; i < N; i++) {
      n[i + n_start].s = n[i + n_start].d = i;
    }
  }


  int link(int x, int y) {
    int ei = new_edge(x, y);
    x += n_start;
    y += n_start;
    reroot(x);
    reroot(y);
    n[n[x].c[2] = ei].c[0] = x;
    n[n[y].c[2] = ei].c[1] = y;
    fix(ei);
    merge_back(ei, ei + 1);
    return ei;
  }

  void cut(int ei) {
    int rei = ei + 1;
    auto p = split(ei);
    if(same_root(p.first, rei)) {
      auto q = split(rei);
      int left = q.first;
      int center = n[q.second].c[1];
      int right = n[p.second].c[1];
      n[center].c[2] = 0;
      n[right].c[2] = 0;
      merge_back(left, right);
    }
    else {
      splay(ei);
      ei = n[ei].c[1];
      n[ei].c[2] = 0;
      auto q = split(rei);
      splay(p.first);
      int left = p.first;
      int center = q.first;
      int right = n[q.second].c[1];
      n[right].c[2] = 0;
      merge_back(left, right);
    }
  }
};

int euler_tour_tree::ni = 1;
euler_tour_tree::node euler_tour_tree::n[2020];

