# C++ primer section7

## 类



#### 定义类

如下代码，我还加了一些使用的函数定义：

```c++
struct Sales_data
{
    //数据成员
    string bookNo; //书号
    unsigned units_sold = 0; //售出册数
    double revenue = 0; //总销售收入

    //成员函数
    string isbn() const //返回书本isbn号，这里的const待会解释
    {
        return bookNo;
    }
    Sales_data& combine(const Sales_data&); //函数声明
    double avg_price() const; //返回售出书籍的均价，这里的const也待会解释
}
```

##### 定义成员函数

我们来看一下isbn函数，你别看这个函数简单，细细推导的话，我们可以从这个函数中学到很多东西。 我们先来看一下我们是怎么调用这个函数的：

```c++
Sales_data current;
current.isbn()
```

我们是用点运算符来访问current对象的isbn成员来调用的，想想是谁在调用成员函数呢？是这个类的对象在调用，所以isbn函数返回的bookNo是current这个对象的bookNo，所以实际的过程是这样的：

```c++
Sales_data::isbn(&current); //调用的时候传入了current的地址
```

而在类内部，任何对类成员的直接访问都被看作**this的隐式引用**，所以，我们也可以把isbn函数写成：

```c++
string isbn() const
{
    return this->bookNo;
}
```

那我们来调用试试：

```c++
const Sales_data test;
test.isbn(); //这里会报错
```

为什么会报错呢？我们来分析下，上面这句调用的话用this写出来的话等价于：

```c++
Sales_data::isbn(&test);
//在类内部执行的时候是这样的
string isbn() 
{
    return this->bookNo;
}
```

**也就是说，类在默认情况下对this指针的隐式引用的this，都是一个const指针，它本身是const但是他指向的对象是不是他不知道，所以这种情况下就产生了矛盾**

问题就出在**this指针**上，因为this是**指针常量**，**它只是本身值不变**，但它**可以改变它所指对象的值（它是顶层const）**，而它所指向的test对象是const的，所以引发了矛盾。 也就是说，我们如果不加const的话，这个函数只能被非const对象调用。 现在，我们知道了const的作用了：**修改隐式this指针的类型**，**使它成为顶层和底层都是const的类型。 本质上isbn的函数是这样的**：

```c++
string isbn(const Sales_data *const this)
{
    return this->bookNo;
}
```

怎么样，还是有点复杂吧，然而你知其所以然之后，你就可以一直写成最简单的形式了啊：

```c++
string isbn() const //返回书本isbn号，这里的const待会解释
{
    return bookNo;
}
```

C++官方说法：**this是隐式地，没办法直接将它声明成指向常量的指针**，于是设计者们就想出了这样的办法：**把const放在参数列表之后表示this是一个指向常量的指针（它本身也是个常量哦）**。 像这样使用const的成员函数被称作**常量成员函数**。

**终于知道这个参数列表后的const到底是为了什么的了**

**也就是说这个const是为了对付常量对象的**

```
graph LR
常量成员函数-->常量和非常量对象都可调用
graph LR
普通成员函数-->非常量对象才能调用
```

### 编译器对类的处理顺序

编译器分两步处理类：

1. **首先编译成员变量的声明**
2. **再编译成员函数 所以啊，成员函数可以随意使用类中的其他成员变量不用管它们出现的次序。**

**类的成员变量写在哪里都可以**

##### 在类的外面也可以定义其成员函数

我们来定义一下类中声明的avg_price函数，只要在函数名前面加一个作用域说明符::就行了：

```c++
double Sales_data::avg_price() const
{
    if(units_sold>0) //有卖出书的话
    {
        return revenue/units_sold;
    }
    else
    {
        return 0;
    }
}
```

编译器看到 avg_price 这个函数是在 Sales_data 中，就会把它当成在类内部了。

下面我们来看一下combine函数的定义：

```c++
Sales_data& Sales_data::combine(const Sales_data &rhs)
{
    units_sold += rhs.units_sold;
    revenue += rhs.revenue;
    return *this; //返回调用该函数的对象
}
```

上面的&都是引用，不是取地址啊，不要连这个都搞错了。 这个函数值得介绍的是返回值，为什么要这样返回呢？为了可以这样调用： a.combine(b).combine(c) 至少我觉得是这样。

**在上一章讨论过，函数的返回值是引用的话，前提是非常量引用，是可以作为左值被修改的，而引用返回作为左值更是可以调用**

##### 定义read和print函数

我们来定义一下前面用过的这两个函数：

```c++
//输入的信息包括ISBN、售出总数和售出价格
istream &read(istream &is, Sales_data &item)
{
    double price = 0;
    is >> item.bookNo >> item.units_sold >> price;
    item.revenue = price * item.units_sold;
    return is;
}
//print自己写
```

**这个函数就解释一个地方，返回类型是istream，因为IO类不能被拷贝。**

**对于不能拷贝的流或者io，都推荐使用将其引用作为形参或者返回引用来解决问题**

## 构造函数

**每个类都分别定义了其对象初始化的方式，就是通过构造函数来控制其对象的初始化过程；构造函数的任务是：初始化对象的数据成员，只要类的对象被创建，就会执行构造函数。** 

构造函数非常复杂，这一节我们只介绍些基础的。
 **构造函数的名字和类名相同且没有返回类型，可以重载。** 
 **当我们创建类的一个const对象时，直到构造函数完成初始化过程对象才算是常量，所以构造函数在const对象的构造过程中可以向其写值。也就是即便是创建const对象，构造函数也是生成他的一个过程**

读到这不知道你有没有疑问，我们前面的那个Sales_data类并没有搞这个什么构造函数，不是说也可以完成[项目]()拿到2000块吗？比如我们写过这样的代码：

```c++
Sales_data current;
```

那这个current是怎么初始化的呢？
这是我们编译器的功劳：**如果我们的类没有显式地定义构造函数，编译器就会为我们隐式地定义一个默认构造函数**，是不是很6，那这个默认构造函数是怎么初始化数据成员呢？

- **如果存在类内的初始值，用它来初始化成员**

  ```c++
  unsigned units_sold = 0; //售出册数
  double revenue = 0; //总销售收入
  ```

- 否则，**默认初始化该成员：bookNo为空字符串** 

最好是我们不用编译器默认生成构造函数，原因如下：

- 因为我们自己才知道我们想把数据成员初始化成什么， 
- **有些成员比如指针默认初始化，值未定义会出问题，很重要**， 
- **还有就是类包含类，被包含的那个没有类没有默认构造函数，那就报错了** 
- **编译器只有在我们没有定义构造函数时，才会生成默认构造函数**，也就是说，如果我们定义了一个构造函数，且它不是默认构造函数，那我们就没有默认构造函数可以使用了。

我们来改造一下Sales_data类，给它搞几个构造函数：

```c++
struct Sales_data
{
    Sales_data() = default;

    Sales_data(const string &s) : bookNo(s) {}

    Sales_data(const string &s, unsigned n, double p) :
            (bookNo(s), units_sold(n), revenue(p*n)) {};

    Sales_data(istream &);
}
```

是不是很难懂啊，因为还有些知识没介绍，一个个来

##### = default

 **不接受任何实参的构造函数是默认构造函数**，所以第一个是默认构造函数，= default的意思是，我们要求编译器生成构造函数。= default在类内部是内联，在类外部不是内联。

```c++
Sales_data(const string &s) : bookNo(s) {}
```

开始的s是传入参数，s用来初始化成员变量bookNo，这就是个简便写法，看得懂就行，等价于：

```c++
Sales_data(const string &s)
{
     bookNo = s;
}
```

#### 在类的外部定义构造函数

我们来在类外面实现一下最后一个构造函数：

```c++
Sales_data::Sales_data(istream &is)
{
    read(is, *this); //调用原来的read函数从is中读取一条交易信息存入this所指的对象中
}
```

我们除了要初始化对象之外，**还要控制拷贝、赋值和销毁对象时发生的行为**，怎么样，再一次体会到了C++赋予程序员的自由以及责任吧，不过别紧张，现在暂时还不学，暂时都用编译器合成的。

### 访问控制与封装

这个太简单，我就不啰嗦了

```c++
class Sales_data
{
private:
    //数据成员
    string bookNo; //书号
    unsigned units_sold = 0; //售出册数
    double revenue = 0; //总销售收入

public:    
    //成员函数
    string isbn() const //返回书本isbn号，这里的const待会解释
    {
        return bookNo;
    }
    Sales_data& combine(const Sales_data&); //函数声明
    double avg_price() const; //返回售出书籍的均价，这里的const也待会解释
};
```

**private后面的成员只能被类的成员函数访问，不能被使用该类的代码访问**

```c++
graph LR
struct-->默认定义在第一个访问说明符之前的是public
graph LR
Aclass-->默认定义在第一个访问说明符之前的是privat,这个很重要
```

**struct和class其他没有任何差别**

那么问题来了，**Sales_data类的数据成员都是private的，那我们的类外面的函数read等函数就无法编译了，因为它们无法访问成员变量**。
 **解决方法就是让函数跟类做朋友，友元函数登场了**：

```c++
struct Sales_data
{
    friend istream &read(istream&, Sales_data&);

private:
    //数据成员
    string bookNo; //书号
    unsigned units_sold = 0; //售出册数
    double revenue = 0; //总销售收入

public:
    //成员函数
    string isbn() const //返回书本isbn号，这里的const待会解释
    {
        return bookNo;
    }
    Sales_data& combine(const Sales_data&); //函数声明
    double avg_price() const; //返回售出书籍的均价，这里的const也待会解释
};
istream &read(istream&, Sales_data&) //声明在类外且无作用域符号，为非成员函数
{
    double price = 0;
    is >> item.bookNo >> item.units_sold >> price;
    item.revenue = price * item.units_sold;
    return is;
}
```

**这里read函数的位置很暧昧，首先它是在类里面声明的**，**并且C++规定友元只能在类内部声明**，**但是它不能被public等访问控制符修饰**，因为它不是类的成员函数，这样之后，**我们在类外面声明定义它**。

<font color=red>**友元函数只能在类的内部声明，但是他不能被public这种访问控制符修饰！！！！**</font>

<font color=red>**然后需要在类外面进行声明定义，定义的时候不用谢写friend，也不用写类名，因为他本来就不是类的成员函数**</font>

注意我这句话啊，**在外面声明定义它**，我们不是已经在类内部声明过了吗，外面只是定义而已啊，你是不是写错了？不是的，我故意这么写的，因为，**友元的声明不同于一般的函数声明，它只是用来指定访问权限**，**其实没有函数声明的作用**，所以，我们在类外部是要声明定义它的

**构造函数可以调用友元函数，构造函数的定义必须在友元函数的声明之后**

接下来就来介绍这些特性：**类型成员、类的成员的类内初始值、可变数据成员、内联成员函数、从成员函数返回*this、如何定义并使用类类型以及友元类的更多知识。**

#### 类成员再探

我们现在先来定义一个类，Screen屏幕类：

```c++
class Screen
{
public:
    typedef string::size_type pos; //类型别名，别忘了啊
    Screen() = default //强制生成默认构造函数

    //自定义构造函数，别忘了这个初始化方式
    Screen(pos ht, pos wd, char c): height(ht), width(wd), contents(ht*wd, c){}

    //定义在类内部的成员函数都是隐式的内联函数
    char get() const //别忘了这边的const是干啥的
    {
        return contents[cursor]; //读取光标中的字符
    }

    inline char get(pos ht, pos wd) const; //显式内联声明，重载哦
    Screen &move(pos r, pos c); //函数声明

private:
    pos cursor = 0;
    pos height = 0, width = 0;
    string contents;
};


inline //在类外面定义内联函数
Screen &Screen::move(pos r, pos c)
{
    pos row = r * width; //计算行的位置
    cursor = row + c; //列
    return *this; //返回左值对象
}
//重载get函数
char Screen::get(pos r, pos c) const
{
    pos row = r * width;
    return contents[row + c];
}
//其实在声明和定义处都写inline也可以，推荐在类外面写，更易理解
```

**定义在类内部的成员函数都是隐式的内联函数,以上也得出了一个结论，要想写非内联的成员函数，只能在类内声明在类外不用inline定义。**

##### 强行可变数据成员

这个应用很少，我觉得这个设定有点奇怪，理解倒是好理解的，你自己看代码吧：

```c++
class Screen
{
public:
    void some_member() const; //成员函数声明

private:
    mutable size_t access_ctr; //mutable即使在const对象内也能被修改
}

void Screen::some_member() const
{
    ++access_ctr; //你看，可以改变吧
};
```

**mutable没咋用过，可能是特定情境下有用**

##### 类数据成员的初始值

在定义好Screen类后，我们要来定义一个窗口[管理类]()Window_mgr，用它来表示显示器上的多个Screen（因为屏幕上可以出现多个窗口），这个类应该包含一个Screen的vector。在默认情况下，我们希望这个类有一个默认初始化的Screen，这时候就可以用到类内初始值了：

```c++
class Window_mgr
{
private:
    vector<Screen> screens{ Screen(24, 80, ' ') }
}
```

过程是这样的：我们用了**列表初始化**去初始screens这个Window_mgr类的类内成员变量，在列表初始化内容中，我们调用了Screen类的**构造函数去实例化一个匿名对象**。

我给Screen类添加一些函数以及调用它们的代码，你来判断有没有问题==

```c++
//定义，上面的寒素也都包括在里面，我不重复了
class Screen
{
public:
    Screen &set(char);
    const Screen &display();
};
inline Screen &Screen::set(char c)
{
    contents[cursor] = c;
    return *this;
}
const Screen &display()
{
    cout << this->contents << endl; 
    return *this;
}
//调用
Screen myScreen;
myScreen.move(4, 0).set('$'); //对吗，为什么
myScreen.display().set('*'); //对吗，为什么
```

**把一个返回的非const对象构造为const是可以的，但是构造后虽然也是左值但没法改变他啊，所以错了**

第一个调用对，第二个错，因为**第一个返回的是引用，是左值，是它的身份，所以可以连续调用**；第二个错是因为**display函数返回的是常量引用，无法通过常量引用去改变它的值**。 那我们应该怎么办呢？简单的方法是，你把display函数的const去掉就好了啊，但是我们觉得不妥，**因为display函数不改变对象内容，设置成const是一个很好的行为**，我们的解决办法是重载：

```c++
class Screen
{
public:
    Screen &display()
    {
        do_display();
        return *this;
    }
    const Screen &display()
    {
        do_display();
        return *this;
    }
private:
    void do_display() const
    {
        cout << this->contents << endl;
    }
};
```

我们把打印的任务交给了一个private函数，对外的接口是两个重载函数，试着来调用一下这样行不行：

```c++
Screen myScreen(2, 4);
const Screen blank();
myScreen.display().set('*'); //对
blank.display(); //对
blank.display(); //错
```

**第三行那个式子确实是达到目的了，后两个不确定啥意思**

#### 类类型

**每个类定义了唯一的类型。对于两个类来说，即使它们的成员完全一样，这两个类也是不同类型**，不能互相赋值（我的理解是，类的名字也算类的类型，所以它们不同），我们使用类类型就跟在、使用内置类型一样的定义，这也是C++赋予我们的方便：

```c++
int a;
Screen b;
```

##### 类的声明

```c++
class Screen;
```

**向程序中引入名字Screen并且指明它是一种类类型**，但是我们不知道它里面有什么。

对于一个类来说，**我们必须定义它之后再去创建它的对象**，不然编译器不知道给你这个对象分配多少内存。
得到一个结论，**一个类的成员类型不能是它自己，但是可以是它的指针或者引用**。

**类和其他类型一样，如果想在不同文件使用，得声明一下，然后再用**

#### 友元再探

 **这一段我就总结了一句话，都可以是朋友。** 

##### 类之间的友元

```c++
class Screen
{
    friend class Window_mgr;    
    //Window_mgr可以访问Screen所有成员
}
```

##### 令成员函数成为友元

这个之前引出友元概念的时候就介绍了：

```c++
class Screen
{
    friend void Window_mgr::clear();    
    //这里是跟人家的成员函数做朋友
}
```

**所以如果只有个别函数想用其他类的成员数据，就不用把人家一大家子都当朋友**

#### 函数重载和友元

**a和b是重载函数，a是Screen的朋友，不代表b也是，一句话，把重载函数看成不同的函数**

#### 友元函数和作用域

这个书上的讲法它拗口了，看了我半天，总结如下，必须在类外部声明友元函数之后，类才能去调用它：

```c++
struct X
{
    friend void f(){} //友元函数，定义在类内部
    X(){ f(); } //默认构造函数调用f
    //这种调用时错误的，因为f没有被声明

    //俩成员函数的声明
    void g();
    void h();
}

void X::g(){f();} //错误，f没有被声明
void f(); //好，现在声明了
void X::h() {f();} //这样就对了
```

**在类的外部之前声明一下，构造函数就可以调用友元函数**

**构造函数可以调用友元函数，构造函数的定义必须在友元函数的声明之后**

## 类的作用域

**每个类都有自己的作用域**。下面这句话有点拗口：**在类的作用域之外，普通的数据和函数成员只能由对象、引用或者指针使用成员访问运算符**.（这是个点）来访问：

```c++
//我们还是使用前面的屏幕类Screen为例
Screen::pos ht = 24, wd = 80;
Screen scr(ht, wd, ' '); //调用构造函数实例化一个对象scr
Screen *p = &scr; //p是指向对象的指针
char c = scr.get(); //通过scr对象，调用成员函数get去获取字符
c = p->get(); //通过指针调用get函数
```

我再来给Window_mgr窗口[管理类]()加个成员函数：

```c++
class Window_mgr
{
public:
    //向窗口添加一个Screen，返回它的编号
    ScreenIndex addScreen(const Screen&);
};
Window_mgr::ScreenIndex Window_mgr::addScreen(const Screen &s)
{
    screens.push_back(s);
    return screens.size()-1;
}
```

**这个函数名前面有域作用符，所以，后面的形参和函数体都在类内，可以直接使用screens等成员变量，而返回类型在函数名之前，不算在类内，它要访问在类内定义的ScreenIndex就必须加上域作用符。**

**这个作用域真的无语....只要函数名前面有了作用域，后面都不用再加了**

**但是因为返回对象在他前面，所以还得加**

##### 没事找事的C++

看看代码就知道我为啥这么说了：

```c++
int height = 1;
class Screen
{
public:
    typedef string::size_type pos;
    void f(pos height)
    {
        cursor = width * height; //这里的height是多少
    }
private:
    pos cursor = 0;
    pos height = 0, width = 0;
};
```

height是1，解释是这样的，height是形参，是可以从外面传进来的，**编译器在编译f函数时，会去找这个height，这时候它还不知道下面private里面有个height（当然我是这么解释的）**，然后它就用了上面那个height=1，我们能不能强行用类里面的height呢，当然可以：

```c++
void Screen::f(pos height)
{
    cursor = width * this->height;
    //或者
    cursor = width * Screen::height;
}
```

当然，我们并不建议这么写，不过你得懂，这就是C++烦人的地方。

那要是我就想用外面的那个全局变量妖艳***呢？也行。。。

```c++
void Screen::f(pos height)
{
    cursor = width * ::height; //用的是外面那个全局的
}
```

 **当然，我的建议就是，那么多名字，取个不一样的就好了。**

**这里真的很烦人，刚说过类的数据成员会提前声明，所以写到成员函数的哪里都ok，这里还去找全局变量去了.....只能说尽量别这么写吧**

## 构造函数再探

**构造函数的初始值有时候必不可少**，什么时候必不可少呢？当**成员变量是const或者引用的时候**，原因么，你懂的：

```c++
class ConstRef
{
public:
    ConstRef(int ii);
private:
    int i;
    const int ci;
    int &ri;
};
```

这样去构造一个类的时候很容易犯错，比如你写这样一个构造函数：

```c++
ConstRef::ConstRef(int ii)
{
    i = ii; //正确
    ci = ii; //错误：不能给const赋值
    ri = i; //错误：引用没有初始化
}
```

所以说啊：**我们初始化const或者引用类型的数据成员的唯一机会就是通过构造函数初始值**，于是我们可以这么写：

```c++
ConstRef::ConstRef(int ii) : i(ii), ci(ii), ri(i){}
```

书上有一句话可以称为金玉良言：***如果成员是const、引用或者属于某种未提供默认构造函数的类类型，我们必须通过构造函数初始值列表为这些成员提供初值**。*

**所以尽量能不赋值就不赋值，初始化列表不香吗**

**成员的初始化顺序与它们在类定义中的出现顺序一致，而与初始化列表无关**，这句话经常考，什么意思呢，看例子：

```c++
class X
{
    int i;
    int j;
public:
    X(int val) : j(val), i(j){}
};
```

这样是错的，对吧？**因为i和j的初始化顺序是按照定义来的**，**先有i后有j**，所以，你怎么能用j去初始化i呢？
**最好的方式是用构造函数传进来的参数去初始化成员，这样我们就不用考虑初始化顺序了**：

```c++
X(int val) : i(val), j(val){}
```

**但是也能看出声明的顺序和构造函数的初始化顺序尽量保持一致**

##### 默认实参和构造函数

通常，我们说默认构造函数是没有参数的，但我们也可以这么干：

```c++
class Sales_data
{
public:
    Sales_data(string s = "") : bookNo(s){}
}
```

你可能觉得这个函数是一个普通的构造函数，但是，**其实它是这个类的默认构造函数**，**它用到了默认实参**，这么写的好处是，你**可以不传参数，也能调用**（不就相当于默认构造函数了吗），你不传参数的时候，它就用空字符串s去初始化bookNo，**传的话就按照传的来，一式两用**，还是很不错的。

#### 委托构造函数

这个偷懒方法也是丧心病狂，我给个例子吧，就以Sales_data类为例：

```c++
class Sales_data
{
public:
    //这是函数一，是一个普通的构造函数
    Sales_data(string s, unsigned cnt, double price) : bookNo(s), units_sold(cnt), revenue(cnt*price){}

    //接下来就是各种偷懒方法了，注意看
    Sales_data(): Sales_data("", 0, 0){} //函数二是默认构造函数，委托函数一帮忙初始化，也可以认为是调用了函数一
    Sales_data(string s): Sales_data(s, 0, 0){} //函数三接受一个string参数，委托函数一帮忙初始化
    Sales_data(istream &is): Sales_data()
    {
        read(is, *this);
    }
    //函数四复杂些，它先委托函数二，就是默认构造函数，函数二去委托函数一，这些函数执行完成后，再执行函数四的函数体
    //调用read函数读取给定的istream
};
```

**感觉委托构造函数就是调用其他的构造函数....**

#### 默认构造函数的作用

记住一条好习惯：如果定义了其他的构造函数，**最好也提供一个默认构造函数**，下面来两个没有默认构造函数的经典错误：

```c++
class NoDefault
{
public:
    NoDefault(const string &);
    //定义了构造函数，但没有默认构造函数，而且编译器不会生成，这样这个类本身不会有问题，但是用起来很容易出问题
};

struct A
{
    NoDefault a;
};
A a; //你这样看着挺正常，A没有写构造函数，于是编译器会自动生成默认构造函数去初始化成员变量，可惜啊，这个成员变量的
//类型是类类型NoDefault，而这个该死的类类型又没有默认构造函数，还不让编译器自己生成，于是就报错了

//还有一种错误是这样的：
struct B
{
    B(){}
    NoDefault b;
    //b作为类类型成员，在构造函数没有被初始化，于是它会调用默认构造函数初始化，结果该死的NoDefault没有默认构造函数
};
```

**所以不管怎样，给我加上该死的默认构造函数**

### 隐式的类类型转换

如果**构造函数只接受一个实参**，那它实际上定义了一种**隐式转换机制**，什么意思呢，以Sales_data为例，该类有一个只接受一个string的构造函数，所以啊，我们可以把string转换为Sales_data类的对象：

```c++
Sales_data b;
string a = "1";
b.combine(a); //这里a被转换为Sales_data对象，编译器创建了一个临时对象
```

这种只带一个参数的构造函数，我们也把它称为**转换构造函数**。

但是我们只允许一步类类型转换，举个例子：

```c++
item.combine("1");
//这样是错的，因为字符串常量到string是一步，string到类对象时一步，两步不行的
//不过我们可以用显式地写
item.combine(string("1"));
item.combine(Sales_data("1"))
//总之，隐式的只能帮你一步
```

**隐式转换只能转换一次，不能多步，而且只接受一个实参的构造函数，其实这样的用法很不清晰，但很容易考**

### 抑制在构造函数中的隐式转换

这个又是C++牛逼也蛋疼的地方，提出了类类型隐式转换，我又可以阻止它，不得不说给了程序员极大的权力，也是需要极大的责任心的，在转换构造函数（只接受一个实参的构造函数）前**加explicit关键字就可以阻止隐式转换，注意，explicit关键字只对类内的转换构造函数有用，而且被声明为explicit的构造函数只能用于直接初始化，不能拷贝初始化：** 

```c++
class Sales_data
{
public:
    Sales_data() = default;
    explicit Sales_data(const string &s): bookNo(s){}
    explicit Sales_data(istream&);
};

Sales_data item;
string b = "1"
item.combine(b); //这样就不行了
explicit Sales_data::Sales_data(istream& is){} //这样也不行，在外面了
Sales_data item1(b); //这样可以
Sales_data item2 = item1; //不行，explicit声明的不能拷贝初始化

//但是，多事C++又说我们还是可以强行转换：
item.combine(Sales_data(b)); //可以
item.combine(static_cats<Sales_data>(cin)); //可以
```

**这很恶心，但也是C++自由度大的地方，explicit可以规避很多不好的操作，这样就不会出现隐式转换了，当然显式的转换当然是可以的。**

**但同时explicit声明的构造函数不能拷贝初始化就很蛋疼，那也就是只能直接初始化，把拷贝初始化理解为赋值初始化就好，是模仿一个现有的对象为壳子来初始化，这个壳子可以是现有的，也可以是临时构造的**

#### 聚合类

聚合类是一个概念，使得用户可以直接访问其成员，它有特殊的初始化语法，那么什么样的类是聚合类呢？满足下面四个条件：

1. **所有成员都是public** 

2. **没有定义任何构造函数** 

3. **没有类内初始值** 

4. **没有基类，也没有virtual函数**（以后介绍） 聚合类举例：

   ```c++
   struct Data
   {
    int val;
    string s;
   };
   //实例化
   Data a = {0, "a"}; //一定要按类内定义的顺序
   ```

这类挺鸡肋的，跟自定义的数据类型似的

#### 字面值常量类

之前我们学过，constexpr函数的参数和返回类型都得是字面值类型。这回说到了类，就有字面值常量类： 数据成员都是字面值类型的聚合类是字面值常量类
或者
满足以下条件：

1. **数据成员均为字面值类型** 
2. **类至少有一个constexpr构造函数** 
3. **有类内初始值的话，该值必须是常量表达式，即便该成员是类类型，这个类也要有自己的constexpr构造函数** 
4. **类必须使用析构函数的默认定义，该成员负责销毁对象** 

##### constexpr构造函数

**之前提过，构造函数不能是const的**，但是字面值常量类的构造函数可以是constexpr的，且必须至少有一个constexpr构造函数。 如果不是默认的构造函数，那么constexpr类的构造函数基本就是空函数体，因为以下两点：

1. **constexpr函数的要求是唯一可执行语句时return语句**（之前说过的规定，忘了自己回去翻） 
2. 构造函数不能包含返回语句 所以就只好函数体为空了。 **constexpr构造函数必须初始化所有数据成员，初始值或者使用constexpr构造函数或者是一条常量表达式。** 说了那么多，来举个字面值常量类的例子吧： 

```c++
class Debug
{
public:
    //带有一个默认实参，它也是默认构造函数哦
    constexpr Debug(bool b = true): hw(b), io(b), other(b){}

private:
    bool hw;
    bool io;
    bool other;
};
```

这个其实也用不上

## 类的静态成员

我们目前学到的类的内容都是**实例化后每个对象各自拥有的**，有时候，有时候，**类需要它的一些成员与类本身直接相关**。例如，**一个银行账户类可能需要一个数据成员来表示当前的基准利率**，我们希望利率与类关联，而不是和对象关联，更加重要的是，**一旦利率变化，我们希望所有的对象都能使用新值**

#### 声明静态成员

我们就来写个银行账户类：

```c++
class Account
{
public:
    void calculate()
    {
        amaunt += amount * intereatRate;
    }
    static double rate(){return interestRate;}
    static void rate(double);

private:
    string owner;
    double amount;
    static double interestRate;
    static double initRate();
};
```

**类的静态成员存在于对象之外**，所以啊，每个Account对象只有两个数据成员：owner和amount，而interestRate是类所有，**所有对象共享**。 **因为静态成员函数不与任何对象绑定**，所以**没有this指针**，**不能声明为const**的。

**类中的静态对象比较特殊，当然也很重要，首先要知道的是静态对象不属于对象属于类，所以更没有所谓的this指针，因为这个是指向对象的，所以更不能声明为const了**

#### 使用类的静态成员

使用类作用域运算符::直接访问：

```c++
double r;
r = Account::rate();
```

我们还是可以用对象去访问静态成员，毕竟共享

```c++
Account c1;
Account *ac2 = &ac1;
r = ac1.rate();
r = ac2->rate();
```

以上是在类外访问，如果在类内呢？你看上面的calculate函数就知道了，直接访问。

**所以不管是在类内还是类外，我们可以这么总结，类和对象都可以直接使用，大家共享的**

<font color=red>所以先知道了一点，成员函数是可以用静态成员的，而且大家一起用！</font>

#### 定义静态成员

突然想起个小问题：类成员函数的声明只能放在类内，对不？（对的，求讨论）
类内部定义前面的代码已经有了，就是加个关键字static；**在类外部定义静态成员时，不能重复static关键字，该关键字只能出现在类内部的声明语句**。

**因为类的静态数据成员不属于任何一个对象，所以它们不是在实例化时被定义的**，也就是说，**构造函数不负责初始化它们，而且一般来说，我们要在类的外部定义和初始化每一个静态成员**。

**类似全局变量，静态数据成员定义在任何函数之外，它一旦被定义，就一直存在于程序的整个生命周期中**。

我们来定义一个在类内已经声明的static成员：

```c++
double Account::interestRate = initRate();
//为什么不用在initRate函数前面加Account::呢
//因为从类名开始，剩下的部分都处于类的作用域之内了。
```

**作用域又来了，让人无语，一个句子出现了一个类型后面就在作用域内了，真的牛皮**

#### 静态成员的类内初始化

刚刚说过，类的静态成员应该在类外定义初始化，于是C++又来破坏自己定的规矩了：我们可以为静态成员提供const整数类型的类内初始值，不过要求静态成员必须是constexpr的：

```c++
class Account
{
public:
    static double rate(){return interestRate;}
    static void rate(double);

private:
    static constexpr int period = 30;
    double daily_tbl[period];
};
```

为什么这儿要把初始化放在类内呢？因为period的唯一用途就是去定义daily_tbl的维度，那我们就直接在类内定义一下，外面反正不用。**值得注意的是，在外面还是要定义一下，相当于声明一下**：

```c++
constexpr int Account::period; //不带初始值的定义，
```

**我的总结就是，什么狗屁东西，食之无味弃之可惜。真的恶心...**

#### 静态成员能用于某些场景，而普通成员不能

```c++
class Bar
{
    private:
        static Bar m1; //这个逆天吧，可以自己的类型
        statuc int &m2; //逆天吧，未初始化的引用
};
```

主要是因为，**静态成员一般在外面定义初始化，所以，在类内可以是不完全类型，胡作非为**。

还有一个区别是，**静态成员可以作为默认实参**，666

```c++
class Screen
{
public:
    Screen& clear(char = bkground); //666
private:
    static const char bkground; 
};
```

**非静态数据成员不能作为默认实参，因为它是属于对象的，你用对象的值去作为默认实参，结果是无法真正提供一个对象来获取成员的值，会引发错误。**

**静态成员函数没有this指针，它不能返回非静态成员，因为除了对象会调用它外，类本身也可以调用。**

  **静态成员函数可以直接访问该类的静态成员变量和静态成员函数，而不能直接访问类的非静态成员变量，如果一定要访问非静态成员变量则必须通过参数传递的方式得到一个对象名，然后通过对象名来访问。**

<font color=red>所以又知道了，因为没了this指针，静态函数没法默认获得对象的指针了，所以也无法调用非静态对象了，除了人工给赋予一个对象的形参，但是可以使用静态的</font>