#include "../../../Header/Manager/Concrete/GameManager.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <ctime>
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Manager/Concrete/UIManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/ScoreManager.h"

GameManager::GameManager()
{
	#pragma region SDL
	//��ʼ��SDL���������ϵͳ����ΪSDL_Init��������0��ʾ�ɹ������Դ˴���һ���������ȡ��
	InitAssert(!SDL_Init(SDL_INIT_EVERYTHING), u8"Failed To Init SDL");
	//��ʼ��SDL_ttf�⣻TTF_Init��������0��ʾ�ɹ�
	InitAssert(!TTF_Init(), u8"Failed To Init SDL_ttf");
	//��ʼ��SDL_image��ĸ�֧�ָ�ʽ��IMG_Init�������ط���ֵ��ʾ�ɹ�������ֱ�Ӵ��뼴��
	InitAssert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"Failed To Init SDL_image");
	//��ʼ��SDL_mixer���֧�ָ�ʽ��Mix_Init�������ط���ֵ��ʾ�ɹ�
	InitAssert(Mix_Init(MIX_INIT_MP3), u8"Failed To Init SDL_mixer");
	//�򿪻�������������Mix_OpenAudio(��Ƶ������, ������Ƶ��ʽ, ������, ��Ƶ���뻺������С)
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	#pragma endregion

	#pragma region LoadData
	//���������ļ�
	ConfigManager* _config = ConfigManager::Instance();
	InitAssert(_config->LoadConfig("Data/Configs.json"), u8"Failed To Load Configs.json");
	InitAssert(_config->LoadMap("Data/Map.csv"), u8"Failed To Load Map.csv");
	#pragma endregion

	#pragma region Window&Renderer
	//����Ļ������ʾһ����������ض��ߴ��һ����ʽ�Ĵ���
	window = SDL_CreateWindow(_config->basicPrefab.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_config->basicPrefab.windowWidth, _config->basicPrefab.windowHeight, SDL_WINDOW_SHOWN);
	//��ⴰ���Ƿ��ʼ���ɹ�
	InitAssert(window, "Failed To Create Window");

	//������Ⱦ��������window�ϣ���������λʹ����������Ⱦ����
	//SDL_RENDERER_ACCELERATED��Ӳ������
	//SDL_RENDERER_PRESENTVSYNC����ֱͬ������һ��Ϊ��������Ļ˺�ѣ������ڴ�����Ƶ֡������ʾ��ˢ���ʲ�ͬ��������ʾ����
	//SDL_RENDERER_TARGETTEXTURE������ȾĿ������Ϊ����������Ⱦ����ͼƬ���ٽ�ͼƬ��Ⱦ�����ڣ�������Ƭ��ͼ������
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	//�����Ⱦ���Ƿ��ʼ���ɹ�
	InitAssert(renderer, "Failed To Create Renderer");
	#pragma endregion

	#pragma region LoadResource
	//������Դ
	InitAssert(ResourceManager::Instance()->LoadResource(renderer), u8"Failed To Load Resources");
	//���ɳ�������
	InitAssert(SceneManager::Instance()->Init(renderer), u8"Failed To Init SceneManager");
	#pragma endregion
}

GameManager::~GameManager()
{
	//ע������˳����������Ⱦ���봰��
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//����ͷſ�ĳ�ʼ�����ݣ���Ϊ���������������ڿ�����ڵ�
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

int GameManager::Run(int _argc, char** _argv)
{
	//���ù�ģ
	vehicleSpawner.LoadScenario(0);  // ������Ϊ 0=С��1=�У�2=��

	#pragma region LimitFPS
	//�˺�����ȡһ�������ܣ����Ƚϸߣ���ʱ�����������ص�ֵ����ʱ��������������Ϊ��ʱ������㣬ͨ����������Ƶ�ʲ�������
	Uint64 _lastCounter = SDL_GetPerformanceCounter();
	//Ƶ�ʼ�ÿһ��˼�ʱ���������ٴ�
	Uint64 _counterFreq = SDL_GetPerformanceFrequency();
	#pragma endregion

	//��Ϸ��ѭ��
	while (!isQuit)
	{
		#pragma region LimitFPS
		//��ȡ��ǰ�ļ�ʱ��������������������ѭ��ǰ�õ��ļ�ʱ����������
		Uint64 _currentCounter = SDL_GetPerformanceCounter();
		//�����ת��Ϊ˫���ȸ��㣬����Ƶ�ʵõ�ÿ��ѭ����ʱ��������λΪ�룩
		double _delta = (double)(_currentCounter - _lastCounter) / _counterFreq;
		//����ǰ�Ĵ�����Ϊ��㣬������һ��ѭ��
		_lastCounter = _currentCounter;
		//��̬��ʱ����֡�ʣ�����֡�ʳ������޶�ֵ����ô�ͽ������ʱ���ӳٵ���ֹ��ѭ��Ƶ�ʹ��죻����1000�ǽ���ת��Ϊ���룬��Ϊ�������λ̫������Ȳ���
		if (_delta * 1000 < 1000.0 / fps)
			SDL_Delay((Uint32)(1000.0 / fps - _delta * 1000));
		#pragma endregion

		//��ȡ�������¼��Ա�֤������������
		while (SDL_PollEvent(&event))
			OnInput();

		//���ݸ��¼��
		OnUpdate(_delta);

		#pragma region Render
		//ȷ����Ⱦ����ɫΪ���ڣ���͸����������RGB��ɫ��Alpha����¼ͼ���͸������Ϣ��256���Ҷȣ�
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//ʹ���趨�Ĳ�͸����ɫ����������ڴﵽ������Ч��
		SDL_RenderClear(renderer);

		//�ھ���������׼���󣬽��о������Ⱦ��ͼ
		OnRender();
		//����Ⱦ�����ݸ��µ����ڻ�������
		SDL_RenderPresent(renderer);
		#pragma endregion

		ScoreManager scmgr;
		double output_time = scmgr.score_timer();

	}

	return 0;
}

void GameManager::InitAssert(bool _flag, const char* _errMsg)
{
	//�����ʼ���ɹ�����ô���·�����ֱ�ӷ���
	if (_flag) return;
	//�����ʼ��ʧ�ܣ���ô����SDL����һ��������Ϣ���ڣ��ú���(��Ϣ����, ���ڱ���, ��ϸ������Ϣ, ��Ϊ˭���Ӵ���)
	//���ĸ����������NULL����ô�����ڲ���Ϊ�Ӵ��ڣ������д��봰�ڣ���ô�����ڽ���Ϊ����Ĵ��ڵ��Ӵ��ڣ����Ӵ��ڲ��ص����������޷����û�����
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Game Init Failed", _errMsg, window);
	//��-1������������
	exit(-1);
}

void GameManager::OnInput()
{
	//������ڵ��˳���ʱ������SDL_QUIT�¼�
	if (event.type == SDL_QUIT)
		isQuit = true;

	//��ͼ�е����λ�ü��
	SceneManager::Instance()->OnInput(event);
	//UI�������İ�������
	UIManager::Instance()->OnInput(event);
}

void GameManager::OnUpdate(double _delta)
{
	vehicleSpawner.OnUpdate(_delta); 

	//���¸�����������
	SceneManager::Instance()->OnUpdate(_delta);
	ChargeableManager::Instance()->OnUpdate(_delta);
	UIManager::Instance()->OnUpdate(renderer);
}

void GameManager::OnRender()
{
	//������ͼ����Ⱦ
	SceneManager::Instance()->OnRender(renderer);
	//UIӦ�������Ⱦ���Ա�֤ʼ�������ϲ�
	UIManager::Instance()->OnRender(renderer);
	//�Ҳ�����RimUI��סChargeableManager����Ⱦ
	ChargeableManager::Instance()->OnRender(renderer);
}