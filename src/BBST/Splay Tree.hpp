const int SPLAY_ARRAY_NODE = 1010101;

template<class K, class V>
struct splay_array {
  struct node;
  static struct node n[SPLAY_ARRAY_NODE];
  static int ni;
  struct node {
    int c[3];
    K key;
    V val;
    V f;
    int sz;
    bool rev;

    node() {}
    node& operator[](int dir) { return n[c[dir]]; }
  };
  static int new_node(const V& key, const V& val) {
    int i = ni++;
    n[i].key = key;
    n[i].val = val;
    n[i].f = val;
    n[i].sz = 1;
    return i;
  }
  static void fix(int i) {
    if(!i) return;
    n[i].sz = 1 + n[i][0].sz + n[i][1].sz;
    n[i].f = n[i][0].f + n[i].val + n[i][1].f;
  }
  static void reverse(int i) {
    if(!i) return;
    n[i].rev ^= true;
    // here reversing operation
    // example swap(fold, revfold)
  }
  static void push(int i) {
    if(!i) return;
    if(n[i].rev) {
      swap(n[i].c[0], n[i].c[1]);
      reverse(n[i].c[0]);
      reverse(n[i].c[1]);
      n[i].rev = false;
    }
  }
  static int child_dir(int i) {
    if(n[i].c[2]) {
      if(n[i][2].c[0] == i) return 0;
      return 1;
    }
    return 2;
  }
  static void rotate(int x, int dir) {
    int p = n[x].c[2];
    int x_dir = child_dir(x);
    int y = n[x].c[dir ^ 1];

    n[n[y][dir].c[2] = x].c[dir ^ 1] = n[y].c[dir];
    n[n[x].c[2] = y].c[dir] = x;
    n[y].c[2] = p;
    if(p) n[p].c[x_dir] = y;
    fix(n[x].c[dir ^ 1]);
    fix(x);
  }
  static void splay(int i) {
    push(i);
    int i_dir;
    int j_dir;
    while(n[i].c[2]) {
      int j = n[i].c[2];
      if(n[j].c[2]) {
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
  }
  static int leftist_node(int i) {
    if(push(i), n[i].c[0]) i = n[i].c[0];
    return i;
  }
  static int merge_raw(int l, int r) {
    int leftist = leftist_node(r);
    splay(leftist);
    n[n[l].c[2] = leftist].c[0] = l;
    fix(leftist);
    return leftist;
  }
  int endn;
  int root;
  splay_array() {}
  splay_array(int i) {
    endn = ni++;
    root = endn;
    n[n[i].c[2] = root].c[0] = i;
    fix(root);
  }
  int end_node() const { return endn; }
  int find_by_index(int pos) {
    int i = root;
    while(push(i), n[i][0].sz != pos) {
      if(pos < n[i][0].sz) {
        i = n[i].c[0];
      }
      else {
        pos -= n[i][0].sz + 1;
        i = n[i].c[1];
      }
    }
    splay(root = i);
    return i;
  }
  int lower_bound_by_key(const K& key) {
    int i = root;
    int p = endn;
    while(push(i), i != 0) {
      if(i == endn) {
        i = n[i].c[0];
      }
      else if(key == n[i].key) {
        p = i;
        break;
      }
      else if(key < n[i].key) {
        p = i;
        i = n[i].c[0];
      }
      else {
        i = n[i].c[1];
      }
    }
    splay(root = p);
    return p;
  }
  void splay_node(int i) {
    splay(root = i);
  }
  void merge(splay_array right) {
    splay(endn);
    root = n[endn].c[0];
    root = merge_raw(n[endn].c[0], right.root);
    endn = right.endn;
  }
  // i belong right, return left
  splay_array split(int i) {
    splay(root = i);
    i = n[root].c[0];
    n[i].c[2] = n[root].c[0] = 0;
    fix(root);
    return splay_array(i);
  }
  void insert_dir(int i, int target, int dir) {
    splay(root = i);
    int x = n[i].c[dir];
    n[n[target].c[2] = i].c[dir] = target;
    n[n[x].c[2] = target].c[dir] = x;
    fix(target);
    fix(i);
  }
  void erase(int i) {
    assert(endn != i);
    splay(root = i);
    int l = n[i].c[0];
    int r = n[i].c[1];
    n[l].c[2] = n[r].c[2] = n[i].c[0] = 0;
    root = merge_raw(l, r);
  }
};

template<class K, class V>
typename splay_array<K, V>::node splay_array<K, V>::n[SPLAY_ARRAY_NODE];
template<class K, class V>
int splay_array<K, V>::ni = 1;
