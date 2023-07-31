# ぬるぽチェッカー

c++のスマートポインタを置き換え、nullptrにアクセスしたときに変数名とともに例外を投げます

## usage
* `std::shared_ptr<T>` → `npchk::shared_ptr<T>`
* (その他の型も後で作るかも)
* 変数の定義時に`NPCHK`マクロを使用してください
```c++
std::shared_ptr<T> var;
```
↓
```c++
NPCHK(npchk::shared_ptr<T>, var1);
```

## example
```c++
#include "npchk.hpp"
#include <memory>
#include <iostream>

struct Hoge {
    int val;
    Hoge(int val) : val(val) {}
};

NPCHK(npchk::shared_ptr<Hoge>, hoge1, hoge2);

int main() {
    hoge1 = std::make_shared<Hoge>(3);
    std::cout << hoge1->val << std::endl;
    std::cout << hoge2->val << std::endl;
}
```

```
3
terminate called after throwing an instance of 'std::runtime_error'
  what():  hoge2 is nullptr
中止 (コアダンプ)
```
