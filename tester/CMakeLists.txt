cmake_minimum_required(VERSION 2.6)
set (CMAKE_BUILD_TYPE "Debug") 

include(FindQt4)

set(QT_USE_QTCORE true)
set(QT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
set(QT_USE_QTWEBKIT true)
include (${QT_USE_FILE})

add_executable(updater
  updater.cpp
  )

TARGET_LINK_LIBRARIES(updater ${QT_LIBRARIES} )

# add_executable(rxch
#   rxch.cpp
#   )

# TARGET_LINK_LIBRARIES(rxch ${QT_LIBRARIES} )
set (wk_HEADERS
  wkencoding.h
)
QT4_WRAP_CPP(wk_MOC_SRC ${wk_HEADERS})

add_executable(wk
  wkencoding.cpp
  ${wk_MOC_SRC}
  )
TARGET_LINK_LIBRARIES(wk ${QT_LIBRARIES} )

################## manyou api client
set(myapi_HEADERS
manyou_api_client.h
)
QT4_WRAP_CPP(myapi_SRC ${myapi_HEADERS})

add_executable(myapi
  ../src/utils.cpp
  ../src/modules/md5.cpp
  ####
  myapi.cpp
  manyou_api_client.cpp
  ${myapi_SRC}
)

TARGET_LINK_LIBRARIES(myapi  ${QT_LIBRARIES} )