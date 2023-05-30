struct strongly_connected_components {
  vector<vector<int>> g;
  vector<vector<int>> rg;
  vector<int> vis;
  vector<int> vs;
 
  vector<int> group;
  vector<vector<int>> comps;
  int groups;
 
  strongly_connected_components(int N): g(N), rg(N), vis(N, 0), group(N, -1) {}
 
  void add_edge(int a, int b) {
    g[a].push_back(b);
    rg[b].push_back(a);
  }
 
  void dfs(int v) {
    vis[v] = 1;
    for(auto& t: g[v]) {
      if(!vis[t]) dfs(t);
    }
    vs.push_back(v);
  }
 
  void rdfs(int v, int k) {
    vis[v] = 1;
    group[v] = k;
    comps.back().push_back(v);
    for(auto to: rg[v]) {
      if(!vis[to]) rdfs(to, k);
    }
  }
 
  void build() {
    for(int i = 0; i < g.size(); i++) {
      if(!vis[i]) dfs(i);
    }
    vis.assign(g.size(), false);
    groups = 0;
    for(int i = g.size(); i --> 0; ) {
      if(!vis[vs[i]]) {
        comps.push_back(vector<int>());
        rdfs(vs[i], groups++);
      }
    }
  }
 
  vector<vector<int>> build_compressed_graph() {
    vector<vector<int>> cg(groups);
    for(int i = 0; i < g.size(); i++) {
      for(auto& j: g[i]) {
        cg[group[i]].push_back(group[j]);
      }
    }
    return cg;
  }
};
