/****************************************************************
**
** Preferences
** 2013-11-25, Dr. Rainer Sieger
**
****************************************************************/

#include "Application.h"

const QString PrefDate = "PanConverter, 2013-11-25";

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 11.3.2004

/*! @brief Sichern der Programmeinstellungen */

void MainWindow::savePreferences()
{
    #if defined(Q_OS_LINUX)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    #if defined(Q_OS_MAC)
        QSettings settings( getPreferenceFilename(), QSettings::NativeFormat );
    #endif

    #if defined(Q_OS_WIN)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    settings.beginGroup( QCoreApplication::applicationName() );

    settings.beginGroup( "Window" );
    settings.setValue( "WindowX", x() );
    settings.setValue( "WindowY", y() );
    settings.setValue( "WindowHeight", height() );
    settings.setValue( "WindowWidth", width() );
    settings.endGroup();

    settings.beginGroup( "Dialog" );
    settings.setValue( "DialogX", posDialog.x() );
    settings.setValue( "DialogY", posDialog.y() );
    settings.endGroup();

    settings.beginGroup( "ParameterDialog" );
    settings.setValue( "DialogHeight", sizeParameterDialog.height() );
    settings.setValue( "DialogWidth", sizeParameterDialog.width() );
    settings.endGroup();

    // General settings
    settings.beginGroup( "GeneralSettings" );
    settings.setValue( "NumOfProgramStarts", gi_NumOfProgramStarts );
    settings.setValue( "Path", gs_Path );
    settings.setValue( "FileFormat", gs_FilenameFormat );
    settings.setValue( "CodecInput", gi_CodecInput );
    settings.setValue( "CodecOutput", gi_CodecOutput );
    settings.setValue( "CodecDownload", gi_CodecDownload );
    settings.setValue( "EOL", gi_EOL );
    settings.setValue( "Extension", gi_Extension );
    settings.endGroup();

    // Google Earth
    settings.beginGroup( "GoogleEarth" );
    settings.setValue( "FilenameGoogleEarthProgram", gs_FilenameGoogleEarthProgram );
    settings.setValue( "StartGoogleEarth", gb_startGoogleEarth );
    settings.setValue( "IconColor", gi_IconColor );
    settings.setValue( "IconSize", gi_IconSize );
    settings.setValue( "TracklineWidth", gi_TracklineWidth );
    settings.setValue( "TracklineColor", gi_TracklineColor );
    settings.endGroup();

    settings.endGroup();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 21.11.2007

/*! @brief Laden der Programmeinstellungen */

void MainWindow::loadPreferences()
{
    #if defined(Q_OS_LINUX)
        gi_CodecInput                       = _UTF8_;               //!< UTF-8
        gi_CodecOutput                      = _UTF8_;
        gi_CodecDownload                    = _UTF8_;
        gi_EOL                              = _UNIX_;
    #endif

    #if defined(Q_OS_MAC)
        gi_CodecInput                       = _APPLEROMAN_;        //!< Apple Roman
        gi_CodecOutput                      = _APPLEROMAN_;
        gi_CodecDownload                    = _APPLEROMAN_;
        gi_EOL                              = _UNIX_;
    #endif

    #if defined(Q_OS_WIN)
        gi_CodecInput                       = _LATIN1_;            //!< Latin-1 = ISO 8859-1
        gi_CodecOutput                      = _LATIN1_;
        gi_CodecDownload                    = _LATIN1_;
        gi_EOL                              = _WIN_;
    #endif

    #if defined(Q_OS_LINUX)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    #if defined(Q_OS_MAC)
        QSettings settings( getPreferenceFilename(), QSettings::NativeFormat );
    #endif

    #if defined(Q_OS_WIN)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    settings.beginGroup( QCoreApplication::applicationName() );

    settings.beginGroup( "Window" );
    posWindow.setX( settings.value( "WindowX", 100 ).toInt() );
    posWindow.setY( settings.value( "WindowY", 100 ).toInt() );
    sizeWindow.setHeight( settings.value( "WindowHeight", 250 ).toInt() );
    sizeWindow.setWidth( settings.value( "WindowWidth", 600 ).toInt() );
    settings.endGroup();

    settings.beginGroup( "Dialog" );
    posDialog.setX( settings.value( "DialogX", 110 ).toInt() );
    posDialog.setY( settings.value( "DialogY", 110 ).toInt() );
    settings.endGroup();

    settings.beginGroup( "ParameterDialog" );
    sizeParameterDialog.setHeight( settings.value( "DialogHeight", 100 ).toInt() );
    sizeParameterDialog.setWidth( settings.value( "DialogWidth", 100 ).toInt() );
    settings.endGroup();

    // General settings
    settings.beginGroup( "GeneralSettings" );
    gi_NumOfProgramStarts = settings.value( "NumOfProgramStarts", 0 ).toInt();
    gs_Path               = settings.value( "Path", "" ).toString();
    gs_FilenameFormat     = settings.value( "FileFormat", "zz%a_%N" ).toString();
    gi_CodecInput         = settings.value( "CodecInput", gi_CodecInput ).toInt();
    gi_CodecOutput        = settings.value( "CodecOutput", gi_CodecOutput ).toInt();
    gi_CodecDownload      = settings.value( "CodecDownload", gi_CodecDownload ).toInt();
    gi_EOL                = settings.value( "EOL", gi_EOL ).toInt();
    gi_Extension          = settings.value( "Extension", _TXT_ ).toInt();
    settings.endGroup();

    // Google Earth
    settings.beginGroup( "GoogleEarth" );
    gs_FilenameGoogleEarthProgram = settings.value( "FilenameGoogleEarthProgram", "Browse through the Google Earth program file with \"Browse ...\"" ).toString();
    gb_startGoogleEarth           = settings.value( "StartGoogleEarth", false ).toBool();
    gi_IconColor                  = settings.value( "IconColor", _RED_ ).toInt();
    gi_IconSize                   = settings.value( "IconSize", 40 ).toInt();
    gi_TracklineWidth             = settings.value( "TracklineWidth", 20 ).toInt();
    gi_TracklineColor             = settings.value( "TracklineColor", _ORANGE_ ).toInt();
    settings.endGroup();

    settings.endGroup();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermitteln des Dateinamens fuer die Programmeinstellungen.
*   @return Name der Datei fuer die Programmeinstellungen. Dateiname ist gleich "_ERROR_",
*   wenn Betriebssystem ungleich Windows, Mac OS oder Unix ist.
*/

QString MainWindow::getPreferenceFilename()
{
    QString s_PrefFilename = "";

    #if defined(Q_OS_LINUX)
        s_PrefFilename = QDir::homePath() + QLatin1String( "/.config/" ) + QCoreApplication::organizationName() + "/" + QCoreApplication::applicationName() + "/" + QCoreApplication::applicationName() + QLatin1String( ".conf" );
    #endif

    #if defined(Q_OS_MAC)
        s_PrefFilename = QDir::homePath() + QLatin1String( "/Library/Preferences/" ) + QCoreApplication::organizationDomain().section( ".", 1, 1 ) + "." + QCoreApplication::organizationDomain().section( ".", 0, 0 ) + "." + QCoreApplication::applicationName() + QLatin1String( ".plist" );
    #endif

    #if defined(Q_OS_WIN)
        if ( QCoreApplication::applicationName().toLower().endsWith( "portable" ) )
        {
            s_PrefFilename = QCoreApplication::applicationDirPath() + "/" + QCoreApplication::applicationName() + QLatin1String( ".ini" );
        }
        else
        {
            QSettings cfg( QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName() );
            s_PrefFilename = QFileInfo( cfg.fileName() ).absolutePath() + "/" + QCoreApplication::applicationName() + "/" + QCoreApplication::applicationName() + QLatin1String( ".ini" );
        }
    #endif

    return( s_PrefFilename );
}
