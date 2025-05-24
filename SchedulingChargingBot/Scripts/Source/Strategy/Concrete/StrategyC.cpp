#include "../../../Header/Strategy/Concrete/StrategyC.h"
#include "../../../Header/Manager/Concrete/ChargeableManager.h"
#include "../../../Header/Manager/Concrete/SceneManager.h"
#include "../../../Header/Manager/Concrete/ScoreManager.h"

void StrategyC::UpdateMovement(Chargeable* _chargeable)
{
    Robot* _robot = dynamic_cast<Robot*>(_chargeable);
    if (!_robot || _robot->IsBusy()) return;

    Vector2 _robotPos = _robot->GetPosition();
    Vector2 _pilePos = { _robotPos.x >= 640.0 ? 1280.0 : 0.0, _robotPos.y >= 448.0 ? 896.0 : 0.0 };
    Vector2 _toPile = _pilePos - _robotPos;

    //没有目标时默认回桩
    if (!_robot->bestTarget)
    {
        Vector2 _returnVel = _toPile.Normalized();
        _returnVel.x *= _robot->GetSpeed();
        _returnVel.y *= _robot->GetSpeed();
        _robot->SetVelocity(_returnVel);
    }

    if (_robot->GetCurrentElectricity() <= 0)
    {
        _robot->SetVelocity({ 0, 0 });
        return;
    }

    static ChargeableManager* _cm = ChargeableManager::Instance();
    const auto& _vehicles = _cm->GetVehicleList();

    Vehicle* _bestVehicle = nullptr;
    double _bestScore = -1.0;
    double _minDistance = std::numeric_limits<double>::max();
    double _retDist = 0.0;

    for (Vehicle* _v : _vehicles)
    {
        if (_v->IsBusy() || !_v->NeedElectricity() || _v->isMoving)
            continue;

        Vector2 _vPos = _v->GetPosition();
        double _dist = (_robotPos - _vPos).Length();

        double distance_v = (_vPos - _robotPos).Length();
        if (_v->isTargeted && distance_v > _v->targetedDistance) continue;

        Vector2 _vPile = { _vPos.x >= 640.0 ? 1280.0 : 0.0, _vPos.y >= 448.0 ? 896.0 : 0.0 };
        double _moveDist = _dist + (_vPos - _vPile).Length();
        double _requiredElec = _moveDist / 20;

        if (_robot->GetCurrentElectricity() < _requiredElec)
            continue;

        //PSO启发式
        //Score = Value / Cost
        double _value = _v->GetTargetElectricity() - _v->GetCurrentElectricity();
        //避免除0
        double _score = _value / (_dist + 1);
        if (_v->isTargeted && _value < _v->targetedValue) continue;

        if (_score > _bestScore)
        {
            _bestScore = _score;
            _bestVehicle = _v;
            _minDistance = _dist;
            _retDist = (_vPos - _vPile).Length();
        }
    }
    //最佳目标
    if (_bestVehicle)
    {
        if (!_bestVehicle->isTargeted || _bestVehicle->targetedValue < _bestScore)
        {
            if (_bestVehicle->isTargeted && _bestVehicle->isTargeted != _robot)
                ((Robot*)_bestVehicle->isTargeted)->bestTarget = nullptr;

            _bestVehicle->isTargeted = _robot;
            _bestVehicle->targetedDistance = _minDistance;
            _bestVehicle->targetedValue = _bestScore;

            if (_robot->bestTarget && _robot->bestTarget != _bestVehicle)
                ((Vehicle*)(_robot->bestTarget))->isTargeted = nullptr;

            _robot->bestTarget = _bestVehicle;
            _robot->lowestElectricity = _retDist / 20;
        }
    }
    else
        _robot->bestTarget = nullptr;

    if (_robot->bestTarget && !_robot->isCharger && !((Vehicle*)(_robot->bestTarget))->isMoving)
    {
        Vector2 _dir = _robot->bestTarget->GetPosition() - _robot->GetPosition();
        Vector2 _vel = _dir.Normalized();
        _vel.x *= _robot->GetSpeed();
        _vel.y *= _robot->GetSpeed();
        _robot->SetVelocity(_vel);
    }
}