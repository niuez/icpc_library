tuple<long long, long long, long long> ext_gcd(long long a, long long b) {
  if(b == 0ll) {
    return tuple<long long, long long, long long>{ a, 1ll, 0ll };
  }
  else {
    auto [g, q, p] = ext_gcd(b, a%b);
    return tuple<long long, long long, long long>{ g, p, q-a/b*p };
  }
  ;
}
tuple<long long, long long> chinese_remainder_theorem(vector<tuple<long long, long long>> const& tms) {
  long long r = 0ll;
  long long M = 1ll;
  for(auto [t, m]: tms) {
    auto [g, p, q] = ext_gcd(M, m);
    if((t-r)%g != 0ll) {
      return tuple<long long, long long>{ 0ll, -1ll };
    }
    ;
    long long tmp = (t-r)/g*p%(m/g);
    r = r+M*tmp;
    M = M*(m/g);
  };
  return tuple<long long, long long>{ (r%M+M)%M, M };
}
