/**
 * @class MVLEM
 * @brief The MVLEM class.
 * @author T
 * @date 14/09/2017
 * @version 0.1.1
 * @file MVLEM.h
 * @addtogroup Beam
 * @ingroup Element
 * @{
 */

#ifndef MVLEM_H
#define MVLEM_H

#include <Element/Element.h>

class MVLEM final : public Element {
    static const unsigned b_node, b_dof;
};

#endif

//! @}
