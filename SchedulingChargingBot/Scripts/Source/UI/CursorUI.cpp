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

	switch (_event.type)
	{
	case SDL_MOUSEMOTION:
	{
		//��ȡʵʱ���ָ��λ��
		cursorPosition.x = _event.motion.x;
		cursorPosition.y = _event.motion.y;

		//������ָ���Ƿ���ʵ����Χ��
		for (Robot* _robot : _cm->GetRobotList())
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
		for (Vehicle* _vehicle : _cm->GetVehicleList())
		{
			//��¼��ǰ�ؾߵ�Rect
			static SDL_Rect _vehicleRect;
			_vehicleRect.x = (int)(_vehicle->GetPosition().x - TILE_SIZE / 2);
			_vehicleRect.y = (int)(_vehicle->GetPosition().y - TILE_SIZE / 2);
			_vehicleRect.w = TILE_SIZE;
			_vehicleRect.h = TILE_SIZE;
			if (SDL_PointInRect(&cursorPosition, &_vehicleRect))
			{
				isTouchChargeable = true;
				//�ֲ�����飬�ڲ���return����
				return;
			}
			else
				isTouchChargeable = false;
		}
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		for (Robot* _robot : _cm->GetRobotList())
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
			if (SDL_PointInRect(&cursorPosition, &_vehicleRect))
			{
				_vehicle->isCursorDragging = true;
				//����switch����case�ڵľֲ����򣬷�ֹѡ�ж��
				return;
			}
		}
	}
	break;
	case SDL_MOUSEBUTTONUP:
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
	}
	default:
		break;
	}
	//case SDL_MOUSEWHEEL
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