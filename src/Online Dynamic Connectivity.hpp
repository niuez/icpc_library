const int NODE_SIZE = 303030 * 6;
struct euler_tour_tree {
  using T = long long;

  struct node;
  static struct node n[NODE_SIZE];
  static int ni;

  struct node {
    int s, d;
    int c[3];
    int sz;
    int flag;
    T val;
    T Sigma;
    node(): sz(1) {}
    inline node& operator[](int d) { return n[c[d]]; }
  };///

  int new_edge(int s, int d, bool hi) {
    int i = ni++;
    int ri = ni++;
    n[i].s = n[ri].d = s;
    n[i].d = n[ri].s = d;
    n[i].sz = n[ri].sz = 0;
    n[i].flag = hi;
    return i;
  }///

  static void fix(int i) {
    n[i].sz = (n[i].s == n[i].d) ? 1 : 0;
    if(n[i].c[0]) n[i].sz += n[i][0].sz;
    if(n[i].c[1]) n[i].sz += n[i][1].sz;
    n[i].flag &= 0b0101;
    n[i].flag |= n[i].flag << 1;
    if(n[i].c[0]) n[i].flag |= n[i][0].flag & 0b1010;
    if(n[i].c[1]) n[i].flag |= n[i][1].flag & 0b1010;
    n[i].Sigma = n[i].val;
    if(n[i].c[0]) n[i].Sigma += n[i][0].Sigma;
    if(n[i].c[1]) n[i].Sigma += n[i][1].Sigma;
  }///

  static int child_dir(int i) {
    if(n[i].c[2]) {
      if(n[i][2].c[0] == i) { return 0; }
      else if(n[i][2].c[1] == i) { return 1; }
    }
    return 2;
  }///

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
  }///

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
  }///

  static int merge_back(int l, int r) {
    if(!l) return r;
    if(!r) return l;
    while(n[l].c[1]) l = n[l].c[1];
    splay(l);
    n[n[r].c[2] = l].c[1] = r;
    fix(l);
    return l;
  }///

  static pair<int, int> split(int i) {
    splay(i);
    int l = n[i].c[0];
    n[i].c[0] = n[l].c[2] = 0;
    fix(i);
    return { l, i };
  }///

  static void reroot(int v) {
    auto p = split(v);
    merge_back(p.second, p.first);
    splay(v);
  }///

  static bool same_root(int i, int j) {
    if(i) splay(i);
    if(j) splay(j);
    while(n[i].c[2]) i = n[i].c[2];
    while(n[j].c[2]) j = n[j].c[2];
    return i == j;
  }///

  int n_start;
  unordered_map<long long, int> emp;
  euler_tour_tree() {}
  euler_tour_tree(int N): n_start(ni) {
    ni += N;
    for(int i = 0; i < N; i++) {
      n[i + n_start].s = n[i + n_start].d = i;
    }
  }///

  bool edge_exist(int x, int y) {
    if(x > y) swap(x, y);
    return emp.count(((long long)x << 32) | (long long)y);
  }///

  void link(int x, int y, bool hi) {
    if(x > y) swap(x, y);
    int ei = new_edge(x, y, hi);
    assert(!emp.count(((long long)x << 32) | (long long)y));
    emp[((long long)x << 32) | (long long)y] = ei;
    x += n_start;
    y += n_start;
    reroot(x);
    reroot(y);
    n[n[x].c[2] = ei].c[0] = x;
    n[n[y].c[2] = ei].c[1] = y;
    fix(ei);
    merge_back(ei, ei + 1);
  }///

  void cut(int x, int y) {
    if(x > y) swap(x, y);
    auto iter = emp.find(((long long)x << 32) | (long long)y);
    int ei = iter->second;
    int rei = ei + 1;
    emp.erase(iter);

    auto p = split(ei);
    if(p.first && same_root(p.first, rei)) {
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
  }///

  bool same_tree(int x, int y) {
    return same_root(x + n_start, y + n_start);
  }///

  int tree_size(int x) {
    x += n_start;
    splay(x);
    return n[x].sz;
  }///

  void subedge_set(int x, bool val) {
    x += n_start;
    splay(x);
    if(val) n[x].flag |= (0b0100);
    else n[x].flag &= ~(0b0100);
    fix(x);
  }///

  void add_val(int x, T val) {
    x += n_start;
    splay(x);
    n[x].val += val;
    fix(x);
  }///
  T tree_sum(int x) {
    x += n_start;
    splay(x);
    return n[x].Sigma;
  }///

  template<class Func>
  void hilevel_edges(int v, Func f) {
    int i = v + n_start;
    splay(i);
    while(i && (n[i].flag & 0b0010)) {
      while(1) {
        if(n[i].flag & 0b0001) {
          f(n[i].s, n[i].d);
          splay(i);
          n[i].flag &= ~(0b0001);
          fix(i);
          break;
        }
        else if(n[i].c[0] && (n[i][0].flag & 0b0010)) i = n[i].c[0];
        else i = n[i].c[1];
      }
    }
  }///
  template<class Func>
  int subedges(int v, Func f) {
    int i = v + n_start;
    splay(i);
    while(i && (n[i].flag & 0b1000)) {
      while(1) {
        if(n[i].flag & 0b0100) {
          if(f(n[i].s)) {
            return 1;
          }
          splay(i);
          break;
        }
        else if(n[i].c[0] && (n[i][0].flag & 0b1000)) i = n[i].c[0];
        else i = n[i].c[1];
      }
    }
    return 0;
  }///

};

int euler_tour_tree::ni = 1;
euler_tour_tree::node euler_tour_tree::n[NODE_SIZE];

struct online_dynamic_connectivity {
  int N;
  vector<euler_tour_tree> ett;
  vector<vector<unordered_set<int>>> E;

  online_dynamic_connectivity(int N): N(N) {
    ett.emplace_back(N);
    E.emplace_back(N);
  }///

  void link(int x, int y) {
    if(ett[0].same_tree(x, y)) {
      if(E[0][x].size() == 0) ett[0].subedge_set(x, 1);
      if(E[0][y].size() == 0) ett[0].subedge_set(y, 1);
      E[0][x].insert(y);
      E[0][y].insert(x);
    }
    else {
      ett[0].link(x, y, true);
    }
  }///

  void replace(int x, int y, int level) {
    for(int k = 0; k < level; k++) {
      ett[k].cut(x, y);
    }
    for(int k = level; k --> 0;) {
      if(ett[k].tree_size(x) > ett[k].tree_size(y)) swap(x, y);
      ett[k].hilevel_edges(x, [&](int s, int d) { ett[k + 1].link(s, d, true); });
      int res = ett[k].subedges(x, [&](int s) {
        for(auto iter = E[k][s].begin(); iter != E[k][s].end(); ) {
          int d = *iter;
          iter = E[k][s].erase(iter);
          E[k][d].erase(s);
          if(E[k][s].size() == 0) ett[k].subedge_set(s, 0);
          if(E[k][d].size() == 0) ett[k].subedge_set(d, 0);
          if(ett[k].same_tree(s, d)) {
            if(E[k + 1][s].size() == 0) ett[k + 1].subedge_set(s, 1);
            if(E[k + 1][d].size() == 0) ett[k + 1].subedge_set(d, 1);
            E[k + 1][s].insert(d);
            E[k + 1][d].insert(s);
          }
          else {
            for(int kk = k + 1; kk --> 0;) {
              ett[kk].link(s, d, kk == k);
            }
            return 1;
          }
        }
        return 0;
        });
      if(res) return;
    }
  }///

  void cut(int x, int y) {
    for(int k = 0; k < ett.size(); k++) {
      if(E[k][x].count(y)) {
        E[k][x].erase(y);
        E[k][y].erase(x);
        if(E[k][x].size() == 0) ett[k].subedge_set(x, 0);
        if(E[k][y].size() == 0) ett[k].subedge_set(y, 0);
        return;
      }
    }
    for(int k = ett.size(); k --> 0;) {
      if(ett[k].edge_exist(x, y)) {
        if(k + 1 == ett.size()) {
          ett.emplace_back(N);
          E.emplace_back(N);
        }
        replace(x, y, k + 1);
      }
    }
  }///
  void add_val(int x, long long val) {
    ett[0].add_val(x, val);
  }///
  int size(int x) {
    return ett[0].tree_size(x);
  }///
  long long sum(int x) {
    return ett[0].tree_sum(x);
  }///
  bool same(int x, int y) {
    return ett[0].same_tree(x, y);
  }///
};

