#ifndef _OBJECT_POOL_HPP_
#define _OBJECT_POOL_HPP_

#include <vector>
#include <chrono>
#include <algorithm>
#include <iostream>

template <typename T>
class ObjectPool
{
private:
	std::vector<T*> busyObjects;                          //�洢��Ծ����
	std::vector<T*> freeObjects;                          //�洢���ö���

	size_t initialCapacity = 100;                         //����صĳ�ʼ������С
	size_t maximumCapacity = 1000;                        //��������ݵļ�������
	size_t expandAmount = 100;                            //�����ÿ�����ݵķ���

	std::chrono::steady_clock::time_point lastShrinkTime; //�ϴ�����ʱ��
	int shrinkCheckDuration = 60;                         //ÿ����������һ������

public:
	ObjectPool();
	ObjectPool(size_t, size_t, size_t);                   //��ʼ�������������������ݷ���
	~ObjectPool();

	T* AcquireObject();                                   //�ӳ��л�ȡһ�����ö���
	void ReleaseObject(T*);                               //�ͷ�һ�����ö���س���

	const std::vector<T*>& GetBusyObjects() const;        //��ȡ��Ծ�����б�

private:
	bool Expand(size_t);                                  //��������
	void Shrink(size_t);                                  //��������
};

template<typename T>
ObjectPool<T>::ObjectPool()
{
	//ʹ��Ĭ��ֵ������һ�����ö���
	for (size_t i = 0; i < initialCapacity; i++)
		freeObjects.emplace_back(new T());

	//��ʼ��ʱ���
	lastShrinkTime = std::chrono::steady_clock::now();
}

template<typename T>
ObjectPool<T>::ObjectPool(size_t _initialCapacity, size_t _maximumCapacity, size_t _expandAmount)
	:initialCapacity(_initialCapacity), maximumCapacity(_maximumCapacity), expandAmount(_expandAmount)
{
	//����һ�����ö���
	for (size_t i = 0; i < initialCapacity; i++)
		freeObjects.emplace_back(new T());

	//��ʼ��ʱ���
	lastShrinkTime = std::chrono::steady_clock::now();
}

template<typename T>
ObjectPool<T>::~ObjectPool()
{
	for (T* _obj : busyObjects)
		delete _obj;
	for (T* _obj : freeObjects)
		delete _obj;
}

template<typename T>
T* ObjectPool<T>::AcquireObject()
{
	//�������
	if (freeObjects.size() == 0)
	{
		//�������ʧ�����׳��쳣����ɷ��ؿ�ָ�룩
		if (!Expand(expandAmount))
			throw std::runtime_error("Object Pool Expand Failed");
	}

	//ת�ƶ��󵽻�Ծ�б�
	T* _target = freeObjects.back();
	freeObjects.pop_back();
	busyObjects.emplace_back(_target);
	return _target;
}

template<typename T>
void ObjectPool<T>::ReleaseObject(T* _target)
{
	//����ҵ�Ŀ��
	auto _it = std::find(busyObjects.begin(), busyObjects.end(), _target);
	if (_it != busyObjects.end())
	{
		//��������״̬
		(*_it)->Reset();
		//����ת�Ƶ������б�
		freeObjects.emplace_back(*_it);
		busyObjects.erase(_it);
		std::cout << "Successfully Release The Object Into Free Pool\n";
	}

	//����Ƿ���Ҫ����
	auto _now = std::chrono::steady_clock::now();
	auto _durationSinceLastShrink = std::chrono::duration_cast<std::chrono::seconds>(_now - lastShrinkTime).count();
	if (_durationSinceLastShrink >= shrinkCheckDuration)
	{
		//������ж���ռ�ܶ�������50%���ϣ�������
		if ((freeObjects.size() + busyObjects.size()) / 2 <= freeObjects.size())
		{
			//����Ӧ�����ݵ��������˴������������б��75%
			Shrink((int)(freeObjects.size() * 0.75));
			lastShrinkTime = _now;
		}
	}
}

template<typename T>
const std::vector<T*>& ObjectPool<T>::GetBusyObjects() const
{
	return busyObjects;
}

template<typename T>
bool ObjectPool<T>::Expand(size_t _expandAmount)
{
	//���շ����������ö����б���������������
	size_t _currentCapacity = freeObjects.size() + busyObjects.size();
	size_t _actualExpandAmount = std::min(_expandAmount, maximumCapacity - _currentCapacity);
	//�޷��������ݣ���������ʧ��
	if (_actualExpandAmount <= 0) return false;
	//���ݳɹ�
	for (size_t i = 0; i < _actualExpandAmount; i++)
		freeObjects.emplace_back(new T());
	std::cout << "Successfully Expand ObjectPool By " << _actualExpandAmount << " Objects\n";
	return true;
}

template<typename T>
void ObjectPool<T>::Shrink(size_t _shrinkAmount)
{
	//���շ����������ö����б������ڳ�ʼ����
	size_t _currentCapacity = freeObjects.size() + busyObjects.size();
	size_t _actualShrinkAmount = std::min(_shrinkAmount, _currentCapacity - initialCapacity);
	//ֱ�Ӵ�ĩβɾ�������õ��ĵ�����ʧЧ
	for (size_t i = freeObjects.size() - _actualShrinkAmount; i < freeObjects.size(); i++)
		delete freeObjects[i];
	freeObjects.erase(freeObjects.end() - _actualShrinkAmount, freeObjects.end());
	std::cout << "Successfully Shrink ObjectPool By " << _actualShrinkAmount << " Objects\n";
}

#endif