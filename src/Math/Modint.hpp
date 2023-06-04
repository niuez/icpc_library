using i64 = long long;
template<i64 MD> struct modint {
  using M = modint<MD>;
  i64 a;
  modint(const i64 x = 0): a((x%MD+MD)%MD){}
  M& s(i64 v) {
    a = v < MD ? v : v - MD;
    return *this;
  }///
  i64 value() const { return a; }
  M inv() const { return this->pow(MD-2); }
  M pow(i64 r) const {
    M ans(1);
    M x = *this;
    while(r) {
      if(r & 1) ans *= x;
      x *= x;
      r >>= 1;
    }
    return ans;
  }///
  M& operator+=(M r) { return s(a + r.a); }
  M& operator-=(M r) { return s(a + MD - r.a); }
  M& operator*=(M r) { return s(a * r.a % MD); }
  M& operator/=(M r) { return *this *= r.inv(); }
  M operator+(M r) const { return M(*this) += r; }
  M operator-(M r) const { return M(*this) -= r; }
  M operator*(M r) const { return M(*this) *= r; }
  M operator/(M r) const { return M(*this) /= r; }
  M operator-() const { return M(0) - M(*this); }
  ///
};
template<const i64 MD> ostream& operator<<(ostream& os, const modint<MD>& m) { return os << m.value(); }
