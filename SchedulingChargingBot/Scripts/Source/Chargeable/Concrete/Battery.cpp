#include "../../../Header/Chargeable/Concrete/Battery.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"

Battery::Battery()
{
	#pragma region SetAnimation
	//获取纹理数据
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Battery)->second;

	animIdle.SetLoop(true); animIdle.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharged.SetLoop(true); animCharged.SetAnimFrames(_sheet, 3, 1, { 1 });
	animCharger.SetLoop(true); animCharger.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//无法主动移动
	speed = 0;

	//初始化当前电量为最大电量
	currentElectricity = 100.0;
	//电池不需要转换状态，只要放在那里充电就行，所以只有一个状态Charged
	isCharged = true;
}

void Battery::OnUpdate(double _delta)
{
	Chargeable::OnUpdate(_delta);
	movingDrainTimer.OnUpdate(_delta);

	#pragma region ChangeState
	static ChargeableManager* _cm = ChargeableManager::Instance();
	static const std::map<size_t, SDL_Rect>& _stationRects = SceneManager::Instance()->map.GetStationRects();
	#pragma endregion

	#pragma region ChargedRect
	//可以换电的区域大小是2*2瓦片区域。
	chargedRect.x = (int)(position.x - (TILE_SIZE + TILE_SIZE / 2));
	chargedRect.y = (int)(position.y - (TILE_SIZE + TILE_SIZE / 2));
	chargedRect.w = TILE_SIZE * 2;
	chargedRect.h = TILE_SIZE * 2;
	#pragma endregion
}

void Battery::ChangeState(std::string state)
{
	//抽象类必须取消纯虚函数定义才能实例化，这里给一个默认实现，即什么也不写
}

bool Battery::IsBusy() const
{
	//这里必须返回一个值，所以随便写了
	return true;
}
