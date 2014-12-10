/* 2011-05-31                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::convertTSG( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i               = 1;
    int         n               = 0;

    int			stopProgress	= 0;

    QString     s               = "";
    QString     s_DateTime      = "";
    QString     s_Position      = "";
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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting TSG data..." ), sl_Input.count() );

// **********************************************************************************************

    tout << "Event label" << "\t" << "Date/Time" << "\t" << "Latitude" << "\t" << "Longitude" << "\t";
    tout << "Depth, water [m]" << "\t" << "Temperature, water [C]@ITS-90" << "\t" << "Salinity []" << s_EOL;

    tout << "PS-track";

    while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        s_DateTime = sl_Input.at( i ).section( "\t", 0, 0 );
        s_DateTime.replace( " ", "T" );

        s_Position = sl_Input.at( i ).section( "\t", 1, 2 );
        s_Position.replace( " ", "" );

        s = sl_Input.at( i ).section( "\t", 3, 4 );
        s.replace( " ", "" );
        s.replace( "-999.000000", "" );

        if ( s != "\t" )
            tout << "\t" + s_DateTime << "\t" << s_Position << "\t5\t" << s << s_EOL;

        s = sl_Input.at( i ).section( "\t", 5, 6 );
        s.replace( " ", "" );
        s.replace( "-999.000000", "" );

        if ( s != "\t" )
            tout << "\t" + s_DateTime << "\t" << s_Position << "\t11\t" << s << s_EOL;

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

void MainWindow::doConvertTSG()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert TSG data..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = convertTSG( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting TSG data was canceled" ) );

    onError( err );
}

