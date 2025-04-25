#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"

bool SceneManager::Init(SDL_Renderer* _renderer)
{
	//�決��Ƭ��ͼ����
	return GenerateTilemapTexture(_renderer);
}

void SceneManager::OnInput(const SDL_Event& _event)
{
	//���ָ���ƶ��¼�
	if (_event.type == SDL_MOUSEMOTION)
	{
		//��ȡ���ָ������
		cursorPosition.x = _event.motion.x;
		cursorPosition.y = _event.motion.y;
	}
}

void SceneManager::OnUpdate(double _delta)
{
}

void SceneManager::OnRender(SDL_Renderer* _renderer)
{
	//��mapTexture��Ⱦ��mapRect�Ӵ������и����������
	SDL_RenderCopy(_renderer, mapTexture, nullptr, &mapRect);
}

SDL_Point SceneManager::GetCursorPosition() const
{
	return cursorPosition;
}

SDL_Point SceneManager::GetCursorTileIdx() const
{
	static SDL_Point _pt;
	_pt.x = (cursorPosition.x % TILE_SIZE == 0) ? cursorPosition.x / TILE_SIZE - 1 : cursorPosition.x / TILE_SIZE;
	_pt.y = (cursorPosition.y % TILE_SIZE == 0) ? cursorPosition.y / TILE_SIZE - 1 : cursorPosition.y / TILE_SIZE;
	//std::cout << "CursorPosition=(" << cursorPosition.x << "," << cursorPosition.y << "),CursorTileIdx=(" << _pt.x << "," << _pt.y << ")\n";
	return _pt;
}

bool SceneManager::GenerateTilemapTexture(SDL_Renderer* _renderer)
{
	#pragma region PrepareMapTextureCanvas
	//��ȡ��Ϸ��ͼ��Ƭ��ͼ�ĳ�����
	const TileMap& _tileMap = map.GetTileMap();

	//������Ⱦ������Ƭ��ͼ��������
	int _tileMapWidth = (int)map.GetWidthTileNum() * TILE_SIZE;
	int _tileMapHeight = (int)map.GetHeightTileNum() * TILE_SIZE;

	//������Ҫ����Ⱦ����Ļ�ϵĵ�ͼ����ͼƬ���б༭
	//��һ��������Ⱦ��������Ϊ�������������ϻ�������������λ��������Ⱦ�����Ŀ��
	//�����Ų���ʽ��SDL_PIXELFORMAT_ARGB8888��8λ��0~255��Χ��Alpha��RGB��������Ȩ����SDL_TEXTUREACCESS_TARGET�������������Ϊ��������Ⱦ����Ⱦ��
	mapTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, _tileMapWidth, _tileMapHeight);
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
	SDL_SetRenderTarget(_renderer, mapTexture);

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
	for (int y = 0; y < map.GetHeightTileNum(); y++)
	{
		for (int x = 0; x < map.GetWidthTileNum(); x++)
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
			SDL_RenderCopy(_renderer, _tileSetTexture, &_src, &_dst);
			#pragma endregion
		}
	}

	//�ÿ���ȾĿ�꣨����Ⱦ���ڣ�
	SDL_SetRenderTarget(_renderer, nullptr);

	return true;
}