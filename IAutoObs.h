#pragma once


/**
 * interface for AutoObs
 */
class IAutoObs
{

private:

    //singleton instance
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
    

	virtual void onUnitComplete(BWAPI::Unit* unit) = 0;


	virtual void onUnitShow(BWAPI::Unit* unit) = 0;

	virtual void onUnitHide(BWAPI::Unit* unit) = 0;

	virtual void onRemoveUnit(BWAPI::Unit* unit) = 0;

};