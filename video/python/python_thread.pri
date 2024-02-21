

#filename:     python_thread.pri 
#time:         20240221-161322 
#author:       yunke120 

HEADERS += $$PWD/pythonthread.h 
SOURCES += $$PWD/pythonthread.cpp 

INCLUDEPATH += G:/MiniConda3/envs/yolo/include 
INCLUDEPATH += G:/MiniConda3/envs/yolo/Lib/site-packages/numpy/core/include/numpy 

LIBS += -LG:/MiniConda3/envs/yolo/libs \ 
           -lpython3 \ 
           -lpython310 \ 
