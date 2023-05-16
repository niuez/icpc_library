struct osa_k {
  using T = int;
  vector<T> min_fact;

  // O(NlogN)
  static vector<T> min_prime_factor(int n) {
    vector<T> res(n);
    iota(begin(res), end(res), 0);
    for(int i = 2; i * i < n; i++) {
      if(res[i] < i) continue;
      for(int j = i * i; j < n; j += i) {
        if(res[j] == j) res[j] = i;
      }
    }
    return res;
  }

  void build(int n) {
    min_fact = min_prime_factor(n);
  }

  // O(logN)
  vector<pair<T, int>> prime_factors(int n) const {
    vector<pair<T, int>> res;
    while(n > 1) {
      if(res.empty() || res.back().first != min_fact[n]) {
        res.push_back({ min_fact[n], 0 });
      }
      res.back().second++;
      n /= min_fact[n];
    }
    return res;
  }
  
  // The divisors are not sorted
  // O(logN + |divisors|)
  template<class F>
  void enumerate_divisors(int n, F f) const {
    vector<pair<T, int>> prime_facts = prime_factors(n);
    if(prime_facts.empty()) {
      f(1);
      return;
    }
    vector<int> cnt(prime_facts.size());
    vector<int> acc(prime_facts.size(), 1);
    while(true){
      f(acc.front());
      int i = 0;
      for(; i < prime_facts.size(); i++) {
        if((cnt[i]++) == prime_facts[i].second) {
          cnt[i] = 0;
        }
        else {
          acc[i] *= prime_facts[i].first;
          break;
        }
      }
      if(i == prime_facts.size()) {
        break;
      }
      while(i --> 0) {
        acc[i] = acc[i + 1];
      }
    }
  }
};
