/**
* @class C3D8
* @brief The C3D8 class defines C3D8 C3D8R elements.
* @author T
* @date 13/08/2017
* @version 0.1.0
* @file C3D8.h
* @addtogroup Cube
* @ingroup Element
* @{
*/

#ifndef C3D8_H
#define C3D8_H

#include <Element/Element.h>

class C3D8 : public Element
{
public:
    C3D8();
    ~C3D8();

    virtual void initialize(const shared_ptr<Domain>&) override;
    virtual int updateStatus() override;
    virtual int commitStatus() override;
    virtual int clearStatus() override;
    virtual int resetStatus() override;
};

#endif

//! @}
