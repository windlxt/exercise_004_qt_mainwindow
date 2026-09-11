// #include <iterator> // IWYU pragma: keep
#include <chrono>
#include <qlogging.h>
#include <thread>
#include <random>
#include <QDebug>


// 1.随机时间睡眠等待 ===========================
void random_sleep(int a, int b)
{
    // 随机数引擎，建议定义为static避免每次重新初始化（放在函数内也可以）
    static std::random_device rd;
    static std::mt19937 gen(rd());
    // 最小值300，最大值1200
    std::uniform_int_distribution<int> dist(a, b);
    int ms = dist(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

