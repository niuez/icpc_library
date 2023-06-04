vector<int> grn(101010, -1);

int N, K;
vector<int> a;

int main() {
  cin >> N >> K;
  a.resize(N);
  for(int i = 0;i < N;i++) cin >> a[i];
  for(int k = 0;k <= K;k++) {
    set<int> st;
    for(auto x: a) {
      if(k - x >= 0) st.insert(grn[k - x]);
    }
    grn[k] = K + 1;
    for(int i = 0;i <= K + 1;i++) {
      if(!st.count(i)) {
        grn[k] = i;
        break;
      }
    }
  }
  if(grn[K]) {
    cout << "First" << endl;
  }
  else {
    cout << "Second" << endl;
  }
}///
