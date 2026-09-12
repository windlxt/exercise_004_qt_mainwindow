// #include <iterator> // IWYU pragma: keep
#include <chrono>
#include <qlogging.h>
#include <thread>
#include <random>
#include <QDebug>


// 1.随机时间睡眠等待 ===========================
void random_sleep(int a, int b)
{
    // 随机数引擎，定义为thread_local，线程内初始化1次（放在函数内也可以），各线程独立
    // 不能用static，这是进程1份，多线程操作会引发数据竞争，程序会崩溃
    thread_local std::random_device rd;
    thread_local std::mt19937 gen(rd());    
    thread_local std::uniform_int_distribution<int> dist(a, b); // 闭区间，包括最小值a，最大值b
    // 整数分布是闭区间；浮点数 uniform_real_distribution 才是半开区间 [a,b)

    int ms = dist(gen); //生产随机数ms
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

