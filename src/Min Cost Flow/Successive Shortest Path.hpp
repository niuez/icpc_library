struct ssp {
  using C = int;
  struct edge {
    int from;
    int to;
    C cap;
    C cost;
    int rev;
  };
  int n;
  int s, t;
  vector<vector<edge>> g;

  ssp(int n, int s, int t): n(n), s(s), t(t), g(n) {}
  void add_edge(int from, int to, int cap, int cost) {
    g[from].push_back({ from, to, cap, cost, (int)(g[to].size()) });
    g[to].push_back({ to, from, 0, -cost, (int)(g[from].size() - 1) });
  }
  vector<int> level;
  vector<int> iter;
  C dinic_dfs(int v, C f) {
    if(v == t) return f;
    else {
      C now = f;
      for(int& i = iter[v]; i < g[v].size(); i++) {
        auto& e = g[v][i];
        if(e.cap > 0 && level[e.to] > level[e.from]) {
          C c = min(now , e.cap);
          C d = dinic_dfs(e.to, c);
          e.cap -= d;
          g[e.to][e.rev].cap += d;
          now -= d;
          if(now == 0) return f - now;
        }
      }
      return f - now;
    }
  }

  C min_cost_flow(C f) {
    vector<C> po(n,0);
    C ans = 0;
    auto sel = [&](const edge& e) {
      return e.cost + po[e.from] - po[e.to];
    };
    while(f > 0) {
      vector<C> dist(n,0);
      vector<bool> vis(n,false);
      vector<int> p(n,-1);
      vector<int> pe(n,-1);
      using P = pair<C,int>;
      priority_queue<P,vector<P>,greater<P>> que;
      que.push({dist[s] ,s});
      vis[s] = true;
      while(!que.empty()) {
        int v = que.top().second;
        C d = que.top().first;
        que.pop();
        if(dist[v] < d) continue;
        for(int i = 0;i < g[v].size();i++) {
          auto& e = g[v][i];
          int u = e.to;
          if(e.cap == 0) continue;
          if(!vis[u] || dist[u] > dist[v] + sel(e)) {
            vis[u] = true;
            dist[u] = dist[v] + sel(e);
            p[u] = v;
            pe[u] = i;
            que.push({dist[u] , u});
          }
        }
      }
      if(p[t] == -1) break;

      C ff = f;
      for(int u = t;u != s;u = p[u])
        ff = min(ff, g[p[u]][pe[u]].cap);
      for(int u = t;u != s;u = p[u]) {
        ans += ff * g[p[u]][pe[u]].cost;
        g[p[u]][pe[u]].cap -= ff;
        g[u][g[p[u]][pe[u]].rev].cap += ff;
      }
      f -= ff;
      for(int i = 0;i < n;i++)
        po[i] += dist[i];
    }
    if(f != 0) return -1;
    return ans;
  }
};

