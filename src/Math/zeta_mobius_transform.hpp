template<class T>
void superset_zeta_transform(int n, vector<T>& a) {
  for (int k = 0; k < n; ++k) {
    for (int s = 0; s < (1 << n); ++s) {
      if (!((s >> k) & 1)) {
        a[s] += a[s ^ (1 << k)];
      }
    }
  }
}///
template<class T>
void subset_zeta_transform(int n, vector<T>& a) {
  for (int k = 0; k < n; ++k) {
    for (int s = 0; s < (1 << n); ++s) {
      if (((s >> k) & 1)) {
        a[s] += a[s ^ (1 << k)];
      }
    }
  }
}///
template<class T>
void superset_mobius_transform(int n, vector<T>& a) {
  for (int k = 0; k < n; ++k) {
    for (int s = 0; s < (1 << n); ++s) {
      if (!((s >> k) & 1)) {
        a[s] -= a[s ^ (1 << k)];
      }
    }
  }
}///

template<class T>
void subset_mobius_transform(int n, vector<T>& a) {
  for (int k = 0; k < n; ++k) {
    for (int s = 0; s < (1 << n); ++s) {
      if (((s >> k) & 1)) {
        a[s] -= a[s ^ (1 << k)];
      }
    }
  }
}///
