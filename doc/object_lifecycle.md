# object_lifecycle


## World Object 的生命周期

在 cpp 的语义中，对象的生命周期有
{
    1. 构造
    2. 析构
}

在 Unity 的 MonoBehaviour 语义中，MonoBehaviour 的生命周期
{
    1. Awake + Start
    2. OnEnable
    3. OnDisable
    4. OnDestroy
}


## Hierarchy Tree 上 Entity 析构的执行流程和实现

cpp 对象的析构要分为两种情况
{
    1. 在栈上分配内存的，离开作用域时。
    2. 在堆上分配内存的，delete 时。
}

需求
{
    1. 析构任意 WorldObject 时，按照继承树依次调用析构函数。
    2. 析构 Entity 时，析构其所有的 Component。
    3. 析构 Entity 时，析构其所有的 Children Entity。
    4. 析构 Entity 时，从其 Parent Entity 中移除自己。
}


## 只使用析构函数

1. 继承树。使用虚析构函数，cpp 语言机制会依次调用衍生类和子类的析构函数。
2. 在 Entity 的析构函数中，析构所有的 Component。
{
    所有的 Entity 上 Component 都是通过 AddComponent 接口添加上去的，AddComponent 使用的是 new 分配在堆上。
}
3. 在 Transform 的析构函数中，析构有所的 Children Entity。
4. 从 Parent Entity 中移除自己，这里存在陷阱和优化
{
    1. 实现方式一，直接在 Parent Entity 中移除自己。这样会改变 Parent 的 ComponentMap，那么 Parent 不能直接遍历 ComponentMap，即可能构成在集合迭代时增删增删集合元素。要么迭代前备份，要么标记等迭代结束后删除。
    2. 实现方式二，借助 Tree 结构。如果是被删除的元素是根节点，那么直接从 Parent 中移除，否则不用移除，因为 Parent 也即将被销毁了。但是这样，需要为 WorldObject 引入变量来辨别是否为被删除的根结点。
}


## 使用析构函数 + Destroy 函数

把所有(非对象默认管理资源)的 Release 操作，归结到 Destroy 函数中。

即，可以主动地调用 Destroy 释放所有(非对象默认管理资源)，也可以通过(析构函数)来调用 Destroy。

这里有个陷阱。
{
    1- 调用衍生类 Destroy ，应该手动调用基类的 Destroy。
    2- Entity 的 Destroy，应该调用 Component 的 Destroy，和 Children Entity 的 Destroy。
    3- 如果是由析构函数调用的 Destroy，那么实际的调用顺序是
    {
        衍生类的析构 -> 衍生 Destroy -> 基类 Destroy
        衍生类的析构 -> 基类的析构 -> 基类 Destroy 
    }
    基类 Destroy 被重复调用了。
}

Destroy 实现时，需要采取部分措施

```
void Transform::Destroy()
{
    if (InDestroy())
        return;
    SetFlagDestroyStart();

    // destroy self

    Component::Destroy();

    // destroy children

    SetFlagDestroyEnd();
}
```

