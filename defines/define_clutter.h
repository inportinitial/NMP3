#ifndef DEFINE_CLUTTER_H
#define DEFINE_CLUTTER_H

#include<QStringList>
#include"define_DirPath.h"

#define MUSIC_SUFFIX QStringList({"mp3"})

#define PAUSE 0
#define PLAY 1

#define PY_LoadMP3ToLrcWeb_FILE_PATH APP_ROOT_PATH+"/"+QString("MyPy/LoadMP3ToLrcWeb.pyw")
#define PY_Ctrl_W_Hotkey_FILE_PATH APP_ROOT_PATH+"/"+QString("MyPy/Ctrl_W_Hotkey.pyw")
#define PY_LoadLrcToLrcWeb_FILE_PATH APP_ROOT_PATH+"/"+QString("MyPy/LoadLrcToLrcWeb.pyw")

#define CHECK_IF_NEW_LRC_BIRTH_INTERVAL 1000

#define MK_LRC_WEB QString("https://lrc-maker.github.io/#/")

#define X_DISTANCE_RIGHT_CLICK_MENU_DISPAIR_WHEN_MOUSE_LEAVE 30
#define Y_DISTANCE_RIGHT_CLICK_MENU_DISPAIR_WHEN_MOUSE_LEAVE 30

#endif // DEFINE_CLUTTER_H
