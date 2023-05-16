template<class T>
struct suffix_array_search {
  int N;
  vector<T> str;
  vector<int> rnk;
  vector<int> sa;
  vector<int> lcp;


  vector<int> seg;
  int seg_n;

  //sparse_table<int> st;
  
  template<class I>
  suffix_array_search(I b, I e, int k): str(b, e) {
    N = str.size();
    sa = sa_is(str, k);
    rnk.resize(N + 1);
    lcp.resize(N + 1);

    for(int i = 0;i < N + 1;i++) {
      rnk[sa[i]] = i;
    }
    lcp[0] = 0;
    int h = 0;

    for(int i = 0;i < N + 1;i++) {
      if(rnk[i] + 1 < N + 1) {
        int j = sa[rnk[i] + 1];
        for(; j + h < N + 1 && i + h < N + 1; h++) {
          if(str[j + h] != str[i + h]) break;
        }
        lcp[rnk[i] + 1] = h;
        if(h > 0) h--;
      }
    }
  }

  void build_segment() {
    seg_n = 1;
    while(seg_n < N + 1) seg_n <<= 1;
    seg.resize(seg_n * 2, 1e9);
    for(int i = 0;i + 1 < N + 1;i++) {
      seg[i + seg_n - 1] = lcp[i + 1];
    }
    for(int i = seg_n - 1; i --> 0;) {
      seg[i] = min(seg[(i << 1) + 1], seg[(i << 1) + 2]);
    }
  }

  int compare(const vector<T>& t, int pos) {
    int i = 0;
    while(pos + i < str.size() && i < t.size()) {
      if(str[pos + i] < t[i]) {
        return 1;
      }
      else if(str[pos + i] > t[i]) {
        return -1;
      }
      i++;
    }
    if(i == t.size()) return 0;
    else return 1;
  }
  

  // fast!
  bool contain(const vector<T>& t) {
    int L = 0;
    int R = sa.size();
    while(R - L > 1) {
      int M = (L + R) >> 1;
      if(compare(t, sa[M]) >= 0) {
        L = M;
      }
      else {
        R = M;
      }
    }
    return compare(t, sa[L]) == 0;
  }
};
