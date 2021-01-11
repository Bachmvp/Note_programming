
#include <iostream>
#include <vector>
using namespace std;
//归并排序处理列表
struct ListNode {
    int data;
    ListNode* next;
    ListNode(int data = 0, ListNode* next = NULL)
            :data(data), next(next) {}
};
bool bubbleSort_slow(vector<int>& array);//冒泡排序a
void bubbleSort_fast(vector<int>& array);//冒泡排序b
void mergeSort(vector<int>& array,int lo,int hi);//归并排序
void merge(vector<int>& array, int lo,int mi,int hi);//归并排序part
ListNode* mergeSort(ListNode*& head);//归并排序 处理列表
ListNode* merge(ListNode*& head,ListNode*& mid);//归并排序part 处理列表

void selectSort(vector<int>& array);//选择排序
void insertSort(vector<int>& array);//插入排序
void insertSort_inverse(vector<int>& array);//插入排序的反向迭代器版本

void bucketSort(vector<int>& array);//桶排序,最简单的情况 知道了要排序的数字的大小，并且浪费最多的空间 hash(key)=key,并且桶内只有一个元素
void bucketSort_better(vector<int>& array);//桶排序，减少空间，修改散列函数可以让一个桶子装多个元素，所以桶排序涵盖比较广，你可以设计自己的散列函数，把所有元素分类到有限的桶中，再分别用别的排序方法
void countingSort(vector<int>& array);//计数排序也比较傻...（只适用于一定范围整数）这个版本的计数排序可以说是第一个桶排序的强化版本，是可以处理相同值的情况下的，第一个桶排序版本是把对应位置置为1，计数排序是往上加1，这样有相同的也会输出出来，同时他先遍历找了一下最小值和最大值，然后减小了空间，增加了一些时间
void radixSort(vector<int>& array);//基数排序，基数排序(Radix Sort)是桶排序的扩展，它的基本思想是：将整数按位数切割成不同的数字，然后按每个位数分别比较。
void heapSort(vector<int>& array);//堆排序，堆即优先级队列，一般使用大顶堆和小顶堆，类似于完全二叉树，不维护全序，只维护父节点比子节点的大小关系，提取从根处开始，把最后一位提到根处然后下滤，插入会插入最后一位再上滤
void heapBuild(vector<int>& array,int root,int length);//建堆，输入父节点，对父节点和子节点进行堆的处理,需要输入长度
void quickSort(vector<int>& array,int left, int right);//快排，找分割点，左边都小于分割点，右边都大于分割点，如何找分割点，先挖坑，左右向里面查找填坑
void shellSort(vector<int>& array);//希尔排序，是插入排序的衍生，为了减少时间，用n/2的间隔来把整个序列分组，然后分别插入排序，一直到最后一次性插入排序完成
int main() {
    vector<int> arrayT{3,5,1,4,7,9};
    vector<int> arrayT2{22,44,14,15,90,87,67,36};
    ListNode l0(0);
    ListNode l1(3);
    ListNode l2(2);
    ListNode l3(5);
    ListNode l4(4);
    ListNode l5(1);
    l0.next = &l1;
    l1.next = &l2;
    l2.next = &l3;
    l3.next = &l4;
    l4.next = &l5;
    ListNode* head=&l0;
    ListNode* head_sort=mergeSort(head);
    while(head_sort){
        cout<<head_sort->data<<endl;
        head_sort=head_sort->next;
    }
    //bool sorted=bubbleSort_slow(arrayT);
    //bubbleSort_fast(arrayT);
    //mergeSort(arrayT,0,arrayT.size()-1);
    //selectSort(arrayT);
    //insertSort(arrayT);
    //insertSort_inverse(arrayT);
    //bucketSort(arrayT);
    //bucketSort_better(arrayT2);
    //countingSort(arrayT2);
    //radixSort(arrayT2);
    //heapSort(arrayT2);
    //quickSort(arrayT2,0,arrayT2.size()-1);
    shellSort(arrayT2);
    for(auto it=arrayT2.begin();it!=arrayT2.end();++it){
        cout<<*it<<endl;
    }
    return 0;
}//可以发现这个冒泡排序中 是当检测到已经有序的时候就会停止，在这一层面是正确的
//但是有一种特殊的情况就是 如果只有开头的一段需要排序，而后面的一大段都不需要排序，那么我们就不需要每次都扫描到最后了
//我们可以在每次记录下最后交换的下标，然后下一次就扫描到那里为止
bool bubbleSort_slow(vector<int>& array){
    bool sorted=false;
    int left=0;
    int right=array.size()-1;
    while(!sorted){
        sorted=true;
        while(left<right){
            if(array[left]>array[left+1]){
                sorted=false;
                swap(array[left],array[left+1]);
            }
            //cout<<left<<endl;
            ++left;
        }
        left=0;
        --right;
    }
    return sorted;
}
//没有考虑如果输入的数组本身就是有序的，会陷入死循环，所以要加一个flag
void bubbleSort_fast(vector<int>& array){
    bool sorted=false
    int left=0;
    int right=array.size()-1;
    int last=right;
    while(left<last && !sorted){
        sorted=true
        while(left<right){
            if(array[left]>array[left+1]){
                sorted=false
                last=left;
                swap(array[left],array[left+1]);
            }
            ++left;
        }
        left=0;
        right=last;
    }
}

//归并排序，主要是分治和递归的方法
//1，无序向量的递归分解，2.有序向量的逐层归并
//分解几乎不消费复杂度，主要为归并，一次归并是O（n）,n个元素需要归并logn次，所以复杂度为O（nlogn）
void mergeSort(vector<int>& array,int lo,int hi){
    if(lo>=hi){return;}//递归基 lo,hi是要排序的左右端
    int mi=(lo+hi)/2;
    mergeSort(array,lo,mi);//分解左半端
    mergeSort(array,mi+1,hi);//分解右半端
    merge(array,lo,mi,hi);//合并
}
//这里的归并是对已经拆开的两个有序数组进行合并，所以和之前双指针的部分中所提到的相同。
//只要是对有序的数组或者链表进行合并都可以使用这个方法
void merge(vector<int>& array, int lo,int mi,int hi){
    int i=lo;//i表示左半端的第一个
    int k=0;//k是我们临时存储有序数组的迭代器，所以从0开始
    int j=mi+1;//j表示右半端的第一个
    vector<int> tempArray(hi-lo+1);
    while(i<=mi&&j<=hi){//只要有一个不满足就停下
        if(array[i]<array[j])//比较大小，依次放入临时数组
            tempArray[k++]=array[i++];
        else
            tempArray[k++]=array[j++];
    }
    while(i<=mi)//最后检查
        tempArray[k++]=array[i++];
    while(j<=hi)
        tempArray[k++]=array[j++];
    for(i=lo,k=0;i<=hi;++i,++k){//把临时数组赋回数组
        array[i]=tempArray[k];
    }
}
//归并排序 处理列表
//切割链表，我们需要计算出四个指针，分别是左端的开始，左端的结尾，右端的开始，右端的结尾
//因为我们打算用快慢指针去切割链表，快指针到null的时候也就是慢指针到mid+1的时候，但这个时候需要把左端的尾指针->next赋值一个null。所以需要这四个指针
ListNode* mergeSort(ListNode*& head){
    if(head==NULL||head->next==NULL)
        return head;
    ListNode* slow_mid_pre=head;//慢指针，最后指向mid+1的前一个，也就是左端的尾指针
    ListNode* slow_mid=head;//慢指针，最后指向mid+1
    ListNode* fast_right=head;//快指针，最后指向右端的最后一个
    while(fast_right!=NULL&&fast_right->next!=NULL){//快指针一次跑两格，所以需要检查next
        slow_mid_pre=slow_mid;
        slow_mid=slow_mid->next;//慢指针一格
        fast_right=fast_right->next->next;//快指针两格
    }
    slow_mid_pre->next=NULL;//左半端最后必须赋值null
    //merge
    ListNode* head_sub=mergeSort(head);
    ListNode* mid_sub=mergeSort(slow_mid);
    return merge(head_sub,mid_sub);
}
ListNode* merge(ListNode*& head,ListNode*& mid){
    ListNode head_tmp(0,NULL);//新的链表的head的前一个，要定义一个空节点，但不能是NULL，因为空指针不能有next
    ListNode* tmp=&head_tmp;
    ListNode* i=head;
    ListNode* j=mid;
    while(i&&j){//和之前的归并排序的合并方法一致
        if(i->data<=j->data){
            tmp->next=i;
            tmp=tmp->next;
            i=i->next;
        }
        else{
            tmp->next=j;
            tmp=tmp->next;
            j=j->next;
        }
    }
    if(i)//对被剩下来的最后一个点 放入列表
        tmp->next=i;
    else
        tmp->next=j;
    return head_tmp.next;
}
//选择排序
void selectSort(vector<int>& array){
    int size=array.size();
    int index_scan=1;
    vector<int>::iterator begin_it=array.begin();//每一次扫描后把无序队列最小值交换到的目标位置（有序队列的最后一个点）
    //扫描次数
    for(;index_scan<size;++index_scan){
        vector<int>::iterator index_min=array.begin()+index_scan-1;//无序队列的第一个点
        //遍历无序队列，从无序队列的第二个点开始遍历，因为第一个点用作比较目标就可以了
        for(vector<int>::iterator it=array.begin()+index_scan;it!=array.end();++it){
            if(*it<*index_min){
                index_min=it;
            }
        }
        //如果无序队列的第一个点就是最小的，就没必要交换
        if(*index_min!=*begin_it){
            int temp=*begin_it;
            *begin_it=*index_min;
            *index_min=temp;
        }
        ++begin_it;//移动到有序队列的最后
    }
}
//插入排序
void insertSort(vector<int>& array){
    int size=array.size();
    const vector<int>::iterator it_begin=array.begin();//有序队列的begin迭代器
    for(int index_scan=1;index_scan<size;++index_scan){
        vector<int>::iterator it=array.begin()+index_scan;//要插入的值的迭代器
        int value_insert=*it;//找到要插入的值
        --it;//向前推一位找到有序队列的尾部迭代器，之后会从后往前推进
        /*while(it!=it_begin&&value_insert<*it){
            *(it+1)=*it;//执行交换，也就是如果正常插入的值<有序队列的队尾向前推进的值，就交换(但这里没把要插入的值赋给前一个值，没必要，因为插入的值我们已经单独拿出来了),方便之后直接插入
            --it;
        }*///这里会出现一个问题，如果使数组的下标的话，很容易我们可以让下标=-1，之后插入值++i，但这里是迭代器，记住迭代器是不能在begin()之前的，所以我加了一个flag来进行判断插入的值是否正好是在begin
        //*(it+1)=value_insert;//执行插入
        //改正版本1，使用flag应对特殊情况
        bool value_insert_atbegin=false;
        while(value_insert<*it){
            *(it+1)=*it;//执行交换，也就是如果正常插入的值<有序队列的队尾向前推进的值，就交换(但这里没把要插入的值赋给前一个值，没必要，因为插入的值我们已经单独拿出来了),方便之后直接插入
            if(it!=it_begin){--it;}
            else{
                value_insert_atbegin=true;
                break;
            }
        }
        if(value_insert_atbegin)
            *it=value_insert;
        else
            *(it+1)=value_insert;
    }
}//插入排序对于数组来说，下标和迭代器的索引方式要注意区别
//对于插入排序中遇到的begin的前一个位置的迭代器无法表示的情况，反向迭代器不就可以解决了吗
void insertSort_inverse(vector<int>& array){
    int size=array.size();
    const vector<int>::reverse_iterator it_rend=array.rend();//有序队列的反向end迭代器
    for(int index_scan=1;index_scan<size;++index_scan) {
        vector<int>::reverse_iterator it = array.rend() - index_scan-1;//要插入的值的反向迭代器
        int value_insert = *it;//找到要插入的值
        ++it;//向前推一位找到有序队列的尾部迭代器，之后会从后往前推进（反向迭代器，所以往前是+）
        while(it!=it_rend&&value_insert<*it){
            *(it-1)=*it;//执行交换，也就是如果正常插入的值<有序队列的队尾向前推进的值，就交换(但这里没把要插入的值赋给前一个值，没必要，因为插入的值我们已经单独拿出来了),方便之后直接插入
            ++it;
        }//这里我们不需要flag，反向迭代器成功解决问题
        *(it-1)=value_insert;//执行插入
    }
}

//桶排序，基于散列表，假设排列的是0-9的int
void bucketSort(vector<int>& array){
    if(array.empty())
        return;
    int size=array.size();
    vector<int> bucketList(10);//初始化10个桶，因为0-9，初始化为0
    for(int i=0;i<size;++i){
        bucketList[array[i]]=1;//hash(key)=key,所以在key的地方标为1
    }
    for(int i=0,j=0;i<10;++i){
        if(bucketList[i]!=0){//因为桶的下标是有序的，只要有1的就是原来的元素，依次输出就好了
            array[j]=i;
            ++j;
        }
    }
}
//桶排序，可以用更少的空间处理更多的元素，假设为1-99
//如果用之前的算法需要100个桶，这里仍然使用10个桶
void bucketSort_better(vector<int>& array){
    if(array.empty())
        return;
    int size=array.size();
    vector<int> bucketList[10];//初始化10个桶，但每个桶中都有一个变长数组
    for(int i=0;i<size;++i){//遍历array
        bucketList[array[i]/10].push_back(array[i]);//hash(key)=key/10,因为1-99，所以得数为1-9，分别推到桶中
    }
    for(int i=0;i<10;++i){//遍历所有桶
        mergeSort(bucketList[i],0,bucketList[i].size()-1);//桶内排序，可以用好点的方法来，现在用的归并排序
    }
    for(int i=0,index_array=0;i<10;++i){//遍历桶
        for(int j=0;j<bucketList[i].size();++j){//遍历桶中的元素
            array[index_array]=bucketList[i][j];//排序好了，可以顺序读取了
            index_array++;
        }
    }
}
void countingSort(vector<int>& array){
    if(array.empty())
        return;
    int size=array.size();
    int max=INT_MIN,min=INT_MAX;
    for(int i=0;i<size;++i){//找最大最小值
        max=(array[i]>max)? array[i] : max;
        min=(array[i]<min)? array[i] : min;
    }
    int len_bucket=max-min+1;//这样就减少桶的数量了
    vector<int> bucketList(len_bucket);//初始化
    for(int i=0;i<size;++i){
        bucketList[array[i]-min]++;//hash(key)=key-min,因为桶从min开始的，然后有一个就在桶位置加1，如果有相同的就知道多少个相同的
    }
    for(int i=0,index_array=0;i<len_bucket;++i){
        for(int j=0;j<bucketList[i];++j){//假如是2，那就连续输出两次i(下标才是array的value)
            array[index_array]=i+min;//但是i从0开始，要加上min
            index_array++;
        }
    }
}

void radixSort(vector<int>& array){
    if(array.empty())
        return;
    int size=array.size();
    int max_bit=0;
    for(int i=0;i<size;++i){//找到array中的数值的最高位
        int max_bit_temp=0;
        for(int exp=1;array[i]/exp>0;exp*=10)
            ++max_bit_temp;
        max_bit=(max_bit_temp>max_bit)? max_bit_temp:max_bit;
    }
    cout<<max_bit<<endl;
    vector<int> bucketList[10];//初始化10个桶，但每个桶中都有一个变长数组
    for(int bit=0,exp=1;bit<max_bit;++bit,exp*=10){//几个数位就排序几次,先个位再十位
        for(int i=0;i<size;++i){//这里注意怎么处理，其实是如果是个位则 a%10然后/1 十位a%100/10 百位a%1000/100
            int index_temp=array[i]%(exp*10);
            int index=index_temp/exp;
            bucketList[index].push_back(array[i]);
        }
        int index_array=0;
        for(int i=0;i<10;++i){//把排好的桶中元素放回array中
            for(int j=0;j<bucketList[i].size();++j){
                array[index_array]=bucketList[i][j];
                ++index_array;
            }
            //！！！！！！没有清理每个桶的vector元素!!!!!!!!
            bucketList[i].clear();
        }

    }
}
//堆与优先级队列联系比较紧密，主要是上浮和下沉。
//主要是父节点，左节点，右节点的关系
void heapBuild(vector<int>& array,int root, int length) {//root在这个函数里改变就可以了，不必用引用
    //int value_root=array[root];//父节点的值
    int index_lc = 2 * root + 1;//子节点的下标为2k+1,2k+2;
    int index_rc = 2 * root + 2;//如果根节点在数组中的位置为0，那么子节点为2k+1,2k+2.但如果根节点在数组中的位置为1，那么子节点为2k,2k+1.
    int index_min = root;
    /*
    while(index_lc<length){//依次处理子节点作为父节点，循环
        if(array[index_rc]>array[index_lc])
            index_min=index_lc;//找左右子节点的最小值
        if(index_rc<length && array[index_rc]<array[index_lc])
            index_min=index_rc;//找左右子节点的最小值
        if(index_min!=root)
            std::swap(array[index_min],array[root]);
        else
            break;
        root=index_min;//继续下滤找子节点的子节点
        index_lc=2*root+1;
        index_rc=2*root+2;//这里其实使用递归更整洁，但使用了迭代
        index_min=root;
    }*/
    if (index_lc < length && array[index_min] < array[index_lc])
        index_min = index_lc;//比较父节点和左节点
    if (index_rc < length && array[index_rc] > array[index_min])
        index_min = index_rc;//比较父节点和右节点，堆不在乎两个孩子互相的大小
    if (index_min != root) {
        std::swap(array[index_min], array[root]);
        heapBuild(array,index_min,length);
    }
}
void heapSort(vector<int>& array){
    if(array.empty())
        return;
    int len=array.size();
    for(int i=len/2-1;i>=0;--i){//从倒数第二层开始，每一层的第一个都是2n+1
        heapBuild(array,i,len);//建堆
    }
    for(auto it=array.begin();it!=array.end();++it){
        cout<<*it<<endl;
    }
    for(int i=len-1;i>=1;--i){
        std::swap(array[0],array[i]);
        heapBuild(array,0,i);
    }
}

void quickSort(vector<int>& array,int left, int right){
    if(left<right){
        int i=left;
        int j=right;
        int x=array[i];
        while(i<j){
            while(i<j&&array[j]>=x)//右边向左边推进找第一个小的值
                --j;
            if(i<j)
                array[i++]=array[j];//顺次填坑，赋值后下标再加一
            while(i<j&&array[i]<x)//左边向右边推进找第一个大的值
                ++i;
            if(i<j)
                array[j--]=array[i];
        }
        array[i]=x;
        quickSort(array,left,i-1);
        quickSort(array,i+1,right);
    }
}

void shellSort(vector<int>& array){
    int size=array.size();
    for(int gap=size/2;gap>0;gap/=2)//这是gap的循环，假如10个元素，gap5为5组 排序，然后2组 排序，然后1组 排序
        for(int i=gap;i<size;++i)//这是每一次gap里的遍历每个组
            for(int j=i-gap;j>=0&&array[j]>array[j+gap];j-=gap)//然后执行插入排序，向前互换 j要大于等于0！交换到最后为止
                std::swap(array[j],array[j+gap]);

}

//补充一下对于官方sort函数如何自定义比较关系
//首先sort函数的比较形参是一个函数即可，不像是优先级队列里必须是个类
//关于sort函数的自定义函数，一般是先定义一个标准的函数，再把函数名传给sort的第三个形参，但C++11里的lambda省事太多直接[]
//如果想定义函数来做，那么有一个注意点就是，sort的compare要求的是一个普通函数指针，不能是一个成员函数指针
//类成员函数指针与普通函数指针不是一码事。前者要用.*与->*运算符来使用，而后者可以用*运算符（称为“解引用”dereference，或称“间址”indirection）。
//普通函数指针实际上保存的是函数体的开始地址，因此也称“代码指针”，以区别于C / C++最常用的数据指针。
//而类成员函数指针就不仅仅是类成员函数的内存起始地址，还需要能解决因为C++的多重继承、虚继承而带来的类实例地址的调整问题，所以类成员函数指针在调用的时候一定要传入类实例对象。
//指针函数是一个正常函数，返回一个指针
//函数指针是一个指针指向一个函数，然后这个指针可以用来当函数用，或者成为形参
//所以如果在类中，比较函数必须为静态才是普通函数指针，如果在类外，正常函数声明即可

//那么接下来就是sort比较函数的规定
//可以看到comp的定义：comp函数返回一个bool类型的值，这个值表示了在严格弱排序中（可以理解为升序排序）第一参数是否位于第二个参数之前。
//也就是说如果comp返回true，则第一个参数小于第二个参数，sort根据compare的返回值将第一个参数排在第二个参数之前。
//如果comp返回false，则第一个参数大于第二个参数，sort根据compare的返回值将第一个参数排在第二个参数之后。
//总结起来就是：
//sort函数根据comp函数的返回值，对comp函数的两个参数排序。
//如果comp返回true，排序为“参数1”“参数2”，否则排序为“参数2”“参数1”。
//想要升序排列，则return parameter1<parameter2
//想要降序排列，则return parameter1>parameter2

//一个示例
//static bool compare(const string& s1, const string& s2)
//{
//    string ab = s1 + s2;
//    string ba = s2 + s1;
//    return ab < ba; //升序排列。如改为ab > ba, 则为降序排列
//}

//优先级队列的比较方式更复杂，需要声明为类
//struct Comp {
//bool operator() (ListNode* l1, ListNode* l2) {
//    return l1->val > l2->val;
//}
//};