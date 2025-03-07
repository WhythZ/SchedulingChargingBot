#include "../../../Header/Manager/Concrete/GameManager.h"
//���塢ͼ�񡢻���֧��
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/UIManager.h"
#include "../../../Header/Manager/Concrete/RobotManager.h"
#include "../../../Header/Manager/Concrete/VehicleManager.h"
#include "../../../Header/Manager/Concrete/BatteryManager.h"

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
	InitAssert(GenerateTilemapTexture(), u8"Failed To Genrate Scene Texture");
	#pragma endregion

	//����ʵ����
	RobotManager::Instance()->SpawnAt({ 2,3 });
	VehicleManager::Instance()->SpawnAt({ 3,3 });
	BatteryManager::Instance()->SpawnAt({ 4,3 });
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
	}

	return 0;
}

SDL_Rect GameManager::GetMapRect() const
{
	return mapRect;
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
	//���ָ���ƶ��¼�
	if (event.type == SDL_MOUSEMOTION)
	{
		//��ȡ���ָ������
		cursorPosition.x = event.motion.x;
		cursorPosition.y = event.motion.y;
	}
}

void GameManager::OnUpdate(double _delta)
{
	//���¸�����������
	UIManager::Instance()->OnUpdate(renderer);
	RobotManager::Instance()->OnUpdate(_delta);
	VehicleManager::Instance()->OnUpdate(_delta);
	BatteryManager::Instance()->OnUpdate(_delta);
}

void GameManager::OnRender()
{
	#pragma region GameWindow
	//��mapTexture��Ⱦ��mapRect�Ӵ������и����������
	SDL_RenderCopy(renderer, mapTexture, nullptr, &mapRect);
	#pragma endregion

	RobotManager::Instance()->OnRender(renderer);
	VehicleManager::Instance()->OnRender(renderer);
	BatteryManager::Instance()->OnRender(renderer);
	//UIӦ�������Ⱦ���Ա�֤ʼ�������ϲ�
	UIManager::Instance()->OnRender(renderer);
}

bool GameManager::GenerateTilemapTexture()
{
	#pragma region PrepareMapTextureCanvas
	//��ȡ��Ϸ�ĵ�ͼ����Ƭ��ͼ�ĳ�����
	const Map& _map = ConfigManager::Instance()->map;
	const TileMap& _tileMap = _map.GetTileMap();

	//������Ⱦ������Ƭ��ͼ��������
	int _tileMapWidth = (int)_map.GetWidthTileNum() * TILE_SIZE;
	int _tileMapHeight = (int)_map.GetHeightTileNum() * TILE_SIZE;

	//������Ҫ����Ⱦ����Ļ�ϵĵ�ͼ����ͼƬ���б༭
	//��һ��������Ⱦ��������Ϊ�������������ϻ�������������λ��������Ⱦ�����Ŀ��
	//�����Ų���ʽ��SDL_PIXELFORMAT_ARGB8888��8λ��0~255��Χ��Alpha��RGB��������Ȩ����SDL_TEXTUREACCESS_TARGET�������������Ϊ��������Ⱦ����Ⱦ��
	mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, _tileMapWidth, _tileMapHeight);
	//��Ⱦʧ�ܣ�����ͼƬ̫���Դ治�����򷵻�false
	if (!mapTexture) return false;
	#pragma endregion

	#pragma region PrepareMapRect
	//���ĵ�ͼ�������Ⱦλ�ã�SDL_Rect���󣺳�Աx��y��ʾ����ͼƬ�ľ������ϽǶ�������ꡢ��Աw��h��ʾ���εĿ�ߣ�
	//���ö���õģ��̶��ģ����ڿ�ȼ�ȥǶ�ڴ����м�ľ��ε�ͼ�Ŀ�ȣ��ο����ء��ֵĽṹ�����ٳ���2���ɵõ���ͼ����������϶���ĺ����꣬������ͬ��
	mapRect.x = (ConfigManager::Instance()->basicPrefab.windowWidth - _tileMapWidth) / 2;
	mapRect.y = (ConfigManager::Instance()->basicPrefab.windowHeight - _tileMapHeight) / 2;
	//��ߵ�ֵ�Ǿ��Եģ�x��y�����ʾ�ĵ��λ��������ڴ��ڵģ�������ֱ�Ӹ�ֵ����
	mapRect.w = _tileMapWidth;
	mapRect.h = _tileMapHeight;
	#pragma endregion

	//���õ�ͼ����Ļ����Ⱦģʽ��SDL_BLENDMODE_BLEND������͸���ȵ���Ⱦ��
	SDL_SetTextureBlendMode(mapTexture, SDL_BLENDMODE_BLEND);
	//����Ⱦ��renderer����ȾĿ��ָ����mapTexture�ϣ���ʹ����ɺ�һ��Ҫ�ǵ��ÿ�Ŀ�굽nullptr�ϣ�
	SDL_SetRenderTarget(renderer, mapTexture);

	#pragma region PrepareSegmentTileSetTexture
	//��ȡTileSet������ͼƬ׼����������и�ȡ��
	//������ResourceManager::GetInstance()->GetSpritePool()[SpriteResID::Tile_TileSet]��ȡ���ڵ���Դ����
	//��Ϊ������û��Ŀ��ID�Ļ���unordered_map�ͻᴴ��һ������GetSpritePool()���ص���const����Դ�أ��ǲ������������ܲ����ĸ��ĵ�
	//������find(XXX)���صľ�����XXXΪ���ļ�ֵ�ԣ�->secondָ����ȡ���ü�ֵ�Ե�ֵ
	SDL_Texture* _tileSetTexture = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tileset)->second;

	//�洢TileSet�Ŀ��
	int _tileSetWidth, _tileSetHeight;
	//��ѯ�������ԣ���һ���������루�����ʵģ������ָ�룬�ڶ�������λ������������ʽ�����Ȩ�ޣ��ò������ÿռ��ɣ�
	//�����崫���ߵĵ�ַ�������ʵ��������Ŀ����Ϣ�ͻᱻ�洢����Ӧ�Ĵ����ַ��
	SDL_QueryTexture(_tileSetTexture, nullptr, nullptr, &_tileSetWidth, &_tileSetHeight);

	//�����TileSet��һ�п�����ȡ������Ƭ������ͼƬ�Ŀ�ȡ���TILE_SIZE�Ĵ�С�йأ��˴���������ͨ�õ�ͼƬ�и��㷨����������չ��
	int _rowTileNum = (int)std::ceil((double)(_tileSetWidth / TILE_SIZE));
	//�����TileSet��һ�п�����ȡ������Ƭ
	int _columnTileNum = (int)std::ceil((double)(_tileSetHeight / TILE_SIZE));
	#pragma endregion

	//����Ƭ��mapTexture������Ⱦ
	for (int y = 0; y < _map.GetHeightTileNum(); y++)
	{
		for (int x = 0; x < _map.GetWidthTileNum(); x++)
		{
			//�洢TileSet�еĶ�Ӧ��Ƭ�زĵľ�������ͼƬ��λ�ã��������ϽǶ�������xy+���ο��wh��
			SDL_Rect _src;
			//����������Ƭ��ͼ�е���Ƭ����ָ����Ӧ����ȾTileSet�е��ض��ز�����
			const Tile& _tile = _tileMap[y][x];

			//_src������Ⱦ��mapTexture�е�_dstλ����
			const SDL_Rect& _dst =
			{
				//Ŀ����Ƭ�������ϽǶ��������
				x * TILE_SIZE, y * TILE_SIZE,
				//��Ƭ�Ǳ߳��̶���������
				TILE_SIZE, TILE_SIZE
			};

			#pragma region RenderTiles
			//����_tile�ڴ洢����Ϣ������Ӧ����β�ȡTileSet�Ի�ȡ��ȷ��ͼƬ
			_src =
			{
				//����TileSetһ����5����Ƭ��������Ϊ11������0��ʾ��һ����Ƭ������ô��Ҫȡ��3�У�11/5=2���ĵ�2�У�11%5=1����Ƭ��Ϊ��ȾĿ��
				(_tile.tilesetIdx % _rowTileNum) * TILE_SIZE,
				(_tile.tilesetIdx / _rowTileNum) * TILE_SIZE,
				//��߹̶����䣬ûɶ��˵��
				TILE_SIZE, TILE_SIZE
			};
			//���������_src������Ⱦ��Ŀ������_dst��
			//�ڶ�����λ�Ĳ���������SDL_Texture*����������λ��SDL_Rect*������ǰ�ߵĲü�
			//���Ĳ���λ��SDL_Rect*�������Ƕ�mapTexture��Ҳ��SDL_Texture*���Ĳü�
			SDL_RenderCopy(renderer, _tileSetTexture, &_src, &_dst);
			#pragma endregion
		}
	}

	//�ÿ���ȾĿ�꣨����Ⱦ���ڣ�
	SDL_SetRenderTarget(renderer, nullptr);

	return true;
}