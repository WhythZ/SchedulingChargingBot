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
	//初始化SDL库的所有子系统；因为SDL_Init函数返回0表示成功，所以此处第一个传入参数取反
	InitAssert(!SDL_Init(SDL_INIT_EVERYTHING), u8"Failed To Init SDL");
	//初始化SDL_ttf库；TTF_Init函数返回0表示成功
	InitAssert(!TTF_Init(), u8"Failed To Init SDL_ttf");
	//初始化SDL_image库的各支持格式；IMG_Init函数返回非零值表示成功，所以直接传入即可
	InitAssert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"Failed To Init SDL_image");
	//初始化SDL_mixer库的支持格式；Mix_Init函数返回非零值表示成功
	InitAssert(Mix_Init(MIX_INIT_MP3), u8"Failed To Init SDL_mixer");
	//打开混音器的声道，Mix_OpenAudio(音频采样率, 解码音频格式, 声道数, 音频解码缓冲区大小)
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	#pragma endregion

	#pragma region LoadData
	//加载配置文件
	ConfigManager* _config = ConfigManager::Instance();
	InitAssert(_config->LoadConfig("Data/Configs.json"), u8"Failed To Load Configs.json");
	InitAssert(_config->LoadMap("Data/Map.csv"), u8"Failed To Load Map.csv");
	#pragma endregion

	#pragma region Window&Renderer
	//从屏幕中心显示一个带标题的特定尺寸的一般样式的窗口
	window = SDL_CreateWindow(_config->basicPrefab.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_config->basicPrefab.windowWidth, _config->basicPrefab.windowHeight, SDL_WINDOW_SHOWN);
	//检测窗口是否初始化成功
	InitAssert(window, "Failed To Create Window");

	//加载渲染器到窗口window上，第三参数位使用了三种渲染技术
	//SDL_RENDERER_ACCELERATED：硬件加速
	//SDL_RENDERER_PRESENTVSYNC：垂直同步，是一种为了消除屏幕撕裂（产生于窗口视频帧率与显示器刷新率不同步）的显示技术
	//SDL_RENDERER_TARGETTEXTURE：将渲染目标设置为纹理，即先渲染纹理图片，再将图片渲染到窗口，用于瓦片地图的生成
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	//检测渲染器是否初始化成功
	InitAssert(renderer, "Failed To Create Renderer");
	#pragma endregion

	#pragma region LoadResource
	//加载资源
	InitAssert(ResourceManager::Instance()->LoadResource(renderer), u8"Failed To Load Resources");
	//生成场景纹理
	InitAssert(SceneManager::Instance()->Init(renderer), u8"Failed To Init SceneManager");
	#pragma endregion
}

GameManager::~GameManager()
{
	//注意析构顺序，先销毁渲染器与窗口
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//最后释放库的初始化内容，因为上面两个是依赖于库而存在的
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

int GameManager::Run(int _argc, char** _argv)
{
	//设置规模
	vehicleSpawner.LoadScenario(0);  // 可设置为 0=小，1=中，2=大

	#pragma region LimitFPS
	//此函数获取一个高性能（精度较高）计时器，函数返回的值（计时器跳的总数）作为计时器的起点，通过作差后除以频率才有意义
	Uint64 _lastCounter = SDL_GetPerformanceCounter();
	//频率即每一秒此计时器会跳多少次
	Uint64 _counterFreq = SDL_GetPerformanceFrequency();
	#pragma endregion

	//游戏主循环
	while (!isQuit)
	{
		#pragma region LimitFPS
		//获取当前的计时器跳的总数，用以与主循环前得到的计时器总数作差
		Uint64 _currentCounter = SDL_GetPerformanceCounter();
		//作差后转换为双精度浮点，除以频率得到每次循环的时间间隔（单位为秒）
		double _delta = (double)(_currentCounter - _lastCounter) / _counterFreq;
		//将当前的次数作为起点，进行下一次循环
		_lastCounter = _currentCounter;
		//动态延时控制帧率，若是帧率超过了限定值，那么就将多余的时间延迟掉防止主循环频率过快；乘以1000是将秒转化为毫秒，因为秒这个单位太大而精度不高
		if (_delta * 1000 < 1000.0 / fps)
			SDL_Delay((Uint32)(1000.0 / fps - _delta * 1000));
		#pragma endregion

		//拉取并处理事件以保证窗口正常交互
		while (SDL_PollEvent(&event))
			OnInput();

		//数据更新检测
		OnUpdate(_delta);

		#pragma region Render
		//确定渲染的颜色为纯黑（不透明），接收RGB三色与Alpha（记录图像的透明度信息的256级灰度）
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//使用设定的不透明黑色填充整个窗口达到清屏的效果
		SDL_RenderClear(renderer);

		//在经历了上述准备后，进行具体的渲染绘图
		OnRender();
		//将渲染的内容更新到窗口缓冲区上
		SDL_RenderPresent(renderer);
		#pragma endregion

		ScoreManager scmgr;
		double output_time = scmgr.score_timer();

	}

	return 0;
}

void GameManager::InitAssert(bool _flag, const char* _errMsg)
{
	//如果初始化成功，那么无事发生，直接返回
	if (_flag) return;
	//如果初始化失败，那么就用SDL创建一个报错消息窗口，该函数(消息类型, 窗口标题, 详细报错信息, 作为谁的子窗口)
	//第四个参数如果填NULL，那么报错窗口不作为子窗口；若是有传入窗口，那么报错窗口将作为传入的窗口的子窗口，若子窗口不关掉，父窗口无法被用户交互
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Game Init Failed", _errMsg, window);
	//以-1结束程序运行
	exit(-1);
}

void GameManager::OnInput()
{
	//点击窗口的退出键时触发的SDL_QUIT事件
	if (event.type == SDL_QUIT)
		isQuit = true;

	//地图中的鼠标位置检测
	SceneManager::Instance()->OnInput(event);
	//UI管理器的按键输入
	UIManager::Instance()->OnInput(event);
}

void GameManager::OnUpdate(double _delta)
{
	vehicleSpawner.OnUpdate(_delta); 

	//更新各管理器数据
	SceneManager::Instance()->OnUpdate(_delta);
	ChargeableManager::Instance()->OnUpdate(_delta);
	UIManager::Instance()->OnUpdate(renderer);
}

void GameManager::OnRender()
{
	//场景地图的渲染
	SceneManager::Instance()->OnRender(renderer);
	//UI应当最后渲染的以保证始终在最上层
	UIManager::Instance()->OnRender(renderer);
	//我不想让RimUI遮住ChargeableManager的渲染
	ChargeableManager::Instance()->OnRender(renderer);
}