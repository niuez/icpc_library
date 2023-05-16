class bitvector {
  using B = uint_least64_t;
  static constexpr int wordsize = 64;
  
  vector<B> bit;
  vector<int> sum;
  
public:

  bitvector() : bit(), sum() {} 
  bitvector(const int size)
    : bit(size / wordsize + 1, 0), sum(size / wordsize + 1, 0) {}

  void set(const int i) {
    bit[i / wordsize] |= static_cast<B>(1) << (i % wordsize);
  }
  void build() {
    for (int i = 1; i < bit.size(); i++) {
      sum[i] = sum[i - 1] + __builtin_popcountll(bit[i - 1]);
    }
  }

  int at(const int i) const {
    return bit[i / wordsize] >> (i % wordsize);
  }

  // count of ones in [0, i)
  int rank(const int i) const {
    return sum[i / wordsize]
      + __builtin_popcountll(bit[i / wordsize] & (static_cast<B>(1) << (i % wordsize)) - 1);
  }

  // count of ones in [0, i)
  int rank(const int i, const int b) const {
    int ans = sum[i / wordsize]
      + __builtin_popcountll(bit[i / wordsize] & (static_cast<B>(1) << (i % wordsize)) - 1);
    if(b) return ans;
    else return i - ans;
  }
};

class wavelet_matrix {
  using I = int;


  const int depth;
  const int len;
  vector<bitvector> mat;
  vector<int> spl;

public:

  wavelet_matrix(const vector<I>& arr, int de)
    : depth(de),
      len(arr.size()),
      mat(vector<bitvector>(depth, bitvector(arr.size()))),
      spl(vector<int>(depth, 0)) {
        vector<int> idx(len);
        vector<int> left(len), right(len);
        for(int i = 0;i < len;i++) idx[i] = i;
        for(int d = depth; d-- > 0;) {
          int l = 0, r = 0;
          
          for(int i = 0; i < len; i++) {
            int k = (arr[idx[i]] >> d) & 1;
            if(k) right[r++] = idx[i], mat[d].set(i);
            else left[l++] = idx[i];
          }
          mat[d].build();
          spl[d] = l;
          swap(idx, left);
          for(int i = 0; i < r; i++) idx[i + l] = right[i];
        }
      }

  I at(int i) const {
    I x = static_cast<I>(0);
    for(int d = depth; d-- > 0;) {
      int k = mat[d].at(i);
      x |= (static_cast<I>(k) << d);
      i = mat[d].rank(i, k) + spl[d] * k;
    }
    return x;
  }

  // counting elements that equal to x in range [left, right)
  int rank_x(int left, int right, I x) const {
    for(int d = depth; d-- > 0;) {
      int k = ((x >> d) & 1);
      left = mat[d].rank(left, k) + spl[d] * k;
      right = mat[d].rank(right, k) + spl[d] * k;
    }
    return right - left;
  }

  // sorted(arr[left..right])[i]
  I quantile(int left, int right, int i) const {
    I x = static_cast<I>(0);
    for(int d = depth; d-- > 0;) {
      int cnt = mat[d].rank(right, 0) - mat[d].rank(left, 0);
      int k = (i < cnt) ? 0 : 1;
      if(k == 1) {
        i -= cnt;
        x |= (1 << d);
      }
      left = mat[d].rank(left, k) + spl[d] * k;
      right = mat[d].rank(right, k) + spl[d] * k;
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
    for(int d = depth; d --> 0;) {
      int k = (x >> d) & 1;
      int l = mat[d].rank(left, 1);
      int r = mat[d].rank(right, 1);
      if(k == 0) {
        mo += r - l;
        left -= l;
        right -= r;
      }
      else {
        le += (right - left) - (r - l);
        left = l + spl[d];
        right = r + spl[d];
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
      int lr = mat[d].rank(left, 1);
      int rr = mat[d].rank(right, 1);
      return
        rangefreq(left - lr, right - rr, x, y, l, d) +
        rangefreq(lr + spl[d], rr + spl[d], x, y, l + (1 << d), d);
    }
  }

  int rangefreq(int left, int right, I x, I y) const {
    return rangefreq(left, right, x, y, 0, depth);
  }
};
