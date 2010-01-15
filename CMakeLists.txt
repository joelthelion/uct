PROJECT(uct_bots)
cmake_minimum_required(VERSION 2.4)

#libraries
ADD_LIBRARY(board
uct.cpp player.cpp utils.cpp
boardc4.cpp boardc5.cpp boardblocks.cpp boardothello.cpp board.cpp)

#tests
ADD_EXECUTABLE(test_nested test_nested.cpp)

ADD_EXECUTABLE(test_board test_board.cpp)
TARGET_LINK_LIBRARIES(test_board board)

ADD_EXECUTABLE(test_uct test_uct.cpp)
TARGET_LINK_LIBRARIES(test_uct board)

#production
ADD_EXECUTABLE(bot_bot bot_bot.cpp)
TARGET_LINK_LIBRARIES(bot_bot board)

ADD_EXECUTABLE(human_bot human_bot.cpp)
TARGET_LINK_LIBRARIES(human_bot board)

#gui
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

SET(SRCS qboardc4.cpp qgamethread.cpp qgamewindow.cpp)
SET(HDRS qboardc4.h qgamethread.h qgamewindow.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})
ADD_EXECUTABLE(bot_bot_gui bot_bot_gui.cpp ${MOC_SRCS} ${SRCS})
TARGET_LINK_LIBRARIES(bot_bot_gui board ${QT_LIBRARIES})
