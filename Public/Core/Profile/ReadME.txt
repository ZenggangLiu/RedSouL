--- 如何使用Profile ---
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
    // 使用PROFILE()宏来定义Profile Sample
    // NOTE： 给定的名字必须时静态字符串
    PROFILE("Recursive Func(5 Times)");

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

// 定义Profile Sample 树的Root
int
main (
    int               argc,
    const char *const argv[])
{
    // 必须使用{ } 来进行区域分割
    {
        // total branch time: 1 sec + 2 sec + 5 sec = 8 sec
        PROFILE("Branch I");
        {
            // 1 sec
            PROFILE("First Child");
            Func_AA();
        }

        {
            // 2 sec
            PROFILE("Second Child");
            Func_AB();
        }

        {
            // 5 times and each time 1 sec
            // totally 5 sec
            PROFILE("Recursive Child");
            Func_Recur();
        }
    }

    {
        // 3 sec
        PROFILE("Branch II");
        Func_BA();
    }

    // 使用PROFILE_DUMP() 来得到Sample Tree的文字描述
    PROFILE_DUMP();
    /*
     - Branch I: [#used]: 1, [Total]: 8.0280, [Avg Total]: 8.0280s, [Self]: 0.0001, [Avg Self]: 0.0001
       - First Child: [#used]: 1, [Total]: 1.0045, [Avg Total]: 1.0045s, [Self]: 1.0045, [Avg Self]: 1.0045
       - Second Child: [#used]: 1, [Total]: 2.0046, [Avg Total]: 2.0046s, [Self]: 2.0046, [Avg Self]: 2.0046
       - Recursive Child: [#used]: 1, [Total]: 5.0188, [Avg Total]: 5.0188s, [Self]: 0.0000, [Avg Self]: 0.0000
         - Recursive Func(5 Times): [#used]: 5, [Total]: 5.0187, [Avg Total]: 5.0187s, [Self]: 5.0187, [Avg Self]: 5.0187
     - Branch II: [#used]: 1, [Total]: 3.0041, [Avg Total]: 3.0041s, [Self]: 3.0041, [Avg Self]: 3.0041 
     */