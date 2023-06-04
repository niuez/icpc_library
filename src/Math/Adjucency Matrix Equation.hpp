struct adjucency_matrix_equation {
  int V;
  int E;
  vector<vector<pair<int, int>>> G;
  adjucency_matrix_equation(int n): V(n), E(0), G(n) {}

  void add_edge(int v, int u) {
    G[v].emplace_back(u + V, E);
    G[u].emplace_back(v, E);
    E++;
  }///
  
  template<class T>
  vector<T> solve(const vector<T>& c) const {
    vector<int> vis(V, 0);
    vector<T> ans(E, 0);

    auto dfs = [&](auto f, int v) -> T {
      vis[v] = 1;
      T r = c[v];
      for(auto e: G[v]) {
        int t = e.first;
        int ei = e.second;
        if(vis[t < V ? t : t - V]) continue;
        T y;
        if(t < V) ans[ei] = (y = f(f, t));
        else      ans[ei] = T(0) - (y = f(f, t - V));
        r = r + y;
      }
      return r;
    };

    for(int v = 0; v < V; v++) {
      if(vis[v]) continue;
      T y = dfs(dfs, v);
      if(y != T(0)) return vector<T>();
    }
    return ans;
  }///
};
