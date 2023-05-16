/*
## calc dp[i][j] = min[i<=k<j](dp[i][k] + dp[k + 1][j]) + f(i,j). takes O(N^2) and Memory O(N^2)
## f is required of Quandrangle Inequality and Monotone on the Lattice Intervals.
## QI ... f(a or b) + f(a and b) >= f(a) + f(b)
## MLI ...if a is partial of b , f(a) <= f(b)
*/
template<typename F>
int mongeDP(int N, F f) {
  vector<vector<int>> dp(N, vector<int>(N, 1e18));
  vector<vector<int>> k(N, vector<int>(N));
  for(int i = 0; i < N; i++) {
    dp[i][i] = 0;
    k[i][i] = i;
  }
  for(int d = 1; d < N; d++) {
    for(int i = 0; i < N - d; i++) {
       int j = i + d;
       int l = k[i][j - 1];
       int r = k[i + 1][j];
       for(int s = l; s < r + 1; s++) {
         if(s + 1 >= N) break;
         if(dp[i][s] + dp[s + 1][j] <= dp[i][j]) {
           dp[i][j] = dp[i][s] + dp[s + 1][j];
           k[i][j] = s;
         }
       }
       dp[i][j] += f(i, j);
    }
  }
  return dp[0][N - 1];
}
