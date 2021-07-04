1. 由于我们无法获得主线程的Handle，所有对于DevMainThread，我们只能保存它的Thread ID
2. Job可以之间定义为std::function的functor：typedef std::function<void(void)> JobT
   - 还是使用其它的保存方式？
3. Ring Buffer:
RP：下一个读入位置
RF：当前读入的翻转/Flip计数：从Buffer的尾部翻转到头部的次数
WP：现一个写入位置
WF：当前写入的Flip计数
UB：使用的字节数
FB：空闲的字节数
#BUFF：Buffer的大小

=== 起始状态：===
  WP
  |
  v
  0   1   2   3   4   5   6
+---+---+---+-------+---+---+
| = | = | = | = | = | = | = |
+---+---+---+-------+---+---+
  ^
  |
  RP
#BUFF = 7
RP = 0
RF = 0
WP = 0
WF = 0
UB = WP - RP + (WF-RF)*#BUFF = 0 - 0 + 0*7 = 0
FB = #BUFF - UB = 7 - 0 = 7


=== 经过一些读写操作：===
                      WP
                      |
                      v
  0   1   2   3   4   5   6
+---+---+---+-------+---+---+
| = | = | = | B | B | = | = |
+---+---+---+-------+---+---+
              ^
              |
              RP
#BUFF = 7
RP = 3
RF = A
WP = 5
WF = A
UB = WP - RP + (WF-RF)*#BUFF = 5 - 3 + 0*7 = 2
FB = #BUFF - UB = 7 - 2 = 5


写入3个字符:
      WP
      |
      v
  0   1   2   3   4   5   6
+---+---+---+-------+---+---+
| C | = | = | B | B | C | C |
+---+---+---+-------+---+---+
              ^
              |
              RP
RP = 3
RF = A
WP = 1
WF = A+1
UB = 5 = WP - RP + (WF-RF)*#BUFF = 1 - 3 + 1*7 = -2 + 1*7 = 5
FB = #BUFF - UB = 7 - 5 = 2


写入5个字符:
              WP
              |
              v
  0   1   2   3   4   5   6
+---+---+---+-------+---+---+
| D | D | D | B | B | D | D |
+---+---+---+-------+---+---+
              ^
              |
              RP
RP = 3
RF = A
WP = 3
WF = A+1
UB = 7 = WP - RP + (WF-RF)*#BUFF = 3 - 3 + 1*7 = 0 + 1*7 = 7
FB = #BUFF - UB = 7 - 7 = 0
