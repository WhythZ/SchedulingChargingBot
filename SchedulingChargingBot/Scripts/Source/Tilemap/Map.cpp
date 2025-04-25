#include "../../Header/Tilemap/Map.h"
#include <iostream>
#include <sstream>
#include <fstream>

bool Map::Load(const std::string& _csvPath)
{
	//载入文件，并判断该路径文件是否能打开
	std::ifstream _file(_csvPath);
	if (!_file.good()) return false;

	//临时的瓦片地图，若是读取失败则无需赋值给成员变量的瓦片地图
	TileMap _tileMapTemp;
	//记录当前每个被读取的瓦片应被读入tileMapTemp的哪个索引位置，y为行索引，x为列索引
	int _xIdx = -1, _yIdx = -1;

	//逐行进行实际的图格读取，每行含有多个瓦片
	std::string _lineBuf;
	//每次循环分别将文件的某行读取存放到lineBuf
	while (std::getline(_file, _lineBuf))
	{
		//将一整行瓦片数据字符串进行统一化修剪
		_lineBuf = TrimString(_lineBuf);
		//如果这一行为空，那么跳过这一行
		if (_lineBuf.empty())
			continue;

		//将行索引初始化为0，即从第一行开始读，每读一行都会递增以定位下一行
		_yIdx++;
		//std::vector<std::vector<Tile>>外圈的vector增加一个元素位（实际意义是临时地图开辟了一行预备进行line的数据读入）
		_tileMapTemp.emplace_back();

		//将每行的数据转化为流，不然无法被getline函数读取
		std::stringstream _lineBufStream(_lineBuf);
		//存储每个瓦片字符串数据
		std::string _tileBuf;
		//以逗号为间隔从一行字符串中读取单个瓦片字符串数据（"x\x\x\x"形式）后对每个瓦片字符串进行数据解析
		while (std::getline(_lineBufStream, _tileBuf, ','))
		{
			//第一次循环时，列坐标被初始化为-1+1=0，定位到第一行第一列的位置，每读取一个瓦片，列坐标都会递增以定位下一列也即下一个瓦片
			_xIdx++;
			//std::vector<std::vector<Tile>>内圈的vector增加一个元素位用于存储新的瓦片数据读入
			_tileMapTemp[_yIdx].emplace_back();

			//读取tileBuf字符串中的瓦片数据并载入临时地图对应瓦片上；也可以用tileMapTemp[yIdx].back()
			LoadTileFromString(_tileBuf, _tileMapTemp[_yIdx][_xIdx]);
		}

		//将_xIdx恢复原位
		_xIdx = -1;
	}

	//关闭文件
	_file.close();

	//对被加载完成的临时地图进行检验，此处对于全空的地图或者第一行为空的地图返回false表示加载失败
	if (_tileMapTemp.empty() || _tileMapTemp[0].empty())
		return false;
	//加载成功的话就可以放心赋值
	tileMap = _tileMapTemp;

	//在地图被加载完成后，生成地图缓存，并返回true结束函数
	GenerateMapCache();
	return true;
}

TileMap Map::GetTileMap() const
{
	return tileMap;
}

std::unordered_map<size_t, SDL_Point> Map::GetStationIdxPool() const
{
	return stationIdxPool;
}

std::unordered_map<size_t, SDL_Point> Map::GetVehicleIdxPool() const
{
	return vehicleIdxPool;
}

size_t Map::GetHeightTileNum() const
{
	if (tileMap.empty())
		return 0;
	//即返回行的数量
	return tileMap.size();
}

size_t Map::GetWidthTileNum() const
{
	if (tileMap.empty())
		return 0;
	//即返回第一行的列数
	return tileMap[0].size();
}

std::string Map::TrimString(const std::string _str)
//若字符串的首尾出现了空格，会很难被发现，所以提供一个剪切操作以统一，如"   x\x,y\y   "被剪切后得到"x\x,y\y"
{
	//找到字符串中的第一个非空字符（非空格、非制表符）的索引；注意使用的是size_t类型
	size_t _beginIdx = _str.find_first_not_of(" \t");
	//如果没有找到，那么返回空字符串
	if (_beginIdx == std::string::npos)
		return "";
	//找到字符串中的最后一个非空字符索引
	size_t _endIdx = _str.find_last_not_of(" \t");

	//不含空字符的字符串的长度
	size_t _idxRange = _endIdx - _beginIdx + 1;
	//返回从第一个非空字符算起、上述长度的字符串
	return _str.substr(_beginIdx, _idxRange);
}

void Map::LoadTileFromString(const std::string _tileBuf, Tile& _tile)
{
	//对这个字符串进行首尾空字符的去除
	std::string _trimmedTileBuf = TrimString(_tileBuf);

	//用一个数组临时存储被解析出来的数据
	std::vector<int> _values;

	//进行瓦片各层信息的录入；注意'\\'是转义字符
	std::stringstream _tileBufStream(_trimmedTileBuf);
	std::string _valueBuf;
	while (std::getline(_tileBufStream, _valueBuf, '\\'))
	{
		//临时用于存储读取到的瓦片成员的信息
		int _value;

		//无异常的话就正常进行类型转换
		try
		{
			//字符串转整型
			_value = std::stoi(_valueBuf);
		}
		//若是抛出了std::invalid_argument&异常（出现了非数值字符），就执行以下内部的语句
		catch (const std::invalid_argument&)
		{
			//在Tile类的成员变量的取值意义设定中，0代表Tileset中的首个瓦片，或是代表功能层的无功能
			_value = 0;
		}

		//将读取到的数值先后存入数组
		_values.push_back(_value);
	}

	//检测数组尺寸，空的话表示数据缺失；地形层的值小于0无意义，等于0表示取Tileset中首个纹理
	_tile.tilesetIdx = (_values.size() < 1 || _values[0] < 0) ? 0 : _values[0];
	//数组尺寸为1的话，说明功能层的数据缺失，否则等于0表无功能，正数生成车辆，负数充电桩
	_tile.functionFlag = (_values.size() < 2) ? 0 : _values[1];
}

void Map::GenerateMapCache()
//这种在编译阶段缓存（TileMap中存储的）静态数据的方法称为烘培
{
	//可复用的局部静态点
	static SDL_Point _pt;

	//遍历地图的每一个瓦片单元格
	for (int y = 0; y < GetHeightTileNum(); y++)
	{
		for (int x = 0; x < GetWidthTileNum(); x++)
		{
			//临时使用一个常量瓦片引用存储
			const Tile& _tile = tileMap[y][x];

			//检查功能层的值
			if (_tile.functionFlag == 0)
				continue;
			//记录车辆生成点的索引
			else if (_tile.functionFlag > 0)
				vehicleIdxPool[_tile.functionFlag] = { x,y };
			//记录充电桩的索引，注意取负号
			else
				stationIdxPool[-_tile.functionFlag] = { x,y };
		}
	}
}
