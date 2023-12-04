struct trie {
  constexpr static int C = 26;
  struct node {
    array<int, C> ch;

    node() {
      ch.fill(-1);
    }
  };

  vector<node> ns;
  int r;
  int m;

  trie(int mergin) {
    m = mergin;
    r = ns.size();
    ns.push_back( node() );
  }

  void add(const string& s) {
    int i = 0;
    int k = r;
    while(i < s.size()) {
      // k: strict prefix
      int d = s[i] - m;
      if(ns[k].ch[d] == -1) {
        ns[k].ch[d] = ns.size();
        ns.push_back( node() );
      }
      k = ns[k].ch[d];
      i++;
    }
    // k: end
  }
};
