template<class T>
struct Compression {
  vector<T> v;

  Compression(){}
  void add(const T& t) { v.push_back(t); }
  void build() {
    sort(begin(v), end(v));
    v.erase(unique(begin(v), end(v)), end(v));
  }
  int comp(const T& x) const {
    return lower_bound(begin(v), end(v), x) - begin(v);
  }
  int size() const { return v.size(); }
};

template<class T>
struct plane_to_line {
  using P = pair<T, T>;

  vector<pair<P, int>> elems;
  vector<int> yarray;
  vector<int> xstart;
  Compression<T> X, Y;

  plane_to_line(const vector<P>& a): elems(a.size()) {
    for(int i = 0;i < a.size();i++) {
      elems[i] = { a[i], i };
      X.add(a[i].first);
      Y.add(a[i].second);
    }
    sort(begin(elems), end(elems));
    X.build();
    Y.build();

    xstart.resize(X.size() + 1);
    yarray.resize(elems.size());
    int x = 0;
    for(int i = 0;i < elems.size();i++) {
      if(!i || elems[i - 1].first.first < elems[i].first.first) {
        xstart[x++] = i;
      }
      yarray[i] = Y.comp(elems[i].first.second);
    }
    xstart[X.size()] = elems.size();
  }

  struct rangefreq_arg {
    int left;
    int right;
    int x;
    int y;
  };

  rangefreq_arg to_rangefreq(T xl, T xr, T yl, T yr) const {
    int cxl = xstart[X.comp(xl)];
    int cxr = xstart[X.comp(xr)];
    int cyl = Y.comp(yl);
    int cyr = Y.comp(yr);
    return rangefreq_arg { cxl, cxr, cyl, cyr };
  }
};
