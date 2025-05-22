#include "../../../Header/Strategy/Concrete/StrategyB.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Manager/Concrete/ScoreManager.h"
#include "../../../Header/Chargeable/Concrete/Robot.h"
#include "../../../Header/Chargeable/Concrete/Vehicle.h"

void StrategyB::UpdateMovement(Chargeable* _chargeable)
{
    Robot* _robot = dynamic_cast<Robot*>(_chargeable);

    Vector2 _robotPos = _robot->GetPosition();
    //最近充电桩位置
    Vector2 PilePos;
    PilePos.x = _robotPos.x >= 640 ? 1280 : 0;
    PilePos.y = _robotPos.y >= 448 ? 896 : 0;
    Vector2 _returnPath = PilePos - _robotPos;

    if (!_robot->IsBusy())
    {
        Vector2 _pileVelocity;
        _pileVelocity.x = _returnPath.x / _returnPath.Length() * _robot->GetSpeed();
        _pileVelocity.y = _returnPath.y / _returnPath.Length() * _robot->GetSpeed();
        //初始就设置Robot向充电桩跑
        _robot->SetVelocity(_pileVelocity);
    }

    //如果电量为0，直接停止
    if (_robot->GetCurrentElectricity() <= 0)
    {
        _robot->SetVelocity({ 0, 0 });
        return;
    }

    if (!_robot || _robot->IsBusy()) return;

    static ChargeableManager* _cm = ChargeableManager::Instance();
    static ScoreManager* _sm = ScoreManager::Instance();
    const auto& _vehicles = _cm->GetVehicleList();

    //BestTargeted
    Vehicle* _bT = nullptr;
    //最大价值
    double _maxValue = -1.0;
    //返回充电桩距离
    double _rD = 0.0;

    for (Vehicle* _v : _vehicles)
    {
        if (_v->IsBusy() || !_v->NeedElectricity() || _v->isMoving) continue;

        Vector2 _vPos = _v->GetPosition();
        //离锁定的Vehicle最近充电桩位置
        Vector2 _vPilePos;
        _vPilePos.x = _vPos.x >= 640 ? 1280 : 0;
        _vPilePos.y = _vPos.y >= 448 ? 896 : 0;
        double _distanceToVehicle = (_robotPos - _vPos).Length();
        //_vPilePos即离载具最近的充电桩位置
        double _distanceToStation = (_vPos - _vPilePos).Length();

        //可达性检查（足够电量移动到目标与返回充电桩）
        double _moveDistance = (_distanceToVehicle - 96) + _distanceToStation;
        if (_robot->GetCurrentElectricity() < _moveDistance / 30) continue;

        //计算价值
        double _value = CalculateVehicleValue(_v, _sm->GetPassTime(), _distanceToVehicle, _distanceToStation);
        if (_v->isTargeted && _value <= _v->TargetedValue)
            continue;
        if (_value > _maxValue) 
        {
            _maxValue = _value;
            _bT = _v;
            _rD = _distanceToStation;
        }
    }

    //设置最佳目标
    if (_bT)
    {
        if (!_bT->isTargeted)
        {
            _bT->isTargeted = _robot;
			_bT->TargetedValue = _maxValue;
            if (((Vehicle*)(_robot->bestTarget)) != nullptr && ((Vehicle*)(_robot->bestTarget)) != _bT)
                ((Vehicle*)(_robot->bestTarget))->isTargeted = nullptr;
            _robot->bestTarget = _bT;
            _robot->lowestElectricity = _rD / 30;
        }
        else if (_bT->isTargeted && (_bT->TargetedValue < _maxValue))
        {
            if (_bT->isTargeted != _robot)
            {
                ((Robot*)(_bT->isTargeted))->bestTarget = nullptr;
                _bT->isTargeted = _robot;
            }
            _bT->TargetedValue = _maxValue;
            if (((Vehicle*)(_robot->bestTarget)) != nullptr && ((Vehicle*)(_robot->bestTarget)) != _bT)
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

double StrategyB::CalculateVehicleValue(Vehicle* _vehicle, double _currentTime, double _d2V, double _d2C)
{
    //避免除以极小值
    double _timeLeft = std::max(1e-6, _vehicle->GetLeaveTime() - _currentTime);
    double _energyNeeded = std::max(0.0, _vehicle->GetTargetElectricity() - _vehicle->GetCurrentElectricity());
    double _energyPerSecond = _energyNeeded / _timeLeft;
    double _distanceSum = std::max(1e-9, _d2V + _d2C);

    //价值函数：高紧急度 + 短往返距离
    return _energyPerSecond * (100.0 / _distanceSum);
}