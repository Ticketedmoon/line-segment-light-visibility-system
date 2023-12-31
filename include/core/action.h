#pragma once

#ifndef LSLVS_ACTION_H
#define LSLVS_ACTION_H

class Action
{
    public:
        enum class Type
        {
            MOVE_UP = 0,
            MOVE_DOWN = 1,
            MOVE_LEFT = 2,
            MOVE_RIGHT = 3,
            CURSOR_MOVE = 4,
            CURSOR_SELECT = 5,
            MENU_SELECT = 6,
            SCENE_EXIT = 7,
        };
        enum class Mode
        {
            PRESS,
            RELEASE
        };

        Action(Type type, Mode mode);
        Type getType();
        Mode getMode();

    private:
        Type m_type;
        Mode m_mode;

};


#endif //LSLVS_ACTION_H