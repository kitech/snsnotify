include (FindQt4)

#ADD_DEFINITIONS(-D_WINDOWS -D_UNICODE)
IF (WIN32)
  IF (MSVC)
    #tell msvc compiler to use main instead of winmain as the
    #application entry point
    SET(QT_USE_QTMAIN TRUE) 
  ENDIF(MSVC)
  include_directories("Z:\\cross\\tidy\\include .")
  link_directories("Z:\\cross\\tidy\\lib")
  set(win32_extra_libs "WS2_32")
  set(browser_EXE_RC "browser.rc")
ELSE (WIN32)
  include_directories(/opt/tidylib/include .)
  link_directories("/opt/tidylib/lib")
ENDIF (WIN32)

set (QT_USE_QTNETWORK true)
set (QT_USE_QTSQL true)
set (QT_USE_QTWEBKIT true)
set (QT_USE_QTUITOOLS true)
set (QT_USE_QTXML true)

include (${QT_USE_FILE})

##no use??????
#QT4_ADD_TRANSLATION(hehe.qm translations/nsm_zh_CN.ts)
QT4_ADD_RESOURCES(BROWSER_RESOURCES_CPP data/data.qrc htmls/htmls.qrc)

set (browser_SRCS
  autosaver.cpp 
  bookmarks.cpp 
  browserapplication.cpp 
  browsermainwindow.cpp 
  chasewidget.cpp 
  cookiejar.cpp 
  downloadmanager.cpp 
  edittableview.cpp 
  edittreeview.cpp 
  history.cpp 
  modelmenu.cpp 
  networkaccessmanager.cpp 
  searchlineedit.cpp 
  settings.cpp 
  squeezelabel.cpp 
  tabwidget.cpp 
  toolbarsearch.cpp 
  urllineedit.cpp 
  webview.cpp 
  xbel.cpp 
  main.cpp
)

set (browser_moc_HDRS
  autosaver.h 
  bookmarks.h 
  browserapplication.h 
  browsermainwindow.h 
  chasewidget.h 
  cookiejar.h 
  downloadmanager.h 
  edittableview.h 
  edittreeview.h 
  history.h 
  modelmenu.h 
  networkaccessmanager.h 
  searchlineedit.h 
  settings.h 
  squeezelabel.h 
  tabwidget.h 
  toolbarsearch.h 
  urllineedit.h 
  webview.h 
  xbel.h
)
set (browser_UIS
  addbookmarkdialog.ui 
  bookmarks.ui 
  cookies.ui 
  cookiesexceptions.ui 
  downloaditem.ui 
  downloads.ui 
  history.ui 
  passworddialog.ui 
  proxy.ui 
  settings.ui
)

#################
QT4_WRAP_UI (browser_UIS_H  ${browser_UIS} )

QT4_WRAP_CPP(browser_moc_SRCS  ${browser_moc_HDRS})


add_executable (kitbrowser WIN32
  #main.cpp
  ${browser_SRCS}
  ########
  ${browser_UIS_H}
  ${browser_moc_SRCS}
  ${BROWSER_RESOURCES_CPP}
  ${browser_EXE_RC}
)

TARGET_LINK_LIBRARIES(kitbrowser ${QT_LIBRARIES} tidy ${win32_extra_libs})

#########this section will generate a console windows application
if (WIN32)
   add_executable (browser2
     #main.cpp
     ${browser_SRCS}
     ########
     ${browser_UIS_H}
     ${browser_moc_SRCS}
     ${BROWSER_RESOURCES_CPP}
     ${browser_EXE_RC}
   )

   TARGET_LINK_LIBRARIES(browser2 ${QT_LIBRARIES} tidy ${win32_extra_libs} )
endif (WIN32)
#Z:/Qt/VC-4.4.1/lib/QtUiTools.lib
#/usr/lib/qt4/libQtUiTools.a
