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
	std::vector<T*> busyObjects;                          //存储活跃对象
	std::vector<T*> freeObjects;                          //存储闲置对象

	size_t initialCapacity = 100;                         //对象池的初始容量大小
	size_t maximumCapacity = 1000;                        //对象池扩容的极限容量
	size_t expandAmount = 100;                            //对象池每次扩容的幅度

	std::chrono::steady_clock::time_point lastShrinkTime; //上次缩容时间
	int shrinkCheckDuration = 60;                         //每隔多少秒检测一次缩容

public:
	ObjectPool();
	ObjectPool(size_t, size_t, size_t);                   //初始容量、极限容量、扩容幅度
	~ObjectPool();

	T* AcquireObject();                                   //从池中获取一个闲置对象
	void ReleaseObject(T*);                               //释放一个闲置对象回池中

	const std::vector<T*>& GetBusyObjects() const;        //获取活跃对象列表

private:
	bool Expand(size_t);                                  //池子扩容
	void Shrink(size_t);                                  //池子缩容
};

template<typename T>
ObjectPool<T>::ObjectPool()
{
	//使用默认值，开辟一堆闲置对象
	for (size_t i = 0; i < initialCapacity; i++)
		freeObjects.emplace_back(new T());

	//初始化时间点
	lastShrinkTime = std::chrono::steady_clock::now();
}

template<typename T>
ObjectPool<T>::ObjectPool(size_t _initialCapacity, size_t _maximumCapacity, size_t _expandAmount)
	:initialCapacity(_initialCapacity), maximumCapacity(_maximumCapacity), expandAmount(_expandAmount)
{
	//开辟一堆闲置对象
	for (size_t i = 0; i < initialCapacity; i++)
		freeObjects.emplace_back(new T());

	//初始化时间点
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
	//检测扩容
	if (freeObjects.size() == 0)
	{
		//如果扩容失败则抛出异常（或可返回空指针）
		if (!Expand(expandAmount))
			throw std::runtime_error("Object Pool Expand Failed");
	}

	//转移对象到活跃列表
	T* _target = freeObjects.back();
	freeObjects.pop_back();
	busyObjects.emplace_back(_target);
	return _target;
}

template<typename T>
void ObjectPool<T>::ReleaseObject(T* _target)
{
	//如果找到目标
	auto _it = std::find(busyObjects.begin(), busyObjects.end(), _target);
	if (_it != busyObjects.end())
	{
		//清除对象的状态
		(*_it)->Reset();
		//将其转移到闲置列表
		freeObjects.emplace_back(*_it);
		busyObjects.erase(_it);
		std::cout << "Successfully Release The Object Into Free Pool\n";
	}

	//检查是否需要缩容
	auto _now = std::chrono::steady_clock::now();
	auto _durationSinceLastShrink = std::chrono::duration_cast<std::chrono::seconds>(_now - lastShrinkTime).count();
	if (_durationSinceLastShrink >= shrinkCheckDuration)
	{
		//如果空闲对象占总对象数的50%以上，则缩容
		if ((freeObjects.size() + busyObjects.size()) / 2 <= freeObjects.size())
		{
			//计算应当缩容的数量，此处是削减空闲列表的75%
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
	//按照幅度扩容闲置对象列表，不超过极限容量
	size_t _currentCapacity = freeObjects.size() + busyObjects.size();
	size_t _actualExpandAmount = std::min(_expandAmount, maximumCapacity - _currentCapacity);
	//无法继续扩容，返回扩容失败
	if (_actualExpandAmount <= 0) return false;
	//扩容成功
	for (size_t i = 0; i < _actualExpandAmount; i++)
		freeObjects.emplace_back(new T());
	std::cout << "Successfully Expand ObjectPool By " << _actualExpandAmount << " Objects\n";
	return true;
}

template<typename T>
void ObjectPool<T>::Shrink(size_t _shrinkAmount)
{
	//按照幅度缩减闲置对象列表，不低于初始容量
	size_t _currentCapacity = freeObjects.size() + busyObjects.size();
	size_t _actualShrinkAmount = std::min(_shrinkAmount, _currentCapacity - initialCapacity);
	//直接从末尾删除，不用担心迭代器失效
	for (size_t i = freeObjects.size() - _actualShrinkAmount; i < freeObjects.size(); i++)
		delete freeObjects[i];
	freeObjects.erase(freeObjects.end() - _actualShrinkAmount, freeObjects.end());
	std::cout << "Successfully Shrink ObjectPool By " << _actualShrinkAmount << " Objects\n";
}

#endif