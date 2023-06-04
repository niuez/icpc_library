// doubleのときは, midを変える
template<class T,const T ide,class Compare = greater<T>>
struct li_chao{
  struct Line{
    T a,b;
    bool OK;
    Line(): a(0), b(0), OK(false) {}
    Line(T a,T b) : a(a) , b(b), OK(true) {}
    T get(T x){
      if(OK) {
        return a * x + b;
      }
      else {
        return ide;
      }
    }
  };///
 
  struct Node{
    Line line;
    Node *lhs,*rhs;
    Node() : line(), lhs(nullptr), rhs(nullptr) {}
    Node(Line l) : line(l) , lhs(nullptr) , rhs(nullptr){}
  };
 
  const T MI,MA;
 
  Node * root;
 
  Compare comp;
 
  T comp_get(const T & x,const T & y){
    if(comp(x , y)) return x;
    else return y;
  }
 
  li_chao(T mi , T ma) : MI(mi), MA(ma) , root(nullptr){}
  ///
 
  Node * insert(Node * p,T l,T r,Line & line){
    if(l > r) {
      return p;
    }
    if(!p) return new Node(line);
    if(comp(p->line.get(l) , line.get(l)) && comp(p->line.get(r) ,line.get(r))){
      return p;
    }
    if(!comp(p->line.get(l) , line.get(l)) && !comp(p->line.get(r) ,line.get(r))){
      p->line = line;
      return p;
    }
    T mid = (l + r) / 2;
    if(r - l == 1) {
      mid = l;
    }
    if(comp(line.get(mid) , p->line.get(mid))) swap(p->line , line);
    if(comp(line.get(l) , p->line.get(l))){
      p->lhs = insert(p->lhs , l , mid , line);
    }
    else{
      p->rhs = insert(p->rhs , mid + 1, r , line);
    }
    return p;
  }///

  Node* insert_rec(Node* p, T l, T r, Line line, T a, T b) {
    if(r < a || b < l) {
      return p;
    }
    if(a <= l && r <= b) {
      return insert(p, l, r, line);
    }
    T mid = (l + r) / 2;
    if(r - l == 1) {
      mid = l;
    }
    if(!p) {
      p = new Node();
    }
    p->lhs = insert_rec(p->lhs, l, mid, line, a, b);
    p->rhs = insert_rec(p->rhs, mid + 1, r, line, a, b);
    return p;
  }///
 
  void add_line(T a,T b){
    Line l(a , b);
    root = insert(root,MI,MA,l);
  }

  void add_line_range(T a, T b, T l, T r) {
    Line line(a, b);
    root = insert_rec(root, MI, MA, line, l, r);
  }///
 
  T get(Node * p,T l,T r,T t){
    if(!p) return ide;
    T mid = (l + r) / 2;
    if(r - l == 1) {
      mid = l;
    }
    if(t <= mid) return comp_get(p->line.get(t) , get(p->lhs , l, mid,t));
    else return comp_get(p->line.get(t),get(p->rhs,mid + 1 ,r , t));
  }

  T get_rec(Node* p, T l, T r, T t, T a, T b) {
    if(!p) return ide;
    if(r <= a || b <= l) {
      return ide;
    }
    if(a <= l && r <= b) {
      return get(p, l, r, t);
    }
    T mid = (l + r) / 2;
    return comp_get(get_rec(p, l, mid, t, a, b), get_rec(p, mid + 1, r, t, a, b));
  }///
 
  T get(T x){
    return get(root,MI,MA,x);
  }

  T get_range(T x, T l, T r) {
    return get_rec(root, MI, MA, l, r);
  }
};///
