#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

//可继承单例Manager模板类实现
template <typename M>
class Manager
{
protected:
	static M* manager;

public:
	static M* Instance();

protected:
	Manager() = default;
	~Manager() = default;
	Manager(const Manager&) = delete;
	Manager& operator=(const Manager&) = delete;
};

//初始化静态成员变量
template <typename M>
M* Manager<M>::manager = nullptr;

//模板函数必须在头文件中实现，不能拆分到另一个源文件中实现
template <typename M>
M* Manager<M>::Instance()
{
	if (!manager)
		manager = new M();
	return manager;
}

#endif
