#ifndef _NONCOPYABLE_H_
#define _NONCOPYABLE_H_
namespace Trluper{

/// @brief 继承该类说明对象无法拷贝、赋值
class Noncopyable{
public:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
}

}

#endif