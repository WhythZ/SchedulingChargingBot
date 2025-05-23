#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

//可继承单例Manager模板类实现
template <typename M>
class Manager
{
public:
	static M* Instance();

protected:
	Manager() = default;
	~Manager() = default;
	Manager(const Manager&) = delete;
	Manager& operator=(const Manager&) = delete;
};

//模板函数必须在头文件中实现，不能拆分到另一个源文件中实现
template <typename M>
M* Manager<M>::Instance()
{
	//创建局部静态变量，并返回其引用，保证线程安全
	static M manager;
	return &manager;
}

#endif
