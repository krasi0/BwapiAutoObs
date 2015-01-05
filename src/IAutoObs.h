#pragma once


/**
 * interface for AutoObs
 */
class IAutoObs
{

private:

    // singleton instance
    static IAutoObs* _instance;


public:

    /**
     * Singleton instance
     */
    static IAutoObs* getInstance()
    {
        return IAutoObs::_instance;
    }

    /**
     * setter for the Singleton instance
     */
    static void setInstance(IAutoObs* instance)
    {
        IAutoObs::_instance = instance;
    }

    /**
    * repositions the screen so that the coords of the given pos are at its center (px coords)
    */
    virtual void reposition(BWAPI::Position pos) = 0;

    /**
     * setter
     */
    virtual void toggle() = 0;

    
	/**
     * called on the first game frame
     */
    virtual void onGameStart() = 0;


    /**
     * called on every new game frame
     */
    virtual void onGameUpdate() = 0;
    

	virtual void onUnitComplete(const BWAPI::Unit unit) = 0;


	virtual void onUnitShow(const BWAPI::Unit unit) = 0;

	virtual void onUnitHide(const BWAPI::Unit unit) = 0;

	virtual void onRemoveObject(const BWAPI::Unit unit) = 0;

};