--- 如何使用CPU Profiling ---
1. 定义 PROFILING_MODE=1
2. 链接 QuartzCore framework(Apple)
3. 可以定义一下子函数：
void Func_AA()
{
    // waiting 1 second
    Core::sleepCallingThread(1000);
}

void Func_AB()
{
    // waiting 2 seconds
    Core::sleepCallingThread(2000);
}

void Func_Recur()
{
    static UInt8 recur_count = 5;
    // 使用PROFILING()宏来定义一个Profile Sample
    // NOTE： 给定的名字必须时静态字符串
    PROFILING("Recursive Func(5 Times)");

    if (--recur_count)
    {
        Func_Recur();
    }
    // waiting 1 second
    Core::sleepCallingThread(1000);
}

void Func_BA()
{
    // waiting 3 second
    Core::sleepCallingThread(3000);
}

// 定义Profiling Sample树的Root
int
main (
    int               argc,
    const char *const argv[])
{
    // 必须使用{ } 来进行区域分割
    {
        // total branch time: 1 sec + 2 sec + 5 sec = 8 sec
        PROFILING("Branch I");
        {
            // 1 sec
            PROFILING("First Child");
            Func_AA();
        }

        {
            // 2 sec
            PROFILING("Second Child");
            Func_AB();
        }

        {
            // 5 times and each time 1 sec
            // totally 5 sec
            PROFILING("Recursive Child");
            Func_Recur();
        }
    }

    {
        // 3 sec
        PROFILING("Branch II");
        Func_BA();
    }

    // 使用PROFILING_DUMP() 来得到Sample Tree的文字描述
    PROFILING_DUMP();
    /*
     - Branch I: [#used]: 1, [Total]: 8.0287, [Avg Total]: 8.0287s, [Self]: 0.0003, [Avg Self]: 0.0003
       - First Child: [#used]: 1, [Total]: 1.0031, [Avg Total]: 1.0031s, [Self]: 1.0031, [Avg Self]: 1.0031
       - Second Child: [#used]: 1, [Total]: 2.0043, [Avg Total]: 2.0043s, [Self]: 2.0043, [Avg Self]: 2.0043
       - Recursive Child: [#used]: 1, [Total]: 5.0210, [Avg Total]: 5.0210s, [Self]: 0.0000, [Avg Self]: 0.0000
         - Recursive Func(5 Times): [#used]: 5, [Total]: 5.0210, [Avg Total]: 5.0210s, [Self]: 5.0210, [Avg Self]: 5.0210
     - Branch II: [#used]: 1, [Total]: 3.0047, [Avg Total]: 3.0047s, [Self]: 3.0047, [Avg Self]: 3.0047
     */
