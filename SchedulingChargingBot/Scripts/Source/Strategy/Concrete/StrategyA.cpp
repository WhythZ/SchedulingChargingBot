#include "../../../Header/Strategy/Concrete/StrategyA.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"

void StrategyA::UpdateMovement(Chargeable* _chargeable)
{
    Robot* _robot = dynamic_cast<Robot*>(_chargeable);

    Vector2 _robotPos = _robot->GetPosition();
    //������׮λ��
    Vector2 _pilePos;
    _pilePos.x = _robotPos.x >= 640 ? 1280 : 0;
    _pilePos.y = _robotPos.y >= 448 ? 896 : 0;
    Vector2 _returnPath = _pilePos - _robotPos;

    if(!_robot->IsBusy())
    {
        Vector2 _pileVelocity;
        _pileVelocity.x = _returnPath.x / _returnPath.Length() * _robot->GetSpeed();
        _pileVelocity.y = _returnPath.y / _returnPath.Length() * _robot->GetSpeed();
        //��ʼ������Robot����׮��
        _robot->SetVelocity(_pileVelocity);
    }

    //�������Ϊ0��ֱ��ֹͣ
    if (_robot->GetCurrentElectricity() <= 0)
    {
        _robot->SetVelocity({ 0, 0 });
        return;
    }
    
    if (!_robot || _robot->IsBusy()) return;

    static ChargeableManager* _cm = ChargeableManager::Instance();
    const auto& _vehicles = _cm->GetVehicleList();

    //BestTargeted
    Vehicle* _bT = nullptr;
    //ReturnDistance
    double _rD = 0.0;
    double _minDistance = std::numeric_limits<double>::max();

    for (Vehicle* _v : _vehicles)
    {
        if (_v->IsBusy() || !_v->NeedElectricity() || _v->isMoving)
            continue;

        Vector2 _vPos = _v->GetPosition();
        double _distance = (_vPos - _robotPos).Length();
        if (_v->isTargeted && _distance >= _v->targetedDistance)
            continue;

        //�ɴ��Լ�飨�㹻�����ƶ���Ŀ���뷵�س��׮��
        Vector2 _vPilePos;
        _vPilePos.x = _vPos.x >= 640 ? 1280 : 0;
        _vPilePos.y = _vPos.y >= 448 ? 896 : 0;
        //��������Vehicle������׮λ��
        double _moveDistance = (_distance) + (_vPos - _vPilePos).Length();
        //������룬�ұ����ǲ����ƶ��Ķ��󣬲�Ȼ�ᵼ��bTȷ��ʱ��λ�ú�ʵ��λ�ò�ͬ
        //���³�����ê�������޻����˳�������������ҳ�����...
        if ((_robot->GetCurrentElectricity() >= _moveDistance / 30) && (_distance < _minDistance))
        {
            _minDistance = _distance;
            _bT = _v;
            _rD = (_vPos - _vPilePos).Length();
        }
        //if (_v->isTargeted->GetCurrentElectricity() == 100
        //    && (_robot->GetCurrentElectricity() >= _moveDistance / 19)
        //    && (_distance < _minDistance))
        //{   
        //    _bT = nullptr;
        //    _v->isTargeted = nullptr;
        //    _robot->bestTarget = nullptr;
        //    _robot->lowestElectricity = 0.0;
        //    _minDistance = std::numeric_limits<double>::max();
        //    _rD = 0.0;
        //}
    }

    if (_bT)
    {
        if (!_bT->isTargeted)
        {
            _bT->isTargeted = _robot;
            _bT->targetedDistance = _minDistance;
            if (((Vehicle*)(_robot->bestTarget)) != nullptr && ((Vehicle*)(_robot->bestTarget)) != _bT)
                ((Vehicle*)(_robot->bestTarget))->isTargeted = nullptr;
            _robot->bestTarget = _bT;
            _robot->lowestElectricity = _rD / 30;
        }
        else if (_bT->isTargeted && (_bT->targetedDistance > _minDistance))
        {
            if(_bT->isTargeted != _robot)
            {
				((Robot*)(_bT->isTargeted))->bestTarget = nullptr;
                _bT->isTargeted = _robot;
            }
            _bT->targetedDistance = _minDistance;
            if(((Vehicle*)(_robot->bestTarget)) != nullptr && ((Vehicle*)(_robot->bestTarget)) != _bT)
                ((Vehicle*)(_robot->bestTarget))->isTargeted = nullptr;
            _robot->bestTarget = _bT;
            _robot->lowestElectricity = _rD / 30;
        }
    }
    else
        _robot->bestTarget = nullptr;

    if (_robot->bestTarget && !_robot->isCharger)
    {
        Vector2 _dir = (_robot->bestTarget)->GetPosition() - _robot->GetPosition();
        Vector2 _velocity = { 0, 0 };
        _velocity.x = _dir.x / _dir.Length() * _robot->GetSpeed();
        _velocity.y = _dir.y / _dir.Length() * _robot->GetSpeed();

        _robot->SetVelocity(_velocity);
        return;
    }
}