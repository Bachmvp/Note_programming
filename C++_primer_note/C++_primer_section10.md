# C++ primer section10

## 泛型算法

我们前一章学习了容器，不知道你有没有发现，其实容器是一个模板类，就是说在类的上面还有一层，看下面这句话：

```c++
vector<int> a;
```

这里面包含了模板类vector->类vector<int>->对象a，也就是说vector不独立于任何数据类型，它是在数据类型之上的那一层，模板类。

这个特征跟我们这一章要学的泛型[算法]()有些类似，我们有很多的标准库容器，那我们是不是要给每个标准库容器写[算法]()呢？C++不是这样做的，这样太麻烦了，C++的做法是提供一组[算法]()，独立于任何特定的容器，这些[算法]()是类型无关的，是**泛型（generic）**的：就是说它们可以用于不同类型的容器和不同类型的元素。

在前面学的顺序容器中，我们提供的操作都很基本，增删元素，访问头尾元素等，这一章我们会提供更多有用的操作（**而且它们基本可以用于所有的容**器）

### 概述

大多数[算法]()**都定义在algorithm**中。

[算法]()一般不直接操作容器，而是**遍历由两个迭代器指定的一个元素范围**（这个就是有点泛型的意思了）。我们来举个例子，假定有一个int的vector，我们要判断里面是否包含一个特定值，代码如下：

```c++
int val = 24;
auto res = find(vec.begin(), vec.end(), val);
cout << (res == vec.end() ? "不存在" : "存在") << endl;
```

是不是超级简单？而且这个**find函数可以适用于各种容器，是泛型的**。

为什么会这么6这么神奇呢，我们来仔细看看find[算法]()是怎么做的： find[算法]()**通过迭代器去依次访问元素**，找到就停止，直到尾后元素。

这些都不依赖于容器所保存的元素类型或者是容器类型，**它就是迭代器去访问**。

迭代器令[算法]()不依赖于容器，这个我们知道了，还有一句话是这样的，但[算法]()依赖于元素类型的操作，这句话怎么理解呢？因为[算法]()在执行时肯定要操作元素，比如我们的find[算法]()，它至少要使用==符号来判等，所以就要求元素类型支持 ==，这里的类型是int，当然支持了。

**[算法](https://www.nowcoder.com/jump/super-jump/word?word=算法)本身不会执行容器的操作，它们只会运行于迭代器之上**

### 初识泛型[算法](https://www.nowcoder.com/jump/super-jump/word?word=算法)

**我们学习的泛型[算法](https://www.nowcoder.com/jump/super-jump/word?word=算法)基本都对一个范围内的元素进行操作**，这个范围由两个参数表示，前一个指向要处理的第一个元素，后一个指向尾元素之后的位置。

#### 只读[算法]()

只读取其输入范围内的元素，从不改变元素。例如之前的find函数，再来一个例子：

```c++
int sum = accumulate(vec.cbegin(), vec.cend(), 0);
```

这个函数是对容器内元素求和，第三个参数是求和的初值，这个[算法]()要求容器内的元素类型支持+，再来一个例子，string定义了+，所以可以对string求和：

```c++
int sum = accumulate(vec.cbegin(), vec.cend(), string("")); //泛型，跟上面几乎一样
```

**这个accumulate很好用，在算法题里经常使用**

**操作两个序列的算法**

```c++
equal(vec1.cbegin(), vec1.cend(), vec2.cbegin());
```

vec2元素数量要大于等于vec1，前面每个元素都对应相等蔡返回true

#### 写容器元素的[算法]()

举个例子：

```c++
fill(vec.begin(), vec.end(), 0); //将所有元素重置为0
//但是算法不会去检查写这个操作，例如
vector<int> v;
fill_n(v.beg(), 10, 0); //这个函数企图把v开头的10个元素置为0
//但是会出错，而且不会报编译错误

//再介绍一个函数back_inserter，定义在iterator头文件
vector<int> vv;
auto it = back_inserter(vec); //返回一个插入迭代器，通过向插入迭代器赋值
*it = 24; //就可以成功插入了，vv中有一个元素24
//这里我们调用了它，作用是用插入迭代器来插值
fill_n(back_insertet(v), 10, 0);
```

**fill 和fill_n还算是见过**

#### 拷贝[算法]()

甩代码：

```c++
int a1[] = {0, 1, 2, 3, 4};
int a2[sizeof(a1)/sizeof(*a1)]; //相同元素个数
auto ret = copy(begin(a1), end(a1), a2); //把a1的内容拷贝给a2，返回a2尾元素之后的值
```

还有一个replace[算法]()：

```c++
replace(list.begin(), list.end(), 0, 42); //把list中所有的0改为42
//还有一种重载，保留了原来的list
replace(list.cbegin(), list.cend(), back_inserter(ivec), 0, 42);
//list不变，ivec包含list的一份拷贝，不过里面的0都变成42了
```

#### 重排容器元素的[算法]()

我们要通过一个任务来学习这部分，假定我们要化简一个vector，使得里面保存的单词只出现一次：

```c++
//假设原来的vector为a, b, d, c, a, b
void Unique(vector<string> &words)
{
    sort(word.begin(), words.end()); //a, a, b, b, c, d
    auto end_unique = unique(words.begin(), words.end()); //a, b, c, d, a, b
    //end_unique的位置就是第一个重复的元素位置
    words.eraser(end_unique, wors.end()); //a, b, c, d
}
```

### 定制操作

很多[算法]()都会比较元素，默认情况下，这类[算法]()使用元素类型的  <或==运算符来完成比较，但是有两种情况可能需要我们多做一些工作：

1. 我们希望的[排序]()顺序与   <定义的顺序不同 
2. 我们保存的元素没有定义<运算符 在这两种情况下，我们需要重载sort的默认行为。

**这里很重要的，尤其是sort这种算法，太常用了**

#### 向[算法]()传递参数

我们现在要完成这样一个任务：对一个vector     按照其单词长度   [排序]()，长度相同的再按照字典序排列。  
对于这个任务，我们要定义自己的比较规则（因为默认的规则是字典序），我们要使用一个sort的重载版本，这个版本接受第三个参数，这个参数**是一个谓词**。
谓词：**一个可调用的表达式，返回结果是一个能用作条件的值**，**谓词可以接受一个或两个参数，分别称为一元谓词和二元谓词**。
好了，我们来完成这项任务：

```c++
//之前的那个函数，字典序排列且去重
void Unique(vector<string> &words) 
{
    sort(word.begin(), words.end());
    auto end_unique = unique(words.begin(), words.end());
    words.eraser(end_unique, wors.end());
}

//自定义函数，用长度比较，待会作为第三个参数，谓词
bool isShorter(const string &s1, const string &s2)
{
    return s1.sie() < s2.size();
}

int main()
{
    vector<string> words; //假装有内容
    Unique(words); //先字典序
    stable_sort(words.begin(), words.end(), isShorter); //再按长度
    //用stable_sort稳定排序是为了让长度相同的单词还是保持字典序不变
    for(const auto &s : words)
    {
        cout << s << " ";
    }
    cout << endl;
    return 0;
}
```

**不是类里面用不用加static，默认升序，返回true则a<b**

#### lambda表达式

这东西我一直觉得很难，希望这次能借这个机会再好好学习一下，嫌弃我说不清楚的话建议看原书。。。
首先，我们为什么要有这个表达式呢？还是跟之前的谓词有关，因为谓词最多只能接受两个参数，但我们有时候希望进行的操作需要更多的参数，我们来举个例子，现在我们要修改上面写的程序，求大于等于一个给定长度的单词有多少，打印输出这些单词，我们先来写一下这个函数的框架，看看会有什么问题用目前的知识无法解决的：

```c++
void biggies(vector<string> &words, vector<string>::size_type sz)
//size相当于unsigned int
{
    //开始的步骤和之前一样
    Unique(words);
    stable_sort(words.begin(), words.end(), isShorter);

    //接下来要做的是获取一个迭代器，指向第一个满足size>sz的元素
    //然后就可以从这个元素开始依次打印输出了
}
```

所以，我们现在的问题就是要在一个vector中寻找第一个大于等于给定长度的元素。
这个问题看似简单，我们来分析一下：
标准库中有一个[算法]()find_if用来查找第一个具有特定大小的元素，它接受三个参数，前两个是一对迭代器表示输入范围，第三个参数是一个一元谓词，find_if[算法]()对输入序列中每个元素调用这个谓词，它返回第一个使谓词返回非0值的元素。 看着挺好用的，那么问题在哪呢？**问题就在第三个参数是一元谓词**，我们在编写这个谓词函数（我是这么叫的）的时候，**肯定要传给它两个参数，一个string和一个长度**，这就有问题了，因为人家是一元谓词，无法接受两个参数，所以啊，here comes lambda.

##### 介绍lambda（兰木达）

一个lambda表达式表示一个**可调用的代码单元**，我们可以理解为**未命名的内联函数**，一个lambda表达式具有如下形式：
[**capture list**] (parameter list) -> return type {function body}
我们熟悉的有形参列表，返回类型（必须尾置），函数体
 **这个捕获列表是lambda表达式所在函数中定义的局部变量的列表**，要好好理解这句话，这句话对后面理解lambda的作用很大。

我们可以忽略形参列表和返回类型，但必须包含捕获列表和函数体：

```c++
auto f = [] {return 42;} //定义了一个可调用对象f，不接受参数，返回42
cout << f() << endl; //打印42
```

##### 向lambda传递参数

 **lambda不能有默认参数**，这是规定。
作为一个带参数的lambda例子，我们来写一个与isShorter函数完成相同功能的lambda：

```c++
[] (const string &a, const string &b){ return a.szie() < b.szie();}
```

**空捕获列表表明此lambda不使用它所在的函数中的任何局部变量**，我们可以使用此lambda来调用stable_sort函数：

```c++
stable_sort(
            words.begin(), word.end(), 
            [] (const string &a, const string &b){ return a.szie() < b.szie();}
            );
```

##### 使用捕获列表

不要忘了我们为什么要引出lambda这个概念，我们现在就来解决这个问题：编写一个可以传递给find_if的可调用表达式，我们希望这个表达式能将输入序列中的每个string的长度与biggies函数中的sz参数进行比较。 我们是怎么来传递多余信息呢？答案就是捕获列表，**一个lambda通过将局部变量包含在其捕获列表中来使用：**

```c++
[sz] (const string &a){ return a.szie() >= sz; };
//sz是lambda表达式所在函数的变量，是lambda捕获来的
```

##### 调用find_if

使用这个lambda就可以搞定了：

```c++
auto wc = find_if(wors.begin(), wors.end(), 
                    [sz] (const string &a){ return a.szie() >= sz; });
```

**这个操作真的没看懂，本来以为lambda是用来简约代码的**

**还可以用来捕获包含他的函数的局部变量？解决的问题还是因为一元谓词但是需要两个变量？**

##### 调用find_if

使用这个lambda就可以搞定了：

```c++
auto wc = find_if(wors.begin(), wors.end(), 
                    [sz] (const string &a){ return a.szie() >= sz; });
```

##### 完整的biggies

```c++
void biggies(vector<string> &words, vector<string>::size_type sz)
{
    Unique(words);
    stable_sort(
            words.begin(), word.end(), 
            [] (const string &a, const string &b){ return a.szie() < b.szie();}
            );

    auto wc = find_if(wors.begin(), wors.end(), 
                    [sz] (const string &a){ return a.szie() >= sz; });

    for_each(wc, words.end(), [](const string &s){cout << s << " ";});
    cout << endl;

}
```

#### lambda捕获和返回

当定义一个lambda时，**编译器生成一个与lambda对应的新的（未命名的）类类型**，目前可以这样理解，当向函数传递一个lambda时，同时定义了一个新类型和该类型的一个对象：**传递的参数就是此编译器生成的类类型的未命名对象。**

##### 捕获方式

捕获是一种传参方式，也分为值捕获和引用捕获，下面分别举例：

```c++
//值捕获，前提是变量可以被拷贝
void f1()
{
    size_t v1 = 24;
    auto f = [v1] { return v1; };
    v1 = 0;
    auto j = f(); //j = 24;
}

//引用捕获，必须保证在捕获时该变量是存在的
void f2()
{
    size_t v1 = 24;
    auto f = [&v1] { return v1; };
    v1 = 0;
    auto j = f(); //j = 0;
}
```

##### 神器：隐式捕获

前面我们都是显式地列出我们希望使用的所在函数中定义的局部变量，这样我们要关心很多，我们可以让编译器来帮助我们做这个事情，让它来推断我们要用哪些变量，例如我们可以重写find_if的lambda：

```c++
//sz为隐式捕获，值捕获方式，引用捕获只要把=换成&即可
wc = find_if(words.begin(), words.end(), [=](const string &s){return s.size()>sz;})
```

**其实也没什么卵用，你在函数体里面还是要自己写**，也就是在你要用很多捕获变量的时候省点力气。

我们还可以混用隐式捕获和显式捕获，如果我们希望对一部分变量采用值捕获，对其他变量采用引用捕获：

```c++
//捕获列表的第一个元素必须是=或&，用来指定默认捕获方式为值或引用
[=, &os](const string &s) {os << s << endl;}
```

##### 可变lambda

==接下来就开始各种搞事情了。。。==
我们知道，对于一个值拷贝的变量，lambda不会改变它的值，本来这样规定就合情合理，但C++说我们也可以改变这个值，**在函数参数列表后加上mutable关键字即可**：牛啊

```c++
void fcn2()
{
    size_t v1 = 24;
    //f可以改变它所捕获的变量的值，即便是值捕获
    auto f = [v1]()mutable{return ++v1;}
    v1 = 0;
    auto j = f(); //j=25
}
```

对于引用捕获来说，它可不可以修改就取决于它绑定的那个变量是不是const的。

##### 指定lambda返回类型

到目前为止，我们所编写的lambda都只有一个return语句，所以我们还没指定过返回类型，这里有个很奇特的设定：**默认情况下，如果一个lambda函数体包含return之外的任何语句，则编译器假定此lambda返回void。**我们来举个例子，把容器中所有的负数转正：

**这里很重要啊，必须只有return!!!**

```c++
transform(vi.begin(), vi.end(), vi.begin(), 
        [](int i){ return i < 0 ? -i : i; });
```

我们再来个看起来跟上面差不多的错误版本：

```c++
transform(vi.begin(), vi.end(), vi.begin(), 
        [](int i)
        { 
            if(i<0){return -i;} 
            else{return i;}
        });
```

报错，因为编译器推断该lambda返回类型为void，但它返回了int。 **我们可以通过指定lambda返回类型来修正它（之前我们一直都忽略了返回类型）**：

```c++
transform(vi.begin(), vi.end(), vi.begin(), 
        [](int i) -> int //通过尾置指定返回类型
        { 
            if(i<0){return -i;} 
            else{return i;}
        });
```

#### 标准库bind函数来替换lambda

- **只在一两个地方使用的简单操作，用lambda最好** 

- 很多地方都要用，最好定义一个函数 之前我们怎么引出lambda还记得吗？find_if函数只接受一元谓词作为第三个参数，而我们要给它传一个sz和一个string，所以没办法，于是我们用lambda，因为它可以捕获sz，所以只需要传一个string就好。

  接下来我们来介绍其他解决这个问题的办法：

  ```c++
  using std::placeholder::_1 
  bool check_size(const string &s, string::size_type sz)
  {
    return s.size() >= sz;
  }
  auto wc = find_if(words.begin(), words.end(),
  bind(check_size, _1, sz)) //_1为占用符
  ```

  **调用func(string)=check_size(string,sz),牛啊**

  这里比较费解的就是**bind函数**，我们可以把bind函数看成是一个通用的函数适配器，**它接受一个可调用对象**，生成一个新的可调用对象来适应参数列表，用人话说就是，**我这个bind调用了check_size这个函数**（可调用对象），**它还是接受那个string的参数**，**但是它把第二个参数绑定到sz上了**，这样不就和lambda差不多意思了嘛。

**这个bind很熟悉啊，在ROS里面的订阅和发布的回调函数不就是这个玩意吗！！！**

##### bind的参数

前面的代码说明，我们可以用bind函数来修正参数的值，更一般的，我们还可以用bind绑定给定调用对象中的参数或重新安排顺序：

```c++
//假设g是一个有两个参数的可调用对象
auto g = bind(f, a, b, _2, c, _1);
//_1_2才是可调用对象g的参数，其他三个是f的参数，已经绑定好了
//g(_1, _2)映射为f(a, b, _2, c, _1)
```

**调用g(X, Y)会调用f(a, b, Y, c, X),这也太牛逼了....**

##### 用bind重排参数顺序

```c++
sort(wors.begin(), word.end(), isShorter); //短到长
sort(wors.begin(), word.end(), bind(isShorter, _2, _1); //长到短
```

默认情况下，**bind函数的占位符的参数是会被拷贝到bind返回的可调用对象中**，当然这就有问题了，**比如有些参数是无法被拷贝的**，下面来举个例子（还是用bind函数来替换lambda表达式）：

**bind是要拷贝参数的！**

```c++
//先来写lambda表达式
for_each(wors.begin(), wors.end(), [&os, c](const string &s){os << s << c;});
//os是一个局部变量，引用一个输出流，c是局部变量char
```

我们可以编写一个函数来完成同样的工作：

```c++
ostream &print(ostream &os, const string &s, char c)
{
    return os << s << c;
}
```

但是，我们不能直接用bind替换，**因为os不能被拷贝**：

```c++
//错误示范
for_each(wors.begin(), wors.end(), bind(print, os, _1, ' '));
```

我们还是有解决办法的，**如果我们希望传递给bind一个对象但是又不拷贝它，就必须使用标准库ref函数,牛逼**：

```c++
for_each(wors.begin(), wors.end(), bind(print, ref(os), _1, ' '));
```

**ref和bind函数都定义在头文件functional中。**

## 再谈迭代器

除了前面介绍的标准库容器迭代器外，C++还在**头文件iterator中定义了额外的集中迭代器**：

- **插入迭代器：绑定到容器上，可用来向容器中插入元素（前面提到过的）** 
- **流迭代器：绑定到输入输出流上，可用来遍历所关联的IO流** 
- **反向迭代器：跟普通的迭代器方向相反（forward_list没有这个，你懂得）** 
- **移动迭代器：移动而不是拷贝元素（以后再详细介绍）**

感觉基本用不太上啊

#### 插入迭代器

```c++
//有三种，back_inserter, front_inserter, inserter
vector<int> vec;
auto it = back_inserter(vec);
*it = 0; //{0}
auto is = front_inserter(vec);
*is = -1; {-1, 0}头插
list<int> lst = {1, 2, 3};
list<int> lst1;
copy(list.cbegin(), lst.cend(), inserter(lst1, lst1.begin()) ); //lst1拷贝了lst
```

#### iostream迭代器

 **虽然iostream不是容器，但标准库还是定义了这些类型对象的迭代器。istream_iterator读取输入流，ostream_iterator向一个输出流写数据。这些迭代器将它们对应的流当作一个特定类型的元素序列来处理，通过使用流迭代器，我们可以使用泛型[算法]()从流对象读取数据以及向其写入数据** 
下面是一个用istream_iterator从标准输入读取数据，存入一个vector的例子：

```c++
istream_iterator<int> in_iter(cin); //从cin读取int
istream_iterator<int> eof; //空的istream_iterator当作istream尾后迭代器
while(in_iter != eof)
{
    vec.push_back(*in_iter++);
}
```

这样写看起来和以前差别不大，我们来个牛逼的写法：

```c++
istream_iterator<int> in_iter(cin), eof;
vector<int> vec(in_iter, eof);
```

**这两行代码和上面的程序完全等价：我们用一对元素范围的迭代器来构造vec，这个构造函数通过in_iter从cin中读取数据，直到遇到文件尾或者遇到不是int的数据为止，从流中读取的数据用于构造vec**

再来个酷炫的，输入数据求和：

```c++
istream_iterator<int> in(cin), eof;
cout << accumulate(in, eof, 0) << endl; //从标准输入中读取数值求和输出
```

标准库不保证迭代器立即从流中读取数据，只保证在第一次解引用迭代器之前读数据已经完成。

##### ostream_iterator操作

| 输出流迭代器操作                | 含义                                                         |
| ------------------------------- | ------------------------------------------------------------ |
| ostream_iterator<T> out(os);    | out将类型为T的值写到输出流os中                               |
| ostream_iterator<T> out(os, d); | out将类型为T的值写到输出流os中，每个值后面都输出一个d。d指向一个空字符结尾的字符数组 |

我们可以用ostream_iterator来输出值的序列：

```c++
ostream_iterator<int> out_iter(cout, " ");
for(auto e : vec)
{
    *out_iter++ = e; //实际将元素e写到cout
}
cout << endl;
```

其实我们还可以这样写（最好还是用上面的，保持一致嘛）：

```c++
for(auto e : vec)
{
    out_iter = e; //*和++运算符实际上不对ostream_iterator对象做任何事，可以忽略
}
```

##### 使用流迭代器处理类类型

我们来用IO迭代器重写一下让我们赚了2000块的书店程序：

```c++
istream_iterator<Sales_item> item_iter(cin), eof;
ostream_iterator<Sales_item> out_iter(cout, "\n");
//读取第一笔交易，并准备读取下一条
Sales_item sum = *item_iter++;
while(item_oter != eof)
{
    if(item_iter->isbn() == sum.isbn())
    {
        sum += *item_iter++;
    }
    else
    {
        out_iter = sum; //输出
        sum = *item_iter++; //读取下一条
    }
}
out_iter = sum; //打印最后一组
```

感觉用的确实不多

逆序打印vec中的元素：

```c++
vector<int> vec = {1, 2, 3};
for(auto r_iter = vec.crbegin(); r_iter != vec.crend(); ++r_iter)
{
    cout << *r_iter << endl;
}
```

反向迭代器使用的时候要注意两点，**一个是元素范围**，**尤其是边界到底是开区间还是闭区间**；**还有要注意的就是++和--时的前进方向**

### 特定容器算法-链表list和forward_list

与其他容器不同，[链表]()定义了独有的一些函数：**sort, merge, remove, reverse和unique**。因为通用版本的**sort要求随机访问迭代器**，**因此不能用于[链表**]()，[链表]()要用**双向迭代器或前向迭代器**：

| 函数                                          | 含义，返回均为void                                           |
| --------------------------------------------- | ------------------------------------------------------------ |
| lst.merge(lst2)(重载版本lst.merge(lst, comp)) | 将lst2的元素并入lst中，二者原先必须是有序的，第一个使用<第二个自定义comp |
| lst.remove(val(重载版：lst.remove_if(pred))   | 删除与val相等或使一元谓词pred为真的所有元素                  |
| **lst.reverse()**                             | **翻转lst的元素顺序**                                        |
| **lst.sort()**(重载版lst.sort(comp))          | 使用     <或自定义比较操作[       排序元素      ]()          |
| lst.unique()(重载版lst.unique(pred))          | 删除同一个值的连续拷贝。第一个版本使用==，第二个使用给定的二元谓词 |

用的也不多

#### splice函数

[链表]()类型独有，主要用来移动一个[链表]()的元素到另一个[链表]()的指定位置：

| 函数                      | 含义                                                         |
| ------------------------- | ------------------------------------------------------------ |
| lst.splice(p, lst2)       | p是一个指向lst中元素的迭代器，函数将lst2的所有元素移动到p之前的位置，并且将元素从lst2中删除，lst2类型必须与lst一样，且不能是同一个[链表]() |
| lst.splice(p, lst2, p2)   | p2是一个指向lst2中元素的迭代器，函数将p2指向的元素移动到lst中，lst2与lst可以是同一个[链表]() |
| lst.splice(p, lst2, b, e) | b和e必须表示lst2中的合法范围，将这个范围的元素移动到lst中，lst和lst2可以是同一个[链表]()，但p不能指向b和e之间的元素 |

还有一个与splice类似的函数splice_after，差别在于移动的位置，用的时候再查询好了，说那么多我自己也记不住。

 **[链表]()的这些特有操作会改变容器**