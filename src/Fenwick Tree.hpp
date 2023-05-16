template<class AbelianMonoid, class Ope, const AbelianMonoid& Ide>
struct fenwick_tree {
  using value_type = AbelianMonoid;

  int n;
  vector<value_type> node;
  Ope ope;

  fenwick_tree(int n_): n(n_), node(n + 1, Ide) {}
  fenwick_tree(const vector<value_type>& init): n(init.size()), node(n + 1, Ide) {
    for(int i = 0;i < init.size(); i++) node[i + 1] = init[i];
    for(int i = 1;i < n;i++) node[i + (i & -i)] = ope(node[i + (i & -i)], node[i]);
  }
  void modify(int i, value_type x) {
    i++;
    while(i <= n) {
      node[i] = ope(node[i], x);
      i += (i & -i);
    }
  }
  // [0, i)
  value_type sum(int i) const {
    value_type ret = Ide;
    while(i > 0) {
      ret = ope(ret, node[i]);
      i -= i & (-i);
    }
    return ret;
  }
};
