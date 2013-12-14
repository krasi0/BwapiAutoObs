#pragma once

#pragma warning(push)
#pragma warning( disable : 4250 )


#include <BWAPI.h>

#include "IAutoObs.h"


/**
 * a concrete class to represent the automatic observer repositioner
 * NB: must use only pure BWAPI functions and no wrapper functions
 * in order to be freely usable and portable
 */
class AutoObs :
    virtual public IAutoObs
{

private:

    //############# Constants ##################
    static const bool _ENABLED = false;

    /**
     * in seconds, will be automatically converted to frames
     */
    static const int _SCREEN_REPOSITION_INTERVAL = 2;

    static const int _SCREEN_REPOSITION_INTERVAL_TO_A_FIGHT = 1;


    static const int _SCREEN_DIMENSION_HORIZONTAL = 20 * 32;
    static const int _SCREEN_DIMENSION_VERTICAL = 15 * 32;


    //############# Fields ##################

    //indicates whether AO is currently enabled
    bool _enabled;


    //the last time the screen was repositioned
    int _lastScreenRepositionFrame;


    //############# Methods ##################


    void _init();


    /**
     * returns a position in the middle of the given positions
     */
    BWAPI::Position AutoObs::_getPositionsCenter(BWAPI::Position pos1, BWAPI::Position pos2);


    /**
     * returns true if the screen can be repositioned now
     */
    bool _canReposition();


    /**
     * returns true if the screen can be repositioned now to fight scene
     */
    bool _canRepositionToAFight();


    /**
        * repositions the screen so that the coords of the given pos are at its center (px coords)
        */
    void _reposition(BWAPI::Position pos);


    /**
     * repositions the screen so that x, y are at its center (px coords)
     */
    void _reposition(int x, int y);


    bool _parseUnits(std::set<BWAPI::Unit*> &units);


public:

    /**
     * Constructor
     */
    AutoObs();


    /**
     * called on the first game frame
     */
    void onGameStart();


    /**
     * called on every new game frame
     */
    void onGameUpdate();


    void onUnitComplete(BWAPI::Unit* unit);


    void onUnitShow(BWAPI::Unit* unit);

    void onUnitHide(BWAPI::Unit* unit);

    void onRemoveUnit(BWAPI::Unit* unit);


    /**
     * setter
     */
    void toggle();

};


#pragma warning(pop)