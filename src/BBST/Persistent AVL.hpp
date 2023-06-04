class persistent_avl_array {
  using T = char;
  struct node;
  using NT = shared_ptr<const node>;
  using NR = const NT&;
  using TR = const T&;
  using split_NT = pair<NT, NT>;
  static int size(NR n) {
    if(n) return n->sz;
    else return 0;
  }///
  static int height(NR n) {
    if(n) return n->he;
    else return 0;
  }///
  struct node {
    using NT = shared_ptr<const node>;
    using NR = const NT&;
    using TR = const T&;
    T val;
    NT ch[2];
    int sz;
    int he;
    node(TR val, NR left, NR right)
      : val(val) {
        sz = 1 + size(left) + size(right);
        he = 1 + max(height(left), height(right));
        assert(abs(height(left)-height(right))<=1);
        ch[0] = left;
        ch[1] = right;
      }
  };///
  static NT balance(TR rootval, NR a, NR b) {
    if(height(a) - height(b) == 2) {
      if(height(a->ch[0]) - height(a->ch[1]) == -1)
        return make_shared<const node>(
            a->ch[1]->val,
            make_shared<const node> (a->val, a->ch[0], a->ch[1]->ch[0]),
            make_shared<const node> (rootval, a->ch[1]->ch[1], b)
            );
      else
        return make_shared<const node>(
            a->val, 
            a->ch[0],
            make_shared<const node> (rootval, a->ch[1], b)
            );
    }
    else if(height(a) - height(b) == -2) {
      if(height(b->ch[0]) - height(b->ch[1]) == 1)
        return make_shared<const node> (
            b->ch[0]->val,
            make_shared<const node> (rootval, a, b->ch[0]->ch[0]),
            make_shared<const node> (b->val, b->ch[0]->ch[1], b->ch[1])
            );
      else
        return make_shared<const node> (
            b->val,
            make_shared<const node>(rootval, a, b->ch[0]),
            b->ch[1]
            );
    }
    else
      return make_shared<const node>(rootval, a, b);
  }///
  static NR back(NR node) {
    if(node->ch[1]) return back(node->ch[1]);
    else return node;
  }///
  static NT push_back(NR node, TR val) {
    if(!node) return make_shared<const struct node>(val, nullptr, nullptr);
    else if(node->ch[1]) return merge(node->val, node->ch[0], push_back(node->ch[1], val));
    else return merge(node->val, node->ch[0], make_shared<const struct node>(val, nullptr, nullptr));
  }///
  static NT pop_back(NR node) {
    if(node->ch[1]) return merge(node->val, node->ch[0], pop_back(node->ch[1]));
    else return node->ch[0];
  }///
  static NT merge_1(TR rootval, NR dst, NR src) {
    if(height(dst) - height(src) <= 1)
      return make_shared<const node>(rootval, dst, src);
    else
      return balance(dst->val, dst->ch[0], merge_1(rootval, dst->ch[1], src));
  }///
  static NT merge_0(TR rootval, NR dst, NR src) {
    if(height(dst) - height(src) <= 1)
      return make_shared<const node>(rootval, src, dst);
    else
      return balance(dst->val, merge_0(rootval, dst->ch[0], src), dst->ch[1]);
  }///
  static NT merge(TR rootval, NR left, NR right) {
    if(height(left) >= height(right)) 
      return merge_1(rootval, left, right);
    else
      return merge_0(rootval, right, left);
  }///
  static split_NT split(NR node, int i) {
    if(i == 0)
      return split_NT(NT(), node);
    else if(i <= size(node->ch[0])) {
      auto sp = split(node->ch[0], i);
      return split_NT(sp.first, merge(node->val, sp.second, node->ch[1]));
    }
    else {
      auto sp = split(node->ch[1], i - size(node->ch[0]) - 1);
      return split_NT(merge(node->val, node->ch[0], sp.first), sp.second);
    }
  }///
  static NR at(NR node, int i) {
    if(i == size(node->ch[0])) return node;
    else if(i < size(node->ch[0])) return at(node->ch[0], i);
    else return at(node->ch[1], i - 1 - size(node->ch[0]));
  }
  NT root;
  ///
public:
  using split_array_type = pair<persistent_avl_array, persistent_avl_array>;
  persistent_avl_array() {}
  persistent_avl_array(NR r) : root(r) {}
  persistent_avl_array merge(persistent_avl_array other) const {
    if(!root) return other;
    else return persistent_avl_array(merge(back(root)->val, pop_back(root), other.root));
  }
  split_array_type split(int i) const {
    auto p = split(root, i);
    return { persistent_avl_array(p.first), persistent_avl_array(p.second) };
  }
  persistent_avl_array push_back(TR val) const {
    return persistent_avl_array(push_back(root, val));
  }
  int len() const { return size(root); }
  TR at(int i) const { return at(root, i)->val; }
  ///
};
