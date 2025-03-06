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

	double Length() const;                     //��ȡ����ģ��
	Vector2 Normalized() const;                //���ر�׼���ĵ�λ����
	bool ApproxZero() const;                   //�����ӽ�Ϊ����ж�

	bool operator==(const Vector2&) const;     //�������е�
	bool operator!=(const Vector2&) const;     //�������в���
	bool operator>(const Vector2&) const;      //�������ģ�������Ҳ�
	bool operator>=(const Vector2&) const;     //�������ģ�����ڵ����Ҳ�
	bool operator<(const Vector2&) const;      //�������ģ��С���Ҳ�
	bool operator<=(const Vector2&) const;     //�������ģ��С�ڵ����Ҳ�

	Vector2 operator+(const Vector2&) const;   //�ӷ�
	Vector2 operator-(const Vector2&) const;   //����
	Vector2 operator*(const double&) const;    //�����˷�
	double operator*(const Vector2&) const;    //�����˷�

	void operator+=(const Vector2&);           //�ӵ���
	void operator-=(const Vector2&);           //������
	void operator*=(const double&);            //�˵���
};

//�������������
std::ostream& operator<<(std::ostream& _cout, const Vector2& _obj);

#endif
