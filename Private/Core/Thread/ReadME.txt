1. 由于我们无法获得主线程的Handle，所有对于DevMainThread，我们只能保存它的Thread ID
2. Job可以之间定义为std::function的functor：typedef std::function<void(void)> JobT
   - 还是使用其它的保存方式？
