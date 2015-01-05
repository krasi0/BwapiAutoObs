#include "AutoObs.h"


// declaration outside the class is required by the linker
IAutoObs* IAutoObs::_instance;


inline void AutoObs::_init()
{
    this->enabled = AutoObs::_ENABLED;

    this->lastScreenRepositionFrame = 0;
}


/**
 * returns a position in the middle of the given positions
 */
BWAPI::Position AutoObs::_getPositionsCenter(BWAPI::Position pos1, BWAPI::Position pos2)
{
    return BWAPI::Position(
               (pos1.x + pos2.x) / 2,
               (pos1.y + pos2.y) / 2
           );
}


/**
    * returns true, if the screen can be repositioned now
    */
inline bool AutoObs::_canReposition()
{
    if (! this->enabled) {
        return false;
    }

    // Too early since the last time.
    if (BWAPI::Broodwar->getFrameCount() < 1
        || BWAPI::Broodwar->getFrameCount() - this->lastScreenRepositionFrame
           < AutoObs::SCREEN_REPOSITION_INTERVAL * BWAPI::Broodwar->getFPS()
    ) {
        return false;
    }

    return true;
}


/**
    * returns true, if the screen can be repositioned now to fight scene
    */
inline bool AutoObs::_canRepositionToAFight()
{
    if (! this->enabled) {
        return false;
    }

    // too early since the last time
    if (BWAPI::Broodwar->getFrameCount() < 1
        || BWAPI::Broodwar->getFrameCount() - this->lastScreenRepositionFrame
           < AutoObs::SCREEN_REPOSITION_INTERVAL_TO_A_FIGHT * BWAPI::Broodwar->getFPS()
    ) {
        return false;
    }

    return true;
}


/**
* repositions the screen so that the coords of the given pos are at its center (px coords)
*/
inline void AutoObs::reposition(BWAPI::Position pos)
{
    this->reposition(pos.x, pos.y);
}


/**
* repositions the screen so that x, y are at its center (px coords)
*/
inline void AutoObs::reposition(int x, int y)
{
    // do not divide by 2 but 3, because of the game menu at the bottom
    auto divisorX = 2;

    auto newX = x - AutoObs::_SCREEN_DIMENSION_HORIZONTAL / divisorX;

    if (newX < 0) {
        newX = 0;
    }

    // do not divide by 2 but 3, because of the game menu at the bottom
    int divisorY = 3;

    int newY = y - AutoObs::_SCREEN_DIMENSION_VERTICAL / divisorY;

    if (newY < 0) {
        newY = 0;
    }

    BWAPI::Broodwar->setScreenPosition(newX, newY);

    this->lastScreenRepositionFrame = BWAPI::Broodwar->getFrameCount();
}


inline bool AutoObs::_parseUnits(const BWAPI::Unitset* units)
{
    if (units->empty()) {
        return false;
    }

    // only consider isAttackFrame
    for (auto& unit : *units) {
        if (! unit->exists()) {
            continue;
        }

        // add some variety - sometimes skip the current unit
        if (rand() % 10 <= 1) {
            continue;
        }

        if (
            false

            || unit->getOrder() == BWAPI::Orders::CastNuclearStrike

            || unit->getOrder() == BWAPI::Orders::CastDarkSwarm

            || unit->getOrder() == BWAPI::Orders::CastEMPShockwave

            || unit->getOrder() == BWAPI::Orders::CastHallucination

            || unit->getOrder() == BWAPI::Orders::CastInfestation

            || unit->getOrder() == BWAPI::Orders::CastMindControl

            || unit->getOrder() == BWAPI::Orders::CastPlague

            || unit->getOrder() == BWAPI::Orders::CastRecall

            || unit->getOrder() == BWAPI::Orders::CastStasisField

            || unit->getOrder() == BWAPI::Orders::CastParasite

            || unit->getOrder() == BWAPI::Orders::CastPsionicStorm

            || unit->getOrder() == BWAPI::Orders::CastMaelstrom

            || unit->getOrder() == BWAPI::Orders::CastLockdown

            || unit->getOrder() == BWAPI::Orders::CastSpawnBroodlings

            || unit->getOrder() == BWAPI::Orders::CastFeedback

            || unit->getOrder() == BWAPI::Orders::CastIrradiate

            || unit->getOrder() == BWAPI::Orders::CastConsume

            || unit->getOrder() == BWAPI::Orders::CastDefensiveMatrix

            || unit->getOrder() == BWAPI::Orders::CastDisruptionWeb
            || unit->getOrder() == BWAPI::Orders::CastEnsnare
            || unit->getOrder() == BWAPI::Orders::CastOpticalFlare
            || unit->getOrder() == BWAPI::Orders::CastRestoration
            || unit->getOrder() == BWAPI::Orders::DarkArchonMeld

            // dropship / bunker loading / unloading
            || unit->getOrder() == BWAPI::Orders::Unload

            || unit->isAttackFrame()

            || unit->getOrder() == BWAPI::Orders::Burrowing

            || unit->getOrder() == BWAPI::Orders::Sieging

            || unit->getOrder() == BWAPI::Orders::Repair

            || unit->getOrder() == BWAPI::Orders::ArchonWarp

            || unit->getOrder() == BWAPI::Orders::BuildNydusExit

        ) {
            // move to a position in the middle of the unit and its target
            BWAPI::Unit targetUnit = NULL;

            // a small chance to anyway focus on the unit and NOT it's target
            if (rand() % 15 >= 1) {
                if (unit->getTarget()
                    && unit->getTarget()->exists()
                    && unit->getTarget()->getPosition() != BWAPI::Positions::None
                    && unit->getTarget()->getPosition() != BWAPI::Positions::Unknown
                ) {
                    targetUnit = unit->getTarget();
                } else if (unit->getOrderTarget()
                           && unit->getOrderTarget()->exists()
                           && unit->getOrderTarget()->getPosition() != BWAPI::Positions::None
                           && unit->getOrderTarget()->getPosition() != BWAPI::Positions::Unknown
                ) {
                    targetUnit = unit->getOrderTarget();
                }
            }

            if (targetUnit) {
                if (rand() % 12 >= 2) {
                    this->reposition(

                        // At 3/4 of the distance towards the target
                        this->_getPositionsCenter(
                            this->_getPositionsCenter(unit->getPosition(), targetUnit->getPosition()),
                            targetUnit->getPosition()
                        )
                    );
                } else {
                    // a small chance to focus on the target itself
                    this->reposition(targetUnit->getPosition());
                }
            } else {
                // otherwise move to the position of the unit
                this->reposition(unit->getPosition());
            }

            return true;
        }
    }

    return false;
}


// public:

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
    // only consider fight scenes
    if (! this->_canRepositionToAFight()) {
        return ;
    }

    // first check for interesting enemy units
    auto enemy = BWAPI::Broodwar->enemy();
    if (enemy

        // X % chance to consider the enemy units first
        // this is to avoid the situation when an single enemy unit is attacking
        // at a not very interesting place
        && rand() % 10 <= 6
        && enemy->allUnitCount(BWAPI::UnitTypes::AllUnits) > 0) {
        auto& units = enemy->getUnits();

        if (!units.empty() && this->_parseUnits(&units)) {
            return;
        }
    }

    // no appropriate enemy units to monitor, so monitor All units
    if (BWAPI::Broodwar->self()) {
        auto& units = BWAPI::Broodwar->self()->getUnits();

        if (! units.empty() && this->_parseUnits(&units)) {
            return;
        }
    }
}


void AutoObs::onUnitComplete(const BWAPI::Unit unit)
{
    if (! this->_canReposition()) {
        return ;
    }


    // ignore neutral units
    if (unit->getPlayer()->isNeutral()) {
        return;
    }

    // Ignore scanner sweep.
    // @TODO: uncomment. if necessary.
    /*if (unit->getType() == BWAPI::UnitTypes::Spell_Scanner_Sweep) {
        return;
    }*/

    // ignore workers
    if (unit->getType().isWorker()
        // supply depots and similar
        || unit->getType() == BWAPI::UnitTypes::Terran_Supply_Depot
        || unit->getType() == BWAPI::UnitTypes::Protoss_Pylon
        || unit->getType() == BWAPI::UnitTypes::Zerg_Overlord

        //vulture spider mines (only 10% chance to show, 90% to skip)
        || (unit->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine
            && rand() % 10 != 1)
    ) {
        return;
    }

    // OutputFunctions::debugPrint("Unit complete: %s", unit->getType().c_str());

    // move to the position of the unit
    this->reposition(unit->getPosition().x, unit->getPosition().y);
}


void AutoObs::onUnitShow(const BWAPI::Unit unit)
{
    if (! this->_canReposition()) {
        return ;
    }


    // ignore neutral units
    if (unit->getPlayer()->isNeutral()) {
        return;
    }

    // ignore scanner sweep
    if (unit->getType() == BWAPI::UnitTypes::Spell_Scanner_Sweep) {
        return;
    }

    // ignore incomplete units unless buildings
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

            // smaller chance for supply providers and workers
            || rand() % 5 < 4
               && (false
                   || unit->getType() == BWAPI::UnitTypes::Terran_Supply_Depot
                   || unit->getType() == BWAPI::UnitTypes::Protoss_Pylon
                   || unit->getType() == BWAPI::UnitTypes::Zerg_Overlord
                   || unit->getType().isWorker())
           )

    ) {
        return;
    }

    // OutputFunctions::debugPrint("Unit show: %s", unit->getType().c_str());


    // move to the position of the unit
    this->reposition(unit->getPosition().x, unit->getPosition().y);
}

void AutoObs::onUnitHide(const BWAPI::Unit unit)
{
}

void AutoObs::onRemoveObject(const BWAPI::Unit unit)
{
}


/**
    * setter
    */
void AutoObs::toggle()
{
    this->enabled = ! this->enabled;
}
