#include "AutoObs.h"


//declaration outside the class is required by the linker
IAutoObs* IAutoObs::_instance;


inline void AutoObs::_init()
{
    this->_enabled = AutoObs::_ENABLED;


    this->_lastScreenRepositionFrame = 0;
}


/**
 * returns a position in the middle of the given positions
 */
BWAPI::Position AutoObs::_getPositionsCenter(BWAPI::Position pos1, BWAPI::Position pos2) 
{
    return BWAPI::Position(
        abs(pos1.x() + pos2.x()) / 2, 
        abs(pos1.y() + pos2.y()) / 2);
}


/**
    * returns true if the screen can be repositioned now
    */
inline bool AutoObs::_canReposition()
{
    if (! this->_enabled) {
        return false;
    }

    //to early since the last time
    if (BWAPI::Broodwar->getFrameCount() < 1
        ||     BWAPI::Broodwar->getFrameCount() - this->_lastScreenRepositionFrame
            < AutoObs::_SCREEN_REPOSITION_INTERVAL * BWAPI::Broodwar->getFPS())
    {
        return false;
    }

    return true;
}


/**
    * returns true if the screen can be repositioned now to fight scene
    */
inline bool AutoObs::_canRepositionToAFight()
{
    if (! this->_enabled) {
        return false;
    }

    // too early since the last time
    if (BWAPI::Broodwar->getFrameCount() < 1
        ||     BWAPI::Broodwar->getFrameCount() - this->_lastScreenRepositionFrame
            < AutoObs::_SCREEN_REPOSITION_INTERVAL_TO_A_FIGHT * BWAPI::Broodwar->getFPS())
    {
        return false;
    }

    return true;
}


/**
    * repositions the screen so that the coords of the given pos are at its center (px coords)
    */
inline void AutoObs::_reposition(BWAPI::Position pos)
{
    this->_reposition(pos.x(), pos.y());
}


/**
    * repositions the screen so that x, y are at its center (px coords)
    */
inline void AutoObs::_reposition(int x, int y)
{
    //do not divide by 2 but 3, because of the game menu at the bottom
    int divisorX = 2;

    int newX = x - AutoObs::_SCREEN_DIMENSION_HORIZONTAL / divisorX;

    if (newX < 0) {
        newX = 0;
    }

    //do not divide by 2 but 3, because of the game menu at the bottom
    int divisorY = 3;

    int newY = y - AutoObs::_SCREEN_DIMENSION_VERTICAL / divisorY;

    if (newY < 0) {
        newY = 0;
    }

    BWAPI::Broodwar->setScreenPosition(newX, newY);

    this->_lastScreenRepositionFrame = BWAPI::Broodwar->getFrameCount();
}


inline bool AutoObs::_parseUnits(std::set<BWAPI::Unit*> &units)
{
    if (units.empty()) {
        return false;
    }

    //only consider isAttackFrame
    for (std::set<BWAPI::Unit*>::iterator i = units.begin(); i != units.end(); i++) {
        if (! (*i)->exists()) {
            continue;
        }

        // add some variety - sometimes skip the current unit
        if (rand() % 10 <= 1) {
            continue;
        }

        if (
            false

            || (*i)->getOrder() == BWAPI::Orders::CastNuclearStrike

            || (*i)->getOrder() == BWAPI::Orders::CastDarkSwarm

            || (*i)->getOrder() == BWAPI::Orders::CastEMPShockwave

            || (*i)->getOrder() == BWAPI::Orders::CastHallucination

            || (*i)->getOrder() == BWAPI::Orders::CastInfestation

            || (*i)->getOrder() == BWAPI::Orders::CastMindControl

            || (*i)->getOrder() == BWAPI::Orders::CastPlague

            || (*i)->getOrder() == BWAPI::Orders::CastRecall

            || (*i)->getOrder() == BWAPI::Orders::CastStasisField

            || (*i)->getOrder() == BWAPI::Orders::CastParasite

            || (*i)->getOrder() == BWAPI::Orders::CastPsionicStorm

            || (*i)->getOrder() == BWAPI::Orders::CastMaelstrom

            || (*i)->getOrder() == BWAPI::Orders::CastLockdown

            || (*i)->getOrder() == BWAPI::Orders::CastSpawnBroodlings

            || (*i)->getOrder() == BWAPI::Orders::CastFeedback

            || (*i)->getOrder() == BWAPI::Orders::CastIrradiate

            || (*i)->getOrder() == BWAPI::Orders::CastConsume

            || (*i)->getOrder() == BWAPI::Orders::CastDefensiveMatrix

            || (*i)->getOrder() == BWAPI::Orders::CastDisruptionWeb
            || (*i)->getOrder() == BWAPI::Orders::CastEnsnare
            || (*i)->getOrder() == BWAPI::Orders::CastOpticalFlare
            || (*i)->getOrder() == BWAPI::Orders::CastRestoration
            || (*i)->getOrder() == BWAPI::Orders::DarkArchonMeld

            // dropship / bunker loading / unloading
            || (*i)->getOrder() == BWAPI::Orders::Unload

            || (*i)->isAttackFrame()

            || (*i)->getOrder() == BWAPI::Orders::Burrowing

            || (*i)->getOrder() == BWAPI::Orders::Sieging

            || (*i)->getOrder() == BWAPI::Orders::Repair

            || (*i)->getOrder() == BWAPI::Orders::ArchonWarp

            || (*i)->getOrder() == BWAPI::Orders::BuildNydusExit

            )

        {
            //move to a position in the middle of the unit and its target
            BWAPI::Unit* targetUnit = NULL;

            // a small chance to anyway focus on the unit and NOT it's target
            if (rand() % 10 >= 2) {
                if ((*i)->getTarget()) {
                    targetUnit = (*i)->getTarget(); 
                } else if ((*i)->getOrderTarget()) {
                    targetUnit = (*i)->getOrderTarget(); 
                }
            }

            if (targetUnit) {
                this->_reposition(this->_getPositionsCenter((*i)->getPosition(), targetUnit->getPosition()));
            } else{
                // otherwise move to the position of the unit
                this->_reposition((*i)->getPosition());
            }

            return true;
        }
    }

    return false;
}


//public:

/**
    * Constructor
    */
AutoObs::AutoObs()
{
    IAutoObs::setInstance(this);


    this->_init();
}


/**
    * called on the first game frame
    */
void AutoObs::onGameStart()
{
}


/**
    * called on every new game frame
    */
void AutoObs::onGameUpdate()
{
    //only consider fight scenes
    if (! this->_canRepositionToAFight()) {
        return ;
    }

    std::set<BWAPI::Unit*> units;

    BWAPI::Player* enemy = BWAPI::Broodwar->enemy();
	if (enemy

        // X % chance to consider the enemy units first
        // this is to avoid the situation when an single enemy unit is attacking
        // at a not very interesting place
        && rand() % 10 <= 6
		&& enemy->allUnitCount(BWAPI::UnitTypes::AllUnits) > 0) 
	{
        units = enemy->getUnits();

        if (!units.empty() && this->_parseUnits(units)) {
            return;
        }
    }

    //only consider isAttackFrame
    if (! units.empty() && this->_parseUnits(units)) {
        return;

    }
}


void AutoObs::onUnitComplete(BWAPI::Unit* unit)
{
    if (! this->_canReposition()) {
        return ;
    }


    //ignore neutral units
    if (unit->getPlayer()->isNeutral()) {
        return;
    }

    //ignore scanner sweep
    if (unit->getType() == BWAPI::UnitTypes::Spell_Scanner_Sweep) {
        return;
    }

    //ignore workers
    if (unit->getType().isWorker()
        //supply depots and similar
        || unit->getType() == BWAPI::UnitTypes::Terran_Supply_Depot
        || unit->getType() == BWAPI::UnitTypes::Protoss_Pylon
        || unit->getType() == BWAPI::UnitTypes::Zerg_Overlord

        //vulture spider mines (only 10% chance to show, 90% to skip)
        || (unit->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine
            && rand() % 10 != 1)
        )
    {
        return;
    }

    //OutputFunctions::debugPrint("Unit complete: %s", unit->getType().c_str());

    //move to the position of the unit
    this->_reposition(unit->getPosition().x(), unit->getPosition().y());
}


void AutoObs::onUnitShow(BWAPI::Unit* unit)
{
    if (! this->_canReposition()) {
        return ;
    }


    //ignore neutral units
    if (unit->getPlayer()->isNeutral()) {
        return;
    }

    //ignore scanner sweep
    if (unit->getType() == BWAPI::UnitTypes::Spell_Scanner_Sweep) {
        return;
    } 

    //ignore incomplete units unless buildings
    if (
            (! unit->getType().isBuilding()
            || unit->getType() == BWAPI::UnitTypes::Terran_Supply_Depot
            || unit->getType() == BWAPI::UnitTypes::Protoss_Pylon
            || unit->getType() == BWAPI::UnitTypes::Zerg_Overlord

            //X% chance we won't show it if not able to attack
            || rand() % 5 < 4
                && ! (false
                    || unit->getType() == BWAPI::UnitTypes::Terran_Bunker
                    || unit->getType() == BWAPI::UnitTypes::Terran_Missile_Turret
                    || unit->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon

                    || unit->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony
                    || unit->getType() == BWAPI::UnitTypes::Zerg_Spore_Colony))

        && (! BWAPI::Broodwar->self()->isEnemy(unit->getPlayer())

            //smaller chance for supply providers and workers
            || rand() % 5 < 4
                && (false
                    || unit->getType() == BWAPI::UnitTypes::Terran_Supply_Depot
                    || unit->getType() == BWAPI::UnitTypes::Protoss_Pylon
                    || unit->getType() == BWAPI::UnitTypes::Zerg_Overlord
                    || unit->getType().isWorker())
            )

        )
    {
        return;
    }

    //OutputFunctions::debugPrint("Unit show: %s", unit->getType().c_str());


    //move to the position of the unit
    this->_reposition(unit->getPosition().x(), unit->getPosition().y());
}

void AutoObs::onUnitHide(BWAPI::Unit* unit)
{

}

void AutoObs::onRemoveUnit(BWAPI::Unit* unit)
{

}


/**
    * setter
    */
void AutoObs::toggle()
{
    this->_enabled = ! this->_enabled;
}