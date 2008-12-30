###################
include (FindQt4)

set (QT_USE_QTGUI true)
set (QT_USE_QTNETWORK true)
set (QT_USE_QTWEBKIT true)
set (QT_USE_QTSQL true)

include (${QT_USE_FILE})

IF (WIN32)
  IF (MSVC)
    #tell msvc compiler to use main instead of winmain as the
    #application entry point
    #SET(QT_USE_QTMAIN TRUE) 
  ENDIF(MSVC)
  include_directories(Z:\\cross\\tidy\\include ..)
  link_directories("Z:\\cross\\tidy\\lib")
  #set(win32_extra_libs "WS2_32")
  #set(snotify_EXE_RC "snsnotify.rc")
ELSE (WIN32)
  include_directories(/opt/tidylib/include ../)
  link_directories("/opt/tidylib/lib")
ENDIF (WIN32)

set (uch_moc_HDRS
  feed_record.h
  uch_main_feed.h
)
QT4_WRAP_CPP(uch_moc_SRCS  ${uch_moc_HDRS})


#################
# add_library(hainei_feed SHARED 
#   tidy_wrapper.cpp
#   feed_record.cpp
#   hainei_feed.cpp  
#   )
# TARGET_LINK_LIBRARIES(hainei_feed tidy ${QT_LIBRARIES})

#############
# add_library(xiaonei_feed SHARED 
#   tidy_wrapper.cpp
#   feed_record.cpp
#   xiaonei_feed.cpp
#   )
# TARGET_LINK_LIBRARIES(xiaonei_feed tidy ${QT_LIBRARIES})

#############



#############
add_library(uch_main_feed SHARED 
  tidy_wrapper.cpp
  feed_record.cpp
  uchome_base.cpp
  uch_main_feed.cpp
  md5.cpp
  ${uch_moc_SRCS}
  )
TARGET_LINK_LIBRARIES(uch_main_feed tidy ${QT_LIBRARIES})

#############
