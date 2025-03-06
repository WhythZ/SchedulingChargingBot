#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

//�ɼ̳е���Managerģ����ʵ��
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

//��ʼ����̬��Ա����
template <typename M>
M* Manager<M>::manager = nullptr;

//ģ�庯��������ͷ�ļ���ʵ�֣����ܲ�ֵ���һ��Դ�ļ���ʵ��
template <typename M>
M* Manager<M>::Instance()
{
	if (!manager)
		manager = new M();
	return manager;
}

#endif
