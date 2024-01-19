# 知识点

- 类型转换
（类型说明符）表达式
    实现的功能就是把表达式的值强制转换为类型说明符表示的类型。除了这种强制类型转换方法外，C++还提供了四种类型转换方法，按照适用范围从窄到宽的顺序介绍，先从使用频率比较低的reinterpret_cast开始，然后依次是const_cast，dynamic_cast，最后介绍static_cast

1. reinterpret_cast<类型说明符>(表达式）
    reinterpret_cast：首先从英文字面的意思理解，interpret是“解释，诠释”的意思，加上前缀“re”，就是“重新诠释”的意思；cast在这里可以翻译成“转型”，这样整个词顺下来就是“重新诠释的转型”。
    “reinterpret_cast 运算符并不会改变括号中运算对象的值，而是对该对象从位模式上进行重新解释”。

2. const_cast<类型说明符>(表达式）


3. dynamic_cast<类型说明符>(表达式）


4. static_cast<类型说明符>(表达式）