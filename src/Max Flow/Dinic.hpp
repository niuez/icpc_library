struct dinic {
  using C = int;
  const C INF = 1e9;
  struct edge {
    int to;
    C cap;
    int rev;
  };
  int n;
  vector<vector<edge>> g;

  dinic(int n): n(n), g(n) {}

  void add_edge(int from, int to, C cap, C rev_cap = 0) {
    g[from].push_back({ to, cap, (int)(g[to].size()) });
    g[to].push_back({ from, rev_cap, (int)(g[from].size() - 1) });
  }
  
  vector<int> level;
  vector<int> iter;
  C dfs(const int s, const int v, C mf) {
    if(s == v || mf == 0) return mf;
    for(int& i = iter[v]; i < g[v].size(); i++) {
      int t = g[v][i].to;
      edge& re = g[v][i];
      edge& e = g[t][re.rev];
      if(level[t] >= level[v] || e.cap == 0) continue;
      C f = dfs(s, t, min(mf, e.cap));
      if(f == 0) continue;
      e.cap -= f;
      re.cap += f;
      return f;
    }
    return 0;
  }

  C max_flow(int s, int t) {
    vector<int> que(n);
    C flow = 0;
    while(true) {
      level.assign(n, -1);
      int qi = 0;
      int qr = 0;
      level[s] = 0;
      que[qr++] = s;
      while(qi < qr && level[t]) {
        int v = que[qi++];
        for(const auto& e: g[v]) {
          if(e.cap > 0 && level[e.to] == -1) {
            level[e.to] = level[v] + 1;
            que[qr++] = e.to;
          }
        }
      }
      if(level[t] == -1) break;
      iter.assign(n, 0);
      C tmp;
      while((tmp = dfs(s, t, INF)) > 0) {
        flow += tmp;
      }
    }
    return flow;
  }
};
