# ぬるぽチェッカー

c++のスマートポインタ(や配列)を置き換え、nullptr(や範囲外)にアクセスしたときに変数名とともに例外を投げます

## usage
* STLの各種クラスをnpchkのクラスで置き換えて使います
	* `std::shared_ptr<T>` → `npchk::shared_ptr<T>`
	* `std::array<T, N>` → `npchk::array<T, N>`
		* メンバーの実装は不完全
	* その他の型も後で作るかも
* エラーメッセージの表示のために変数名を登録する必要があります
	* 変数の定義時に`NPCHK`マクロを使用すると自動で設定できます
		* `NPCHK`マクロはnpchkのクラスがネストしていても(例えば`npchk::array<npchk::shared_ptr<int>,3>`とか)使えます
		* 型のテンプレートに`,`を含む場合は型名全体を`( )`で囲う
		```c++
		// std::shared_ptr<T>, var の代わりに
		NPCHK(npchk::shared_ptr<T>, var);
		```
	* または`var.setName("var");`

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
