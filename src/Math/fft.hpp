const double pi = std::acos(-1);///

vector<complex<double>> fast_fourier_transform(vector<complex<double>> a) {
  int n = a.size();
  for(int s = n >> 1; s >= 1; s >>= 1) {
    complex<double> zeta = std::polar(1.0, 2 * pi / (double)(s << 1));
    for(int i = 0;i < n;i += (s << 1)) {
      complex<double> zi = 1.0;
      for(int j = 0;j < s;j++) {
        complex<double> t = a[i + j] - a[s + i + j];
        a[i + j] = a[i + j] + a[s + i + j];
        a[s + i + j] = t * zi;
        zi = zi * zeta;
      }
    }
  }
  return a;
}///

vector<complex<double>> inverse_fast_fourier_transform(vector<complex<double>> a) {
  int n = a.size();
  for(int s = 1; s < n; s <<= 1) {
    complex<double> zeta = std::polar(1.0, -1 * 2 * pi / (double)(s << 1));
    for(int i = 0; i < n; i += (s << 1)) {
      complex<double> zi = 1;
      for(int j = 0;j < s;j++) {
        complex<double> t = a[s + i + j] * zi;
        a[s + i + j] = a[i + j] - t;
        a[i + j] = a[i + j] + t;
        zi = zi * zeta;
      }
    }
  }
  int inv_n = 1 / (double)n;
  for(int i = 0;i < n;i++) a[i] *= inv_n;
  return a;
}///
