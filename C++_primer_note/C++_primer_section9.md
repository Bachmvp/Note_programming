# C++ primer section9

## 顺序容器

这部分的内容你在写程序的时候肯定是处处都能用到的，而且会让你的程序很简洁。本章其实是**第三章内容，讲了string和vector**的拓展，详细地介绍了**标准库顺序容器**的知识。

一个容器就是一些特定类型对象的集合。顺序容器为程序员提供了控制元素存储和访问顺序的能力。这种顺序不依赖于元素的值，而是与元素加入容器时的位置相对应。

### 顺序容器概述

所有顺序容器都提供了**快速顺序访问元素**的能力，但不同容器在两个方面的性能不同：

1. **向容器中添加或删除元素** 
2. **非顺序（随机）访问容器中的元素** 

| 容器         | 特征                                                         |
| ------------ | ------------------------------------------------------------ |
| vector       | **可变大小数组**，支持**快速随机访问**，**在尾部之外的位置插入或删除元素较慢**，**因为在内存中元素使连续存储的，所以快速随机访问很容易**，头的位置再加上你要访问的顺序就好，**添加删除元素就需要移动它之后的所有元素，所以慢**，<font color=red>总而言之就是，除了在尾部添加元素以外，插入元素是很慢的，离尾部越远越慢，但随机访问很快</font> |
| list         | **双向[链表]()**，只支持**双向顺序访问**，**在任意位置插入删除都快**，**内存中不连续**，通过指针指向前后元素<font color=red>总而言之就是，插入元素很快，但是没法随机访问</font> |
| deque        | **双端队列**。支持**快速随机访问**，**在头尾位置插入或删除元素很快**，**deque在内存中的情况介于vector和list之间**，是多个连续的存储块（类似vector），然后在一个映射，<font color=red>这个比较特殊，也是最牛逼的容器，介于上面两个之间，既可以快速访问，头尾插入都很快</font> |
| forward_list | **单向[链表]()**，只支持**单向顺序访问**，**在[链表]()任意位置插入删除都快** |
| array        | **固定大小数组**，支持**快速随机访问**，**不能添加或删除**，<font color=red>就是不可变长数组，鸡肋，不用</font> |
| string       | **与vector类似的容器**，但**专门用于保存字符**，**随机访问快**，**在尾部插入删除快**<font color=red>和vector一样</font> |

你可以看到每一个容器都对性能有所侧重，都是有不同的灵活度，那我们在选择容器的时候应该选哪种呢？

- 除非你有很好的理由选择其他容器，否则一律用vector 

- **程序要求随机访问，用vector或deque** 

- **程序需要很大的额外开销，不要用list或forward_list，因为它们再去连其他内存开销大** 

- 程序只在头尾插入删除元素，用deque 虽然这些原则很有用，但在实际应用的时候，你一定要自己衡量好性能，只要你掌握了表格中各个容器的特点，相信你能做出最合适的选择。

### 容器库概览

这部分介绍对**所有容器**都适用的操作
一般来说，每个容器都定义在一个头文件中，文件名和类型名相同，例如，deque定义在头文件deque中。

**容器均定义为模板类**，对于大多数，我们必须提供额外信息来生成特定的容器类型：

```c++
list<int> a;
vector<double> b;
```

#### 对容器可以保存的元素类型的限制

顺序容器几乎可以保存任意类型的元素，包括它自己。也有一些例外，**比如说，我要保存一个类类型的对象，而这个类没有默认构造函数，于是，我得自己给它提供一个元素初始化器**：

```c++
//假定a是一个没有默认构造函数的类型
vector<a> v1(10, init); //正确：init是一个a对象，相当于提供了初值，v1中有10个init
vector<a> v2(10); //错误：无法初始化
```

#### 迭代器

写一个程序看看吧，输出容器中所有元素：

```c++
vector<int> a(10, 1);
auto beg = a.begin();
while(beg != a.end())
{
    cout << *beg++ << endl;
}
```

**迭代器可加可减**，**但forward_list不支持减，理由显而易见。**

#### 容器类型成员

**每个容器都定义了多个类型**，如前面图片中的第一部分类型别名，我们其实已经用过三种：size_type、iterator和const_iterator。 大多数容器还提供反向迭代器，反向迭代器++就是上一个元素。

**通过类型别名，我们可以在不了解容器中元素类型的情况下使用它**：

```c++
list<string>::iterator iter; //iter是通过list<string>定义的一个迭代器类型
vector<int>::difference_type count;
//count是通过vector<int>定义的一个difference_type类型
```

#### begin和end成员

```c++
list<string> a = {"1", "2", "3"};
auto it1 = a.begin(); //list<string>::iterator
auto it2 = a.rbegin(); //list<string>::reverse_iterator
auto it3 = a.cbegin(); //list<string>::const_iterator
auto it4 = a.crbegin(); //list<string>::const_reverse_iterator
```

当不需要修改时，最好用cbegin

#### 容器定义和初始化

**每个容器类型都定义了一个默认构造函数**。除了array之外，**其他容器的默认构造函数都会创建一个指定类型的空容器，且都可以接受指定容器大小和元素初始值的参数**。也就是可以vector\<int\>()

##### 将一个容器初始化为另一个容器的拷贝

两种方式：

1. **直接拷贝整个容器,这不就是拷贝初始化？** 

2. **拷贝由一个迭代器对指定的元素范围**（array除外） 二者在元素类型的要求严格程度上有所不同，**第一种容器类型和容器内元素类型必须完全匹配，第二种容器类型无所谓**，因为被迭代器隐藏了嘛，容器内元素类型能转换就行。看代码：

   ```c++
   list<string> a = {"1", "2", "3"}; //列表初始化
   vector<const char*> b = {"4", "5", "6"};
   list<string> a1(a); //正确
   deque<string> a2(a); //错误：容器类型不匹配
   vector<string> b1(b); //错误：容器内元素类型不匹配
   forward_list<string> b2(b.begin(), b.end()); //正确
   ```

##### 顺序容器大小相关的构造函数

这个么，也还是好用的：

```c++
vector<int> a1(10, -1); //10个-1
vector<int> a2(10); //10个0
vector<string> a3(10); //10个空string
```

如果元素类型是**内置类型**或者**有默认构造函数的类类型**，**可以在初始化的时候只为构造函数提供一个容器大小参数；如果没有默认构造函数，必须要显式地提供初值**。

这个还是很常用的，要记住

##### 标准库array具有固定大小

与内置数组一样，array的大小也是类型的一部分：

```c++
array<int, 3>; //类型是3个int的数组

array<int, 10> a1; //10个0
array<int, 3> a2 = {1, 2, 3}; //列表初始化
array<int, 3> a3 = {5}; //5, 0, 0
```

**我们不能对内置数组进行拷贝或对象赋值，但array可以（可能这也是为什么搞出array的原因）**：

**终于发现了array的一个有用的地方**

```c++
array<int, 3> a = {1, 2, 3};
array<int, 3> copy = a;
```

#### 赋值和swap

```c++
vector<int> a1 = {1, 2, 3, 4, 5};
vector<int> a2 =a1;
a2 = {1, 2}; //这样都行哦，大小变为2

array<int, 5> b1 = {1, 2, 3, 4, 5};
b1 = {1}; //这样不行
```

关于array，书上是这样说的：**由于右边对象大小可能和左边对象大小不同，所以array索性不支持用花括号赋值和assgin。**

**记住vector就行了**

##### 使用assign（仅顺序容器）

**赋值运算符要求两边运算对象类型相同**，**使用assign可以从一个不同但是相容的类型来赋值**：

```c++
list<string> name;
vector<const char*> old;
names = old; //错误，容器类型不匹配
names.assign(old.cbegin(), old.cend()); //正确，char*能转换为string
names.assign(10, "h"); //assign重载版本，10个h
```

**assign函数真的没用过，功能还是很强大的，强于赋值**

##### 使用swap

```c++
vector<string> s1(10);
vector<string> s2(20);
swap(s1, s2); //交换后，s1有20个元素，s2有10个元素
```

除了array之外（它是真正交换元素了），**交换两个容器的操作很快，是常数时间**，**因为swap只是交换了两个容器的内部数据结构**，并**没有真正移动元素，所以啊，原来那些指针、引用都还是指向原来的。** array就会指向交换后的了，毕竟人家来真的。

**swap后 指向这几个容器的迭代器不会失效，仍然指向swap之前所指向的那些元素值（可以理解为跟随元素的移动而移动），只不过他们所属的容器已经发生了改变**

 **也就是指向元素的指针不会失效**

**与其他容器不同，对一个string调用swap会让指针、引用、迭代器等失效。**

##### 容器大小操作

| 名称     | 含义                                           |
| :------- | :--------------------------------------------- |
| size     | 返回容器中元素个数                             |
| empty    | 只在size为0时返回true                          |
| max_size | 返回一个大于或等于该容器所能容纳的最大元素个数 |

##### 关系运算符

容器装的元素类型支持关系运算，我们才能来用它，先来看内置类型的，它们肯定被实现得很好，毕竟C++高手

```c++
vector<int> v1 = {1, 3, 5, 7, 9, 12};
vector<int> v2 = {1, 3, 9};
vector<int> v3 = {1, 3, 5, 7};
vector<int> v4 = {1, 3, 5, 7, 9, 12};
v1 < v2;
v1 > v3; 
v1 == v4;
```

关系运算中判等用==，大小用<，来个错误示范：

```c++
vector<Sales_data> a, b;
if(a < b) //错误，没定义<运算符
```

**这个不要用，没什么用处**

## 顺序容器操作

前面介绍的那些是所有容器都支持的，我们接下来介绍的只适用于顺序容器（以后还会介绍关联容器）。 

####  向顺序容器添加元素 

 不知道为啥原书篇幅超多，我觉得直接看代码就很明了，所以我就写代码了： 

```c++
list<int> a = {1, 2, 3}; //注释为a的元素内容{1, 2, 3}
a.push_back(4); //{1, 2, 3, 4}，array和forwar_list不支持
a.push_front(0); //{0, 1, 2, 3, 4}，只有list,forward_list和deque支持
```

 **push_front支持的不多，就链表和双向队列这种支持**

以上两种可以方便地在头尾添加元素，更进一步，**insert允许我们在容器任意位置中插入多个元素**，**vector,deque,list,string都支持insert**：**每个insert接受一个迭代器作为第一个参数**： 

**不要下标，要的是迭代器，还是插到这个迭代器的前面**

**能不用就不用吧...,容易搞错**

```c++
list<string> slist = {"Jay"};
auto iter = slist.begin();
slist.insert(iter, "Hello"); //将Hello添加到iter之前的位置

//insert还有重载版本
vector<string> svec;
svec.insert(svec.end(), 10, "May"); //在末尾插10个May
vector<string> a = {"1", "2"};
slist.insert(slist.begin(), a.end()-1, a.end()); //插入了"2"
slist.insert(slist.begin(), slist.begin(), slist.end());
//错误，要拷贝的迭代器不能指向自己
```

 现在我要在vector中实现一个功能，每次插入的单词都插在头部： 

```c++
vector<string> a;
auto it = a.begin();
string word;
while(cin >> word)
{
it = a.insert(it, word);
}
```

 因为insert会返回当前位置，调用insert会在前一个位置插入，相当于调用了push_front

**新标准还引入了三个新成员-emplace_front,emplace和emplace_back**，**这些操作时构造元素，不是拷贝元素**，它们仨分别于push_front,insert和push_back对应： 

```c++
vector<Sales_data> c;
c.emplace_back("1", 25, 16.8);
//在c的末尾构造一个Sales_data对象（调用三个参数的构造函数）

c.emplace_back("1", 25, 16.8); //这样不对的，隐式转换只能对应一个形参，你这参数也太多了
c.emplace_back(Sales_data("1", 25, 16.8)) //这样可以
```

**直接构造，并不是拷贝**

#### 访问元素 

 **每个顺序容器都有一个front函数，返回首元素的引用**；**除forward_list之外的都有一个back，返回尾元素的引用**，还是看代码好理解： 

```c++
vector<string> c = {"1", "2"};
if(!c.empty)
{
auto v1 = *c.begin(), v2 = c.front(); //v1,v2都是首元素的拷贝

auto last = c.end();
auto v3 = *(--last); //除forward_list之外
auto v4 = c.back(); //也除forward_list之外
}
```

**这个还是有点用的，但是返回的是引用，也就是个左值呗**

**c[n]和c.at(n) 返回下标为n的元素的引用**： 

```c++
if(!c.empty)
{
auto &v = c.back(); //获得尾元素的引用
v = “3”； //改变了c中的元素
auto v2 = c.back();
v2 = "0"; //没改c的元素，是个引用
}
```

 与以前一样，**如果用auto变量保存这些函数的返回值，并且要改变容器内的值，必须要将变量定义为引用类型**。 下标一定要在合理范围内，这个程序员自己要注意，来个错误示范： 

```c++
vector<int> a;
cout << a[0]; //错了
```

**at和下标是一样的**

#### 删除元素 

```c++
vector<int> a = {1, 2, 3, 4, 5, 6};
a.pop_front(); //删除首元素
a.pop_back(); //删除尾元素

//删除a中所有奇数
auto it = a.begin();
while(it != a.end())
{
if(*it % 2)
{
it = a.eraser(it); //删除奇数
}
else
{
++it;
}
}

//删除所有元素，两种方式
a.clear();
a.eraser(a.begin(), a.end());
```

删除首尾元素 pop_back pop_front 这个首元素就不要用了

e**rase()不是输入下标吗，这里怎么是迭代器，我看错了，确实是迭代器，而且不要在循环里删除这么用，可以auto it=vec.erase(it);这样会给他一个新指针**

**那也就是说像insert，erase指定位置的删除都是用迭代器才行啊**

#### 特殊的forward_list操作 

 全部在下图了，也好记，用了再找也行：
 把删除奇数元素的代码用forward_list重写一遍，要注意两个迭代器，一个指向要处理的元素，一个指向它前面那个元素： 

```c++
forward_list<int> a = {1, 2, 3, 4, 5, 6};

auto pre = a.before_begin(); //首前元素
auto cur = a.begin(); //首元素
while(cur != a.end())
{
if(*it % 2)
{
cur = a.eraser_after(pre); //删除并移动cur
}
else
{
pre = cur; //下一个
++cur;
}
}
```

不常用

#### 改变容器大小 

 除了array之外，我们可以用**resize**来改变容器大小： 

-  **如果当前大小大于所要求的的大小，容器后面的元素会被删除** 

-  **如果当前大小小于新大小，会将新元素添加到容器后部** 

  ```c++
  list<int> a(10, 42); //10个42
  a.resize(15); //后面再加5个0，是默认初始化的，
  //如果是类类型，要么就有默认构造函数，要么就提供初始值
  a.resize(25, -1); //后面再加10个-1
  a.resize(5); //从末尾删除20个元素，就剩5个42
  ```

   **缩小容器，指向被删除元素的迭代器、引用和指针都会失效**；**对vector、string和deque进行resize也可能导致它们失效。**

**resize可以用，但慎用**

#### 容器操作可能使迭代器失效 

 **使用失效的指针、引用或迭代器是很严重的错误**，我们来仔细分析一下，分添加和删除两种情况
 添加元素： 

-  **容器是vector或string：** 
-  **如果存储空间被重新分配，则指针等全部失效** 
-  未重新分配，插入位置之后的那些都失效，总之就是内存位置变了就不行了 
-  对于deque，插入除了首尾之外的位置就会失效；如果在首尾添加，迭代器失效，引用和指针不会失效 
-  对于list和forward_list，都还是有效的

**删除元素**：
 （被删除的元素对应的那三样肯定挂了） 

-  对于list和forward_list都有效 
-  对于deque， 
-  如果在首尾之外的任何位置删除，都失效； 
-  如果删除尾元素，尾后迭代器失效，其他不影响，如果删除首元素 
-  如果删除首元素，都不受影响 
-  对于vector和string，被删除元素之前的都有效，所以尾后迭代器总会失效

##### 注意函数返回迭代器的位置 

 我们来写一个函数，删除偶数元素，复制每个奇数元素： 

```c++
vector<int> vi = {0, 1, 2, 3, 4, 5};
auto iter = vi.begin();
while(iter != vi.end())
{
if(*iter % 2)
{
iter = vi.insert(iter, *iter);
iter += 2; //跳过当前元素和复制元素
}
else
{
iter = vi.eraser(iter); //删除偶数元素
//不用向前移动迭代器，因为eraser返回删除元素的下一个
}
}
```

**可以看到insert返回的是当前元素的迭代器的前一个，所以继续循环的时候不光加1，还得把你插入的数量加上才行，erase就是返回删除后的下一个，就不用再加了**

##### 不要保存end返回的迭代器 

```c++
vector<int> vi = {0, 1, 2, 3, 4, 5};
auto begin = vi.begin();
auto end = vi.end();
while(begin != end)
{
begin = v.insert(begin, 42);
++begin; //跳过刚刚加入的元素
}
```

 **有问题的，因为end记住了一个以前的end，更好的应该这样：** 

**主要是容器尺寸都变了**

```c++
vector<int> vi = {0, 1, 2, 3, 4, 5};
auto begin = vi.begin();
while(begin != vi.end())
{
begin = v.insert(begin, 42);
++begin; 
}
```

## vector对象是如何增长的

这一部分我们来仔细研究一下**vector在内存中的管理**。
我们已经学过，**vector的元素在内存中是连续存储的**，这样一来，如果我在添加元素的时候，没有空间去容纳新元素应该怎么办呢？vector是这么做的：

1. **把已有元素移动到新空间中** 
2. **然后添加新元素** 
3. **释放旧存储空间** 那么问题又来了，我们的新空间要分配多大呢？**一般来说貌似是旧空间的两倍**，反正就是会预留一些空间

**也就是说vector不是在已有的旧内存空间扩容的，是直接拷贝到一个新的更大的空间了，所以相当于搬家了**

#### 管理容量的成员函数

| 成员函数          | 作用                                                         |
| ----------------- | ------------------------------------------------------------ |
| c.size()          | 目前含有元素的数量                                           |
| c.capacity()      | 所能保存的最大元素数量（不重新分配内存空间），只适用vector和string |
| c.shrink_to_fit() | 将capacity()减小为size()相同大小（适用vector、string、deque） |
| c.reserve(n)      | 分配至少能容纳n个元素的内存空间                              |

reserve()：**只有当当前内存空间不够用的时候，reserve才会重新分配内存空间（可能比n还要大）**，够用的话这个函数什么也不做，这样的话，reserve永远不会减少容器所占用的内存空间。

还有一个resize函数已经用过的，它只改变容器中元素的个数，不会改变容器的容量。

#### capacity和size

直接看代码：

```c++
vector<int> ivec;
ivec.push_back(1);
cout << ivec.size() << ivec.capacity();
```

size肯定是1，capacity依赖于标准库的具体实现，肯定大于等于1

### 额外的string操作

除了顺序容器的共同操作之外，string还有些额外操作，在此介绍。

#### 构造string的其他方法

| string(cp, n)            | s是cp指向的数组中前n个字符的拷贝（此数组至少包含n个字符）    |
| ------------------------ | ------------------------------------------------------------ |
| string s(s2, pos2)       | s是string s2从下标pos2开始的字符的拷贝（若pos2>s2.size()，此函数行为未定义） |
| string s(s2, pos2, len2) | s是string s2从下标pos2开始的len2个字符的拷贝（若pos2>s2.size()，函数行为未定义；若len2太大，只拷贝剩余元素） |

**以上这些构造函数接受一个string或者一个const char*参数**，还可以接受指定拷贝字符的数量，如果传的参数是string，还可以指定一个下标来指出从哪里开始拷贝。

**string这里的参数又成下标了，可能考虑到拷贝的不都是string过来的**

**C++设计的string还是很厉害的，接受了C的char*数组**

接下来就是一些破规矩了：

1. 当我们从一个**const char***创建string时，**指针指向的数组必须以空字符结尾，拷贝操作遇到空字符停止**：

   ```c++
   const char *cp = "Hello World!!!" //以空字符结束的数组
   string s1(cp); //拷贝cp直到遇到空字符；s1 = "Hello World!!!"
   ```

2. **如果我们还传递给构造函数一个计数值，数组就不必以空字符结尾**：

   ```c++
   char a[] = {'H', 'i'}; //不是以空字符结尾
   string s2(a, 2); //虽然a不是以空字符结尾，但因为构造函数给了计数值，所以可以的
   ```

3. 如果我们未传递计数值且数组也未以空字符结尾，或者给定的计数值大于数组大小，则非法，构造函数行为未定义：

   ```c++
   string s3(a); //非法
   string s4(cp, 100); //非法
   ```

#### substr

取子字符串：

```c++
string s("hello world");
string s2 = s.substr(0, 5) //s2 = "hello"
string s3 = s.substr(6) //s2 = "world"
string s4 = s.substr(6, 11) //s2 = "word"，从6开始，最多拷贝到末尾
string s2 = s.substr(12) //抛出一个out_of_range异常
```

**substr接受的参数都是下标**

#### 改变string的其他杂七杂八

```c++
string s1("hello world");
string s2("C++ Primer");
s1.insert(s1.size(), 5, '!'); //s1 = "hello world!!!!!"
s1.eraser(s1.szie()-5, 5); //s1 = "hello world"

s2.append(" 4th Ed."); //s2 = "C++ Primer 4th Ed."
s2.replace(11, 5, "Fifth"); //s2 = "C++ Primer Fifth Ed."
```

改变string可能还有一些要注意的地方，这个要靠你实际用的时候注意了。

#### string搜索操作

一共6个操作，都很好好记，一般来说满足了一般的需求： 找到就返回指定字符的下标，找不到就返回npos（string::npos）
首先说明一下传入参数args的含义，args必须是一下形式之一：

 c, pos | 从s中位置pos开始查找字符c，pos默认为0 ---|--- s2, pos |从s中位置pos开始查找字符串s2，pos默认为0 cp, pos | 从s中位置pos开始查找指针cp指向的以空字符结尾的C风格字符串，pos默认为0 cp, pos, n | 从s中位置pos开始查找指针cp指向的数组前n个字符，pos和n无默认值

上面的参数看着很烦，其实就是从指定位置来构造一个字符串，好在原字符串中查找。好的，现在来说搜索函数

| 函数                      | 含义                                        |
| ------------------------- | ------------------------------------------- |
| s.find(args)              | 查找s中args第一次出现的位置                 |
| s.rfind(args)             | 查找s中args最后一次出现的位置               |
| s.find_first_of(args)     | 查找s中args中任意一个字符第一次出现的位置   |
| s.find_last_of(args)      | 查找s中args中任意一个字符最后一次出现的位置 |
| s.find_first_not_of(args) | 查找s中第一个不在args中的字符               |
| s.find_last_not_of(args)  | 查找s中最后一个不在args中的字符             |

我们来举例子用用看：

```c++
string river = "西调西"; 
auto first = river.find("西"); //返回0
auto last = river.rfind("西"); //返回2
```

#### compare函数

compare有很多重载的版本，好好看下面的代码就知道了：

```c++
string s1("hello");
string s2("hi");

s1.compare(s2); //hello跟hi比，都是按字典序
s1.compare(0, 2, s2); //he和hi比
s1.compare(0, 3, s2, 0, 1); //hel和h比

char *p = "bye";
s1.compare(p); //hello和bye比
s1.compare(0, 2, cp); //he和bye
s1.compare(0, 2, cp, 2); //he和by
```

这个比较复杂，倒不是不用，还得记，太麻烦

#### 数值转换

C++11新标准引入了一些函数，可以实现数值数据与string之间的来回转换：

```c++
int i = 42;
string s = to_string(i); //s = "42"
double d = stod(s); //string->double

string s2 = "pi = 3.14哈哈哈";
//第一个非空白字符必须是数值中可能出现的字符
d = stod(s2.substr(s2.find_first_of("+-.0123456789"))); //d = 3.14
```

其他还有转换为long,unsigned long等不再一一介绍。

这里还是很重要的，像**to_string**,**stoi,stod**

### 容器适配器

除了顺序容器外，标准库还定义了三个**顺序容器适配器**：**stack栈、queue队列、priority_queue优先队列**。

到底适配器啥意思呢？适配器就是一种机制，能让某种事物的行为看起来像另一种事物。一个容器适配器接受一种已有的容器类型，使其行为看起来像另一种不同的类型。

我觉得大概意思是这样，**栈、队列、优先队列这些数据结构都是大家比较常用的，C++也不好意思不支持，但是又偷懒不想去实现它们，于是把原来顺序容器的那些再通过适配器转换成这三个数据结构。**

默认情况下，**stack和queue是基于deque实现的**，**priority_queue是在vector之上实现的**：

**优先级队列竟然是基于vector**

```c++
stack<int> stk(deq); //从deq拷贝元素到stk
```

适配器要求能在头尾添加元素和返回头尾元素，所以不支持这些的顺序容器就没有适配器了，比如array，人家不能改大小不能增加删除啊。

#### 栈适配器

C++自己定义了头文件stack（虽然我觉得可能是引入了顺序容器比如deque），栈是先入后出的：

```c++
stack<int> in;
for(size_t i = 0; x != 10; ++i)
{
    in.push(i); //压入元素，还有pop删除栈顶元素，top返回栈顶等
}
```

再提一句：虽然stack是基于deque实现的，但不能使用push_back，**必须用自己的push**，这样看起来就像两个不同的事物，不就满足了适配器的意思吗

#### 队列适配器

队列先进先出，操作自己用的时候找，无非就是返回头尾，添加删除代替等。

priority_queue允许我们为队列的元素简历优先级，举个例子，我们的优先队列装的元素是一个类person，它有属性姓名，年龄，学历等，我们就可以以其中任意一个属性为优先级来[排序]()，只要重载  <运算符就好，怎么重载<呢？也很简单，不过要之后再学。