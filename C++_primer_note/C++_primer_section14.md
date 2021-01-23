# C++ primer section14

## 重载运算与类类型转换

这一章其实很简单，目的只有一个，让我们**在使用自定义的类像内置类型一样自然，而且运算符的操作可以自己定义（通过重载）** 

### 基本概念

- 重载运算符就是函数名由**关键字operator**和其后要定义的运算符号共同组成 
- 重载运算符**函数**的**参数数量**与该运算符**作用的运算对象数量一样**，例如对于**+**，**左侧运算对象传递给第一个参数**，**右侧运算对象传递给第二个参数**。**当它是成员函数时，this绑定到左侧运算对象，成员运算符函数的参数数量比运算对象的数量少一个。** 
- 除了**重载的函数调用运算符operator()之**外，**其他重载运算符不能有默认实参（其实主要还是要保持跟内置类型一致）** 

上面规矩挺多，其实都是为了让类类型与内置类型一致。

对于一个运算符函数来说，它或者是类的成员，或者至少含有一个类类型的参数：

```c++
int operator+(int, int); //不行，不能给内置类型重定义
```

我们可以**重载大部分运算符**，但**只能重载已有的运算符**，而且优先级也是跟原来的运算符一样，算是戴着镣铐的舞蹈吧，讲道理还是为了跟内置类型保持一致

#### 调用重载的运算符函数的方式

有两种，**一种是普通的函数调用**，**一种是运算符调用**，例如我重载了+，就是说我有一个operator+函数，那我有两种方式调用：

```c++
data1 + data2; //推荐这种
operator+(data1, data2);	
```

#### 某些运算符不应该被重载

有些运算对象的求值顺序规则无法保留，所以我们最好还是不重载它们，**主要有&&, ||, 还有逗号运算符**

我们也不重载**逗号运算符和取地址运算符**：因为C++已经定义了这两种运算符用于类类型的含义，相当于C++已经帮我们重载好了，我们就不要去添乱了。

#### 选择作为成员函数还是非成员函数

**大部分都最好作为成员函数**，除了下面这一条：
具有对称性的运算符可能转换任意一端的运算对象，例如算符、相等性、关系和位运算符等。

我们刚说了，大部分最好作为类的成员函数**，那么它的左侧运算对象必须是运算符所属类的一个对象**：

```c++
string s = "hey";
string a = s + "a"; //对
string b = "a" + s; //也对，因为string把+定义为非成员函数，至少一个是string类类型也满足
string c = "x" + "y"; //错，两个对象都不是string类类型
```

**所以这里理解为什么+这个必须得有一个是string了吧**

## 接下来的部分我们就分别来重载各个运算符

### 输入输出运算符

我们来为老朋友Sales_data写个输出运算符：

```c++
ostream &operator<<(ostream &os, const Sales_data &item)
{
    os << item.isbn() << " " << item.units_sold << " " << item.revenue << " " ;
    return os;
}
```

该函数返回一个**ostream的引用**，比较疑惑的可能是第一个参数，**它是一个非const的引用，非const是因为向流写入内容会改变其状态**；**引用是因为我们无法拷贝ostream对象啊**。

#### 输入输出运算符必须是非成员函数

为什么这么强行规定呢，我们来看看在实际调用的时候我们是怎么用的：

```c++
cout << a ; //a是一个Sales_data对象
```

我们假设**<<是成员函数**，它是**二元运算符**，所以**它的左侧对象必须是类类型**，就是说cout得是Sales_data类，显然不对，所以只能把<<声明为非成员函数，**然而它们这些输入输出的基本都是要访问private的数据成员的，于是就把它们声明为友元函数了。**

#### 重载输入运算符

我们还是帮Sales_data写：

```c++
istream &operator>>(istream &is, Sales_data &item)
{
    double price;
    is >> item.bookNo >> item.units_sold >> price;
    if(is) //检查是否输入成功
    {
        item.revenue = item.units_sold * price;
    }
    else
    {
        item = Sales_data(); //输入失败，调用默认构造函数初始化
    }
    return is;
}
```

### 算术和关系运算符

通常，我们把算术和关系运算符定义为**非成员函数**以**允许对左侧或右侧的运算对象进行转换**；我们还会把**形参定义为常量引用**，因为这些运算符不会改变运算对象的状态：

```c++
Sales_data operator+(const Sales_data &lhs, const Sales_data &rhs)
{
    Sales_data sum = lhs; //因为我不去修改lhs和rhs，所以要搞个sum变量出来
    sum += rhs; //这里调用的+=也是重载的运算符，后面会定义它的
    return sum;
}
```

#### 相等运算符

```c++
bool operator==(const Sales_data &lhs, const Sales_data &rhs)
{
    return lhs.isbn() == rhs.isbn() &&
           lhs.units_sold() == rhs.units_sold &&
           lhs.revenue == rhs.revenue;
}

bool operator!=(const Sales_data &lhs, const Sales_data &rhs)
{
    return !(lhs == rhs);
}
```

#### 关系运算符

通常**关联容器**和一些[算法]()（**例如[排序**]()）会用到**小于运算符**，所以定义operator  <会比较有用，如果类同时定义了==运算符，则要与它保持一致，例如两个对象是!=的，那么一个对象应该<另一个。< span>
我们的老朋友Sales_data没有必要定义<，你可以考虑下为啥。

#### 赋值运算符

我们已经学了**拷贝赋值和移动赋值**了，它们是**把类的一个对象赋值给该类的另一个对象**，其实啊，我们还可以让等号右边不是该类对象，比如：

```c++
vector<string> v;
v = {"a", "b"};
```

我们就来重载这个，把这种赋值方式带到我们的StrVec类中：

```c++
class StrVec
{
public:
    StrVec &operator=(std::initializer_list<string> il)
    {
        auto data = lloc_n_copy(il.begin(), il.end()); //分配刚好的内存并拷贝
        free(); //释放this原有的空间
        elements = data.first; //头指针
        first_free = cap = data.second; //尾指针
        return *this;
    }
};
```

##### 复合赋值运算符

```c++
Sales_data& Sales_data::operator+=(const Sales_data &rhs)
{
    units_sold += rhs.units_sold;
    revenue += rhs.revenue;
    return *this;
}
```

**有些运算符的重载只有一个参数，就是右边的参数，因为返回的值就是加到左侧的参数上，左侧就是this，那哪里才能会默认this呢，肯定是在类里面，也就是成员函数啊**

#### 下标运算符[]

表示容器的类通常可以通过元素在容器中的位置来访问元素，这些类都会定义**下标运算符operator[]**

我们最好**同时定义下标运算符的常量版本和非常量版本**，这样的话，当作用于一个常量对象时，下标运算符返回常量引用以保证我们无法修改它：

```c++
class StrVec
{
public:
    string& operator[](size_t n)
    {
        return elements[n];
    }
    const string& operator[](size_t n) const //别忘了第二个const干嘛的
    {
        return elements[n];
    }

private:
    string *elements; //指向首元素的指针
};
```

我们来用用：

```c++
StrVec svec = {"a"};
const StrVec cvec = svec;
if(svec.size())
{
    svec[0] = "x"; //可以
    cvec[0] = "y"; //不行
}
```

#### 递增和递减运算符

迭代器中我们经常通过这两个运算符来**移动迭代器**，因为它们很可能改变所操作对象的状态，所以我们一般将其设定为**成员函数**。
递增递减有前置和后置版本，我们先介绍前置

##### 定义前置

我们用以前的一个类StrBlobStr：

```c++
class StrBlobPtr
{
public:
    StrBlobPtr& operator++(); //前置
    StrBlobPtr& operator--();
};
//check函数的第一个参数小于vector大小，则正常返回；到达尾部就抛出异常
StrBlobPtr& StrBlobPtr::operator++()
{
    check(curr, "已经到尾了"); //如果已经到了尾后位置，就无法递增
    ++curr; //后移
    return *this;
}
StrBlobPtr& StrBlobPtr::operator--()
{
    --curr;
    check(curr, "首元素无法递减"); //curr是0的话，--会产生一个很大的值size_t，肯定大于vector
    return *this;
}
```

##### 区分前后置运算符

我们为了实现重载，**在后置函数中强行加上一个没用的形参**：

```c++
class StrBlobPtr
{
public:
    StrBlobPtr operator++(int); //后置
    StrBlobPtr operator--(int);
};
```

对于后置版本，我们要记录对象的状态，因为它还有用呢，所以我们决定返回的是原值，而不是递增递减后的值（为了与内置版本一致，我们返回值而不是引用）：

```c++
//后置
StrBlobPtr StrBlobPtr::operator++(int)
{
    StrBlobPtr ret = *this; //记录原值
    ++*this; //调用了前置版本，已经包含了错误检测
    return ret;
} 
StrBlobPtr StrBlobPtr::operator--(int)
{
    StrBlobPtr ret = *this; //记录原值
    --*this;
    return ret;
}
```

##### 调用前置和后置运算符

```c++
StrBlobPtr a1 = {"a", "b"};
StrBlobPtr p(a1);
p.operator++(0); //调用后置
p.operator++(); //调用前置
```

### 成员访问运算符

在迭代器类和智能指针类常用到解引用*和箭头->，我们来为StrBlobPtr类添加：

```c++
class StrBlob
{
public:
    string operator*() const //与乘法是重载关系，参数不同
    {
        auto p = check(curr, "超范围了");
        return (*p)[curr];
    }
    string* operator->() const
    {
        return & this->opertaor*(); //调用解引用*来实现 
    }
};
```

我们来使用下，就会发现毫无违和感了：

```c++
StrBlob a1 = {"a", "b", "c"};
StrBlob p(a1);
*p = "xyz";
cout << p->size() << endl; //3
```

### 函数调用运算符

我们来做个很有意思的事情：

```c++
struct absInt
{
    int operator()(int val) const
    {
        return val < 0 ? -val : val;
    }
};
```

我们定义了一个类，里面**重载了一个运算符()，用来返回绝对值**，我们来调用试试：

```c++
int i = -24;
absInt a;
int ui = a(i); //调用了operator()
```

看着像不像我们调用了一个函数
如果**类定义了调用运算符()，那么该类的对象叫作函数对象**，因为可以调用这种对象，所以我们说这些对象的行为像函数一样。

我们来个复杂点的：

**函数对象的概念很有意思**

#### 带状态的函数对象类

我们要写个打印string的类，默认情况下，我们的类会把内容写入cout中，每个string用空格隔开，也允许用户提供其他可写入的流及分隔符：

```c++
class PrintString
{
public:
    PrintString(ostream &o = cout, char c = ' ') : os(o), sep(c) {}
    void operator() (const string &s) const {os << s << sep;}

private:
    ostream &os; //用于写入的目的流
    char sep; //分隔字符
};
```

上面这个类不难看懂吧，虽然涉及蛮多知识的，但是我们都介绍过了，下面我们来调用试试：

```c++
string s = "hehe";
PrintString printer;
printer(s); //打印hehe，后面跟一空格
PrinterString errors(cerr, '\n');
errors(s); //在cerr中打印s，后面跟换行
```

函数对象还可以作为泛型[算法]()的实参：

```c++
for_each(vs.begin(), vs.end(), PrintString(cerr, '\n'));
```

**第三个参数是PrintString的一个临时对象（看着是不是很像lambda啊，不过它没有捕获什么）**

#### lambda与函数对象的关系

我们来回忆下之前的**lambda表达式**，根据单词的长度进行[排序]()，同样长度的根据字典序排列：

```c++
stable_sort(words.begin(), words.end(), 
        [](const string &a, const string &b {return a.size() < b.size();})
```

那么归根结底，编译器是怎么处理神奇的lambda的呢？其实**直接把lambda当成一个未命名类的未命名对象（这个类的对象还是个函数对象）**

所以，我们可以用一个函数对象去替代上面的lambda（可以回过头想想lambda关于返回类型的规定，与这无关，就是插一句，因为我忘了。。。回过头去看的）：

```c++
class ShorterString
{
public:
    bool operator()(const string &s1, const string &s2) const 
    {
        return s1.size()<s2.size();
    }
};
```

有了这个类后，我们就可以这样写：

```c++
stable_sort(words.begin(), words.end(), ShorterString())
```

第三个参数是**新构建的ShorterString临时对象**。

刚刚我们是用函数对象去替代了一个没有捕获的lambda，接下来我们要来个有捕获列表的lambda，用类去替换：

```c++
//获得第一个size大于给定sz的元素的迭代器
auto wc = find_if(words.begin(), words.end(),
            [sz](const string &a){return a.size() >= sz;}
```

那我们相应的类是这样：

```c++
class SizeComp
{
public:
    SizeComp(size_t n) : sz(n){} //要写个构造函数，用来“捕获变量”
    bool operator()(const string &s) const {return  s.size()>=sz;}
private:
    size_t sz;
};
```

#### 标准库定义的一些函数对象

我在这就用代码举些例子，要用的时候再查好了，基本都定义在头文件functional中：

```c++
plus<int> intAdd;
int sum = intAdd(10, 20); //sum = 30
vector<string> svec = {"f", "z"};
sort(svec.begin(), svec.end(), greater<string>()); //降序排列第三个参数是未命名的函数对象
```

#### 可调用对象与function

我们来搞点事情：

```c++
//四则运算
int add(int i, int j){return i + j;} //普通函数
auto mod = [](int i, int j){return i % j;} //lambda
struct divide //函数对象类
{
    int operator()(int de, int di){return de / di;}
};
```

有没有发现，调用以上这些东西，都是同一种形式：

```c++
int(int, int)
```

所以啊，我们想定义一个函数表，从表中查找要调用的函数，在C++里面，我们可以用map，但是啊，又不行，为啥呢：

```c++
map<string, int(*)(int, int)> binops; //一个string一个函数指针
binops.insert({"+", add}); 
//然而其他的不是函数类型，放不进map里
```

于是我们要有新东西来解决这个问题了

#### 标准库function类型

直接抛代码把，简单来说就是把function作为一个模板，这定义在functional头文件中：

```c++
function<int(int, int)> //我们声明了一个function类型，表示接受两个int、返回一个int的可调用对象
```

我们可以把刚刚那些函数全跟它匹配：

```c++
function<int(int, int)> f1 = add; //函数指针
function<int(int, int)> f2 = divide(); //函数对象类的对象
function<int(int, int)> f3 = [](int i, int j){return i*j;}; // lambda

cout << f1(4, 2); //打印6
cout << f2(4, 2); //2
cout << f3(4, 2); //8
```

有了这个神器，我们就可以建立函数表了：

```c++
map<string, function<int(int, int)>> binops;
//可以理解为后一个参数指定了函数类型（函数名正好不在函数类型里面）
```

我们来重新初始化一下，给它点内容：

```c++
map<string, function<int(int, int)>> binops = 
{
    //四则运算
    {“+”, add},
    {"-", atd::minus<int>()}, //标准库函数对象
    {"*", [](int i, int j){return i*j;}}, //未命名的lambda
    {"/", divide()},
    {"%", mod}
};
```

有了这个函数表，我们就可以很方便很直观地调用了：

```c++
binops["+"](10, 5); //15
binops["-"](10, 5); //5
binops["*"](10, 5); //50
binops["/"](10, 5); //2
binops["%"](10, 5); //0
```

我们最好不要把函数名直接放到函数表中（你看我们的add就是函数指针），因为有重载的话就不知道调用的是哪个了。

### 重载、类型转换与运算符

我们先来看看一个叫转换构造函数的东西，其实是老朋友了：

```c++
string a = "haha";
Sales_data b;
b.combine(a); //combine接受一个Sales_data对象，但是一个string也行，因为隐式转换了
```

其实编译器用这个string构造了一个**临时的Sales_data对象**，用的就是**只接受一个string的构造函数**，所以搞了个定义叫**转换构造函数**。

我也有点忘了，权当复习了。

好了，现在来学点新的：

 **转换构造函数和类型转换运算符共同定义了类类型转换，也叫用户定义的类型转换。**

#### 类型转换运算符

是类的一种特殊成员函数，规定也比较多，类型转换函数的形式如下：

```c++
operator type() const;
```

其中**type表示某种类型**，**这种类型只要是能作为函数返回值的就行**，也就是说**数组类型**和**函数类型**不行，但是**可以有指针和引用**。

类型转换运算符没有显式的返回类型，没有形参，而且还必须定义成类的成员函数；它不改变待转换对象的内容，因此一般可以被定义为const

##### 定义含有类型转换运算符的类

我们来举个例子，我们定义一个比较简单的类，只能表示0-255之间的整数：

```c++
class SmallInt
{
public:
    //构造函数
    SmallInt(int i = 0) : val(i) //默认实参，初始化列表
    {
        if(i<0 || i>255){throw std::out_of_range("不要");}
    }
    //类型转换运算符
    operator int() const {return val;}

private:
    std::size_t val;
};
```

我们来用用：

```c++
SmallInt si; //si = 0
si + 4; //将si通过类型转换运算符，转换成int
si = 3; //将3隐式转换为SmallInt，然后调用编译器合成的拷贝赋值运算符

SmallInt b = 3.14; //内置类型double先转成int，再调用构造函数
b + 3.14; //b先通过类型转换运算符转换为int，然后int变double
```

尽管类型转换函数不负责指定返回类型，但实际上每个类型转换函数都会返回一个对应类型的值。

##### 显式的类型转换运算符

为了防止这种转换莫名其妙，我们采用了这样一个概念：

```c++
class SmallInt
{
public:
    explicit operator int() const {return val}; //多了个关键字
};
```

现在我们要调用这个**类型转换运算符**就没那么容易了，必须要亲自指出来才行：

```c++
si + 3; //错误
static_cats<int>(si) + 3; //正确：显示地调用函数请求转换
```

但是有个例外，如果该表达式作为条件的话，这个类型转换还是会自动发生的。

### 避免有二义性的转换

我们来通过两个例子看一下什么是二义性的转换：

```c++
struct B; //声明一个类
struct A
{
    A() = default; //强制合成默认构造函数
    A(const &B); //转换构造函数B->A
};
struct B //类B的定义
{
    operator A() const; //类型转换运算符B->A
};
```

经过精心定义这两个类之后，我们来看看怎么调用会产生二义性：

```c++
A f(const A&);  //声明了一个函数，参数为A返回类型也为A
B b;
A a = f(b); //这句话的分析就稍微复杂些
//一种可能是，我通过转换构造函数把参数b直接转换为类型A
//还有一种可能是我通过类型转换运算符把参数b转换为类型A
```

这两个函数很可能不一样，所以最后的结果也会不同，如果我们想避免这种情况，要么你在类定义的时候就注意，要么就像下面一样显式调用：

```c++
A a1 = f(b.operator A()); //调用B的类型转换运算符
A a2 = f(A(b)); //调用A的构造函数
```

#### 另一个二义性的例子

我们来构造一个不太好的类：

```c++
Struct A
{
    A(int = 0); 
    A(double); //转换构造函数，但是俩转换原都是算术类型，这不太好
};

long lg;
A a(lg); //不知道用哪个转换构造函数（类型转换运算符也一样）
```

#### 重载函数与转换构造函数

来个复杂的：当几个重载函数的参数分属不同的类类型时，这些类恰好也定义了同样的转换构造函数：

```c++
struct C
{
    C(int);
};
struct D
{
    D(int);
};

void manip(const C&); //函数声明
void manip(const D&);

manip(10); //二义性错误：不知道把10转换为C还是D
//可以显式调用
manip(C(10));

//即使是这样的也会二义性
struct E
{
    E(double); //因为int到double是标准库转换的，
    //很自然，所以还是无法区分
};
```

#### 函数匹配与重载运算

还是抛代码看：

```c++
class SmallInt
{
    friend SmallInt operator+
    (const SmallInt&, const SmallInt&);

public:
    SmallInt(int = 0); //转换源为int的转换构造函数
    operator int() const {return val;} 
    //转换目标为int的类型转换运算符

private:
    size_t val;
};
```

来来来，二义性搞起：

```c++
SmallInt s1, s2;
SmallInt s3 = s1 + s2; //调用重载的operator+
int i = s3 + 0; //二义性
//一种是s3转成int
//另一种是0转成SmallInt，把结果再转成int
```

总之，搞复杂了就要留心二义性，一般你注意我提到的几种情况就差不多了。