template<const int H, const int W>
struct bit_matrix {
  array<bitset<W>, H> m;

  bit_matrix(){} 

  bitset<W>& operator[](int i) { return m[i]; }
  const bitset<W>& operator[](int i) const { return m[i]; }

  template<const int X>
  bit_matrix<H, X> operator*(const bit_matrix<W, X>& r) const {
    bit_matrix<H, X> ans;
    bit_matrix<X, W> tr;
    for(int i = 0;i < W;i++) for(int j = 0;j < X;j++) {
      if(r[i][j]) tr[j].set(i);
    }
    for(int i = 0;i < H;i++) for(int j = 0;j < X;j++) {
      ans[i][j] = (m[i] & tr[j]).count() & 1;
    }
    return ans;
  }
};

template<const int H, const int W>
vector<int> gauss_jordan(bit_matrix<H, W>& A, bool is_extended = false) {
  int rank = 0;
  vector<int> fst;
  for(int j = 0; j < W; j++) {
    if(is_extended && j + 1 == W) break;
    int pivot = -1;
    for(int i = rank; i < H; i++) {
      if(A[i].test(j)) {
        pivot = i;
        break;
      }
    }
    if(pivot == -1) continue;
    swap(A[rank], A[pivot]);
    for(int i = 0;i < H; i++) {
      if(i != rank && A[i][j]) {
        A[i] ^= A[rank];
      }
    }
    fst.push_back(j);
    rank++;
  }
  return fst;
}
