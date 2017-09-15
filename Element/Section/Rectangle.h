/**
 * @class Rectangle
 * @brief A Rectangle class.
 * @author T
 * @date 15/09/2017
 * @version 0.1.0
 * @file Rectangle.h
 */

#ifndef Rectangle_H
#define Rectangle_H

#include <Element/Section/Section.h>

class Rectangle : public Section {
    const double width, height;

public:
    explicit Rectangle(const unsigned& T = 0)
        : Section(T, ST_RECTANGLE)
        , width(0)
        , height(0) {}

    virtual ~Rectangle() {}

    void print() override {}
};

#endif
