/*
 * 文字列S[0, i-1]のprefixとsuffixが最大何文字一致しているか
 */

vector<int> mp(string S) {
  vector<int> A(S.size() + 1);
  A[0] = -1;
  int j = -1;
  for (int i = 0; i < S.size(); i++) {
    while (j >= 0 && S[i] != S[j]) j = A[j];
    j++;
    A[i+1] = j;
  }
  return A;
}

