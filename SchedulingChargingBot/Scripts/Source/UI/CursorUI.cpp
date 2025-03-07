#include "../../Header/UI/CursorUI.h"
#include "../../Header/Manager/Concrete/GameManager.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/RobotManager.h"
#include "../../Header/Manager/Concrete/BatteryManager.h"
#include "../../Header/Manager/Concrete/VehicleManager.h"

void CursorUI::OnInput(const SDL_Event& _event)
{
	static RobotManager* _rm = RobotManager::Instance();
	static VehicleManager* _vm = VehicleManager::Instance();
	static BatteryManager* _bm = BatteryManager::Instance();

	switch (_event.type)
	{
	case SDL_MOUSEMOTION:
	{
		//��ȡʵʱ���ָ��λ��
		cursorPosition.x = _event.motion.x;
		cursorPosition.y = _event.motion.y;

		//������ָ���Ƿ���ʵ����Χ��
		for (Robot* _robot : _rm->GetRobotList())
		{
			//��¼��ǰ�����˵�Rect
			static SDL_Rect _robotRect;
			_robotRect.x = (int)(_robot->GetPosition().x - TILE_SIZE / 2);
			_robotRect.y = (int)(_robot->GetPosition().y - TILE_SIZE / 2);
			_robotRect.w = TILE_SIZE;
			_robotRect.h = TILE_SIZE;
			if (SDL_PointInRect(&cursorPosition, &_robotRect))
			{
				isTouchChargeable = true;
				//�ֲ�����飬�ڲ���return����
				return;
			}
			else
				isTouchChargeable = false;
		}
		for (Vehicle* _vehicle : _vm->GetVehicleList())
		{
			static SDL_Rect _vehicleRect;
			_vehicleRect.x = (int)(_vehicle->GetPosition().x - TILE_SIZE / 2);
			_vehicleRect.y = (int)(_vehicle->GetPosition().y - TILE_SIZE / 2);
			_vehicleRect.w = TILE_SIZE;
			_vehicleRect.h = TILE_SIZE;
			if (SDL_PointInRect(&cursorPosition, &_vehicleRect))
			{
				isTouchChargeable = true;
				return;
			}
			else
				isTouchChargeable = false;
		}
		for (Battery* _battery : _bm->GetBatteryList())
		{
			static SDL_Rect _batteryRect;
			_batteryRect.x = (int)(_battery->GetPosition().x - TILE_SIZE / 2);
			_batteryRect.y = (int)(_battery->GetPosition().y - TILE_SIZE / 2);
			_batteryRect.w = TILE_SIZE;
			_batteryRect.h = TILE_SIZE;
			if (SDL_PointInRect(&cursorPosition, &_batteryRect))
			{
				isTouchChargeable = true;
				return;
			}
			else
				isTouchChargeable = false;
		}
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		//�������л�����
		for (Robot* _robot : _rm->GetRobotList())
		{
			//��¼��ǰ�����˵�Rect
			static SDL_Rect _robotRect;
			_robotRect.x = (int)(_robot->GetPosition().x - TILE_SIZE / 2);
			_robotRect.y = (int)(_robot->GetPosition().y - TILE_SIZE / 2);
			_robotRect.w = TILE_SIZE;
			_robotRect.h = TILE_SIZE;
			//�����괦�ڸ�Rect�ڣ���ѡȡ��ǰ�����һ�����ѡȡһ����
			if (SDL_PointInRect(&cursorPosition, &_robotRect))
			{
				_robot->isCursorDragging = true;
				//��������ֹѡ�ж��
				return;
			}
		}
	}
	break;
	case SDL_MOUSEBUTTONUP:
	{
		//�������л����ˣ�ȡ����ק
		for (Robot* _robot : _rm->GetRobotList())
			_robot->isCursorDragging = false;
	}
	default:
		break;
	}
}

void CursorUI::OnUpdate(SDL_Renderer* _renderer)
{
	static GameManager* _gm = GameManager::Instance();
	SDL_Point _tileIdx = _gm->GetCursorTileIdx();

	//����Ŀ����Ƭ�����ĵ�λ��
	cursorTilePosition = { _tileIdx.x * TILE_SIZE + TILE_SIZE / 2,_tileIdx.y * TILE_SIZE + TILE_SIZE / 2 };
}

void CursorUI::OnRender(SDL_Renderer* _renderer)
{
	//����������Ⱦ��ط���
	static UIManager* _ui = UIManager::Instance();

	//���õ����϶�������
	static SDL_Point _positionLeftUp = { 0,0 };

	//��ͼRect
	static SDL_Rect _mapRect = ConfigManager::Instance()->mapRect;

	#pragma region CursorHover
	//�������������Ⱦ��ͣ��
	if (!isTouchChargeable)
	{
		_positionLeftUp.x = _mapRect.x + (int)(cursorTilePosition.x - TILE_SIZE / 2);
		_positionLeftUp.y = _mapRect.y + (int)(cursorTilePosition.y - TILE_SIZE / 2);
		_ui->DrawTexture(_renderer, TextureResID::CursorHover, _positionLeftUp, { TILE_SIZE,TILE_SIZE });
	}
	#pragma endregion
}