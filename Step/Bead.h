/**
* @class Bead
* @brief A Bead class is a container.
* @author T
* @date 17/08/2017
* @version 0.2.0
* @file Bead.h
*/

#ifndef BEAD_H
#define BEAD_H

#include <suanPan.h>
#include <unordered_map>

class Domain;
class Step;
class Recorder;

using std::unordered_map;

class Bead
{
    unordered_map<unsigned, shared_ptr<Domain>> domains;
    unordered_map<unsigned, shared_ptr<Step>> steps;
    unordered_map<unsigned, shared_ptr<Recorder>> loggers;

public:
    void insert(const shared_ptr<Domain>&);
    void insert(const shared_ptr<Step>&);
    void insert(const shared_ptr<Recorder>&);
};

#endif
