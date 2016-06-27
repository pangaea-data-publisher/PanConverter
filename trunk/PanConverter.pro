# PanConverter
# Rainer Sieger
# Alfred Wegener Institute, Bremerhaven, Germany
# last change: 2014-10-04

macx {
    # creating cache file
    cache()

    # Set SDK
    QMAKE_MAC_SDK = macosx10.11

    # Only Intel binaries are accepted so force this
    CONFIG += x86_64

    # Minimum OS X version for submission is 10.7
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

    # Icon
    ICON = ./Resources/icon/Application.icns

    # Replace default Info.plist
    QMAKE_INFO_PLIST = ./Resources/Info.plist

    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2
    QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}

win32 {
    # OpenSSL
    LIBS += -LC:/Qt/OpenSSL-Win32/bin -lubsec
    INCLUDEPATH += C:/Qt/OpenSSL-Win32/include

    # Icon
    RC_FILE = ./Resources/Application.rc
}

linux-g++ {
}

TARGET = PanConverter
TEMPLATE = app
QT += widgets network xml positioning

INCLUDEPATH += ./Headers

HEADERS = ./Headers/Application.h \
    ./Headers/Globals.h \
    ./Headers/Webfile.h \
    ./Headers/convertPosition.h \
    ./Headers/Ellipsoid.h \
    ./Forms/AboutDialog/AboutDialog.h \
    ./Forms/GeneralOptionsDialog/GeneralOptionsDialog.h \
    ./Forms/GPStoGpxOptionsDialog/GPStoGpxOptionsDialog.h \
    ./Forms/GPStoKmlOptionsDialog/GPStoKmlOptionsDialog.h

SOURCES = ./Sources/ApplicationInit.cpp \
    ./Sources/ApplicationMainWindow.cpp \
    ./Sources/ApplicationErrors.cpp \
    ./Sources/ApplicationPreferences.cpp \
    ./Sources/ApplicationCreateMenu.cpp \
    ./Sources/compressFiles.cpp \
    ./Sources/concatenateFiles.cpp \
    ./Sources/LineCanBeWritten.cpp \
    ./Sources/CliwocConverter.cpp \
    ./Sources/convertCHUAN.cpp \
    ./Sources/convertCrossRefXML.cpp \
    ./Sources/convertMastertrack.cpp \
    ./Sources/convertNOAA-IOAS.cpp \
    ./Sources/convertPosition.cpp \
    ./Sources/convertSOCAT.cpp \
    ./Sources/convertTreeRingXML.cpp \
    ./Sources/convertTSG.cpp \
    ./Sources/convertWOCE-WHP.cpp \
    ./Sources/createGpxFile.cpp \
    ./Sources/createKmlFile.cpp \
    ./Sources/JSONImportHeader.cpp \
    ./Sources/startProgram.cpp \
    ./Sources/Webfile.cpp \
    ./Forms/AboutDialog/AboutDialog.cpp \
    ./Forms/GeneralOptionsDialog/GeneralOptionsDialog.cpp \
    ./Forms/GPStoGpxOptionsDialog/GPStoGpxOptionsDialog.cpp \
    ./Forms/GPStoKmlOptionsDialog/GPStoKmlOptionsDialog.cpp

FORMS = ./Forms/AboutDialog/aboutdialog.ui \
    ./Forms/GeneralOptionsDialog/generaloptionsdialog.ui \
    ./Forms/GPStoGpxOptionsDialog/gpstogpxoptionsdialog.ui \
    ./Forms/GPStoKmlOptionsDialog/gpstokmloptionsdialog.ui
