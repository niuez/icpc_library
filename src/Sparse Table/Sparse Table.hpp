struct sparse_table {
  using B = int;
  inline static B ope(const B& a, const B& b) {
    return std::min(a, b);
  }

  int N;
  vector<vector<B>> t;

  sparse_table(vector<B> A) {
    N = A.size();
    t.resize(bit_width((uint32_t)N) + 1);

    t[0].resize(N);
    for(int i = 0;i < N;i++) {
      t[0][i] = A[i];
    }

    for(int k = 1;(1 << k) <= N;k++) {
      t[k].resize(N);
      for(int i = 0;i + (1 << k) <= N;i++) {
        t[k][i] = ope(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
      }
    }
  }
  // [a, b)
  B query(int a, int b) {
    int k = std::bit_width((std::uint32_t)(b - a)) - 1;
    return ope(t[k][a], t[k][b - (1 << k)]);
  }
};
