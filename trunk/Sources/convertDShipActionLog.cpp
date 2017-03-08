/* 2017-03-04                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::convertDShipActionLog( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i               = 1;
    int         n               = 0;

    int         i_first         = 0;
    int         i_last          = 0;

    int			stopProgress	= 0;

    QString     s_CampaignLabel = "";
    QString     s_EventLabel    = "";
    QString     s_Device        = "";

    QString     s_DateTime1     = "";
    QString     s_Latitude1     = "";
    QString     s_Longitude1    = "";
    QString     s_Elevation1    = "";

    QString     s_DateTime2     = "";
    QString     s_Latitude2     = "";
    QString     s_Longitude2    = "";
    QString     s_Elevation2    = "";

    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( -10 );

// **********************************************************************************************
// open output file

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

    switch ( i_CodecOutput )
    {
    case _SYSTEM_:
        break;
    case _LATIN1_:
        tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case _APPLEROMAN_:
        tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting DShip ActionLog..." ), sl_Input.count() );

// **********************************************************************************************

    tout << "Campaign" << "\t" << "LabelEvent" << "\t" << "Device" << "\t" << "Device (long)" << "\t" << "Action" << "\t" << "DateTimeEvent" << "\t";
    tout << "LatitudeEvent" << "\t" << "LongitudeEvent" << "\t" << "ElevationEvent" << "\t";
    tout << "Action2" << "\t" << "DateTimeEvent2" << "\t" << "LatitudeEvent2" << "\t" << "LongitudeEvent2" << "\t" << "ElevationEvent2" << s_EOL;

// **********************************************************************************************

    i_first = 1;
    i       = 2;

    while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i-1 ).section( "\t", 0, 0 ) != sl_Input.at( i ).section( "\t", 0, 0 ) )
        {
            i_last  = i-1;

            for ( int j = i_first; j <= i_last; j++ )
            {
                if ( sl_Input.at( j ).contains( "station start" ) == true )
                    i_first = j;

                if ( sl_Input.at( j ).contains( "profile start" ) == true )
                    i_first = j;

                if ( sl_Input.at( j ).contains( "station end" ) == true )
                    i_last = j;

                if ( sl_Input.at( j ).contains( "profile end" ) == true )
                    i_last = j;

                if ( sl_Input.at( j ).contains( "max depth/on ground" ) == true )
                {
                    i_first = j;
                    i_last  = j;
                }
            }

            s_CampaignLabel = sl_Input.at( i_first ).section( "\t", 0, 0 ).section( "_", 0, 0 );
            s_EventLabel    = sl_Input.at( i_first ).section( "\t", 0, 0 );

            s_EventLabel.replace( "_Underway", "_underway" );
            s_EventLabel.replace( "_00_", "_0_" );

            s_DateTime1 = sl_Input.at( i_first ).section( "\t", 1, 1 ).left( 16 );
            s_DateTime1.replace( "/", "-" );
            s_DateTime1.replace( " ", "T" );

            s_DateTime2 = sl_Input.at( i_last ).section( "\t", 1, 1 ).left( 16 );
            s_DateTime2.replace( "/", "-" );
            s_DateTime2.replace( " ", "T" );

            s_Latitude1  = QString( "%1" ).arg( sl_Input.at( i_first ).section( "\t", 14, 14 ).toDouble(), 0, 'f', 5 );
            s_Longitude1 = QString( "%1" ).arg( sl_Input.at( i_first ).section( "\t", 15, 15 ).toDouble(), 0, 'f', 5 );

            s_Latitude2  = QString( "%1" ).arg( sl_Input.at( i_last ).section( "\t", 14, 14 ).toDouble(), 0, 'f', 5 );
            s_Longitude2 = QString( "%1" ).arg( sl_Input.at( i_last ).section( "\t", 15, 15 ).toDouble(), 0, 'f', 5 );

            s_Elevation1 = "";

            if ( sl_Input.at( i_first ).section( "\t", 11, 11 ) != "0" )
            {
                if ( sl_Input.at( i_first ).section( "\t", 11, 11 ).toDouble() > 50. )
                    s_Elevation1 = QString( "-%1" ).arg( sl_Input.at( i_first ).section( "\t", 11, 11 ).toDouble(), 0, 'f', 0 );
                else
                    s_Elevation1 = QString( "-%1" ).arg( sl_Input.at( i_first ).section( "\t", 11, 11 ).toDouble(), 0, 'f', 1 );
            }

            s_Elevation2 = "";

            if ( sl_Input.at( i_last ).section( "\t", 11, 11 ) != "0" )
            {
                if ( sl_Input.at( i_last ).section( "\t", 11, 11 ).toDouble() > 50. )
                    s_Elevation2 = QString( "-%1" ).arg( sl_Input.at( i_last ).section( "\t", 11, 11 ).toDouble(), 0, 'f', 0 );
                else
                    s_Elevation2 = QString( "-%1" ).arg( sl_Input.at( i_last ).section( "\t", 11, 11 ).toDouble(), 0, 'f', 1 );
            }

            s_Device = sl_Input.at( i_first ).section( "\t", 3, 3 );

            s_Device.replace( "CTDOZE", "CTD-RO" );
            s_Device.replace( "UCTD", "CTD-UW" );
            s_Device.replace( "MN_S5", "MSN" );
            s_Device.replace( "FLOAT", "ARGOFL" );
            s_Device.replace( "ADCP_150", "VMADCP-150" );
            s_Device.replace( "TSG_KEEL", "TSG" );
            s_Device.replace( "PCO2_GO", "UWPCO2" );
            s_Device.replace( "PCO2_SUB", "UWPCO2" );
            s_Device.replace( "WST", "SWEAS" );
            s_Device.replace( "OCEANET", "UWAOBS" );
            s_Device.replace( "SPM", "SPEC" );

            if ( i_first == i_last )
            {
                tout << s_CampaignLabel + "\t";
                tout << s_EventLabel << "\t" << s_Device << "\t";
                tout << sl_Input.at( i_first ).section( "\t", 2, 2 ) << "\t" << sl_Input.at( i_first ).section( "\t", 4, 4 ) << "\t";
                tout << s_DateTime1 << "\t" << s_Latitude1 << "\t" << s_Longitude1 << "\t" << s_Elevation1 << s_EOL;
            }
            else
            {
                tout << s_CampaignLabel + "\t";
                tout << s_EventLabel << "\t" << s_Device << "\t";
                tout << sl_Input.at( i_first ).section( "\t", 2, 2 ) << "\t" << sl_Input.at( i_first ).section( "\t", 4, 4 ) << "\t";
                tout << s_DateTime1 << "\t" << s_Latitude1 << "\t" << s_Longitude1 << "\t" << s_Elevation1 << "\t";
                tout << sl_Input.at( i_last ).section( "\t", 4, 4 ) << "\t";
                tout << s_DateTime2 << "\t" << s_Latitude2 << "\t" << s_Longitude2 << "\t" << s_Elevation2 << s_EOL;
            }

            i_first = i;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    fout.close();

    resetProgress( i_NumOfFiles );

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-05-31

void MainWindow::doConvertDShipActionLog()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert DShip ActionLog..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = convertDShipActionLog( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

                stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
            }
            else
            {
                err = _FILENOTEXISTS_;
            }
        }

        resetFileProgress( gsl_FilenameList.count() );
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting DShip ActionLog was canceled" ) );

    onError( err );
}

