
#add_subdirectory (browser)
add_subdirectory (modules)
###################
include (FindQt4)

#ADD_DEFINITIONS(-D_WINDOWS -D_UNICODE)
IF (WIN32)
  IF (MSVC)
    #tell msvc compiler to use main instead of winmain as the
    #application entry point
    SET(QT_USE_QTMAIN TRUE) 
  ENDIF(MSVC)
  include_directories("Z:\\cross\\tidy\\include")
  link_directories("Z:\\cross\\tidy\\lib")
  set(win32_extra_libs "WS2_32")
  set(snotify_EXE_RC "snsnotify.rc")
ELSE (WIN32)
  include_directories(/opt/tidylib/include)
  link_directories("/opt/tidylib/lib")
ENDIF (WIN32)

set (QT_USE_QTNETWORK true)
set (QT_USE_QTSQL true)
set (QT_USE_QTWEBKIT true)

include (${QT_USE_FILE})

##no use??????
QT4_ADD_TRANSLATION(hehe.qm translations/nsm_zh_CN.ts)
QT4_ADD_RESOURCES(SNSNOTIFY_RESOURCES_CPP snsnotify.qrc )

set (snotify_UIS
  ui/mainwindow.ui
  ui/login.ui
  ui/feeditemwidget.ui
  ui/about.ui
  #ui/debugwindow.ui
  ui/messagehint.ui
  ui/accountpanel.ui
#  ui/searchbar.ui
  ui/frienditemwidget.ui
  ui/chatwindow.ui
)
QT4_WRAP_UI (snotify_UIS_H  ${snotify_UIS} )

set (snotify_moc_HDRS
  snsnotify.h
  login.h
  systemoption.h
  useroption.h
  feeditemwidget.h
  #letteritemwidget.h
  threadfunctor.h
  about.h
  cookieparser.h
  #debugwindow.h
  feedgearbox.h
 # lettergearbox.h
  messagehint.h
  liveupdate.h
  accountpanel.h
 # searchbar.h
  frienditemwidget.h
  friendlistmodel.h
  frienditemdelegate.h
  chasewidget.h
  sessionmanager.h
  chatwindow.h
)
QT4_WRAP_CPP(snotify_moc_SRCS  ${snotify_moc_HDRS})

set (snotify_SRCS
  snsnotify.cpp
  snsnotify_test.cpp
  login.cpp
  systemoption.cpp
  useroption.cpp
  feeditemwidget.cpp
  feeditemwidget_win.cpp
  #letteritemwidget.cpp
  threadfunctor.cpp
  about.cpp
  cookieparser.cpp
  cookieparser_ie.cpp
  cookieparser_firefox.cpp
  cookieparser_chrome.cpp
  cookieparser_opera.cpp
  opera_file_formats.c
  qlog.cpp
  #debugwindow.cpp
  feedgearbox.cpp
  #lettergearbox.cpp
  messagehint.cpp
  liveupdate.cpp
  accountpanel.cpp
  #searchbar.cpp
  frienditemwidget.cpp
  friendlistmodel.cpp
  frienditemdelegate.cpp
  chasewidget.cpp
  sessionmanager.cpp
  chatwindow.cpp
)

# add_executable (snotify WIN32  
#   main.cpp
#   ${snotify_SRCS}
#   ########
#   ${snotify_UIS_H}
#   ${snotify_moc_SRCS}
#   ${SNSNOTIFY_RESOURCES_CPP}
#   ${snotify_EXE_RC}
# )

#########this section will generate a console windows application
if (WIN32)
  add_executable (snotify2 
    main.cpp
    qlog.cpp
    utils.cpp
    ${snotify_EXE_RC}
    )  
  TARGET_LINK_LIBRARIES(snotify2 ${QT_LIBRARIES} tidy ${win32_extra_libs} )
else (WIN32)
  add_executable (snotify WIN32  
    main.cpp
    qlog.cpp
    utils.cpp
    ${snotify_EXE_RC}
    )

  TARGET_LINK_LIBRARIES(snotify ${QT_LIBRARIES} tidy ${win32_extra_libs} )
endif (WIN32)

add_library (snsmain SHARED
  utils.cpp
  ${snotify_SRCS}
  ########
  ${snotify_UIS_H}
  ${snotify_moc_SRCS}
  ${SNSNOTIFY_RESOURCES_CPP}
  ${snotify_EXE_RC}
)
TARGET_LINK_LIBRARIES(snsmain ${QT_LIBRARIES} tidy ${win32_extra_libs} )





