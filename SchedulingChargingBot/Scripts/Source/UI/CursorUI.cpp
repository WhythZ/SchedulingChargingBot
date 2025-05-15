#include "../../Header/UI/CursorUI.h"
#include "../../Header/Manager/Concrete/GameManager.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../Header/Manager/Concrete/SceneManager.h"

void CursorUI::OnInput(const SDL_Event& _event)
{
	static ChargeableManager* _cm = ChargeableManager::Instance();
	static UIManager* _ui = UIManager::Instance();
	SDL_Point _cursorPosition = _ui->cursorPosition;

	switch (_event.type)
	{
	case SDL_MOUSEMOTION:
	{
		//������ָ���Ƿ���ʵ����Χ��
		for (Robot* _robot : _cm->GetRobotList())
		{
			//��¼��ǰ�����˵�Rect
			static SDL_Rect _robotRect;
			_robotRect.x = (int)(_robot->GetPosition().x - TILE_SIZE / 2);
			_robotRect.y = (int)(_robot->GetPosition().y - TILE_SIZE / 2);
			_robotRect.w = TILE_SIZE;
			_robotRect.h = TILE_SIZE;
			if (SDL_PointInRect(&_cursorPosition, &_robotRect))
			{
				isTouchChargeable = true;
				//�ֲ�����飬�ڲ���return����
				return;
			}
			else
				isTouchChargeable = false;
		}
		for (Vehicle* _vehicle : _cm->GetVehicleList())
		{
			//��¼��ǰ�ؾߵ�Rect
			static SDL_Rect _vehicleRect;
			_vehicleRect.x = (int)(_vehicle->GetPosition().x - TILE_SIZE / 2);
			_vehicleRect.y = (int)(_vehicle->GetPosition().y - TILE_SIZE / 2);
			_vehicleRect.w = TILE_SIZE;
			_vehicleRect.h = TILE_SIZE;
			if (SDL_PointInRect(&_cursorPosition, &_vehicleRect))
			{
				isTouchChargeable = true;
				//�ֲ�����飬�ڲ���return����
				return;
			}
			else
				isTouchChargeable = false;
		}
		for (Battery* _battery : _cm->GetBatteryList())
		{
			//��¼��ǰ�ؾߵ�Rect
			static SDL_Rect _batteryRect;
			_batteryRect.x = (int)(_battery->GetPosition().x - TILE_SIZE / 2);
			_batteryRect.y = (int)(_battery->GetPosition().y - TILE_SIZE / 2);
			_batteryRect.w = TILE_SIZE;
			_batteryRect.h = TILE_SIZE;
			if (SDL_PointInRect(&_cursorPosition, &_batteryRect))
			{
				isTouchChargeable = true;
				//�ֲ�����飬�ڲ���return����
				return;
			}
			else
				isTouchChargeable = false;
		}
	}
	//ע��˴�û��break�����������ƶ��������������������ǲ������
	case SDL_MOUSEBUTTONDOWN:
	{
		//���
		if (_event.button.button == SDL_BUTTON_LEFT)
		{
			//ѡ�ж��󣬽�����ק
			for (Robot* _robot : _cm->GetRobotList())
			{
				//��¼��ǰ�����˵�Rect
				static SDL_Rect _robotRect;
				_robotRect.x = (int)(_robot->GetPosition().x - TILE_SIZE / 2);
				_robotRect.y = (int)(_robot->GetPosition().y - TILE_SIZE / 2);
				_robotRect.w = TILE_SIZE;
				_robotRect.h = TILE_SIZE;
				//�����괦�ڸ�Rect�ڣ���ѡȡ��ǰ�����һ�����ѡȡһ����
				if (SDL_PointInRect(&_cursorPosition, &_robotRect))
				{
					_robot->isCursorDragging = true;
					//����switch����case�ڵľֲ����򣬷�ֹѡ�ж��
					return;
				}
			}
			for (Vehicle* _vehicle : _cm->GetVehicleList())
			{
				//��¼��ǰ�ؾߵ�Rect
				static SDL_Rect _vehicleRect;
				_vehicleRect.x = (int)(_vehicle->GetPosition().x - TILE_SIZE / 2);
				_vehicleRect.y = (int)(_vehicle->GetPosition().y - TILE_SIZE / 2);
				_vehicleRect.w = TILE_SIZE;
				_vehicleRect.h = TILE_SIZE;
				//�����괦�ڸ�Rect�ڣ���ѡȡ��ǰ�����һ�����ѡȡһ����
				if (SDL_PointInRect(&_cursorPosition, &_vehicleRect))
				{
					_vehicle->isCursorDragging = true;
					//����switch����case�ڵľֲ����򣬷�ֹѡ�ж��
					return;
				}
			}
			for (Battery* _battery : _cm->GetBatteryList())
			{
				//��¼��ǰ�ؾߵ�Rect
				static SDL_Rect _batteryRect;
				_batteryRect.x = (int)(_battery->GetPosition().x - TILE_SIZE / 2);
				_batteryRect.y = (int)(_battery->GetPosition().y - TILE_SIZE / 2);
				_batteryRect.w = TILE_SIZE;
				_batteryRect.h = TILE_SIZE;
				//�����괦�ڸ�Rect�ڣ���ѡȡ��ǰ�����һ�����ѡȡһ����
				if (SDL_PointInRect(&_cursorPosition, &_batteryRect))
				{
					_battery->isCursorDragging = true;
					//����switch����case�ڵľֲ����򣬷�ֹѡ�ж��
					return;
				}
			}
		}
	}
	break;
	case SDL_MOUSEBUTTONUP:
	{
		//���
		if (_event.button.button == SDL_BUTTON_LEFT)
		{
			//��������ʵ���ҵ�����ק���Ǹ���ȡ����ק��������������뵽���������Ƭλ��
			for (Robot* _robot : _cm->GetRobotList())
			{
				if (_robot->isCursorDragging)
				{
					_robot->isCursorDragging = false;
					_robot->SetPosition(cursorTilePosition.x, cursorTilePosition.y);
					return;
				}
			}
			for (Vehicle* _vehicle : _cm->GetVehicleList())
			{
				if (_vehicle->isCursorDragging)
				{
					_vehicle->isCursorDragging = false;
					_vehicle->SetPosition(cursorTilePosition.x, cursorTilePosition.y);
					return;
				}
			}
			for (Battery* _battery : _cm->GetBatteryList())
			{
				if (_battery->isCursorDragging)
				{
					_battery->isCursorDragging = false;
					_battery->SetPosition(cursorTilePosition.x, cursorTilePosition.y);
					return;
				}
			}
		}
	}
	break;
	case SDL_KEYDOWN:
	{
		//��ⰴ�µľ��尴��
		switch (_event.key.keysym.sym)
		{
		case SDLK_1:
			//����1�������������Ƭλ������һ��Robot
			_cm->SpawnChargeableAt(ChargeableType::Robot,
				{ cursorTilePosition.x / TILE_SIZE, cursorTilePosition.y / TILE_SIZE });
			break;
		case SDLK_2:
			//����2�������������Ƭλ������һ��Vehicle
			_cm->SpawnChargeableAt(ChargeableType::Vehicle,
				{ cursorTilePosition.x / TILE_SIZE, cursorTilePosition.y / TILE_SIZE });
			break;
		case SDLK_3:
			//����3�������������Ƭλ������һ��Vehicle
			_cm->SpawnChargeableAt(ChargeableType::Battery,
				{ cursorTilePosition.x / TILE_SIZE, cursorTilePosition.y / TILE_SIZE });
			break;
		}
	}
	break;
	default:
		break;
	}
}

void CursorUI::OnUpdate(SDL_Renderer* _renderer)
{
	static SceneManager* _sm = SceneManager::Instance();
	SDL_Point _tileIdx = _sm->GetCursorTileIdx();

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
	static SDL_Rect _mapRect = SceneManager::Instance()->mapRect;

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