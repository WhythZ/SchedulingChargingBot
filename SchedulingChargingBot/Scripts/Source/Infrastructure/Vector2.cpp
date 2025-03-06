#include "../../Header/Infrastructure/Vector2.h"
#include <cmath>

Vector2::Vector2(double _x, double _y) :x(_x), y(_y) {}

double Vector2::Length() const
{
	return sqrt(x * x + y * y);
}

Vector2 Vector2::Normalized() const
{
	//������Ϊ���������򷵻�����������
	if (Length() == 0)
		return Vector2(0, 0);
	//��֮�򷵻ظ����������ϵĵ�λ������ģ��Ϊ1��������
	return Vector2(x / Length(), y / Length());
}

bool Vector2::ApproxZero() const
{
	//�ж���������֮���Ƿ��غ�ʱ������ĳ�����Ƿ񵽴�Ŀ��ص㣩��ֱ�ӽ������ĸ�����Ԫ��x��y��0���Ƚ��ǲ���ȷ�ģ�����ֶ�����Զ�޷����ж�Ϊ�غϵ����
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

//��������������������
std::ostream& operator<<(std::ostream& _cout, const Vector2& _obj)
{
	_cout << "(" << _obj.x << "," << _obj.y << ")";
	return _cout;
}