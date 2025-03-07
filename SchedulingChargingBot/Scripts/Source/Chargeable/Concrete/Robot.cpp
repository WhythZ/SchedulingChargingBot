#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/GameManager.h"

Robot::Robot()
{
	#pragma region SetAnimation
	//获取纹理数据
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Robot)->second;

	animIdling.SetLoop(true); animIdling.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharging.SetLoop(true); animCharging.SetAnimFrames(_sheet, 3, 1, { 1 });
	animDischarging.SetLoop(true); animDischarging.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion
}

void Robot::SetVelocity(Vector2 _velocity)
{
	velocity = _velocity;
}

void Robot::OnUpdate(double _delta)
{
	Chargeable::OnUpdate(_delta);

	//机器人会动，若未被拖拽，则依据速度更新其位置
	static GameManager* _gm = GameManager::Instance();
	if (isCursorDragging)
	{
		position.x = (double)(_gm->GetCursorPosition().x);
		position.y = (double)(_gm->GetCursorPosition().y);
	}
	else
		position += velocity * _delta;

	//防止跑出地图边界
	static SDL_Rect _mapRect = ConfigManager::Instance()->mapRect;
	if (position.x <= _mapRect.x) position.x = _mapRect.x;
	if (position.x >= _mapRect.x + _mapRect.w) position.x = _mapRect.x + _mapRect.w;
	if (position.y <= _mapRect.y) position.y = _mapRect.y;
	if (position.y >= _mapRect.y + _mapRect.h) position.y = _mapRect.y + _mapRect.h;
}