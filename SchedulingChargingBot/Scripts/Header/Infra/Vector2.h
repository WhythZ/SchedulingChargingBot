#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_

#include <iostream>

class Vector2
{
public:
	double x = 0;
	double y = 0;

public:
	Vector2(double, double);
	Vector2() = default;
	~Vector2() = default;

	double Length() const;                     //获取向量模长
	Vector2 Normalized() const;                //返回标准化的单位向量
	bool ApproxZero() const;                   //向量接近为零的判断

	bool operator==(const Vector2&) const;     //向量的判等
	bool operator!=(const Vector2&) const;     //向量的判不等
	bool operator>(const Vector2&) const;      //左侧向量模长大于右侧
	bool operator>=(const Vector2&) const;     //左侧向量模长大于等于右侧
	bool operator<(const Vector2&) const;      //左侧向量模长小于右侧
	bool operator<=(const Vector2&) const;     //左侧向量模长小于等于右侧

	Vector2 operator+(const Vector2&) const;   //加法
	Vector2 operator-(const Vector2&) const;   //减法
	Vector2 operator*(const double&) const;    //常数乘法
	double operator*(const Vector2&) const;    //向量乘法

	void operator+=(const Vector2&);           //加等于
	void operator-=(const Vector2&);           //减等于
	void operator*=(const double&);            //乘等于
};

//声明运算符重载
std::ostream& operator<<(std::ostream& _cout, const Vector2& _obj);

#endif
