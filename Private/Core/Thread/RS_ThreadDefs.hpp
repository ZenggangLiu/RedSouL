#pragma once


namespace Core
{

    // 线程的状态
    enum DevThreadState
    {
        // 无效状态
        DEV_THREAD_STATE_INVALID,

        // 创建但未运转
        DEV_THREAD_STATE_CREATED,
        // 运转中
        DEV_THREAD_STATE_RUNNING,
        // 暂停中
        DEV_THREAD_STATE_SUSPENDED,
        // 销毁
        DEV_THREAD_STATE_TERMINATED,
    };

    // 线程的优先级：Priority
    enum DevThreadPriority
    {
        DEV_THREAD_PRIORITY_TIME_CRITICAL,
        DEV_THREAD_PRIORITY_HIGHEST,
        DEV_THREAD_PRIORITY_ABOVE_NORMAL,
        DEV_THREAD_PRIORITY_NORMAL,
        DEV_THREAD_PRIORITY_BELOW_NORMAL,
        DEV_THREAD_PRIORITY_LOWEST,

        DEV_THREAD_PRIORITY_DEFAULT             = DEV_THREAD_PRIORITY_NORMAL
    };

    // 线程堆栈的大小(字节)
    enum DevThreadStackSize
    {
        // NOTE：
        // - iOS：堆栈的大小将rounded up到页的边界
        // - Win：堆栈的大小将rounded up到64k(allocation granularity)
        DEV_THREAD_STACK_SIZE_64K               =   64 * 1024,
        DEV_THREAD_STACK_SIZE_128K              =  128 * 1024,
        DEV_THREAD_STACK_SIZE_512K              =  512 * 1024,
        DEV_THREAD_STACK_SIZE_1M                = 1024 * 1024,
        DEV_THREAD_STACK_SIZE_2M                = 2018 * 1024,

        // 缺省的堆栈大小
        // - iOS： 512k
        // - Win：   1M：可以通过使用GetCurrentThreadStackLimits()来获得
        DEV_THREAD_STACK_SIZE_DEFAULT           = -1
    };

} // namespace Core

