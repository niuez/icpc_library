#define rep(i,n) for (int i=0; i < (n); i++)
typedef double D; 
typedef complex<D> P;
typedef pair<P, P> L;
typedef vector<P> VP;
const D EPS = 1e-9;
#define X real()
#define Y imag()
#define LE(n,m) ((n) < (m) + EPS)
#define GE(n,m) ((n) + EPS > (m))
#define EQ(n,m) (abs((n)-(m)) < EPS)
///

D dot(P a, P b) {
  return (conj(a)*b).X;
}///
D cross(P a, P b) {
  return (conj(a)*b).Y;
}///
int ccw(P a, P b, P c) {
  b -= a;  c -= a;
  if (cross(b,c) >  EPS) return +1;  // counter clockwise
  if (cross(b,c) < -EPS) return -1;  // clockwise
  if (dot(b,c)   < -EPS) return +2;  // c--a--b on line
  if (norm(b) < norm(c)) return -2;  // a--b--c on line or a==b
  return 0;                          // a--c--b on line or a==c or b==c
}///

// lines are closed
bool isecLP(P a1, P a2, P b) {
  return abs(ccw(a1, a2, b)) != 1;
}///
bool isecLL(P a1, P a2, P b1, P b2) {
  return !isecLP(a2-a1, b2-b1, 0) || isecLP(a1, b1, b2);
}///
bool isecLS(P a1, P a2, P b1, P b2) {
  return cross(a2-a1, b1-a1) * cross(a2-a1, b2-a1) < EPS;
}///
bool isecSS(P a1, P a2, P b1, P b2) {
  return ccw(a1, a2, b1)*ccw(a1, a2, b2) <= 0 &&
         ccw(b1, b2, a1)*ccw(b1, b2, a2) <= 0;
}///
bool isecSP(P a1, P a2, P b) {
  return !ccw(a1, a2, b);
}///


// project p to line a
P proj(P a1, P a2, P p) {
  return a1 + dot(a2-a1, p-a1)/norm(a2-a1) * (a2-a1);
}///
P reflection(P a1, P a2, P p) {
  return 2.0*proj(a1, a2, p) - p;
}///
D distLP(P a1, P a2, P p) {
  return abs(proj(a1, a2, p) - p);
}///
D distLL(P a1, P a2, P b1, P b2) {
  return isecLL(a1, a2, b1, b2) ? 0 : distLP(a1, a2, b1);
}///
D distLS(P a1, P a2, P b1, P b2) {
  return isecLS(a1, a2, b1, b2) ? 0 : min(distLP(a1, a2, b1), distLP(a1, a2, b2));
}///
D distSP(P a1, P a2, P p) {
  P r = proj(a1, a2, p);
  if (isecSP(a1, a2, r)) return abs(r-p);
  return min(abs(a1-p), abs(a2-p));
}///
D distSS(P a1, P a2, P b1, P b2) {
  if (isecSS(a1, a2, b1, b2)) return 0;
  return min(min(distSP(a1, a2, b1), distSP(a1, a2, b2)),
             min(distSP(b1, b2, a1), distSP(b1, b2, a2)));
}///

P crosspointLL(P a1, P a2, P b1, P b2) {
  D d1 = cross(b2-b1, b1-a1);
  D d2 = cross(b2-b1, a2-a1);
  if (EQ(d1, 0) && EQ(d2, 0)) return a1;  // same line
  if (EQ(d2, 0)) throw "kouten ga nai";
  return a1 + d1/d2 * (a2-a1);
}///


D distLC(P a1, P a2, P c, D r) {
  return max(distLP(a1, a2, c) - r, 0.0);
}///

D distSC(P a1, P a2, P c, D r) {
  D dSqr1 = norm(c-a1), dSqr2 = norm(c-a2);
  if (dSqr1 < r*r ^ dSqr2 < r*r) return 0; // use or if segment is in circle
  if (dSqr1 < r*r & dSqr2 < r*r) return r - sqrt(max(dSqr1, dSqr2));
  return max(distSP(a1, a2, c) - r, 0.0);
}///

VP crosspointLC(P a1, P a2, P c, D r) {
  VP ps;
  P ft = proj(a1, a2, c);
  if (!GE(r*r, norm(ft-c))) return ps;

  P dir = sqrt(max(r*r - norm(ft-c), 0.0)) / abs(a2-a1) * (a2-a1);
  ps.push_back(ft + dir);
  if (!EQ(r*r, norm(ft-c))) ps.push_back(ft - dir);
  return ps;
}///

D distCC(P a, D ar, P b, D br) {
  D d = abs(a-b);
  return GE(d, abs(ar-br)) ? max(d-ar-br, 0.0) : abs(ar-br) - d;
}///

VP crosspointCC(P a, D ar, P b, D br) {
  VP ps;
  P ab = b-a;
  D d = abs(ab);
  D crL = (norm(ab) + ar*ar - br*br) / (2*d);
  if (EQ(d, 0) || ar < abs(crL)) return ps;

  P abN = ab * P(0, sqrt(ar*ar - crL*crL) / d);
  P cp = a + crL/d * ab;
  ps.push_back(cp + abN);
  if (!EQ(norm(abN), 0)) ps.push_back(cp - abN);
  return ps;
}///

// tangent = sessen
VP tangentPoints(P a, D ar, P p) {
  VP ps;
  D sin = ar / abs(p-a);
  if (!LE(sin, 1)) return ps;  // ここでNaNも弾かれる
  D t = M_PI_2 - asin(min(sin, 1.0));
  ps.push_back(                 a + (p-a)*polar(sin, t));
  if (!EQ(sin, 1)) ps.push_back(a + (p-a)*polar(sin, -t));
  return ps;
}///

// end points of line are tangent
vector<L> tangentTwoCircle(P a, D ar, P b, D br) {
  vector<L> ls;
  D d = abs(b-a);
  rep (i,2) {
    D sin = (ar - (1-i*2)*br) / d;
    if (!LE(sin*sin, 1)) break;
    D cos = sqrt(max(1 - sin*sin, 0.0));
    rep (j,2) {
      P n = (b-a) * P(sin, (1-j*2)*cos) / d;
      ls.push_back(L(a + ar*n, b + (1-i*2)*br*n));
      if (cos < EPS) break;  // 重複する接線を無視（重複していいならこの行不要）
    }
  }
  return ls;
}///

// gaisin
P circumcenter(P a, P b, P c) {
  a = (a-c)*0.5;
  b = (b-c)*0.5;
  return c + crosspointLL(a, a*P(1,1), b, b*P(1,1));
}///

VP circlesPointsRadius(P a, P b, D r) {
  VP cs;
  P abH = (b-a)*0.5;
  D d = abs(abH);
  if (d == 0 || d > r) return cs;  // 必要なら !LE(d,r) として円1つになる側へ丸める
  D dN = sqrt(r*r - d*d);          // 必要なら max(r*r - d*d, 0) とする
  P n = abH * P(0,1) * (dN / d);
  cs.push_back(a + abH + n);
  if (dN > 0) cs.push_back(a + abH - n);
  return cs;
}///

// (l1, l2) is tangent of circle
VP circlesPointsTangent(P a, P b, P l1, P l2) {
  P n = (l2-l1) * P(0,1);
  P m = (b-a) * P(0,0.5);
  D rC = dot((a+b)*0.5-l1, n);
  D qa = norm(n)*norm(m) - dot(n,m)*dot(n,m);
  D qb = -rC * dot(n,m);
  D qc = norm(n)*norm(m) - rC*rC;
  D qd = qb*qb - qa*qc;  // qa*k^2 + 2*qb*k + qc = 0

  VP cs;
  if (qd < -EPS) return cs;
  if (EQ(qa, 0)) {
    if (!EQ(qb, 0)) cs.push_back((a+b)*0.5 - m * (qc/qb/2));
    return cs;
  }
  D t = -qb/qa;
  cs.push_back(              (a+b)*0.5 + m * (t + sqrt(max(qd, 0.0))/qa));
  if (qd > EPS) cs.push_back((a+b)*0.5 + m * (t - sqrt(max(qd, 0.0))/qa));
  return cs;
}///

P minEnclosingCircle(const VP& ps) {
  P c;
  double move = 0.5;
  rep(i,39) {  // 2^(-39-1) \approx 0.9e-12
    rep(t,50) {
      D max = 0;
      int k = 0;
      rep (j, ps.size()) if (max < norm(ps[j]-c)) {
        max = norm(ps[j]-c);
        k = j;
      }
      c += (ps[k]-c) * move;
    }
    move /= 2;
  }
  return c;
}///


namespace std {
  bool operator<(const P a, const P b) {
    return a.X != b.X ? a.X < b.X : a.Y < b.Y;
  }
}///

VP convexHull(VP ps) {
  int n = ps.size(), k = 0;
  sort(ps.begin(), ps.end());
  VP ch(2*n);
  for (int i = 0; i < n; ch[k++] = ps[i++])
    while (k >= 2 && ccw(ch[k-2], ch[k-1], ps[i]) <= 0) --k;
  for (int i = n-2, t = k+1; i >= 0; ch[k++] = ps[i--])
    while (k >= t && ccw(ch[k-2], ch[k-1], ps[i]) <= 0) --k;
  ch.resize(k-1);
  return ch;
}///

bool isCcwConvex(const VP& ps) {
  int n = ps.size();
  rep (i, n) if (ccw(ps[i], ps[(i+1) % n], ps[(i+2) % n]) == -1) return false;
  return true;
}///

int inConvex(P p, const VP& ps) {
  int n = ps.size();
  int dir = ccw(ps[0], ps[1], p);
  rep (i, n) {
    int ccwc = ccw(ps[i], ps[(i + 1) % n], p);
    if (!ccwc) return 2;  // 線分上に存在
    if (ccwc != dir) return 0;
  }
  return 1;
}///

int inCcwConvex(const VP& ps, P p) {
  int n = ps.size();
  P g = (ps[0] + ps[n / 3] + ps[n*2 / 3]) / 3.0;
  if (g == p) return 1;
  P gp = p - g;

  int l = 0, r = n;
  while (l + 1 < r) {
    int mid = (l + r) / 2;
    P gl = ps[l] - g;
    P gm = ps[mid] - g;
    if (cross(gl, gm) > 0) {
      if (cross(gl, gp) >= 0 && cross(gm, gp) <= 0) r = mid;
      else l = mid;
    }
    else {
      if (cross(gl, gp) <= 0 && cross(gm, gp) >= 0) l = mid;
      else r = mid;
    }
  }
  r %= n;
  D cr = cross(ps[l] - p, ps[r] - p);
  return EQ(cr, 0) ? 2 : cr < 0 ? 0 : 1;
}///

int inPolygon(const VP& ps, P p) {
  int n = ps.size();
  bool in = false;
  rep (i, n) {
    P a = ps[i] - p;
    P b = ps[(i + 1) % n] - p;
    if (EQ(cross(a,b), 0) && LE(dot(a,b), 0)) return 2;
    if (a.Y > b.Y) swap(a,b);
    if ((a.Y*b.Y < 0 || (a.Y*b.Y < EPS && b.Y > EPS)) && LE(cross(a, b), 0)) in = !in;
  }
  return in;
}///

VP convexCut(const VP& ps, P a1, P a2) {
  int n = ps.size();
  VP ret;
  rep(i,n) {
    int ccwc = ccw(a1, a2, ps[i]);
    if (ccwc != -1) ret.push_back(ps[i]);
    int ccwn = ccw(a1, a2, ps[(i + 1) % n]);
    if (ccwc * ccwn == -1) ret.push_back(crosspointLL(a1, a2, ps[i], ps[(i + 1) % n]));
  }
  return ret;
}///

pair<int, int> convexDiameter(const VP& ps) {
  int n = ps.size();
  int i = min_element(ps.begin(), ps.end()) - ps.begin();
  int j = max_element(ps.begin(), ps.end()) - ps.begin();
  int maxI, maxJ;
  D maxD = 0;
  rep(_, 2*n) {
    if (maxD < norm(ps[i]-ps[j])) {
      maxD = norm(ps[i]-ps[j]);
      maxI = i;
      maxJ = j;
    }
    if (cross(ps[i]-ps[(i+1) % n], ps[(j+1) % n]-ps[j]) <= 0) j = (j+1) % n;
    else                                                      i = (i+1) % n;
  }
  return make_pair(maxI, maxJ);
}///

D area(const VP& ps) {
  D a = 0;
  rep (i, ps.size()) a += cross(ps[i], ps[(i+1) % ps.size()]);
  return a / 2;
}///

P centroid(const VP& ps) {
  int n = ps.size();
  D aSum = 0;
  P c;
  rep (i, n) {
    D a = cross(ps[i], ps[(i+1) % n]);
    aSum += a;
    c += (ps[i] + ps[(i+1) % n]) * a;
  }
  return 1 / aSum / 3 * c;
}///

VP voronoiCell(P p, const VP& ps, const VP& outer) {
  VP cl = outer;
  rep (i, ps.size()) {
    if (EQ(norm(ps[i]-p), 0)) continue;
    P h = (p+ps[i])*0.5;
    cl = convexCut(cl, h, h + (ps[i]-h)*P(0,1) );
  }
  return cl;
}///

vector<L> mergeSegments(vector<L> segs) {
  int n = segs.size();
  rep (i,n) if (segs[i].second < segs[i].first) swap(segs[i].second, segs[i].first);

  rep (i,n) rep (j,i) {
    L &l1 = segs[i], &l2 = segs[j];
    if (EQ(cross(l1.second-l1.first, l2.second-l2.first), 0)
        && isecLP(l1.first, l1.second, l2.first)
        && ccw   (l1.first, l1.second, l2.second) != 2
        && ccw   (l2.first, l2.second, l1.second) != 2) {
      segs[j] = L(min(l1.first, l2.first), max(l1.second, l2.second));
      segs[i--] = segs[--n];
      break;
    }
  }
  segs.resize(n);
  return segs;
}///

