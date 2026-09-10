#pragma once
#include <iterator> // IWYU pragma: keep
#include <ratio>
#include <utility>
#include <QDebug>

// 1. 随机时间睡眠
void random_sleep();

// 2.计时装饰器
template<typename Func>
auto time_decorator(Func&& func)
{
    return [func = std::forward<Func>(func)](auto&&... args) mutable {
        auto start = std::chrono::high_resolution_clock::now();
        using RetType = std::invoke_result_t<Func, decltype(args)...>;

        if constexpr (std::is_void_v<RetType>)
        {
            std::invoke(func, std::forward<decltype(args)>(args)...);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> dur = end - start;
            qDebug() << "耗时:" << dur.count() << " ms";            
        }
        else
        {
            auto res = std::invoke(func, std::forward<decltype(args)>(args)...);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> dur = end - start;
            qDebug() << "耗时:" << dur.count() << " ms";
            return res;
        }
    };
}