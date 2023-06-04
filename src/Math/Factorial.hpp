template<class T>
struct factorial {
  vector<T> f;
  vector<T> g;
  vector<T> i;

  void build(int N) {
    f.resize(N);
    g.resize(N);
    i.resize(N);
    f[0] = T(1);
    for(int k = 1; k < N; k++) {
      f[k] = f[k - 1] * T(k);
    }
    g[N - 1] = T(1) / f[N - 1];
    for(int k = N - 1; k --> 0;) {
      g[k] = g[k + 1] * T(k + 1);
    }
    for(int k = 0; k < N; k++) {
      i[k] = f[k - 1] * g[k];
    }
  }
};///
