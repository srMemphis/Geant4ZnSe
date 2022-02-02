#ifndef ACTION_INIT_HH
#define ACTION_INIT_HH

#include "G4VUserActionInitialization.hh"

class ActionInit : public G4VUserActionInitialization
{
public:

    ActionInit();
    virtual ~ActionInit();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif
