#ifndef DEFINE_FILEPATH_H
#define DEFINE_FILEPATH_H

#include<QString>
#include"define_DirPath.h"

#define MUSIC_LIBRARY_PATH MUSIC_LIBRARY_DIR_PATH+QString("/Library.txt")
#define WEB_DOWNLOAD_PATH QString("C:/Users/30903/Downloads")
#define PLAYING_INFO_PATH APP_ROOT_PATH + "/" + QString("playing_info.txt")

#endif // DEFINE_FILEPATH_H
