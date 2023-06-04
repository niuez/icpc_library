struct HeavyLightDecomposition {
  vector<vector<int>> G;
  vector<int> in, out;
  vector<int> sz;
  vector<int> seq;
  vector<int> next;
  vector<int> par;


  HeavyLightDecomposition(int n)
    : G(n), in(n), out(n), sz(n), next(n, -1), par(n, -1) {}
  ///

  void add_edge(int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
  }///

  void dfs_sz(int v, int f) {
    sz[v] = 1;
    for(int i = 0;i < G[v].size();i++) {
      int x = G[v][i];
      if(x == f) continue;
      dfs_sz(x, v);
      par[x] = v;
      sz[v] += sz[x];
      if(sz[G[v][0]] < sz[G[v][i]]) { swap(G[v][0], G[v][i]); }
    }
  }///

  int dfs_eul(int v, int f, int t) {
    in[v] = t++;
    seq.push_back(v);
    for(int i = 0;i < G[v].size();i++) {
      int x = G[v][i];
      if(x == f) continue;
      next[x] = (i == 0) ? next[v] : x;
      t = dfs_eul(x, v, t);
    }
    return out[v] = t;
  }///

  void build(int r) {
    dfs_sz(r, -1);
    dfs_eul(r, -1, 0);
  }///

  const vector<int>& sequence() const { return seq; }
  int lca(int a, int b) const {
    while(true) {
      if(in[b] > in[a]) swap(a, b);
      if(next[b] == next[a]) return b;
      a = par[next[a]];
    }
  }///

  pair<vector<pair<int, int>>, vector<pair<int, int>>> path(int a, int b, bool edge) const {
    vector<pair<int, int>> l, r;
    while(true) {
      if(in[b] > in[a]) { swap(a, b); swap(l, r); }
      if(next[b] == next[a]) {
        l.push_back({ in[b] + !!edge, in[a] + 1 });
        break;
      }
      l.push_back({ in[next[a]], in[a] + 1 });
      a = par[next[a]];
    }
    return { std::move(l), std::move(r) };
  }///

  pair<int, int> subtree(int v, bool edge) { return { in[v] + !!edge, out[v] }; }
};
