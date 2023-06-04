struct sparse_table {
  using Band = int;
  inline Band ope(const Band& a, const Band b) { return min(a, b); }

  int N;
  vector<vector<Band>> table;
  ///

  sparse_table(vector<Band> arr) : N(arr.size()) {
    table.resize(__lg(N) + 1);

    table[0].resize(N);
    for(int i = 0;i < N;i++) {
      table[0][i] = arr[i];
    }

    for(int k = 1;(1 << k) <= N;k++) {
      table[k].resize(N);
      for(int i = 0;i + (1 << k) <= N;i++) {
        table[k][i] = ope(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
      }
    }
  }///
  // [s, t)
  Band query(int s, int t) {
    int k = __lg(t - s);
    return ope(table[k][s], table[k][t - (1 << k)]);
  }///
};
