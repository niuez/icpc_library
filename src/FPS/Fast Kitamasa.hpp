namespace fps {
  VM fast_kitamasa(VM c, i64 n) {
    int k = c.size() - 1;
    VM ic = inv(c, k - 1);
    
    i64 cl = bp2(k - 1 + c.size() - 1);
    i64 il = bp2(k - 1 + ic.size() - 1);
    auto C = dft(c, cl);
    auto I = dft(ic, il);

    i64 bl = bp2(k);
    VM b(bl, M(0));
    b[k - 1] = M(1);
    i64 m = bp2(n);
    while(m) {
      auto B = dft(b, bl * 2);
      for(int i = 0; i < B.size(); i++) B[i] *= B[i];
      auto beta = idft(B);

      auto gam = beta;
      gam.resize(k - 1, M(0));
      auto G = dft(gam, il);
      for(int i = 0; i < G.size(); i++) G[i] *= I[i];
      auto q = idft(G);
      q.resize(cl, M(0));
      for(int i = k - 1; i < cl; i++) q[i] = M(0);
      auto Q = dft(q, cl);
      for(int i = 0; i < Q.size(); i++) Q[i] *= C[i];
      VM cfq = idft(Q);
      cfq.resize(k - 1 + c.size() - 1);
      for(int i = 0; i < beta.size(); i++) beta[i] -= cfq[i];

      b = VM(bl * 2, M(0));
      for(int i = k - 1; i < 2 * k - 1; i++) {
        b[i - (k - 1)] = beta[i];
      }
      if(m & n) {
        M freq = b[0];
        for(int i = 0; i < k - 1; i++) {
          b[i] = b[i + 1] + freq * c[i + 1];
        }
        b[k - 1] = freq * c[k];
      }
      m >>= 1;
    }
    b.resize(k);
    return b;
  }
}///
