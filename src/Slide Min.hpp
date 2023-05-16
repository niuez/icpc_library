template<class T>
struct dynamic_slide_min {
  const int k;
  deque<pair<int, T>> que;
  int i;
  /* k := length of slide */
  dynamic_slide_min(const int k): k(k), i(0) {}
  /* return min value in slide k after inserting x */
  T next(T x) {
    while(!que.empty() && que.back().second >= x) que.pop_back();
    que.push_back({i, x});
    int min_i = que.front().second;
    i++;
    if(i == que.front().first + k) {
      que.pop_front();
    }
    return min_i;
  }
};
