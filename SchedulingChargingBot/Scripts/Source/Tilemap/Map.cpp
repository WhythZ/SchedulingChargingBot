#include "../../Header/Tilemap/Map.h"
#include <iostream>
#include <sstream>
#include <fstream>

bool Map::Load(const std::string& _csvPath)
{
	//�����ļ������жϸ�·���ļ��Ƿ��ܴ�
	std::ifstream _file(_csvPath);
	if (!_file.good()) return false;

	//��ʱ����Ƭ��ͼ�����Ƕ�ȡʧ�������踳ֵ����Ա��������Ƭ��ͼ
	TileMap _tileMapTemp;
	//��¼��ǰÿ������ȡ����ƬӦ������tileMapTemp���ĸ�����λ�ã�yΪ��������xΪ������
	int _xIdx = -1, _yIdx = -1;

	//���н���ʵ�ʵ�ͼ���ȡ��ÿ�к��ж����Ƭ
	std::string _lineBuf;
	//ÿ��ѭ���ֱ��ļ���ĳ�ж�ȡ��ŵ�lineBuf
	while (std::getline(_file, _lineBuf))
	{
		//��һ������Ƭ�����ַ�������ͳһ���޼�
		_lineBuf = TrimString(_lineBuf);
		//�����һ��Ϊ�գ���ô������һ��
		if (_lineBuf.empty())
			continue;

		//����������ʼ��Ϊ0�����ӵ�һ�п�ʼ����ÿ��һ�ж�������Զ�λ��һ��
		_yIdx++;
		//std::vector<std::vector<Tile>>��Ȧ��vector����һ��Ԫ��λ��ʵ����������ʱ��ͼ������һ��Ԥ������line�����ݶ��룩
		_tileMapTemp.emplace_back();

		//��ÿ�е�����ת��Ϊ������Ȼ�޷���getline������ȡ
		std::stringstream _lineBufStream(_lineBuf);
		//�洢ÿ����Ƭ�ַ�������
		std::string _tileBuf;
		//�Զ���Ϊ�����һ���ַ����ж�ȡ������Ƭ�ַ������ݣ�"x\x\x\x"��ʽ�����ÿ����Ƭ�ַ����������ݽ���
		while (std::getline(_lineBufStream, _tileBuf, ','))
		{
			//��һ��ѭ��ʱ�������걻��ʼ��Ϊ-1+1=0����λ����һ�е�һ�е�λ�ã�ÿ��ȡһ����Ƭ�������궼������Զ�λ��һ��Ҳ����һ����Ƭ
			_xIdx++;
			//std::vector<std::vector<Tile>>��Ȧ��vector����һ��Ԫ��λ���ڴ洢�µ���Ƭ���ݶ���
			_tileMapTemp[_yIdx].emplace_back();

			//��ȡtileBuf�ַ����е���Ƭ���ݲ�������ʱ��ͼ��Ӧ��Ƭ�ϣ�Ҳ������tileMapTemp[yIdx].back()
			LoadTileFromString(_tileBuf, _tileMapTemp[_yIdx][_xIdx]);
		}

		//��_xIdx�ָ�ԭλ
		_xIdx = -1;
	}

	//�ر��ļ�
	_file.close();

	//�Ա�������ɵ���ʱ��ͼ���м��飬�˴�����ȫ�յĵ�ͼ���ߵ�һ��Ϊ�յĵ�ͼ����false��ʾ����ʧ��
	if (_tileMapTemp.empty() || _tileMapTemp[0].empty())
		return false;
	//���سɹ��Ļ��Ϳ��Է��ĸ�ֵ
	tileMap = _tileMapTemp;

	//�ڵ�ͼ��������ɺ����ɵ�ͼ���棬������true��������
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
	//�������е�����
	return tileMap.size();
}

size_t Map::GetWidthTileNum() const
{
	if (tileMap.empty())
		return 0;
	//�����ص�һ�е�����
	return tileMap[0].size();
}

std::string Map::TrimString(const std::string _str)
//���ַ�������β�����˿ո񣬻���ѱ����֣������ṩһ�����в�����ͳһ����"   x\x,y\y   "�����к�õ�"x\x,y\y"
{
	//�ҵ��ַ����еĵ�һ���ǿ��ַ����ǿո񡢷��Ʊ������������ע��ʹ�õ���size_t����
	size_t _beginIdx = _str.find_first_not_of(" \t");
	//���û���ҵ�����ô���ؿ��ַ���
	if (_beginIdx == std::string::npos)
		return "";
	//�ҵ��ַ����е����һ���ǿ��ַ�����
	size_t _endIdx = _str.find_last_not_of(" \t");

	//�������ַ����ַ����ĳ���
	size_t _idxRange = _endIdx - _beginIdx + 1;
	//���شӵ�һ���ǿ��ַ������������ȵ��ַ���
	return _str.substr(_beginIdx, _idxRange);
}

void Map::LoadTileFromString(const std::string _tileBuf, Tile& _tile)
{
	//������ַ���������β���ַ���ȥ��
	std::string _trimmedTileBuf = TrimString(_tileBuf);

	//��һ��������ʱ�洢����������������
	std::vector<int> _values;

	//������Ƭ������Ϣ��¼�룻ע��'\\'��ת���ַ�
	std::stringstream _tileBufStream(_trimmedTileBuf);
	std::string _valueBuf;
	while (std::getline(_tileBufStream, _valueBuf, '\\'))
	{
		//��ʱ���ڴ洢��ȡ������Ƭ��Ա����Ϣ
		int _value;

		//���쳣�Ļ���������������ת��
		try
		{
			//�ַ���ת����
			_value = std::stoi(_valueBuf);
		}
		//�����׳���std::invalid_argument&�쳣�������˷���ֵ�ַ�������ִ�������ڲ������
		catch (const std::invalid_argument&)
		{
			//��Tile��ĳ�Ա������ȡֵ�����趨�У�0����Tileset�е��׸���Ƭ�����Ǵ����ܲ���޹���
			_value = 0;
		}

		//����ȡ������ֵ�Ⱥ��������
		_values.push_back(_value);
	}

	//�������ߴ磬�յĻ���ʾ����ȱʧ�����β��ֵС��0�����壬����0��ʾȡTileset���׸�����
	_tile.tilesetIdx = (_values.size() < 1 || _values[0] < 0) ? 0 : _values[0];
	//����ߴ�Ϊ1�Ļ���˵�����ܲ������ȱʧ���������0���޹��ܣ��������ɳ������������׮
	_tile.functionFlag = (_values.size() < 2) ? 0 : _values[1];
}

void Map::GenerateMapCache()
//�����ڱ���׶λ��棨TileMap�д洢�ģ���̬���ݵķ�����Ϊ����
{
	//�ɸ��õľֲ���̬��
	static SDL_Point _pt;

	//������ͼ��ÿһ����Ƭ��Ԫ��
	for (int y = 0; y < GetHeightTileNum(); y++)
	{
		for (int x = 0; x < GetWidthTileNum(); x++)
		{
			//��ʱʹ��һ��������Ƭ���ô洢
			const Tile& _tile = tileMap[y][x];

			//��鹦�ܲ��ֵ
			if (_tile.functionFlag == 0)
				continue;
			//��¼�������ɵ������
			else if (_tile.functionFlag > 0)
				vehicleIdxPool[_tile.functionFlag] = { x,y };
			//��¼���׮��������ע��ȡ����
			else
				stationIdxPool[-_tile.functionFlag] = { x,y };
		}
	}
}
