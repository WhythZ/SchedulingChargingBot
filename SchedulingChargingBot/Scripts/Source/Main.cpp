//����������"C/C++"��"��������"�µ�"���п�"����Ӧ��Ϊ"���߳�(/MT)"����ȷ����̬��������ʱ��

#include "../Header/Manager/Concrete/GameManager.h"

//���˺궨�壬����main������Ԥ����׶λᱻdefineΪSDL_main
#define SDL_MAIN_HANDLED

int main(int _argc, char** _argv)
{
	srand(static_cast<unsigned int>(time(NULL)));
	return GameManager::Instance()->Run(_argc, _argv);
}