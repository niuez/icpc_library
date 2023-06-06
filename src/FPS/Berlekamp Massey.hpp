// s[0] ~ s[n - 1]
// find c = c[1] ~ s[d] with min d
// for d <= i < N, s[i] = sum{1 <= j <= d} c[j] * a[i - j] 
template<class F>
vector<F> berlekamp_massey(const vector<F>& s) {
  int n = s.size();
  vector<F> b { F(1) };
  vector<F> c { F(1) };
  F y(1);
  int shift = 0;
  for(int len = 0; len < n; len++) {
    shift++;
    F x(0);
    for(int i = 0; i < c.size(); i++) {
      x += c[i] * s[len - i];
    }
    if(x == F(0)) { continue; }
    vector<F> old_c = c;
    F freq = x / y; c.resize(max(c.size(), b.size() + shift), F(0));
    for(int i = 0; i < b.size(); i++) {
      c[i + shift] -= freq * b[i];
    }
    if(old_c.size() < c.size()) {
      b = move(old_c);
      y = x;
      shift = 0;
    }
  }
  vector<F> ans(c.size() - 1);
  for(int i = 1; i < c.size(); i++) {
    ans[i - 1] = -c[i];
  }
  return ans;
}///
