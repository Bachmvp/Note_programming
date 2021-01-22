# C++ primer section13

## 拷贝控制

我们已经学过，每个类都定义了一个新类型和在此类型对象上可执行的操作，比如我们可以定义构造函数，用来控制在创建此类型对象时做什么。
在这一章中，我们要介绍一些函数来控制类的行为：
这一章介绍的类的行为有：拷贝、赋值、移动、销毁
手段（特殊的成员函数）有：拷贝构造函数、移动构造函数、拷贝赋值运算符、移动赋值运算符、析构函数 

以上这些都被称为拷贝控制。

| 特殊的成员函数           | 控制类的行为                                                |
| ------------------------ | ----------------------------------------------------------- |
| **拷贝和移动构造函数**   | 用同类型的另一个对象初始化本对象时做什么**（class a(b)）**  |
| **拷贝和移动赋值运算符** | 将一个对象赋予同类型的另一个对象时做什么**（class a = b）** |
| **析构函数**             | 此类型对象销毁时做什么                                      |

当然了，这些也可以不用我们亲自动手，当我们定义的类没有定义这些特殊的成员函数时，**编译器会为我们生成它们**，但是编译器生成的那些函数的行为不一定是我们想要的，**这点和构造函数一样**，所以啊，我们要认识到什么时候需要自己去定义这些操作，这往往也是实现拷贝控制最难的地方。

### 拷贝、赋值与销毁

我们先来介绍最基本的操作-**拷贝构造函数**、**拷贝赋值运算符**和**析构函**数（移动操作是C++11引入的，放在后面介绍） 

#### 拷贝构造函数

拷贝构造函数：

1. **构造函数** 

2. **第一个参数是自身类类型的引用** 

3. **任何额外参数都有默认值**

   ```c++
   class Foo
   {
   public:
    Foo(); // 构造函数
    Foo(const Foo&); //拷贝构造函数，参数最好是const且不要是explicit的，方便大家用
   };
   ```

##### 合成拷贝构造函数

 **不管我们有没有定义拷贝构造函数，编译器都会好心地帮我们合成一个拷贝构造函数。** 
一般来说：合成的拷贝拷贝构造函数会将其参数的成员逐个拷贝到正在创建的对象中，编译器从给定的对象中依次将每个**非static成员**拷贝到正在创建的对象中。
每个成员的类型决定了它如何拷贝：

- 类类型成员：**调用拷贝构造函数来拷贝** 

- 内置类型成员：**直接拷贝** 

- 数组：逐个元素来拷贝一个数组 我们来用老朋友Sales_data作为例子看看：

  ```c++
  class Sales_data
  {
  public:
    Sales_data(const Sales_data)&;
  private:
    string bookNo;
    int units_sold = 0;
    double revenue = 0;
  };
  Sales_data::Sales_data(const Sales_data &orig) : bookNo(orig.bookNo),
  units_sold(orig.units_sold), revenue(orig.revenue) {}
  ```

##### 拷贝初始化和直接初始化的区别

```c++
string dots(10, 's'); //直接初始化
string s(dots); //直接初始化，狗屁直接初始化，这就是拷贝初始化
string s2 = dots; //拷贝初始化
```

**其实很明显，之前一直说赋值初始化和拷贝初始化很像就是这样，当你真正调用s(),这就是单纯的直接初始化，但有=号就是照着模子，这个模子先是一个对象，然后你拷贝初始化一个**

1. 直接初始化：**普通的函数匹配** 
2. 拷贝初始化：**拷贝构造函数**（或之后要介绍的移动构造函数） 

**拷贝初始化除了在定义变量时用=会发生外，还有哪些情况呢（其实跟值传递类似）**：

- **实参传递给非引用形参** ，<font color=red>我们通常说的值传递不就是这东西</font>
- **返回类型为非引用**  <font color=red>函数返回值不就是这东西</font>
- **用花括号列表初始化数组或聚合类**（自己回顾聚合类定义） 其实之前我们在用一些容器的函数时就涉及到这方面的知识了，只不过那会还不方便提，我们在**调用insert或push时，进行的是拷贝初始化**；**用emplace时用直接初始化**

**那也就是拷贝初始化必须不能传值，必须得传个引用类型呗**

##### 为什么拷贝构造函数的参数必须是引用类型

这里的逻辑有点绕，看好了： **假如我们的拷贝构造函数参数是值传递的，我们为了调用它，就要拷贝这个类的对象，怎么拷贝这个对象呢，通过调用拷贝构造函数，这就尴尬了，回到前面去了。。。**

##### 拷贝初始化函数是explicit带来的影响

我们以vector<int>为例，vector接受单一大小参数的构造函数是explicit的：

```c++
vector<int> v1(10); //正确：直接初始化，10个0
vector<int> v2 = 10; //错误：无法隐式转换

//函数参数
void f(vector<int>); //正确
f(10); //错误
f(vector<int>(10)); //正确
```

所以我们**在拷贝构造函数中还是尽量不用explicit吧，允许隐式转换，爱怎么调用就怎么调用**。

##### 编译器可以绕过拷贝构造函数

```c++
string a = "aa"; //这是调用拷贝构造函数
string b("aa"); //这样就略过了拷贝构造函数，直接初始化了
```

**其实我觉得就是构造函数替代了拷贝构造函数。直接构造更好**

#### 拷贝赋值运算符

拷贝构造函数是在定义时用=，拷贝赋值是在赋值时：

```c++
int a = 0; //拷贝构造函数
int b;
b = a; //拷贝赋值运算符
```

**如果类未定义自己的拷贝赋值运算符，编译器会为它合成一个。**（编译器很实诚啊）

**所以其实我之前一直疑问这拷贝构造函数也是=号，赋值运算符也是=号，这不就混了吗，其实是定义和赋值的概念搞混了，一般对于类型来说都有默认构造函数，所以声明的时候就定义了。但是之后再用=号就不是初始化了啊，因为已经初始化过了，是赋值运算符了**

在介绍之前，我们先要了解一个概念：

##### 重载运算符

**重载运算符就是函数**，只不过**函数名是operator关键字后面接表示要定义的运算符的符号组成**，所以啊，**赋值运算符就是operator=**

```c++
class Foo
{
public:
    Foo& operator=(const Foo&); //赋值运算符，通常返回左侧对象的引用，为了可以连着调用成员函数吧
};
```

##### 合成拷贝赋值运算符

我们来直接举例吧：

```c++
Sales_data& Sales_data::operator=(const Sales_data &rhs)
{
    bookNo = rhs.bookNo;
    units_sold = rhs.units_sold;
    revenue = rhs.revenue;
    return *this; //返回此对象的引用
}
```

值得注意的是：**无论是拷贝构造函数还是拷贝赋值运算符，它们大多数是拷贝的作用，但有些情况也会用来禁止该对象的拷贝或赋值**，很神奇吧，后面会有介绍。其实也不算神奇，反正操作都是自己定义，爱怎么来怎么来罢了。

#### 析构函数

析构函数执行与构造函数相反的操作：

1. **构造函数初始化对象的非static数据成员** 
2. 析构函数**释放对象使用的资源**，**并销毁对象的非static成员** 

析构函数是类的一个成员函数，名字是由波浪线加类名构成，没有返回值，没有参数：

```c++
class Foo
{
public:
    ~Foo(); //析构函数，因为它不接受参数，所以不能被重载，一个类只有一个析构函数
};
```

##### 析构函数完成什么工作

1. 构造函数中：<font color=red>成员的初始化是在函数体执行之前完成的，顺序是类中出现的顺序初始化 </font>
2. 析构函数：<font color=red>首先执行函数体，然后销毁成员，按初始化顺序来逆序销毁  </font>

**隐式销毁一个内置指针类型的成员不会delete它所指向的对象（普通指针没有析构函数呀）**，但**智能指针是类类型，是有析构函数的**，所以**智能指针在析构阶段会被自动销毁。**

##### 什么时候会调用析构函数

无论何时一个对象被销毁，就会调用其析构函数：

- 变量在离开其作用域时被销毁 
- 当一个对象被销毁时，其成员被销毁 
- 容器被销毁时，其元素被销毁 
- 动态分配的对象，当指向它的指针被delete时，被销毁 
- 对于临时对象，当创建它的完整表达式结束时被销毁。 对于这些我们中国的祖先就很能理解，就是过河拆桥，飞鸟尽良弓藏，狡兔死走狗烹嘛。 

因为析构函数自动运行，我们的程序可以按需分配资源，不用去担心什么时候释放它们，我们的机制可以为我们保证完成这个析构的任务：

```c++
{
    Sales_data *p = new Sales_data; //p是普通指针
    auto p2 = make_shared<Sales_data>(); //p2是shared_ptr
    Sales_data item(*p); //拷贝构造函数将*p拷贝到item中,这里咋又不是直接初始化了？？？
    vector<Sales_data> vec;
    vec.push_back(*p2); //拷贝p2指向的对象
    delete p; //对p指向的对象执行析构函数
}
//退出局部作用域：item、p2和vec调用析构函数；p2被销毁后，其对象引用计数为0，
//对象被释放；销毁了vec，就销毁了它的元素
```

上面的代码比较复杂，但是仔细看会发现，我们只要管自己new出来的p就好了，其他的析构都不用自己操心。你要是不用new的话，你什么也不用管。

##### 合成析构函数

老样子，未定义的话，编译器会生成：

```c++
class Sales_data
{
public:
    ~Sales_data() {} //成员会被自动销毁，所以不需要做任何事情。。。
}
```

bookNo是string，会调用string的析构函数，释放bookNo所用的内存。

**析构函数其实名不符实：** **析构函数体自身并不直接销毁成员，成员是在析构函数体之后隐含的析构阶段中被销毁的。在整个对象销毁过程中，析构函数体是作为成员销毁步骤之外的另一部分而进行的。**

讲人话就是，销毁人家自己会进行，你要是想另外加点什么操作，就在析构函数体里面加。

#### 三/五法则

这一部分就是要告诉你，**前面介绍的三个函数，如果你要自定义的话最好三个全都自定义了。** 

我们一共要学五个特殊的成员函数，现在已经学了三个了：

1. **拷贝构造函数** 
2. **拷贝赋值运算符** 
3. **析构函数** 

C++允许我们定义任意个数的这些函数，我们的建议是这三个作为整体一起定义或者不定义。接下来就会解释为啥

##### 需要析构函数的类也需要拷贝和赋值操作

当我们决定一个类**是否需要自定义它的拷贝控制成员时**，一个基本原则是确定**它要不要析构函数**，因为**对析构函数的需求更明显**（教材这么说的）。**如果一个类需要一个析构函数，我们几乎可以肯定它也需要一个拷贝构造函数和拷贝赋值运算符。**

我们来看个只定义析构函数的类，分析一下会发生什么问题：

```c++
class HasPtr
{
piblic:
    HasPtr(const string &s = string()) : ps(new string(s)), i(0){} //看看这个构造函数
    ~HasPtr(){delete p;} //自定义析构函数
};
//调用这个类，你能看出哪里错了吗？
HasPtr f(HasPtr hp)
{
    HasPtr ret = hp;
    return ret;
}
```

**f函数返回时，hp和ret都会被销毁，都会调用HasPtr的析构函数**，**都会delete ret和hp中的指针成员**，**但这两个指针指向的是同一个对象，于是它被释放了两次，就错了**。

**相当于就是假如你的类里有个指针，指向的还是个堆的东西，然后你想在析构函数里负责任的给删掉，但是默认的拷贝直接把你的所有东西复制了个遍，然后销毁自己的时候又把那个对象删了一遍，这不就出事了，指针惹的祸**

**其实也可以理解为当你需要写析构的时候，说明一般都有堆的东西在类里**

##### 需要拷贝操作的类也需要赋值操作，反之亦然

拷贝构造函数与赋值运算符互为充要条件（这是个好的建议）

#### 使用=default

我们知道拷贝构造函数的话，无论如何编译器会为我们生成，其他的话，如果你自定义了，编译器就不生成了，但是，**我们可以通过将拷贝控制成员定义为=default来显式地要求编译器生成合成的版本**（这个之前就学过的，反正C++允许你做任何事情）：

```c++
class Sales_data
{
public:
    Sales_data() = default; //生成构造函数
    Sales_data(const Sales_data&) = default; //生成拷贝构造函数（不写也行，但是有区别，内联）
    Sales_data& operator=(const Sales_data &); //赋值运算符
    ~Sales_data() = default; //生成析构函数
};
```

**类内用=default的合成函数，被隐式地声明为内联**；**如果不希望是内联，可以在类外用=default（声明在类内，定义在类外,在类内定义就是内联函数了）**

#### 阻止拷贝

你看，C++是不是搞事情，本来蛮好的拷贝构造函数，赋值运算符的，现在又来个阻止拷贝。。。不过还是有道理的，道理如下：
**我不想让我的类对象被拷贝**（这个需求是比较合理的），于是我**不定义拷贝构造函数**，但是编译器又会自动生成，我能怎么办，只好**定义一个拷贝构造函数来阻止拷贝**。

##### 定义删除的函数

**在函数的参数列表后面加上=delete就是删除的，意思是：我们虽然声明了它们，但是不以任何方式使用它们：**

```c++
struct NoCopy
{
    NoCopy(const NoCopy&) = delete; //阻止拷贝
    NoCopy &operator=(const NoCopy&) = delete; //阻止赋值
};
```

**=delete必须出现在第一次声明函数的时候**（不像default一样），**因为default直到编译器生成代码时才需要知道**，**而delete是需要编译器在声明时就知道，以便禁止试图使用它的操作**。

delete和default还有一个差别：**delete可以用在任何函数（这里介绍的主要是拷贝控制方面而已）**；**而default只能用在合成的默认构造函数或拷贝控制成员。**

##### 析构函数不应该是删除的成员

语法上可能允许，但如果析构函数被删除，我们就无法销毁该类型的对象了。所以啊，**如果你定义了一个删除的析构函数，那编译器就不许你定义这个类的对象，临时对象也不行**。更严格的是，如果你这个类里面的某个成员的类型删除了析构函数，那么，不好意思，你这个类被株连了，也不能定义该类的对象。
下面来个定义了删除析构函数的类，我们还是可以以一定的方式去使用它的（只不过我们不建议这么做）：

```c++
struct NoDtor
{
    ~NoDtor() = delete; //删除析构函数
};
NoDtor nd; //错误：无法定义该类对象
NoDtor *p = new NoDtor(); //正确：但是我们无法delete p
delete p; //错误：NoDtor的析构函数是删除的
```

##### 合成的拷贝控制成员可能是删除的

- 如果一个类**有数据成员不能默认构造、拷贝、复制或销毁，那对应生成的合成成员函数将被定义为删除的。**
- 很好理解：一个成员有删除的或不可访问的析构函数会导致合成的默认和拷贝构造函数被定义为删除的，因为如果不是删除的话，我们就会创建出无法被销毁的对象。
- 对于具有**引用成员**或**无法默认构造的const成员**的类，编译器不会为其合成默认构造函数，因为引用和const都必须在创建的时候就初始化，所以编译器无法去赋值它们，就是说**你在类成员定义的时候就必须去初始化它们**，那你要默认构造函数干嘛，索性就不合成了。

对于最后一个再解释一下：**如果有const，那在定义的时候就初始化了，你在默认构造函数里面再去赋值给const就是错的。这个还是很重要的，经常会忘记**

对于引用，虽然我们可以把一个新值给这个引用，但我们改变的是引用绑定的那个对象，而不是引用本身，这个行为往往不是我们想要的。

所以，索性把它俩都不要了。

以前通过访问控制权限来阻止拷贝的我就不介绍了，大家也不用去了解了。**（通过将析构函数声明为public（允许定义该类的对象）**，且将**拷贝构造函数和拷贝赋值运算符声明为private来阻止拷贝**）

## 拷贝控制和资源管理

这部分我们主要介绍两个内容，通过定义不同的拷贝操作，使自定义的类的行为看起来像一个**值或指针**。

- **类的行为像一个值**，意味着**它应该也有自己的状态**。**当我们拷贝一个像值的对象时，副本和原对象是完全独立的**，**改变副本不会对原对象有任何影响**，反之亦然。 
- **行为像指针的类共享状态**，**当我们拷贝一个这种类的对象时，副本和原对象使用相同的底层数据，改变副本也会改变原对象**，反之亦然。还记得我们的StrBlob类吗，为了实现它我们才使用了智能指针。 

在我们使用过的标准库类中，**标准库容器和string类的行为像一个值**；**shared_ptr类提供类似指针的行为**；还有一种奇葩，他们的行为两个都不像，例如IO类和unique_ptr不允许拷贝或赋值。

接下来我们要实现一个类HasPtr，让它的行为像一个值；然后重新实现它，使其像一个指针：
我们的HasPtr有两个成员，一个int和一个string指针，对于内置类型int，我们就直接拷贝值好了，改变它也不符合常理；我们把重心放到string指针，它的行为决定了该类是像值还是像指针。

#### 行为像值的类

为了像值，**每个string指针指向的那个string对象**，都得有自己的一份拷贝，为了实现这个目的，我们需要做以下三个微小的工作：

1. **定义一个拷贝构造函数，完成string的拷贝，而不是拷贝指针** 
2. 配套的，**定义析构函数来释放string** 
3. **定义一个拷贝赋值运算符来释放对象当前的string**（这话待会会解释滴），**并从右侧运算对象拷贝string** 

我们来抛代码：

```c++
class HasPtr
{
public:
    HasPtr(const string &s = string()) : ps(new string(s)), i(0){} //默认实参，列表初始化

    HasPtr(const HasPtr &p) : ps(new string(*p.ps)), i(p.i){} //拷贝构造函数

    HasPtr& operator=(const HasPtr &); //赋值运算符声明

    ~HasPtr(){delete ps;} //析构函数

private:
    string *ps;
    int i;
};
```

**主要在于拷贝构造函数，它是有副本的**，会拷贝string对象，**所以析构函数要delete来释放内存**。这个类可以好好看看，写得很优雅。

**所谓的副本就是要动态新申请内存？数据成员还是个指针，只不过不是共享，是每个对象的指针单独指一份，所以要new，这样理解了，换句话说，指针或者string都是一样的，指针还成本小一点？**

##### 类值拷贝赋值运算符

情况是这样的：

```c++
a = b;
```

HasPtr对象出现这样的赋值时，干两件事：

1. **销毁左侧运算对象的资源（毕竟它没用了嘛），类似析构函数** 

2. **从右侧运算对象拷贝数据**，类似拷贝构造函数 为了保证一个对象能为它本身赋值，**我们先拷贝右侧对象，再释放左侧资源，并更新指针指向新分配的string**：

   ```c++
   HasPtr& HasPtr::operator=(const HasPtr &rhs)
   {
    auto newp = new string(*rhs.ps); //拷贝底层string
    delete ps; //释放旧内存
    ps = newp; //从右侧对象拷贝数据到本对象
    i = rhs.i;
    return *this; //返回本对象
   }
   ```

#### 定义行为像指针的类

我们可能觉得**拷贝指针就行了嘛**，没那么简单，你还是**要释放内存啊**，而且这个**释放内存的时机很重要：只有当最后一个指向string的HasPtr销毁时，才能释放内存**，所以啊，我们可以用**shared_ptr**，我们这里呢，不用智能指针，弄得麻烦些，让大家看看底层怎么实现引用计数：

```c++
class HasPtr
{
public:
    HasPtr(const string &s = string()) : ps(new string(s)), i(0),
    use(size_t(1)){} //默认实参，列表初始化

    HasPtr(const HasPtr &p) : ps(p.ps), i(p.i), use(p.use){++(*use)}
    //拷贝构造函数，要递增计数器

    HasPtr& operator=(const HasPtr &); //赋值运算符声明

    ~HasPtr() //析构函数
    {
        if(--(*use) == 0) //没人引用了才释放
        {
            delete ps;
            delete use;
        }
    }

private:
    string *ps;
    int i;
    size_t *use; //引用计数
};
```

赋值运算符：

```c++
HasPtr& HasPtr::operator=(const HasPtr &rhs)
{
    ++(*rhs.use); //递增右侧运算对象的引用计数
    if(--(*use) == 0) //递减左侧对象的引用计数并判断是否要释放内存
    {
        delete ps;
        delete use;
    }
    ps = rhs.ps; //拷贝
    i = rhs.i;
    use = rhs.use;
    return *this; //返回本对象
}
```

**所以是否共享数据是一个很重要的点啊**

### 交换操作swap

除了定义拷贝控制成员外，管理资源的类通常还会定义一个**swap函数**用于交换两个元素，对于需要重排元素的[算法]()尤其重要。
比如我们来交换一下前面写的HasPtr（值拷贝版本）：

```c++
HasPtr temp = v1;
v1 = v2;
v2 = temp;
```

这是很自然的一种写法，我们借助中间量temp来交换v1和v2，但是啊，**有时候对象很大**，**你跑去拷贝交换对象很浪费，不如去交换指针更合算**：

```c++
string *temp = v1.ps;
v1.ps = v2.ps;
v2.ps = temp;
```

#### 为自己的类编写swap函数

我们来为HasPtr类写个swap函数，当然写合算那个版本：

```c++
class HasPtr
{
    firend void swap(HasPtr&, HasPtr&); //友元函数，为了能访问private的数据成员
};
inline void swap(HasPtr &lhs, HasPtr &rhs) //声明为内联函数
{
    using std::swap; //为什么这样写就可以调用库函数，以后解释
    swap(lhs.ps, rhs.ps); //调用库函数交换指针
    swap(lhs.i, rhs.i);
}
```

#### 在赋值运算符中使用swap

我们定义了swap函数后，就可以利用它写出更简洁的赋值运算符：

```c++
HasPtr& HasPtr::operator=(HasPtr rhs)
{
    swap(*this, rhs);
    return *this;
}
```

我们来从头到尾仔细看看发生了什么。
我们来调用一下试试：

```c++
lhs = a;
```

这样就调用了赋值运算符： 

1. a是通过**值传递**的方式给**赋值运算符**的，也就是**a拷贝了一个副本rhs** 
2. 在函数体中调用swap函数交换了二者的数据成员，**交换的是指针**哦，**调用后this指向原来rhs的内存，现在rhs指向原来的this内存** 
3. return语句执行后，rhs作为局部对象被销毁，内存也被释放，这个被释放的内存就是原来this指向的内存，也就是调用赋值运算符语句中的lhs 

说了那么多，牛逼的地方在于哪里呢？**在于你都不用管内存拷贝释放等事情，把类写好之后保证你调用的方式是最经济有效的。**

### 拷贝控制示例

虽然通常来说分配资源的类更需要拷贝控制，但也有一些类需要拷贝控制成员的帮助来进行操作，下面我们就来介绍一个例子。 

我们将建立两个类用于邮件处理，两个类命名为Message和Folder，分别表示邮件消息和消息目录。**每个Message对象可以出现在多个Folder中，但是任意给定的Message的内容只有一个副本，这样的话，一条Message的内容改变，则我们从任意Folder来浏览它时看到的都是更新的内容** 

设计（这部分才是最难的，比用代码实现要难，设计永远比实现难啊）： 

- **为了记录Message位于那些Folder中，每个Message保存一个它所在Folder的指针的set；每个Folder也保存一个它包含的Message的指针的set** 
- 提供save和remove操作，用来向一个给定的Folder中添加或删除一条Message 
- **当拷贝Message时，不仅要拷贝Message内容，还要让副本也出现在跟原对象一样的Folder中** 
- 同样的，**销毁一个Message时，我们也要从包含此消息的所有Folder中删除指向此Message的指针。** 
- 赋值时，**左侧Message内容当然会被右侧代替，我们还得更新Folder集合，从原来包含左侧Message的Folder中删除它，并将它添加到包含右侧Message的Folder中。** 

我们这只实现Message，假定Folder类已实现，并且包含类名为**addMsg和remMsg**的成员，分别表示添加和删除。

#### Message类

```c++
class Message
{
    firend class Folder; //友元类
public:
    explicit Message(const string &str = "") : contents(str){}

    Message(const Message&);
    Message& operator=(const Message&);
    ~Message();

    void save(Folder&); //添加Message
    void remove(Folder&); //删除

private:
    string contents; //消息文本
    set<Folder*> folders;

    //一些工具函数
    void add_to_Folders(const Message&);
    void remove_from_Folders();
};
```

##### save和remove成员函数

```c++
void Message::save(Folder &f) //互相添加一下到set中
{
    folder.insert(&f); //将给定的Folder添加到我们的Folder列表中
    f.addMsg(this); //调用类Folder的成员函数，将此Message添加到f中去
}

void Message::remove(Folder &f)
{
    folder.eraser(&f);
    f.remMsg(this);
}
```

##### Message类的拷贝控制成员

当我们拷贝一个Message时，要遍历Folder指针的set，对每个指向原Message的Folder添加一个指向新Message的指针，我们来把这个操作抽象成函数：

```c++
void Message::add_to_Folders(const Message &m)
{
    for(auto f : m.folders)
    {
        f->addMsg(this);
    }
}
```

我们的拷贝构造函数就可以这么写：

```c++
//注意，也是set互相加哦，一个是初始化列表加，一个是调用函数加
Message::Message(const Message &m) : contents(m.contents), folders(m.folders)
{
    add_to_Folder(m);
}
```

##### Message的析构函数

与add类似，当一个Message被销毁时，我们必须从指向此Message的Folder中删除它，把这个操作也抽象成一个函数：

```c++
void Message::remove_from_Folder()
{
    for(auto f : folders
    {
        f->remMsg(this);
    }
}
```

于是，我们可以来写析构函数了：

```c++
Message::~Message()
{
    remove_from_Folders(); //别忘了释放string的content这种操作是不需要我们做的哦
    //别忘了之前说的析构函数的名不符实
}
```

##### Message的拷贝赋值运算符

我们要考虑自赋值的情况：

```c++
//remove和add的顺序不能变，不然就不好自赋值了
Message& Message::operator=(const Message &rhs)
{
    remove_from_Folders();
    contents = rhs.contents;
    folders = rhs.folders;
    add_to_Folders(rhs);
    return *this;
}
```

##### Message的swap函数

```c++
void swap(Message &lhs, Message &rhs)
{
    using std::swap; 

    //将每个消息的指针从其所在的Folder中删除
    for(auto f : lhs.folders) 
    {
        f->remMsg(&lhs);
    }
    for(auto f : rhs.folders)
    {
        f->remMsg(&rhs);
    }

    //交换contents和Folder指针set，调用的都是标准库函数
    swap(lhs.folders, rhs.folders);
    swap(lhs.contents, rhs.contents);

    //将每个Message的指针添加到新Folder中
    for(auto f : lhs.folders)
    {
        f->addMsg(&lhs);
    }
    for(auto f : rhs.folders)
    {
        f->addMsg(&rhs);
    }
}
```

好好设计一个类真的不太容易，但是使用起来就很爽了，造轮子难，用轮子爽

## 动态内存管理类

某些类需要在运行时分配可变大小的内存空间，这种类最好在**底层使用标准容器库**，例如我们的StrBlob类使用vector。
但是，有些类需要**自己进行内存分配**，它基本就要定义自己的**拷贝控制成员**来管理所分配的内存。 

 **我们在这一节将要实现标准库vector类的一个简化版本，它只能用于string，命名为StrVec。**

#### StrVec类的设计

主要参照vector<string>来。
我们将使用**allocator来获得原始内存**，由于它**分配的内存是未构造的**，我们将需要在**添加新元素**是用**allocator的construct成员在原始内存中创建对象**；同样的，我们在**删除元素时就使用destroy成员来销毁函数**。
每个StrVec有三个指针成员指向其元素所使用的内存：

- **elements，指向首元素** 
- **first_free，尾后元素** 
- **cap，指向分配的内存末尾之后的位置** 

StrVec还有一个名为**alloc的静态成员**，其**类型为allocator<string>**。**alloc成员会分配StrVec使用的内存**。我们的类还有4个工具函数：

1. **alloc_n_copy会分配内存，并拷贝一个给定范围内的元素** 
2. **free会销毁构造的元素并释放内存** 
3. chk_n_alloc保证StrVec至少有容纳一个新元素的空间，如果空间不够的话，它会调用reallocate来分配更多内存 
4. reallocate在内存用完时为StrVec分配新内存

#### StrVec类定义

设计好了之后，我们就可以动手写类了（有些函数只有声明，还没实现定义，后面再实现）：

```c++
class StrVec
{
public:
    StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr){} //默认构造函数
    //我看着这个函数还一时没反应过来（可能因为头晕。。。）

    StrVec(const StrVec&); //拷贝构造函数声明
    StrVec &operator=(const StrVec&); //拷贝赋值运算符声明
    ~StrVec(); //析构函数

    //一些常用成员函数
    void push_back(const string&);
    size_t size() const {return first_free - elements;}
    size_t capacity() const {return cap - elements;}
    string *begin() const {return elements;}
    string *end() const {return first_free;}

private:
    static allocator<string> alloc; //分配元素

    void chk_n_alloc()
    {
        if(size() == capacity())
        {
            reallocate();
        }
    }

    pair<string*, string*> alloc_n_copy(const string*, const string*);

    void free();
    void reallocate();

    //数据成员
    string *elements;
    string *first_free;
    string *cap;
};
```

该说明的都已经在注释中说明了。

接下来分别取实现已经声明的函数定义：

##### push_back

```c++
void StrVec::push_back(const string& s)
{
    chk_n_alloc(); //确保有空间
    alloc.construct(first_free++, s); //调用allocator成员construct来插入
    //至于construct怎么搞得咱们就不了解了，有兴趣自己去查把
}
```

##### alloc_n_copy

分配足够的内存来保存给定范围的元素，并将这些元素拷贝到新分配的内存中：

```c++
pair<string*, string*> StrVec::alloc_n_copy(const string *b, const string *e)
{
    auto data = alloc.allocate(e-b); //分配正好的空间
    return {data, uninitialized_copy(b, e, data)};
}
```

在返回语句中完成拷贝工作：
**返回语句中对返回值进行了列表初始化**，返回的pair中，**first指向分配内存的开始位置**（因为data作为名字来用就是首元素地址吧），**second是uninitialized_copy的返回值**，**这个值是一个指向尾后元素的指针**。

##### free

free要干两件事：

1. **destroy元素**，是指确实有内容的元素 

2. **释放分配的内存空间，包括没有元素的内存**

   ```c++
   void StrVec::free()
   {
   if(elements) //确保不是空指针，就是要有元素
   {
       for(auto p = first_free; p != elements;) //逆序的哦（为啥要逆序我不知道）
       //可能是为了重用这部分空间，删除好了之后指针指向首元素
       {
           alloc.destroy(--p);
       }
       alloc.deallocate(elements, cap-elements);
   }
   }
   ```

##### 拷贝控制成员

有了前面的工具函数，实现拷贝控制成员很简单：

```c++
StrVec::StrVec(const StrVec &s) //拷贝构造函数
{
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements = newdata.first;
    first_free = newdata.sevond
}

StrVec::~StrVec() {free();} //析构函数

StrVec &StrVec::operator=(const StrVec &rhs)
{
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second; //都等于
    return *this;
}
```

##### reallocate

我们会用到一些之后要学的函数：

```c++
void StrVec::reallocate()
{
    auto newcapacity = size() ? 2*size() : 1;
    //空就分配一个，不空就变为2倍，好好看看，这个写法很装逼哦

    auto newdata = alloc.allocate(newcapacity); //分配新内存

    auto dest = newdata; //指向新数组的下一个空闲位置
    auto elem = elemments; //指向旧数组的下一个元素

    for(size_t i=0; i != size(); ++i)
    {
        alloc.construct(dest++, move(*elem++));
        //这里的move函数你就理解为把旧数组元素移动到新数组中，不需要拷贝了
    }
    free(); //移动好元素就释放旧内存空间

    //更新数据成员
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}
```

写个东西还真是不容易。

**这里就是大体实现了一个string类，看了看内存分配器是咋骚操作的**

## 对象移动

新标准引入了移动对象的概念，回忆一下，我们在赋值操作时，经常进行这样的操作，**对象拷贝后就立即销毁**，在这些情况下，**移动而非拷贝对象会大幅度提升性能**。 

还有一个引入移动对象的原因：
**有些类型是不能被拷贝的，例如IO类和unique_ptr**，**在旧标准中我们无法在容器中保存它们，因为它们无法被拷贝，就不存在赋值之类的操作，但引入了移动操作后，我们就可以用容器保存它们。**

**标准库容器、string和shared_ptr类既支持移动也支持拷贝；IO类和unique_ptr类可以移动但不能拷贝** 

#### 右值引用

为了支持移动操作，新标准引入了一种很难搞的新概念-**右值引用**：必须绑定到右值的引用（还记得**左值表示身份右值表示值**吗）。 

我们**通过&&来获得右值引用，右值引用只能绑定到一个即将销毁的对象**，所以啊，我们才能自由地**将一个右值引用的资源移动到另一个对象中。**

接下来你要记住哪些表达式返回右值，哪些返回左值，这样才好正确绑定：

| 类型 | 表达式                                                       |
| ---- | ------------------------------------------------------------ |
| 左值 | 返回左值引用的函数、赋值、下标、解引用、前置递增递减运算符   |
| 右值 | 返回非引用类型的函数、算术、关系、位运算符、后置递增递减运算符 |

- **左值引用就可以绑定到类型为左值的表达式** 
- **右值引用**以及**const左值引用**可以绑定到**类型为右值的表达式**

看着有点烦吧，其实也还好，就记住**右值是临时的**，是即将销毁的，左值长期存在，来几个例子看看：

**&&这玩意我是真没见人用过**

```c++
int i = 42;
int &r = i; //正确
int &&rr = i; //错
int &r2 = i * 24; //错
const int &r3 = i * 13; //对
int &&rr2 = i * 2; //对
int &&rr3 = 42; //正确
int &&rr4 = rr3; //错误，rr3本身是变量，是左值
```

- **左值有持久状态** 
- **右值要么是字面常量**（const），**要么是求值过程中创建的临时对象**

因为右值引用只能绑定到临时对象：

1. **所引用的对象将要被销毁**
2. **该对象没有用户** 因为这样，使用右值引用的代码可以自由地接管所引用的对象的资源

##### 标准库move函数

**强行右值**，move算是一个移动构造函数：

**这个move用过啊！！**

**std::move并不能移动任何东西，它唯一的功能是将一个左值强制转化为右值引用，继而可以通过右值引用使用该值，以用于移动语义。**

**右值引用相当于把一个左值强行转化为一个弃子，把他的左值性质移动掉了**

```c++
int a = 12;
int &&b = std::move(a) //move函数告诉编译器，我们要把这个左值当成右值来处理
```

调用move就意味着：**除了对a赋值或销毁外，我们将不再使用它，例如我们不能把它的值赋给别人。**

#### 移动构造函数和移动赋值运算符

这两个成员类似对应的拷贝操作，但是它们从给定对象**窃取资源**而不是拷贝资源

移动构造函数和拷贝构造函数的唯一区别就是**它的引用是右值引用**

除了完成资源移动，移动构造函数还必须确保**移后源对象**处于这样一个状态-销毁它是无害的；特别是，一旦资源完成移动，**源对象**必须**不再指向被移动的资源**-这些资源的所有权已经归属**新对象** 

我们来为老朋友StrVec定义移动构造函数（注意看，它没有分配新内存哦）：

```c++
StrVec::StrVec(StrVec &&s) noexcept : elements(s.elements),
first_free(s.first_free), cap(s.cap) //noexcept表示不抛出异常（具体不解释了，先跳过）
{
    //上面的列表初始化就移动好了，注意参数是右值引用

    //接下来的话保证s进入这样的状态-对其进行析构函数是安全的
    s.elements = s.first_free = s.cap = nullptr;
}
```

千万别忘了函数体里面的那句话，**不然销毁移动后源对象就会释放掉我们刚刚移动的内存了。也就是源对象**
**其实就是s把资源给了新对象，自己都变成空指针，深藏功与名了。**

##### 移动赋值运算符

和赋值运算符类似，也要正确处理自赋值情况：

```c++
StrVec &StrVec::operator=(StrVec &&rhs) noexcept
{
    if(this != &rhs) //检测，不是自赋值再进行下面步骤，是自赋值直接返回
    {
        free(); //释放已有元素（是左侧对象的，就是this的，因为它要接管rhs的，原来的内存就不用了）
        //从rhs窃取资源
        elements = rhs.elements;
        first_free = rhs.first_free;
        cap = rhs.cap;

        //将rhs置于可析构状态
        rhs.elements = rhs.first_free = rhs.cap = nullptr;
    }
    return *this;
}
```

移后源对象要保持有效的，可析构的状态，但最好不要去动它（除了析构它之外），让它安静地功成身退

**其实移动和拷贝的区别就是一个用的左值引用，一个用的右值引用，而且移动的函数要把原来指针给指向null，好不让他销毁包含的元素**

##### 合成的移动操作

编译器这个好朋友，**在某些条件下**，还是会给我们**合成移动操作的-移动构造函数和移动赋值运算符**。
这个**某些条件略苛刻**：
**只有当一个类没有定义任何自己版本的拷贝控制成员，且类的每个非static数据成员都可以移动时**（话说有不能移动的吗，有吧，**比如自定义的类**），**编译器才会为它合成移动构造函数或移动赋值运算符** 

举个栗子：

```c++
struct X
{
    int i; //内置类型可移动
    string s; //string定义了自己的移动操作
}
X x;
X x2 = std::move(x); //调用了合成的移动构造函数
```

接下来要记住一些东西：

1. **移动操作不会隐式定义为删除的函数**（你是要用它的） 
2. **如果我们用=default来要求编译器显式合成移动操作，但是呢，有些成员不能被移动，那编译器怎么办，只好把移动操作都定义为删除的，不让你用了。** 

下面还有六条，关于何时移动操作是删除的，的准则（这句话很拗口，但下面的准则很有逻辑性）：

1. 移动构造函数被定义为删除的条件是：  
   1. **有类成员定义了自己的拷贝构造函数但是没定义移动构造函数** 
   2. **有类成员未定义自己的拷贝构造函数且编译器不能为其合成移动构造函数** 
2. 有类的移动操作被定义为删除的或是private的，那移动操作就是删除的 
3. 类似拷贝构造函数，如果类的析构函数被定义为删除的或是private的，那类的移动构造函数被定义为删除的 
4. 类似拷贝赋值运算符，如果类成员有const或者引用，则类的移动赋值运算符被定义为删除的。 
5. 一个类定义了自己的移动操作，那合成的移动操作就会被定义为删除的 
6. 定义了移动操作的类必须也定义拷贝操作，不然，这些成员被合成为删除的

现在我们在赋值语句中要分清使用的是移动构造函数还是拷贝构造函数了，老规矩，谁更匹配谁上：

```c++
StrVec v1, v2;
v1 = v2; //拷贝赋值
StrVec getVec(istream &); //函数声明，返回非引用，即返回右值
v2 = getVec(cin); //移动赋值
```

这是在StrVec类中**移动操作和拷贝操作都有的情况下**，如果是下面这种情况呢：

```c++
class Foo
{
public:
    Foo() = default; //强行合成默认构造函数
    Foo(const Foo&); //自定义拷贝构造函数（函数声明）
    //所以说，Foo没有移动构造函数
};
Foo x;
Foo y(x); //直接初始化，调用拷贝构造函数，因为x是左值
Foo z(std::move(x)); //还是调用拷贝构造函数，虽然我们把x当右值用，但人家没有移动操作啊
```

概括就是：如果一个类有拷贝构造函数而没有移动构造函数，那其对象的移动是通过拷贝来完成的，这一点对于拷贝赋值运算符和移动赋值运算符也适用。

接下来看一个很神奇的现象：

##### 移动赋值运算符和拷贝赋值运算符可以是同一个函数

我们先抛代码：

```c++
class HasPtr
{
public:
    HasPtr(HasPtr &&p) noexcept : ps(p.ps), i(p.i) {p.ps = 0;} //移动构造函数

    //下面这个函数既是移动赋值运算符又是拷贝赋值运算符，为什么待会说
    HasPtr& operator=(HasPtr rhs)
    {
        swap(*this, rhs);
        return *this;
    }
};
```

我们来仔细看一下赋值运算符，它的参数是**值传递**的，就是说实参传过来的时候要拷贝给形参，那么来了，**拷贝初始化要么是调用拷贝构造函数要么是移动构造函数，这取决于实参的类型**，左值拷贝，右值移动，这样的话，这不就实现了单一的赋值运算符实现拷贝和移动赋值功能了嘛，来来来，举个例子：

```c++
HasPtr hp, hp2; //调用默认构造函数初始化
hp = hp2; //hp2作为变量是个左值，调用拷贝构造函数来赋值
hp = std::move(hp2); //强行右值，调用移动构造函数来移动hp2
```

在swap后，rhs中的指针指向原来的this指向的string，当rhs离开作用域后，这个string就被销毁了，perfect

#### 移动操作的好处-举个例子

我们以之前的邮件类为例：通过定义移动操作，**Message类可以使用string和set的移动操作来避免拷贝contents和folders成员的额外开销。** 

除了**移动folders成员**外，我们还要更新每个指向原Message的Folder-删除指向旧Message的指针，添加指向新Message的指针。 

因为移动操作都需要更新Folder指针，我们把它封装成函数以便复用：

```c++
void Message::move_Folders(Message *m)
{
    folders = std::move(m->folders); //使用set的移动赋值运算符，避免了不必要的拷贝
    for(auto f : folders)
    {
        f->remMsg(m); //从Folder中删除旧Message
        f->addMsg(this); //将本Message添加到Folder中
    }
    m->folders.clear(); //确保销毁m是无害的
}
```

接下来就可以很方便地自定义移动操作了：

```c++
//移动构造函数
Message::Message(Message &&m) : contents(std::move(m.content))
{
    move_Folders(&m); //移动folders并更新Folder指针
}

//移动赋值运算符
Message& Message::operator=(Message &&rhs)
{
    if(this != &rhs) //检查自赋值
    {
        remove_from_Folders(); //销毁this指向的旧状态
        contents = std::move(rhs.contents); //调用move将rhs的content移动到this对象
        move_Folders(&rhs); //更新Folders指向本Message
    }
    return *this;
}
```

##### 移动迭代器

我们通过标准库的make_move_iterator函数来将普通迭代器转换为移动迭代器：

```c++
void StrVEc::reallpcate()
{
    auto newcapacity = size() ? z*size() : 1;
    auto = first = alloc.allocate(newcapacity);
    auto last = uninitial_copy(make_move_iterator((begin()),
    make_move_iterator(end()), first);

    free(); //释放旧空间

    //更新指针
    elements = first;
    first_free = last;
    cap = elements + newcapacity;
}
```

uninitialized_copy对输入序列中每个元素调用construct来将元素“拷贝”到nudist位置，因为我们传给它的表示范围的迭代器是移动迭代器，那相应的解引用运算符生成的就是右值引用，所以construct会使用移动构造函数来构造元素。

 **不要随意使用移动操作，因为你不知道以后源对象是什么状态，而且你也不能再去对它做什么**，当然如果你够自信，都是自己良好定义的，那还是鼓励用的。

我们一般会同时定义拷贝和移动两个操作，这样好处大大滴，以push_back函数为例：

```c++
void push_back(const X&); //拷贝：可以绑定到任意类型的X（因为加了const）
void push_back(X&&); //移动：只能绑定到类型为X的可修改的右值
```

 **区分移动和拷贝的重载函数通常有两个版本：一个接受const T&，另一个接受T&&** 

再来个具体的例子：

```c++
class StrVec
{
public:
    void push_back(const string&); //拷贝元素
    void push_back(string&&); //移动元素
};

void StrVec::push_back(const string& s)
{
    chk_n_alloc(); //确保有新空间
    alloc.construct(first_free++, s); //插入s，并后移first_free
}
void StrVec::push_back(string&& s)
{
    chk_n_alloc(); //确保有新空间
    alloc.construct(first_free++, std::move(s)); //插入s，并后移first_free
}
```

写完后我们来调用试试：

```c++
StrVec vec; 
string s = "some thing";
vec.push_back(s); //调用push_back(const string&），是拷贝的，s可能还要用的
vec.push_back("done"); //调用push_back(string&&)，是移动的，"done"就不用了
```

**拷贝控制左值引用，移动控制右值引用就完事了**

##### 来看点关于左值和右值神奇的东西

我们来看点看上去非法的，但是编译器不会报错的：

```c++
string s1 = "a", s2 = "b";
auto n = (s1+s2).find('a'); //我们居然在一个右值上调用函数，右值代表值啊，
//然而这是可以的。。。主要是因为新旧标准问题
s1 + s2 = "fff"; //这也行的。。。
```

在新标准中，向右值赋值还是算合法的。。。
我们虽然不能去动标准库（它是允许向右值赋值的），但是我们可以在自己的地盘自己定义的类中阻止这种不合理的方式-**我们希望强制左侧运算对象（this）是一个左值。** 

那我们具体该怎么做呢？我们可以在**赋值运算符参数列表**后面加一个**引用限定符**，&表示this指向左值，&&表示右值；而且，引用限定符只能用于（非static）成员函数，且必须同时出现在函数的声明和定义中：

<font color=red>**列表后的const或者&，&&都是修饰this的**</font>

```c++
class Foo
{
public:
    Foo &operator=(const Foo&) &; //拷贝赋值运算符，且只能给左值赋值
};
Foo &Foo::operator(const Foo &rhs) & //声明和定义中都要有引用限定符
{
    return *this;
}
```

好的，我们来用下试试：

```c++
Foo &retFoo(); //函数声明，返回引用，是左值
Foo retVal(); //返回右值
Foo i, j;
i = j; //正确：i是左值
retFoo() = j; //正确：retFoo返回左值
retVal() = j; //错误
i = retVal(); //正确
```

记不记得我们还可以在参数列表后面放const，表示该函数不能修改类中的成员变量，我们得把&放在const之后：

```c++
Foo anothr() const &{};
```

##### 重载

我们直接抛代码：

```c++
class Foo
{
public:
    Foo sorted() &&; //用于可改变的右值
    Foo sorted() const &; //可用于任意类型的Foo
private:
    vector<int> data;
}
Foo Foo::sorted() &&
{
    sort(data.begin(), data.end());
    return *this;
}
Foo Foo::sorted() const &
{
    Foo ret(*this); //我们无法改变this，所以要拷贝个副本来排序返回
    sort(ret.data.begin(), ret.data.end());
    return ret
}
```

编译器会通过函数匹配来确定调用哪个：

```c++
retVal().sorted(); //retval()返回右值，调用Foo::sorted() &&
retFoo().sorted(); //调用左值那个
```

注意，引用限定符是这样的，重载函数，要不你都加，要不就都不加，非常讲义气。