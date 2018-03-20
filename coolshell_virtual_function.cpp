/**
 * https://coolshell.cn/articles/12165.html
 */

#include <bits/stdc++.h>
using namespace std;

typedef void (*Fun)(void);

class Base {
public:
  virtual void f() { cout << "Base::f" << endl; }
  virtual void g() { cout << "Base::g" << endl; }
  virtual void h() { cout << "Base::h" << endl; }
};

int main(int argc, char const *argv[]) {

  Base b;

  Fun pFun = NULL;

  cout << "虚函数表地址：" << (long *)(&b) << endl;
  cout << "虚函数表 — 第一个函数地址：" << (long *)*(long *)(&b) << endl;

  // Invoke the first virtual function
  pFun = (Fun) * ((long *)*(long *)(&b));
  printf("%p\n", pFun);
  pFun();

  pFun = (Fun) * ((long *)*(long *)(&b) + 0); // Base::f()
  pFun();
  pFun = (Fun) * ((long *)*(long *)(&b) + 1); // Base::g()
  pFun();
  pFun = (Fun) * ((long *)*(long *)(&b) + 2); // Base::h()
  pFun();
  return 0;
}
