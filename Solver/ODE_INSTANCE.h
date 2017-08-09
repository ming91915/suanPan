#ifndef ODE_INSTANCE_H
#define ODE_INSTANCE_H

class ODE_INSTANCE : public ODE
{
public:
    ODE_INSTANCE()
        : ODE(1)
    {
    }
    //! Analytical solution: y=@(x)(-exp(-x*x/2)*x*x-2*exp(-x*x/2)+3)/(exp(-x*x/2));
    vec getDY(const double& T, const vec& Y) final { return T * Y + T * T * T; }
};

#endif
