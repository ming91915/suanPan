/**
 * @class Section
 * @brief A Section class.
 * @author T
 * @date 27/07/2017
 * @version 0.1.0
 * @file Section.h
 */

#ifndef SECTION_H
#define SECTION_H

#include <Element/Element.h>

class Section : public Element
{
public:
    explicit Section(const unsigned& T = 0, const unsigned& CT = CT_SECTION)
        : Element(T, CT)
    {
    }

    virtual ~Section() {}

    void print() override;
};

#endif
