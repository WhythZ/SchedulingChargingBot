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
		//获取实时鼠标指针位置
		cursorPosition.x = _event.motion.x;
		cursorPosition.y = _event.motion.y;

		//检测鼠标指针是否在实例范围内
		for (Robot* _robot : _cm->GetRobotList())
		{
			//记录当前机器人的Rect
			static SDL_Rect _robotRect;
			_robotRect.x = (int)(_robot->GetPosition().x - TILE_SIZE / 2);
			_robotRect.y = (int)(_robot->GetPosition().y - TILE_SIZE / 2);
			_robotRect.w = TILE_SIZE;
			_robotRect.h = TILE_SIZE;
			if (SDL_PointInRect(&cursorPosition, &_robotRect))
			{
				isTouchChargeable = true;
				//局部代码块，内部用return跳出
				return;
			}
			else
				isTouchChargeable = false;
		}
		for (Vehicle* _vehicle : _cm->GetVehicleList())
		{
			//记录当前载具的Rect
			static SDL_Rect _vehicleRect;
			_vehicleRect.x = (int)(_vehicle->GetPosition().x - TILE_SIZE / 2);
			_vehicleRect.y = (int)(_vehicle->GetPosition().y - TILE_SIZE / 2);
			_vehicleRect.w = TILE_SIZE;
			_vehicleRect.h = TILE_SIZE;
			if (SDL_PointInRect(&cursorPosition, &_vehicleRect))
			{
				isTouchChargeable = true;
				//局部代码块，内部用return跳出
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
			//记录当前机器人的Rect
			static SDL_Rect _robotRect;
			_robotRect.x = (int)(_robot->GetPosition().x - TILE_SIZE / 2);
			_robotRect.y = (int)(_robot->GetPosition().y - TILE_SIZE / 2);
			_robotRect.w = TILE_SIZE;
			_robotRect.h = TILE_SIZE;
			//如果鼠标处于该Rect内，则选取当前这个（一次最多选取一个）
			if (SDL_PointInRect(&cursorPosition, &_robotRect))
			{
				_robot->isCursorDragging = true;
				//跳出switch语句该case内的局部区域，防止选中多个
				return;
			}
		}
		for (Vehicle* _vehicle : _cm->GetVehicleList())
		{
			//记录当前载具的Rect
			static SDL_Rect _vehicleRect;
			_vehicleRect.x = (int)(_vehicle->GetPosition().x - TILE_SIZE / 2);
			_vehicleRect.y = (int)(_vehicle->GetPosition().y - TILE_SIZE / 2);
			_vehicleRect.w = TILE_SIZE;
			_vehicleRect.h = TILE_SIZE;
			//如果鼠标处于该Rect内，则选取当前这个（一次最多选取一个）
			if (SDL_PointInRect(&cursorPosition, &_vehicleRect))
			{
				_vehicle->isCursorDragging = true;
				//跳出switch语句该case内的局部区域，防止选中多个
				return;
			}
		}
	}
	break;
	case SDL_MOUSEBUTTONUP:
	{
		//遍历所有实例找到被拖拽的那个，取消拖拽，并将其归正对齐到鼠标所在瓦片位置
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

	//计算目标瓦片的中心点位置
	cursorTilePosition = { _tileIdx.x * TILE_SIZE + TILE_SIZE / 2,_tileIdx.y * TILE_SIZE + TILE_SIZE / 2 };
}

void CursorUI::OnRender(SDL_Renderer* _renderer)
{
	//引入纹理渲染相关方法
	static UIManager* _ui = UIManager::Instance();

	//复用的左上顶点坐标
	static SDL_Point _positionLeftUp = { 0,0 };

	//地图Rect
	static SDL_Rect _mapRect = SceneManager::Instance()->mapRect;

	#pragma region CursorHover
	//例外情况，不渲染悬停框
	if (!isTouchChargeable)
	{
		_positionLeftUp.x = _mapRect.x + (int)(cursorTilePosition.x - TILE_SIZE / 2);
		_positionLeftUp.y = _mapRect.y + (int)(cursorTilePosition.y - TILE_SIZE / 2);
		_ui->DrawTexture(_renderer, TextureResID::CursorHover, _positionLeftUp, { TILE_SIZE,TILE_SIZE });
	}
	#pragma endregion
}