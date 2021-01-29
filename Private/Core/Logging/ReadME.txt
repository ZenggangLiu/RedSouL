--- Logging 设计 ---
1. 如果Logging整体禁止，所有对他的引用将被编译成Nix： LOGGING_MODE == 0
2. Logging分为如下等级：Info, Warning, Failure
3. Logging支持Flags。我们并不直接存储Flag的字符串而是使用他Compile Time的Hash进行比较


--- 实现方法 ---
1/2): 我们在RS_Logging.hpp文件中定义一个宏INFO(), WARNING(), FAILURE()。
这些宏全部定义为空如果 LOGGING_MODE=0 在编译的时候定义。
而INFO(), WARNING(), FAILURE()是否定义取决于 ENABLE_INFO_LOGGING， ENABLE_WARNING_LOGGING，
ENABLE_FAILURE_LOGGING 是否定义

Logging的输出流程为：以INFO(FLAG, "SomeInt: %d", 123)为例
INFO(FLAG, "SomeInt: %d", 123)
  --> Core::LoggingHelper::logInfo (const UInt32, const Char *const, ...)
  --> Core::LoggingHelper::logInfo(COMPILE_TIME_HASH(HASH_SEED_LOG, "FLAG"), "SomeInt: %d", 123)
    --> 从变化长度的参数：... 生产输出信息的字符串
      --> LoggingMgr::getRef().logInfo(0xHASH, "SomeInt:123");
       --> 检查FLAG是否Enabled
         --> Logger::logInfoText("SomeInt:123")
           --> ConsoleLogger::logInfoText("SomeInt:123")
             --> 产生时间Tag并使用std::cout进行输出：
              Apple：[Jan-29 15:35:20] 💚 [INFO]: SomeInt:123
              Microsoft: 绿色信息：[Jan-29 15:35:20] [INFO]: SomeInt:123

3): 我们对Logging.cfg中定义的Flag使用murmur 32bit hash。 这个hash与我们在C++代码中定义Flag时候使用的
编译时的hash时一样的


--- 实现备注 ---
1. 使用编译器对宏的可变化长度参数的支持：__VAR_ARGS__
2. 使用编译器可以取消为不逗号：##__VAR_ARGS__
NOTE: 我们使用__VAR_ARGS__来得到变化长度的参数列表。但是C++11要求__VAR_ARGS__至少要有一个参数
解决方法：##__VAR_ARGS__ 这样编译器可以取消如下调用中尾部的逗号:
INFO(Flag, "text") -->
 Core::LoggingHelper::logInfo(COMPILE_TIME_HASH(HASH_SEED_LOG, TOKEN_QUOTE(Flag)), "text",)
如果我们定义INFO()使用##__VAR_ARGS__
INFO(Flag, "text") -->
 Core::LoggingHelper::logInfo(COMPILE_TIME_HASH(HASH_SEED_LOG, TOKEN_QUOTE(Flag)), "text")
