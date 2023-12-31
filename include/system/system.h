#pragma once

#ifndef LSLVS_SYSTEM_H
#define LSLVS_SYSTEM_H

class System
{
    public:
        virtual ~System() = default;
        virtual void execute() = 0;
};

#endif //LSLVS_SYSTEM_H
