# C++ primer section12

## 动态内存

我们这一章来关注一个我们从来没怎么注意过的东西-**内存**，我们来看一段代码：

```c++
int main()
{
    int a = 1;
    return 0;
}
```

这段代码几乎什么都没干，就是定义了一个int类型的对象，我们主要是看这个a在内存中的变化，你注意到这个事情就行。 其实我们有很好的总结：

- **全局对象在程序启动时分配内存，在程序结束时销毁** 
- 对于**局部自动对象**，当我们**进入其定义所在的程序块时被创建，在离开块时销毁** 
- **局部static对象在第一次使用前分配，在程序结束时销毁** 这些呢，都是**C++ 编译器帮我们管理的**，对于这么爱赋予程序员自由的语言，C++也允许我们自己来决定，就是**动态分配对象**：**动态分配的对象的生存期与它们在哪里创建无关，只有显式地被释放时，这些对象才会销毁。**

你可能觉得这个还比较简单，其实这个非常难，尤其在程序比较复杂的时候，所以啊，C++又给我们提供了一些帮助：标准库定义了两个**智能指针类型**来帮助我们管理动态分配的对象，怎么帮助呢？**当一个对象应该被释放时，指向它的智能指针可以确保自动地释放它。**

通过不同方式定义的变量类型在内存中的存储位置也不同，分以下三种情况：

1. **静态内存**：**保存局部static对象**、**类static数据成员**、**定义在任何函数之外的变量** 
2. **栈内存**：**保存定义在函数内的非static对象** 
3. **堆**：**存储动态分配的对象，当动态对象不再使用时，我们必须显式地销毁它们。**

### 动态内存与智能指针

在C++中，动态内存的管理是通过一对运算符来完成：

- **new**，在动态内存中为对象分配空间，并且返回一个指向该对象的指针，可以选择对对象进行初始化 
- **delete**，接受一个动态对象的指针，销毁该对象，并释放与之关联的内存 

这样我们在使用的时候很容易出问题：

- **忘记释放内存，会产生内存泄漏** 
- **在尚有指针引用内存的情况下，释放了它，会产生引用非法内存的指针（非法指针）** 

好了，说了这么多，终于引出了**智能指针**： **智能指针的行为类似常规指针**，**重要的区别是它负责自动释放所指向的对象**，它管理底层指针的方式不同：

1. **shared_ptr允许多个指针指向同一个对象** 
2. **unique_ptr独占所指向的对象** 
3. **weak_ptr，弱引用，指向shared_ptr所管理的对象** 

以上三种类型都定义在**memory头文件**中。

#### shared_ptr类

shared_ptr 允许**多个指针**指向 同一个对象，它基于模板实现，默认初始化的智能指针中保存着一个空指针。

当进行拷贝或者赋值操作时，没个shared_ptr都有一个关联的**计数器**，通常称其为**引用计数**。

我们每次拷贝一个shared_ptr都会使这个引用计数递增（**当局部的shared_ptr离开作用域时，计数器就会递减**），**当引用计数为0（指向对象的最后一个shared_ptr被销毁）时**，**shared_ptr会销毁这个对象（通过shared_ptr的析构函数）**

类似vector，智能指针也是模板：

```c++
shared_ptr<string> p1; //shared_ptr, 指向string
shared_ptr<list<int>> p2; //shared_ptr, 指向list
```

**默认初始化的智能指针中保存着一个空指针**。 **智能指针的使用方式与普通指针类似，解引用一个智能指针返回它指向的对象：**

```c++
if(p1 && p1->empty()) //如果指针本身不为空（有所指对象），且指针所指对象为空
{
    *p1 = "hi";
}
```

##### make_shared函数

**最安全的分配和使用动态内存的方法是调用一个名为make_shared的标准库函数**：**此函数在动态内存中分配一个对象并初始化它**，并且，**返回指向此对象的shared_ptr**：

```c++
shared_ptr<int> p0 = make_shared<int>(); //指向值初始化的int0的share_ptr
shared_ptr<int> p1 = make_shared<int>(42); //指向int42的share_ptr
shared_ptr<string> p2 = make_shared<string>(5,'9'); //指向"99999"的share_ptr
```

智能指针也是模板，所以我们创建它的方法和所有其它模板一样：

```c++
shared_ptr<string> p1;  //指向string的智能指针
shared_ptr<list<int>> p2;   //指向int的list的智能指针12
```

##### 最安全的分配内存的方法：make_shared函数

make_shared函数是标准库函数，也在头文件memory中

```c++
//指向一个值为42的int的shared_ptr
shared_ptr<int> p3 = make_shared<int>(42); 
//指向一个值为"9999999999"
shared_ptr<string> p4 = make_shared<string>(10, '9');
//指向一个初始化值的int
shared_ptr<int> p5 = make_shared_ptr<int>();123456
```

##### 当然，我们也可以将shared_ptr 和new来结合使用

```c++
shared_ptr<int> p1(new int(1024)); //使用直接初始化方式1
```

**注意，使用new的话只能使用直接初始化：**

```c++
shared_ptr<int> p1 = new int(1024); //错误，只能使用直接初始化
```

**使用new可以直接初始化智能指针，但不能赋值初始化给智能指针**

##### shared_ptr的拷贝和赋值

当进行拷贝或赋值操作时，每个shared_ptr都会记录有多少个其他shared_ptr指向相同的对象：

```c++
auto p = make_shared<int>(42); //p指向的对象只有p一个引用者
auto q(p); //p和q指向相同对象，此对象有两个引用者
```

我们可以认为每个shared_ptr都有一个关联的计数器-引用计数： **无论何时我们拷贝一个shared_ptr，都会递增计数器**，举些**不那么明显的拷贝例子**

1. **用一个shared_ptr初始化另一个shared_ptr** 
2. **将它作为参数传递给一个函数** 
3. **作为函数的返回值** 

递减计数器：

1. **给shared_ptr赋予一个新值** 
2. **shared_ptr被销毁（例如一个局部的shared_ptr离开其作用域）** 

一旦一个shared_ptr的计数器变为0，它就会自动释放自己所管理的对象：

```c++
auto r = make_shared<int>(42); //r指向的int只有一个引用者
r = q; //1：给r赋新值，让它指向新地址；2：递增q指向对象的引用计数；
//3：递减r原来指向的对象的引用计数；4：r原来指向的对象已没有引用者，会自动释放
```

##### shared_ptr自动销毁所管理的对象

如何销毁呢？智能指针通过它另一个特殊的成员函数-**析构函数**。 类似构造函数，每个类都有一个析构函数，用于控制此类型的对象销毁时做什么。

shared_ptr的析构函数会递减它所指的对象的引用计数，如果计数为0，**shared_ptr的析构函数就会销毁对象，并释放它占用的内存。**

##### 看看智能指针有多智能

接下来，我会写一段代码，并分析内存情况，只分析这一次让大家知道内存的具体情况，其实这些工作以后都不用管，有内存指针来负责：

```c++
//返回类型为智能指针的函数，所以我们会确保它分配的对象在恰当的时刻释放
shared_ptr factory(T arg)
{
    return make_shared(arg);
}
void use_factory(T arg)
{
    shared_ptr p = factory(arg); //使用p
}//p离开了作用域，它指向的内存被自动释放掉（在这释放是最好的了）
```

程序使用动态内存出于以下三种原因之一：

1. **程序不知道自己要使用多少对象（比如容器类）**
2. **程序不知道所需对象的准确类型**（15章再学）
3. **程序需要在多个对象间共享数据**（接下来介绍）

到目前为止，我们使用的每个对象都拥有自己的元素：

```c++
vector v1; //空的
{ //强行作用域
    vector v2 = {"a"};
    v1 = v2; //拷贝一份给v1
}
//到了这儿，v2已经被销毁了，但是v1还是有元素的，因为是拷贝过来的，独一份，没影响
```

我们接下来要做一个事情，来改善上面的情况。什么事情呢？**我们希望在拷贝的时候，不去拷贝元素，而是引用内存中同一块地方**，v2出了作用域之后，v2本身不能用，但v2对应的元素还是保留，我们把将要实现的模板类称为Blob，来代替vector，具体实现后的效果应该是这样的：

```c++
Blob b1; //空的
{ //强行作用域
    vector b2 = {"a"};
    b1 = b2; //b1和b2共享相同的元素
}
//b2被销毁了，但b2中的元素还在
//b1还是指向最初由b2创建的元素。
```

##### 定义StrBlob类

我们最终会将**Blob类实现为一个模板**，但是我们要到第十六章才会学习模板，所以，在这里，我们就简化一下，先定义一个管理string的类StrBlob。

实现一个新的集合类型最简单的方法就是借助标准库容器，这会省很多事，在这里，我们借助vector来保存元素。

我们当然不能直接用vector，这样还是达不到我们的目的（别忘了我们为什么要提出这个），**我们要把vector保存在动态内存中，这样它的生存期就完全由我们（借助智能指针）来掌控**。

策略：为了实现我们希望的数据共享，**我们为每个StrBlob设置一个shared_ptr来管理动态分配的vector**，**这个shared_ptr会记录有多少个StrBlob共享相同的vector，并在vector的最后一个使用者被销毁时释放vector**（这样是不是很棒啊）

为了简化问题的复杂度，我们的StrBlob只会实现vector的一小部分操作，下面代码是我们正儿八经开始自定义一个类了，要仔细看啊：

```c++
class StrBlob
{
public:
    //类型别名
    typedef vector::size_type size_type;
    //构造函数
    StrBlob(); //别以为这样就好了啊，这就是个函数声明啊
    StrBlob(initializer_list il); //就是列表初始化的意思{"a", "b"}
    //其他成员函数
    size_type size() const {return data->size();}
    //别忘了这里的const是为了让const对象也可以访问
    bool empty() const {return data->empty();}
    //添加和删除元素
    void push_back(const string &t){data->push_back(t);}
    void pop_back();
    //元素访问
    string& front();
    string& back();
private:
    shared_ptr<vector<string>> data; //data是一个指向vector的智能指针
    //如果data[i]不合法，抛出异常
    void check(size_type i, const string &msg) const;
};
```

下面依次来实现类中没实现的函数：

##### StrBlob构造函数

两个构造函数都使用初始化列表来初始化data成员，让它指向一个动态分配的vector，默认构造函数分配的是空vector：

```c++
StrBlob::StrBlob() : data(make_shared<vector<string>>()) {}
//这里的：是初始化列表那个标志，别看不出来啊
StrBlob::StrBlob(initializer_list il) : data(make_shared<vector<string>>(il)){}
```

##### 元素访问成员函数

先检查给定索引是否合法，实现check函数：

```c++
void StrBlob::check(size_type i, const string &msg) const
{
    if(i >= data->size()) {throw out_of_range(msg);}
}
```

pop_back和元素访问成员函数**首先调用check**，如果check成功再继续利用**底层vector**的操作来完成工作：

```c++
string& StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}
string& StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}
void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}
```

**front和back还应该对const重载**（这儿我不想写了）

##### StrBlob的拷贝、赋值和销毁

到这里，其实我们已经实现了最初的目标，我们来分析看看： **我们的StrBlob类只有一个数据成员，是智能指针类型的**，**因此，当我们拷贝、赋值或销毁一个StrBlob对象时，它的shared_ptr成员会被拷贝、赋值或销毁**，这不就已经实现了吗？

### 直接管理内存

我们已经学习了如何用智能指针来管理动态内存，相对来说还是比较简单的，自己可以忽略很多细节。现在我们来一种完全自力更生的，**只借助C++提供的两个运算符来直接管理内存：**

- **new 分配内存** 
- **delete 释放new分配的内存** 这一章越到后面，你越会觉得自己管理真是又烦又容易出错。。。

#### 使用new动态分配和初始化对象

```c++
int a; //在栈内保存，内存变量名为a，值初始化为0
int *pi = new int; //在堆保存，而且用new分配的内存是无名的，所以只能用指针指向它,
//而且它是未初始化的，直接输出的应该是一个奇怪的值
string *ps = new string; //类类型对象会调用默认构造函数初始化，ps指向空string
```

当然我们可以显式初始化它们：

```c++
int *pi = new int(4); //注意和int pi = 4的区别哦
string *ps = new string(10, '9');
vector *pv = new vector{0, 1, 2, 3};
```

也可以有**值初始化**：

1. 对于定义了自己的构造函数的类类型，值初始化和默认初始化一样，都会调用默认构造函数：`string *ps1 = new string; //默认初始化为空string string *ps2 = new string(); //值初始化为空string` 
2. 内置类型就比较适合值初始化了，**因为它没有默认构造函数**，而值初始化有良好的定义：`int *pi1 = new int; //默认初始化：*p1未定义 int *pi2 = new int(); //值初始化为0，pi2指向的内存值为0` 
3. **依赖编译器合成默认构造函数的内置类型成员**：  
   1. 类内没有初始值的话，情况和2一样 
   2. **类内有初始值，就用这个初始值** 

神器auto在这种初始化方式下有所限制：

```c++
auto p1 = new class(T); //p指向一个与T类型相同的对象，该对象用T进行初始化
auto p2 = new class{a, b, c}; //这样不行，括号中只能有单个初始化器，
//因为你这样让auto无法判断p2是什么类型
```

##### 动态分配的const对象

```c++
const int *pci = new const int(24);
const string *pcs = new const string; //和其他const对象一样，一个动态分配的const必须被初始化
```

##### 内存耗尽

虽然现在不太可能，你可以用这个代码试试：

```c++
while(true)
{
    int *p1 = new int(24); //我没试过。。。
}
```

在C++中，如果new不能分配所要求的内存空间，它会抛出一个类型为bad_alloc的异常，不过，我们可以阻止它抛出异常：

```c++
int *p1 = new int; // 如果失败就会抛出异常
int *p2 = new (nothrow) int; //分配失败只会返回一个空指针
```

##### 释放动态内存

为了防止内存耗尽，我们在动态内存使用好后，通过**delete**来将动态内存归还给系统。 **delete接受一个指针，指向我们要释放的对象：** 

```c++
int *p1 = new int(24); 
delete p1; //销毁p1指向的对象，释放对应内存
```

##### 指针值和delete

我们传递给delete的指针必须指向动态分配的内存（或者是空指针）。释放一块不是new分配的内存，或者把相同的指针释放多次，其行为都是未定义的：

```c++
int i, *pi1 = &i, *pi2 = nullptr;
double *pd1 = new double(33), *pd2 = pd1;
delete i; //错误
delete pi1; //未定义的行为
delete pd1; //正确
delete pd2; //未定义，因为pd2指向的内存已经被释放了
delete pi2; //正确
```

##### 动态对象的生存期直到被释放为止

返回指向动态内存的指针（不是智能指针）的函数给调用者增加了一个任务-**调用者必须自己释放内存：**

```c++
//函数factory返回一个指针，指向动态分配的对象
Foo* factory(T arg)
{
    return new Foo(arg);
}
void use_factory(T arg)
{
    Foo *p = factory(arg); //使用p
}//p离开了作用域，但是它指向的内存没有被释放！
```

如何修正呢？我们可以这样：

```c++
void use_factory(T arg)
{
    Foo *p = factory(arg); //使用p
    delete p; //释放了
}
```

如果以后还要用这个指针，就可以甩锅给下一个调用者：

```c++
Foo* use_factory(T arg)
{
    Foo *p = factory(arg); //使用p
    return p; //释放了，释放个*
}
```

##### delete之后重置指针值（真的好麻烦。。。）

我们delete指针后，指针值就是无效了，但是其实指针一般仍然保留着那个动态内存的地址（虽然这个内存已经被释放了）。**在delete之后，这个指针就指向了一块曾经保存数据对象但现在已经无效的内存**（可以把它看成一个痴情的指针，虽然指向的对象内存已经没了，它还是指着那个地址），**这就是空悬指针**。 一般来说，我们是没机会碰到空悬指针的问题的，因为这会已经离开了指针的作用域了；如果还没离开它的作用域，我们还想保留它（~~不知道为啥有这种奇怪的爱好~~），可以将它赋值为**nullptr**，这样就指明了它不指向任何对象。

##### 你以为这样就好了吗，还会有问题。。。

抛一段可能有问题的代码：

```c++
int *p(new int(42)); //p指向动态内存
auto q = p; //q也指向那个内存
delete p; //那个内存被释放
p = nullptr; //指出了p是空指针，解决了空悬指针的问题
```

然而，这样还是会有问题，因为有人可能会去使用q指针啊，然而q指针指向的内存已经被释放了，它是个空悬指针啊！

怎么样，自己搞很麻烦很容易出错吧。。。

### shared_ptr和new结合使用

这个标题是我们的目标，同时我们也要想，为什么要结合着使用。在此之前，先看些基础知识。

#### 智能指针的构造函数是explicit的

可能你已经忘了explicit是什么意思了，我回去翻了下，是**禁止隐式转换**的意思，就因为这个规定，接下来就有一些事情可以搞了：

```c++
shared_ptr p1(new int(24)); //我们知道这样是对的，直接初始化
shared_ptr p2 = new int(24); //那这样呢？错了
```

**原来之前把隐式转换的意思搞错了**

<font color=red>隐式转换指的是直接从一个别的类型变成一个类，而直接初始化是要调用构造函数的，举个例子上面的代码，第一行p1(new int(24)),给构造函数传参数，转化一下这可不叫隐式转换，第二行，p2=new int(24),这里p2必须先隐式调用一个构造函数把这个类型变成自己，再拷贝才行，其实也就是我们说的拷贝初始化，这才是隐式转换</font>

第二个为什么错了呢？因为等号右边用**new构造临时对象时返回的是一个普通指针int***，它试图**隐式转换为智能指针**，但是我们的智能指针的构造函数很无情，不允许隐式转换，所以报错了。 再来个函数返回类型的例子：

```c++
shared_ptr clone(int p)
{
    return new int(p); //错误：试图隐式转换为shared_ptr
}
//正确示例
shared_ptr clone(int p)
{
    return shared_ptr( new int(p) );
}
```

 *默认情况下，**一个用来初始化智能指针的普通指针必须指向动态内存**（**就是说它得指向new出来的**），**因为智能指针会delete啊**。*我们当然也可以让智能指针指向其他类型的资源，但是你得提供自己的释放操作去代替delete。（以后再介绍怎么定义自己的释放操作。）

下面两个图看表格就行

#### 不要混合使用普通指针和智能指针

来个错误例子：

```c++
void process(shared_ptr ptr){}
int *x(new int(24));
process(shared_ptr(x));
int j = *x; //x是空悬指针，自己想想为啥，这段代码很重要啊，好好看看
```

#### 也不要使用get初始化另一个智能指针或为智能指针赋值

还是来一个会导致空悬指针的示例：

```c++
shared_ptr p(new int(42));
int *q = p.get();
{
    shared_ptr(q); //两个独立的智能指针指向相同的内存块（要出事情）
} //q指向的内存被销毁
int foo = *p; //p是空悬指针，报错。
```

#### 其他shared_ptr操作

```c++
shared_ptr p(new int(42));
p = new int(1024); //错误：禁止隐式转换
p.reset(new int (1024)); //这样可以，p指向一个新对象
//有时为了避免p原来的内存被释放，我们还会这么写
if(!p.unique()){ p.reset(new string (*p)); }
```

### 智能指针和异常

使用智能指针的好处：即便代码出现没有catch到的异常，还是能保证内存的释放：

```c++
void f()
{
    shared_ptr sp(new int(42));
    //假设之后的代码出现了没有捕捉到的异常
} //在函数结束时，shared_ptr还是会自动释放内存
//假设代码中还有局部变量来拷贝sp呢，还是会正确释放，除非有函数外部的变量拷贝了sp，想想这是为啥
```

普通指针来管理动态内存的坏处：

```c++
void f()
{
    int* p = new int(42);
    //假设之后的代码出现了没有捕捉到的异常
    delete ip;
}
//虽然代码很好地写了delete ip，但出现异常之后p指向的内存就不会被释放了，根本无法释放
```

**就是如果出现了异常，智能指针还能通过析构好好首尾，但正常指针因为抛出了异常不可能正常运行了，就无法运行delete了**

一些没有定义析构函数的类也会出现类似问题：

```c++
void f()
{
    connect c; //默认初始化
} //如果没有显示销毁c，那c就不会被销毁了
```

我们如何改进呢，用智能指针管理，并且定义一个函数（删除器）用来代替delete函数：

```c++
void end_connect(connect *p){ disconnetc(*p); } //定义了删除器（调用了一个函数）
void f()
{
    connect c;
    shared_ptr p(&c, end_connect); //用p来管理c的内存
}//退出后c就会被正确关闭销毁
```

我们讲了那么多，只是讲了三个智能指针中的一个shared_ptr，还有两个呢：unique_ptr和weak_ptr（~~是不是很烦~~）学好了第一个，那后面两个其实也还算简单，下面我们分别来介绍。

### unique_ptr

一个unique_ptr独有它所指向的对象，与shared_ptr（人尽可夫）不同，unique_ptr比较霸道总裁，某个时刻**只能有一个unique_ptr指向给定的对象**，所以啊，**当unique_ptr被销毁时，它所指向的对象也被销毁。**

```c++
unique_ptr p1;
unique_ptr pt(new int(42));
```

**unique_ptr不支持普通的拷贝或赋值**（因为它独有啊，霸道啊），拷贝给你了不是有两个指向那个内存了吗？

不过啊，**我们可以调用一些函数来将所有权从一个unique_ptr（非const）转移给另一个unique_ptr**：

```c++
unique_ptr p1(new string("hello"));
unique_ptr p2(p1.release()); //p2被初始化为p1原来保存的指针，而将p1置空
unique_ptr p3(new string("hi"));
p2.reset(p3.release); //跟上面效果一样，但形式不同，因为这里不是初始化了，算是赋值
p2.release(); //我们把指针置空了，却没有释放内存。。。
```

#### 传递unique_ptr和返回unique_ptr

不能拷贝unique_ptr有**例外：可以拷贝或赋值一个将要被销毁的unique_ptr：** 

```c++
//函数返回一个unique_ptr
unique_ptr clone(int p)
{
    return unique_ptr(new int(p)); //正确：int*隐式转换到unique_ptr
}
//返回一个局部对象的拷贝
unique_ptr Clone(int p)
{
    unique_ptr ret(new int(p));
    return ret; 
}
```

**真他妈烦...怎么这么多特例**

对于上面这两个函数，编译器很聪明，它知道要返回的对象将要被销毁，所以它会执行一种特殊的拷贝（以后再介绍）。

#### 向unique_ptr传递删除器

unique_ptr默认还是delete删除，也支持自定义，但是它这个自定义很特殊（以后再解释原因），我们来看一下格式（比较复杂的）：

```c++
unique_ptr p (new objT, fcn);
//p指向一个类型为objT的对象，并使用类型为delT的对象释放objT对象
//怎么释放呢？它会调用delT类型对象fcn
```

### weak_ptr

weak_ptr指向**由shared_ptr管理的对象**，但**不改变引用计数**

```c++
auto p = make_shared(24); 
weak_ptr wp(p); //创建weak_ptr要用shared_ptr来初始化它
//由于对象可能不存在，我们在用weak_ptr访问时要调用lock函数来检查对象是不是还在
if(shared_ptr np = wp.lock()) //如果对象存在，lock返回该对象的shared_ptr
{
    //在这用np访问是安全的
}
```

我们可能觉得这个weak_ptr没啥大用，那我们就来举个例子，说明它还是有用的。

#### 核查指针类

还记得我们最开始的StrBolb类吗，我们要为它定义一个指针类StrBlobPtr，可以指向它的对象，该指针会保存一个weak_ptr，指向StrBlob的data成员（在初始化时给它的）。

为什么要用weak_ptr呢？**因为它不会影响一个给定的StrBlob所指向的vector的生存期，但是它可以阻止用户访问一个不存在的vector**：

```c++
class StrBlobPtr
{
public:
    StrBlobPtr() : curr(0){}
    StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz){}
private:
    shared_ptr<vector<string>> check(size_t, const string&) const; //检查下标合法性
    weak_ptr<vector<string>> wptr; //指向vector的弱指针
    size_t curr; //记录在数组中的当前位置
};
```

我们要定义一下check函数，它不仅要检查下标的合法性，还要负责检查指向的vector还在不在：

```c++
shared_ptr<vector<string>> StrBlobPtr::check(size_t i, const string &msg) const
{
    auto ret = wptr.lock; //vector还在吗
    if(!ret)
    {
        throwruntime_error("空悬指针了啊");
    }
    if(i >= ret->size())
    {
        throw out_of_range(msg);
    }
    return ret; //成功的话返回指向vector的shared_ptr
}
```

### 动态数组

我们目前学到的那些动态内存分配，一次只能分配/释放一个对象，**但某些应用需要一次为对象分配很多内存**，C++语言定义了另一种**new表达式语法**，**可以分配并初始化一个对象数组**。 

我们之前构造的动态内存的那个类**StrBlob**，**使用了vector容器作为底层**，**这是一种更简单、快速、安全的方式，因为vector会帮我们处理一些细节，而我们构造的应用基本都没有直接访问动态数组的需求。**

所以说啊，接下来虽然要介绍动态数组，**但一般情况下还是推荐使用容器作为底层**，你看啊，使用容器的类可以使用默认版本的拷贝、赋值和析构操作；**而分配动态数组的类必须自定义这些**。。。

#### new和数组

```c++
int a = 5;
int *pia = new int[a]; //pia指向第一个int，注意这个a是变量哦，在普通的数组里可不能这么用
```

虽然我们通常称**new T[]分配的内存为动态数组**，这个说法其实是不准确的：**因为在用new分配一个数组时，我们没有得到一个数组类型的对象**，**我们得到的数组元素类型的指针**。**由于分配的内存不是一个数组类型，我们不能用begin或end，因为这些函数使用数组维度来返回指向首元素和尾后元素的指针，而维度是数组类型的一部分，我们这没有数组类型，也就没有维度了；同样，我们也不能用范围for语句去处理动态数组的元素。**

##### 初始化动态分配对象的数组

说完了定义，再来介绍如何初始化：

```c++
int *pia = new int[10]; //10个未初始化的int
int *pia2 = new int[10](); //10个值初始化为0的int
int *psa = new string[10]; //10个空string
int *psa2 = new string[10](); //10个空string

int pia3 = new int[5]{0, 1, 2, 3, 4};
string *psa3 = new string[10]{"a", "b", string(3, 'c')}; //前三个定了，后面值初始化
```

动态分配一个空数组是合法的：

```c++
char *cp = new char[0]; //正确：但是cp不能解引用，返回一个合法的非空指针
char arr[0]; //错误
```

这动态数组也太秀了

##### 释放动态数组

为了释放动态数组，我们要在delete和指针之间加一个方括号（自己记住，编译器可能漏了方括号也不知道）：

```c++
delete [] pa; //pa必须指向一个动态分配的数组或为空
delete p; //p必须指向一个动态分配的对象或为空
```

**数组中的元素按逆序销毁。**

**我们必须要加方括号，即便用了类型别名，看起来不像数组似的：**

```c++
typedef int arrT[24];
int *p = new arrT; //看着像分配了一个对象
delete [] p; //还是要方括号，毕竟分配的确实是个数组
```

##### 智能指针和动态数组

标准库用unique_ptr来管理动态数组（应该是重载了），我们必须在对象类型后面加一对方括号：

```c++
unique_ptr<int []> up(new int[10]); //up指向一个包含10个未初始化int的数组
up.release(); //自动用delete[]销毁其指针，这个很方便哦，不用自己定义删除器了
```

我们这里用的unique_ptr提供的操作和之前那个unique_ptr不太一样，**毕竟这里的是指向一个数组的，我们不能使用点和箭头成员运算符，但是我们可以用下标运算符来访问数组中的元素（就记住它指向的是数组，应该说它的对象是数组）：**

```c++
for(size_t i=0; i!=10; ++i)
{
    up[i] = i;
}
```

我们还有一个智能指针啊，**shared_ptr，但是用它管理比较麻烦，我们得自己定义删除器：**

**shared_ptr没有定义关于动态数组的东西**

```c++
shared_ptr<int> sp(new int[10], [](int *p){delete [] p;});
//我们传递给shared_ptr一个lambda作为删除器
```

#### alloctor类

new有一些灵活性的局限：

1. **它将内存分配和对象构造放在一起** 
2. **delete将对象析构和内存释放放在一起** 

你可能没注意过这个问题（好吧，我也没注意过），我们在分配单个对象时，通常希望将内存分配和对象初始化放在一起，因为我们一般是知道对象有什么值才去给它分配内存的。

但是，**在分配一大块内存时，情况就有些不同了，我们希望将内存分配和对象构造分离**，就是说，我们先分配大块内存，在真正需要时才执行对象创建操作。

下面来个很好的示例，显示用new的局限性：

```c++
//用动态分配的string数组保存输入的string
string * const p = new string[100]; //构造100个空string（还好string类有默认构造函数）
string s;
string *q = p;
while(cin >> s && q != p + 100)
{
    *q++ = s;
}
const size_t size = q - p;
delete[] p;
```

**如果输入的string只有3个，那么我们创建了97个没用的，而且前面3个元素被赋值了两次，后面97个元素被默认初始化为空string，这根本不是我们想要的。**

#### allocator类

所以，神器要来了。 

保准库**alloctor**类定义在**头文件memory**中，**帮助我们把内存分配和对象构造分离开。它分配的内存是原始的、未构造的：**

**allocator就是一个分配内存的**

**第二句分配了n个string给一个指针指向最后构造的元素之后的位置**

```c++
alloctor<string> alloc; //可以分配string的alloctor对象
auto const p = alloc.allocate(n); //分配n个未初始化的string（不会调用string的默认构造函数哦）
```

##### allocator分配未构造的内存

```c++
auto q = p; //q指向最后构造的元素之后的位置
alloc.construct(q++); //*q为空字符串
alloc.construct(q++, 10, 'c'); //*q为cccccccccc
alloc.construct(q++, "hi"); //*q为hi
cout << *q << endl; //不行，q指向未构造的内存！

//我们来销毁刚刚构造的对象
while(q != p)
{
    alloc.destroy(--q); //这个--符号的位置和上面++符号的位置要注意啊
}

//当元素被销毁后，我们就可以重新使用这部分内存来保存其他string，也可以将其归还给系统
alloc.deallocate(p, n); //这里我们归还给系统了
```

**可以看到分配器是最原始的方式来分配内存**

##### 拷贝和填充未初始化内存的[算法]()

标准库很周到，还定义了两个**伴随[算法]()**，用来在**未初始化内存中创建对象**： 我们来举个例子，把栈区的一个vector拷贝到堆区，并且在堆区后半部分初始化元素：

```c++
vector<int> vi = {0, 1, 2};
allocator<int> alloc;
auto p = alloc.allocate(vi.szie()*2); //分配两倍动态内存空间
auto q = uninitialized_copy(vi.begin(), vi.end(), p); //拷贝vi到堆区
uninitialized_fill_n(q, vi.size(), 24); //剩余元素初始化为24
```

**allocator是STL的重要组成**，但是一般用户不怎么熟悉他，因为allocator隐藏在所有容器（包括vector）身后，默默**完成内存配置与释放，对象构造和析构的工作**。

![](C:\Users\DLSH\diabate\figure\stl_allocator.png)

从用户代码`std::vector<int> v;`开始，vector的**模板参数class T被替换为int**，同时第二个模板参数因为没有指定，所以为默认模板参数，即`allocator<int>`，这个vector对象v会在内部实例一个`allocator<int>`的对象，用来管理内存。

你注意到了，为什么allocator也需要接收这个模板参数T，管理内存应该不需要知道T的具体类型（int）吧？

前面说了，allocator除了负责内存的分配和释放，**还负责对象的构造和析构**，知道类型才能调用对象的构造和析构函数。而且，对于分配内存，allocator接口设计中有类似于“为n个T类型的对象分配内存”这种批量操作，这就需要知道类型才能算出对象需要的空间了。

你也注意到，容器vector的第二个参数有默认值，那么我们也可以为其指定一个其他的值？也就是说，我们自己按照STL标准实现一个allocator，就可以搭配STL里的容器使用了？

确实是的，假设我们自己写了一个allocator，叫做my_alloc，就可以指定它来为vector分配空间：

### 使用标准库：文本查询程序

接下来这一章，**我们要实现一个很好用的程序，来总结我们这段时间标准库内容的学习** 
程序功能如下： 允许用户在一个给定的文件中查询单词，查询的结果是单词在文件中出现的次数，还有这些单词所在行的列表。（如果一个单词在一行中出现多次，此行只输出一次），行输出的顺序按照给定文件中行的顺序。

举例： 输入：给定文件为-本章C++ Primer英文版内容 单词为：element

输出： element occurs 112 times
 (line 36) A set elements contains only a key;
 (lin 158) operator creates a new element
接下来还有大约100行，都是单词element出现的位置。

**这里可以好好看看**