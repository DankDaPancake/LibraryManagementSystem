#pragma once

#include "patterns/observer/IObserver.hpp"

#include <vector>

using namespace std;

class ISubject {
public:
    virtual ~ISubject() = default;

    virtual void attach(IObserver *observer) = 0;
    virtual void detach(IObserver *observer) = 0;
    virtual void notify() = 0;
};