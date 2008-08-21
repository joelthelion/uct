PROJECT(uct_bots)

#libraries
ADD_LIBRARY(board uct.cpp player.cpp utils.cpp boardc4.cpp board.cpp)
#TARGET_LINK_LIBRARIES(board)
#ADD_LIBRARY(player player.cpp)

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
