#include "ActionInit.hh"

#include "PrimaryGeneratorAction.hh"
#include "StackingAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"

ActionInit::ActionInit()
{
}

ActionInit::~ActionInit()
{
}

void ActionInit::BuildForMaster() const
{
    SetUserAction(new RunAction());
}

void ActionInit::Build() const
{
    SetUserAction(new RunAction());
    SetUserAction(new PrimaryGeneratorAction());
    SetUserAction(new StackingAction());
    SetUserAction(new SteppingAction());
    SetUserAction(new EventAction());
}
