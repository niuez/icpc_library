namespace fps {
  using M = modint<998244353>; // value type
  using D = modint<998244353>; // DFT type
  using VM = vector<M>;
  using VD = vector<D>;
  const int MOD = 998244353;
  const M PRM = M(3);
  ///
  VD dft(VM a, int n) {
    a.resize(n, M(0));
    for(int s = n >> 1; s >= 1; s >>= 1) {
      M zeta = PRM.pow((MOD - 1) / (s << 1));
      for(i64 i = 0; i < n; i += (s << 1)) {
        M zi = 1;
        for(i64 j = 0;j < s;j++) {
          M t = a[i + j] - a[s + i + j];
          a[i + j] += a[s + i + j];
          a[s + i + j] = t * zi;
          zi = zi * zeta;
        }
      }
    }
    return a;
  }
  VM idft(VD a) {
    int n = a.size();
    for(int s = 1; s < n; s <<= 1) {
      M zeta = PRM.pow((MOD - 1) / (s << 1)).pow(MOD - 2);
      for(int i = 0; i < n; i += (s << 1)) {
        M zi = 1;
        for(int j = 0;j < s;j++) {
          M t = a[s + i + j] * zi;
          a[s + i + j] = a[i + j] - t;
          a[i + j] = a[i + j] + t;
          zi = zi * zeta;
        }
      }
    }
    auto inv_n = M(n).pow(MOD - 2);
    for(int i = 0;i < n;i++) a[i] *= inv_n;
    return a;
  }
  ///
  
  int lg2(int n) {
    int k;
    for (k = 0; n != 0; n >>= 1) k++;
    return k - 1;
  }
  
  int bp2(int sz) {
    return 1LL << (lg2(sz - 1) + 1);
  }///
  
  VM mul(const VM& a, const VM& b) {
    int n = a.size() + b.size() - 1;
    int m = bp2(n);
    auto A = dft(a, m);
    auto B = dft(b, m);
    for(int i = 0; i < m; i++) {
      A[i] *= B[i];
    }
    auto R = idft(A);
    R.resize(n);
    return R;
  }///

  VM inv(const VM& a, int n) {
    VM g { M(1) / a[0] };
    for(int i = 1; i < n; i <<= 1) {
      VD ed = dft(a, i << 1);
      VD gd = dft(g, i << 1);
      for(int k = 0; k < ed.size(); k++) ed[k] *= gd[k];
      VM e = idft(ed);
      for(int k = 0; k < i; k++) {
        e[k] = M(0);
        e[k + i] *= M(-1);
      }
      ed = dft(e, i << 1);
      for(int k = 0; k < ed.size(); k++) ed[k] *= gd[k];
      e = idft(ed);
      for(int k = 0; k < i; k++) {
        e[k] = g[k];
      }
      g = e;
    }
    g.resize(n);
    return g;
  }///
  
  VM diff(const VM& a, int n) {
    VM ans(n);
    for(int i = 0; i + 1 < a.size() && i < n; i++) {
      ans[i] = a[i + 1] * M(i + 1);
    }
    return ans;
  }///
  
  VM integral(const VM& a, int n) {
    VM ans(n);
    int m = std::min(n, int(a.size() + 1));
    ans[0] = M(1);
    for(int i = 1; i < m; i++) {
      ans[i] = ans[i - 1] * M(i);
    }
    M finv = M(1) / ans[m - 1];
    for(int i = m; i --> 1;) {
      ans[i] = a[i - 1] * finv * ans[i - 1];
      finv *= M(i);
    }
    ans[0] = M(0);
    return ans;
  }///
  
  VM log(const VM& a, int n) {
    VM in = inv(a, n);
    VM di = diff(a, n);
    int m = bp2(n);
    auto I = dft(in, m * 2);
    auto D = dft(di, m * 2);
    for(int i = 0; i < I.size(); i++) I[i] *= D[i];
    in = idft(I);
    return integral(in, n);
  }///

  VM exp(const VM& a, int n) {
    VM f { M(1) };
    for(int i = 1; i < n; i <<= 1) {
      VM lg = log(f, i << 1);
      for(int j = 0; j < (i << 1); j++) {
        lg[j] = (j < a.size() ? a[j] : M(0)) - lg[j];
      }
      lg[0] += M(1);
      auto F = dft(f, i << 2);
      auto L = dft(lg, i << 2);
      for(int j = 0; j < F.size(); j++) F[j] *= L[j];
      f = idft(F);
      f.resize(i << 1);
    }
    f.resize(n);
    return f;
  }///
}
