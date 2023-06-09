struct union_find {
  vector<int> par;
  union_find(int N): par(N, -1) {}
  int root(int x) {
    return par[x] < 0 ? x : par[x] = root(par[x]);
  }///
  tuple<int, int> unite(int x, int y) {
    x = root(x);
    y = root(y);
    if(x == y) return { -1, -1 };
    if(par[x] > par[y]) swap(x, y);
    par[x] += par[y];
    par[y] = x;
    return { x, y };
  }///
  int size(int x) {
    return -par[root(x)];
  }///
};
