# OI 题目数据生成工具

## 数据生成常用工具(具体使用方法请自行跳转函数)

### 1. 随机数生成器：`random()`

#### 函数说明

![alt text](image-1.png)

#### 代码示例

```cpp
void solve() {
    int n = random( 1, 1000 , OFFSET ) ; // 生成一个位于 [1,1000] 的随机数
    ins << n << "\n"; // 将随机数存入输入字符串流
    return;
}
```

### 2. 随机字符串生成器：`getRandomString()`

#### 函数说明

![alt text](image-2.png)

#### 代码示例

```cpp
void solve() {
    string s = getRandomString( 10 ,"ABCD" ) ; // 生成一个长度为 10 且由 "ABCD" 四种字母组成的字符串
    ss << s << "\n"; // 将字符串存入到输出字符串流
    return;
}
```

3. 随机图生成类：`RandomGraph`

#### 类说明

##### 3.1 类函数生成合法的点 $n$ 和边数 $m$： `nm`

![alt text](image-5.png)

##### 3.2 类函数检查点 $n$ 和 边数 $m$ 的合法性： `check`

![alt text](image-3.png)

##### 3.3 类函数生成随机图的边： `graph`

![alt text](image-4.png)

4. 随机树生成类：`RandomTree`

#### 类说明

##### 4.1 类函数生成随机树： `random_tree`

![alt text](image-6.png)
