BwapiAutoObs
============

In-game auto screen repositioning depending on the current game play action.<br/>
Supports Starcraft Broodwar version 1.16.1, BWAPI 4+ (for BWAPI 3.7.X support, one has to use the commit from Dec 2013). <br/>
Currently in use on http://bots-stats.krasi0.com/ and http://sscaitournament.com/

You could integrate it in your AI or as a part of tournament module. The code relies on BWAPI methods only!
It's very useful when you want to sit back and watch (your) bots play or you would like to stream / record a bots game.

--------------------------------------------------------------------------------------
Instantiating and enabling:
    //auto observer
    this->_autoObs = new AutoObs();
    this->_autoObs->toggle();
--------------------------------------------------------------------------------------
    Using in the different BWAPI events is done as following:
    
    
void ExampleTournamentAI::onFrame()
{
    //autoObs
    this->_autoObs->onGameUpdate();
    ...
--------------------------------------------------------------------------------------
Similar is the usage within the rest of the events like: onUnitShow(BWAPI::Unit* unit), onUnitHide(BWAPI::Unit* unit), etc

--------------------------------------------------------------------------------------

Please provide feedback / report issues so that we could improve the automatic observer even more! Thanks!
