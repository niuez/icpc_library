template<class T>
vector<T> newton_interpolation(vector<pair<T, T>> ps) {
  vector<T> diff(ps.size());
  diff[0] = ps[0].second;
  vector<T> x(ps.size()), y(ps.size());
  for(int i = 0;i < ps.size(); i++) {
    x[i] = ps[i].first, y[i] = ps[i].second;
  }
  for(int i = 1;i < ps.size();i++) {
    vector<T> d(ps.size() - i);
    for(int j = 0;j + i < ps.size(); j++) {
      d[j] = (y[j + 1] - y[j]) / (x[j + i] - x[j]);
    }
    diff[i] = d[0];
    swap(y, d);
  }
 
  vector<T> ans(ps.size());
 
  vector<T> c(ps.size() + 1);
  c[0] = 1;
 
 
  for(int i = 0;i < ps.size();i++) {
    for(int j = 0;j < ps.size();j++) {
      ans[j] += diff[i] * c[j];
    }
    if(i + 1 == ps.size()) break;
 
    vector<T> next(ps.size() + 1);
    for(int j = 0;j < ps.size();j++) {
      next[j + 1] = c[j];
    }
    for(int j = 0;j < ps.size();j++) {
      next[j] -= c[j] * ps[i].first;
    }
    swap(c, next);
  }
  return ans;
}///
