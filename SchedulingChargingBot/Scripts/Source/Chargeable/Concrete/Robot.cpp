#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"

Robot::Robot()
{
	#pragma region SetAnimation
	//��ȡ��������
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

	//�����˻ᶯ�������ٶȸ�����λ��
	position += velocity * _delta;
}