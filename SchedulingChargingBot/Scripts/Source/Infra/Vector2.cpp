#include "../../Header/Infra/Vector2.h"
#include <cmath>

Vector2::Vector2(double _x, double _y) :x(_x), y(_y) {}

double Vector2::Length() const
{
	return sqrt(x * x + y * y);
}

Vector2 Vector2::Normalized() const
{
	//若向量为零向量，则返回零向量即可
	if (Length() == 0)
		return Vector2(0, 0);
	//反之则返回该向量方向上的单位向量（模长为1的向量）
	return Vector2(x / Length(), y / Length());
}

bool Vector2::ApproxZero() const
{
	//判断两个物体之间是否重合时（比如某怪物是否到达目标地点），直接将向量的浮点数元素x、y和0作比较是不精确的，会出现二者永远无法被判定为重合的情况
	return Length() < 0.001;
}

bool Vector2::operator==(const Vector2& _compare) const
{
	return (x == _compare.x) && (y == _compare.y);
}

bool Vector2::operator!=(const Vector2& _compare) const
{
	return !(*this == _compare);
}

bool Vector2::operator>(const Vector2& _compare) const
{
	return Length() > _compare.Length();
}

bool Vector2::operator>=(const Vector2& _compare) const
{
	return Length() >= _compare.Length();
}

bool Vector2::operator<(const Vector2& _compare) const
{
	return Length() < _compare.Length();
}

bool Vector2::operator<=(const Vector2& _compare) const
{
	return Length() <= _compare.Length();
}

Vector2 Vector2::operator+(const Vector2& _plus) const
{
	return Vector2(x + _plus.x, y + _plus.y);
}

void Vector2::operator+=(const Vector2& _plus)
{
	x += _plus.x;
	y += _plus.y;
}

Vector2 Vector2::operator-(const Vector2& _minus) const
{
	return Vector2(x - _minus.x, y - _minus.y);
}

void Vector2::operator-=(const Vector2& _minus)
{
	x -= _minus.x;
	y -= _minus.y;
}

Vector2 Vector2::operator*(const double& _mtp) const
{
	return Vector2(x * _mtp, y * _mtp);
}

void Vector2::operator*=(const double& _mtp)
{
	x *= _mtp;
	y *= _mtp;
}

double Vector2::operator*(const Vector2& _mtp) const
{
	return (x * _mtp.x) + (y * _mtp.y);
}

//左移运算符重载用于输出
std::ostream& operator<<(std::ostream& _cout, const Vector2& _obj)
{
	_cout << "(" << _obj.x << "," << _obj.y << ")";
	return _cout;
}