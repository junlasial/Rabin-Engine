
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_MoveToCorner: public BaseNode<L_MoveToCorner>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
   
}; 
