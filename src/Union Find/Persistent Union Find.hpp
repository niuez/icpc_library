struct partially_persistent_union_find {
  vector<int> data;
  vector<int> last;
  vector<vector<pair<int, int>>> add;

  partially_persistent_union_find(int sz) : data(sz, -1), last(sz, 1e9), add(sz) {
    for(auto & vs: add) vs.push_back({ -1, -1 });
  }///

  int unite(int t, int x, int y) {
    x = find(t, x);
    y = find(t, y);
    if(x == y) return -1;
    if(data[x] > data[y]) swap(x, y);
    data[x] += data[y];
    add[x].push_back({t, data[x]});
    data[y] = x;
    last[y] = t;
    return x;
  }///

  int find(int t, int x) {
    if(t < last[x]) return x;
    return find(t, data[x]);
  }///

  int size(int t, int x) {
    x = find(t, x);
    return -prev(lower_bound(begin(add[x]), end(add[x]), make_pair(t, 0ll)))->second;
  }///
};
