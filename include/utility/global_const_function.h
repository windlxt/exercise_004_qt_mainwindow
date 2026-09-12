#pragma once
#include <iterator> // IWYU pragma: keep
#include <ratio>
#include <utility>
#include <QDebug>
#include <QElapsedTimer>
#include <functional>

// 1. 随机时间睡眠
void random_sleep(int a=500, int b=3000);

// 2.1 计时装饰器,纯C++版本
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
            qDebug() << "【纯C++版】耗时:" << dur.count() << " ms";            
        }
        else
        {
            auto res = std::invoke(func, std::forward<decltype(args)>(args)...);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> dur = end - start;
            qDebug() << "【纯C++版】耗时:" << dur.count() << " ms";
            return res;
        }
    };
}

// 2.2 计时装饰器,QElapsedTimer版本，比 `high_resolution_clock` 跨平台更稳，Qt 项目首选。
template<typename Func>
auto qtime_decorator(Func&& func)
{
    return [func = std::forward<Func>(func)](auto&&... args) mutable {
        QElapsedTimer timer;
        timer.start();

        using RetType = std::invoke_result_t<Func, decltype(args)...>;
        if constexpr (std::is_void_v<RetType>)
        {
            std::invoke(func, std::forward<decltype(args)>(args)...);
            qint64 ns = timer.nsecsElapsed();
            qDebug() << "【Qt版】耗时:" << static_cast<double>(ns)/1000000.0 << " ms";
        }
        else
        {
            auto res = std::invoke(func, std::forward<decltype(args)>(args)...);
            qint64 ns = timer.nsecsElapsed();
            qDebug() << "【Qt版】耗时:" << static_cast<double>(ns)/1000000.0 << " ms";
            return res;
        }
    };
}