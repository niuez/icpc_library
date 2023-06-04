struct Mo {
  vector<int> left, right, order;
  vector<bool> v;
  

  const int width;
  int nl, nr, ptr;

  vector<int> a;
  vector<int> cnt;
  int ans;

  Mo(int n, vector<int> a) : v(n), width((int)sqrt(n)), nl(0), nr(0), ptr(0), a(a), cnt(1010101), ans(0) {}
  ///

  void add_query(int l, int r) {
    left.push_back(l);
    right.push_back(r);
  }///

  void build() {
    order.resize(left.size());
    for(int i = 0;i < left.size();i++) order[i] = i;
    sort(begin(order), end(order), [&](int a, int b) {
        if(left[a] / width != left[b] / width) return left[a] < left[b];
        else return right[a] < right[b];
        });
  }///


  void add(int idx) {
    if(cnt[a[idx]]++ == 0) ans++;
  }///
  void del(int idx) {
    if(--cnt[a[idx]] == 0) ans--;
  }///

  inline void distribute(int idx) {
    v[idx].flip();
    if(v[idx]) add(idx);
    else del(idx);
  }///

  int process() {
    if(ptr == order.size()) return -1;
    const auto id = order[ptr];
    while(nl > left[id]) distribute(--nl);
    while(nr < right[id]) distribute(nr++);
    while(nl < left[id]) distribute(nl++);
    while(nr > right[id]) distribute(--nr);
    return order[ptr++];
  }///
};
