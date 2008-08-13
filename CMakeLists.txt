PROJECT(uct_bots)

#libraries
ADD_LIBRARY(board board.cpp)
ADD_LIBRARY(uct uct.cpp)
ADD_LIBRARY(player player.cpp)

#tests
ADD_EXECUTABLE(test_board test_board.cpp)
TARGET_LINK_LIBRARIES(test_board board)



#production
ADD_EXECUTABLE(bot_bot bot_bot.cpp)
TARGET_LINK_LIBRARIES(bot_bot uct board player)

ADD_EXECUTABLE(human_bot human_bot.cpp)
TARGET_LINK_LIBRARIES(human_bot uct board player)
