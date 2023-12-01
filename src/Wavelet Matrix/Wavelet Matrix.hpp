struct bitv {
  using B = uint_fast64_t;
  const static int W = 64;

  static B popcnt(B x) {
    return __builtin_popcountll(x);
  }

  vector<B> b;
  vector<int> s;

  bitv() : b(), s() {} 
  bitv(const int size)
    : b(size / W + 1, 0), s(size / W + 1, 0) {}

  void set(const int i) {
    b[i / W] |= B(1) << (i % W);
  }
  void build() {
    rep(i, 1, b.size()) {
      s[i] = s[i - 1] + popcnt(b[i - 1]);
    }
  }

  int at(const int i) const {
    return b[i / W] >> (i % W);
  }

  // count of ones in [0, i)
  int rank(const int i, const int v) const {
    int ans = s[i / W] + popcnt(b[i / W] & (B(1) << (i % W)) - 1);
    return v ? ans : i - ans;
  }
};

class WM {
  using I = int;

  const int D;
  const int L;
  vector<bitv> m;
  vector<int> s;

public:

  WM(const vector<I>& A, int de): D(de), L(A.size()) {
    m.resize(D, bitv(A.size()));
    s.resize(D, 0);
    vector<int> p(L);
    vector<int> le(L), ri(L);
    rep(i, 0, L) p[i] = i;
    rrep(d, D, 0) {
      int l = 0, r = 0;
      rep(i, 0, L) {
        int k = (A[p[i]] >> d) & 1;
        if(k) ri[r++] = p[i], m[d].set(i);
        else le[l++] = p[i];
      }
      m[d].build();
      s[d] = l;
      swap(p, le);
      rep(i, 0, r) p[i + l] = ri[i];
    }
  }

  I at(int i) const {
    I x = 0;
    rrep(d, D, 0) {
      I k = m[d].at(i);
      x |= (k << d);
      i = m[d].rank(i, k) + s[d] * k;
    }
    return x;
  }

  // counting elements that equal to x in range [l, r)
  int rank_x(int l, int r, I x) const {
    rrep(d, D, 0) {
      int k = ((x >> d) & 1);
      l = m[d].rank(l, k) + s[d] * k;
      r = m[d].rank(r, k) + s[d] * k;
    }
    return r - l;
  }

  // sorted(arr[left..right])[i]
  I quantile(int l, int r, int i) const {
    I x = 0;
    rrep(d, D, 0) {
      int cnt = m[d].rank(r, 0) - m[d].rank(l, 0);
      int k = (i < cnt) ? 0 : 1;
      if(k == 1) {
        i -= cnt;
        x |= (I(1) << d);
      }
      l = m[d].rank(l, k) + s[d] * k;
      r = m[d].rank(r, k) + s[d] * k;
    }
    return x;
  }

  struct rank_result {
    int le;
    int eq;
    int mo;
  };

  // couting elements that less than x, equal to x, and more than x in range [left, right)
  rank_result rank_less_eq_more(int left, int right, I x) const {
    int le = 0, mo = 0;
    rrep(d, D, 0) {
      int k = (x >> d) & 1;
      int l = m[d].rank(left, 1);
      int r = m[d].rank(right, 1);
      if(k == 0) {
        mo += r - l;
        left -= l;
        right -= r;
      }
      else {
        le += (right - left) - (r - l);
        left = l + s[d];
        right = r + s[d];
      }
    }
    return rank_result { le, right - left, mo };
  }

  int rangefreq(int left, int right, I x, I y, I l, int d) const {
    I r = l + (1 << d);
    if(x <= l && r <= y) {
      return right - left;
    }
    else if(y <= l || r <= x) {
      return 0;
    }
    else {
      d--;
      int lr = m[d].rank(left, 1);
      int rr = m[d].rank(right, 1);
      return
        rangefreq(left - lr, right - rr, x, y, l, d) +
        rangefreq(lr + s[d], rr + s[d], x, y, l + (I(1) << d), d);
    }
  }

  int rangefreq(int left, int right, I x, I y) const {
    return rangefreq(left, right, x, y, 0, D);
  }
};

