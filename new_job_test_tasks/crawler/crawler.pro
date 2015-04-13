#-------------------------------------------------
#
# Project created by QtCreator 2010-10-27T15:07:59
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = crawler
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    crawler_src/download_queue.cpp \
    crawler_src/downloader.cpp \
    crawler_src/crawler.cpp

HEADERS += \
    crawler_src/download_queue.h \
    crawler_src/downloader.h \
    crawler_src/const.h

#add environment variable $BOOST_LIBS with path to compiled Boost librarys. example:env BOOST_LIBS=/usr/local/lib
LIBS += -L$(BOOST_LIBS) -lboost_system -lboost_filesystem -lboost_thread
