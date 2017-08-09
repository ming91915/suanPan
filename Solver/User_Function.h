/**
 * @class User_Function
 * @brief A User_Function class defines solvers used in analysis.
 * @author T
 * @date 03/08/2017
 * @file User_Function.h
 */

#ifndef USER_FUNCTION_H
#define USER_FUNCTION_H

#include <Domain/Tag.h>

class User_Function : public Tag
{
public:
    explicit User_Function(const unsigned& = 0, const unsigned& = CT_USER_FUNCTION);
    virtual ~User_Function();
};

#endif
