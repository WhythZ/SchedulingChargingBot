#include "../../../Header/Chargeable/Concrete/Battery.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"

Battery::Battery()
{
	#pragma region SetAnimation
	//��ȡ��������
	static SDL_Texture* _sheet = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Battery)->second;

	animIdle.SetLoop(true); animIdle.SetAnimFrames(_sheet, 3, 1, { 0 });
	animCharged.SetLoop(true); animCharged.SetAnimFrames(_sheet, 3, 1, { 1 });
	animCharger.SetLoop(true); animCharger.SetAnimFrames(_sheet, 3, 1, { 2 });
	#pragma endregion

	//�޷������ƶ�
	speed = 0;

	//��ʼ����ǰ����Ϊ������
	currentElectricity = 100.0;
	//��ز���Ҫת��״̬��ֻҪ������������У�����ֻ��һ��״̬Charged
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
	//���Ի���������С��2*2��Ƭ����
	chargedRect.x = (int)(position.x - (TILE_SIZE + TILE_SIZE / 2));
	chargedRect.y = (int)(position.y - (TILE_SIZE + TILE_SIZE / 2));
	chargedRect.w = TILE_SIZE * 2;
	chargedRect.h = TILE_SIZE * 2;
	#pragma endregion
}

void Battery::ChangeState(std::string state)
{
	//���������ȡ�����麯���������ʵ�����������һ��Ĭ��ʵ�֣���ʲôҲ��д
}

bool Battery::IsBusy() const
{
	//������뷵��һ��ֵ���������д��
	return true;
}
