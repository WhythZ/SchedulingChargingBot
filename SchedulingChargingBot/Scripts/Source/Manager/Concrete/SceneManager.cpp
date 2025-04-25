#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"

bool SceneManager::Init(SDL_Renderer* _renderer)
{
	//烘焙瓦片地图纹理
	return GenerateTilemapTexture(_renderer);
}

void SceneManager::OnInput(const SDL_Event& _event)
{
	//鼠标指针移动事件
	if (_event.type == SDL_MOUSEMOTION)
	{
		//获取鼠标指针坐标
		cursorPosition.x = _event.motion.x;
		cursorPosition.y = _event.motion.y;
	}
}

void SceneManager::OnUpdate(double _delta)
{
}

void SceneManager::OnRender(SDL_Renderer* _renderer)
{
	//将mapTexture渲染在mapRect从窗口中切割出的区域内
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
	//获取游戏地图瓦片地图的常引用
	const TileMap& _tileMap = map.GetTileMap();

	//最终渲染出的瓦片地图的纹理宽高
	int _tileMapWidth = (int)map.GetWidthTileNum() * TILE_SIZE;
	int _tileMapHeight = (int)map.GetHeightTileNum() * TILE_SIZE;

	//对最终要被渲染在屏幕上的地图纹理图片进行编辑
	//第一参数即渲染器（即作为画笔在纹理画布上画画），第四五位参数即渲染画布的宽高
	//纹理排布格式是SDL_PIXELFORMAT_ARGB8888（8位即0~255范围的Alpha的RGB），访问权限是SDL_TEXTUREACCESS_TARGET（即纹理可以作为画布被渲染器渲染）
	mapTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, _tileMapWidth, _tileMapHeight);
	//渲染失败（比如图片太大，显存不够）则返回false
	if (!mapTexture) return false;
	#pragma endregion

	#pragma region PrepareMapRect
	//更改地图纹理的渲染位置（SDL_Rect对象：成员x和y表示纹理图片的矩形左上角顶点的坐标、成员w和h表示矩形的宽高）
	//利用定义好的（固定的）窗口宽度减去嵌在窗口中间的矩形地图的宽度（参考“回”字的结构）后再除以2即可得到地图纹理矩形左上定点的横坐标，纵坐标同理
	mapRect.x = (ConfigManager::Instance()->basicPrefab.windowWidth - _tileMapWidth) / 2;
	mapRect.y = (ConfigManager::Instance()->basicPrefab.windowHeight - _tileMapHeight) / 2;
	//宽高的值是绝对的（x和y坐标表示的点的位置是相对于窗口的），所以直接赋值即可
	mapRect.w = _tileMapWidth;
	mapRect.h = _tileMapHeight;
	#pragma endregion

	//设置地图纹理的混合渲染模式（SDL_BLENDMODE_BLEND即启用透明度的渲染）
	SDL_SetTextureBlendMode(mapTexture, SDL_BLENDMODE_BLEND);
	//将渲染器renderer的渲染目标指定到mapTexture上（在使用完成后一定要记得置空目标到nullptr上）
	SDL_SetRenderTarget(_renderer, mapTexture);

	#pragma region PrepareSegmentTileSetTexture
	//获取TileSet的纹理图片准备对其进行切割取材
	//不能用ResourceManager::GetInstance()->GetSpritePool()[SpriteResID::Tile_TileSet]获取池内的资源对象
	//因为若池内没有目标ID的话，unordered_map就会创建一个，而GetSpritePool()返回的是const的资源池，是不允许上述可能产生的更改的
	//而函数find(XXX)返回的就是以XXX为键的键值对，->second指的是取出该键值对的值
	SDL_Texture* _tileSetTexture = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tileset)->second;

	//存储TileSet的宽高
	int _tileSetWidth, _tileSetHeight;
	//查询纹理属性，第一个参数传入（被访问的）纹理的指针，第二三参数位传入的是纹理格式与访问权限（用不到就置空即可）
	//第四五传入宽高的地址，被访问的纹理对象的宽高信息就会被存储到对应的传入地址上
	SDL_QueryTexture(_tileSetTexture, nullptr, nullptr, &_tileSetWidth, &_tileSetHeight);

	//计算该TileSet的一行可以提取几个瓦片（这与图片的宽度、宏TILE_SIZE的大小有关，此处并不是最通用的图片切割算法，后续可扩展）
	int _rowTileNum = (int)std::ceil((double)(_tileSetWidth / TILE_SIZE));
	//计算该TileSet的一列可以提取几个瓦片
	int _columnTileNum = (int)std::ceil((double)(_tileSetHeight / TILE_SIZE));
	#pragma endregion

	//逐瓦片对mapTexture进行渲染
	for (int y = 0; y < map.GetHeightTileNum(); y++)
	{
		for (int x = 0; x < map.GetWidthTileNum(); x++)
		{
			//存储TileSet中的对应瓦片素材的矩形纹理图片的位置（矩形左上角顶点坐标xy+矩形宽高wh）
			SDL_Rect _src;
			//被遍历的瓦片地图中的瓦片，其指定了应当渲染TileSet中的特定素材纹理
			const Tile& _tile = _tileMap[y][x];

			//_src将被渲染到mapTexture中的_dst位置上
			const SDL_Rect& _dst =
			{
				//目标瓦片矩形左上角顶点的坐标
				x * TILE_SIZE, y * TILE_SIZE,
				//瓦片是边长固定的正方形
				TILE_SIZE, TILE_SIZE
			};

			#pragma region RenderTiles
			//根据_tile内存储的信息，决定应当如何裁取TileSet以获取正确的图片
			_src =
			{
				//设想TileSet一行有5列瓦片，若索引为11（索引0表示第一个瓦片），那么就要取第3行（11/5=2）的第2列（11%5=1）瓦片作为渲染目标
				(_tile.tilesetIdx % _rowTileNum) * TILE_SIZE,
				(_tile.tilesetIdx / _rowTileNum) * TILE_SIZE,
				//宽高固定不变，没啥好说的
				TILE_SIZE, TILE_SIZE
			};
			//将从纹理块_src拷贝渲染到目标区块_dst上
			//第二参数位的参数类型是SDL_Texture*，第三参数位是SDL_Rect*，即对前者的裁剪
			//第四参数位的SDL_Rect*对象则是对mapTexture（也是SDL_Texture*）的裁剪
			SDL_RenderCopy(_renderer, _tileSetTexture, &_src, &_dst);
			#pragma endregion
		}
	}

	//置空渲染目标（即渲染窗口）
	SDL_SetRenderTarget(_renderer, nullptr);

	return true;
}