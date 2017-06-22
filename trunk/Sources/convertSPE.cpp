/* 2011-05-31                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::convertSPE( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i               = 1;
    int         j               = 0;
    int         k               = 0;
    int         n               = 0;

    int			stopProgress	= 0;

    bool        b_datafound     = false;

    QString     tempStr         = "";
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

    while ( ( b_datafound == false ) && ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).startsWith( "$DATA:") == true )
            b_datafound = true;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    if ( b_datafound == true )
    {
        tempStr = sl_Input.at( i++ ).split( QRegularExpression( "\\s+" ) ).join( "\t" );
        k       = tempStr.section( "\t", 1, 1 ).toInt();

        tout << "Channel" << "\t" << "cps" << s_EOL;

        while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
        {
            tempStr = sl_Input.at( i ).split( QRegularExpression( "\\s+" ) ).join( "\t" );

            n = NumOfSections( tempStr );

            for ( j=1; j<n; j++ )
                tout << QString( "%1\t%2" ).arg( k++ ).arg( tempStr.section( "\t", j, j ) ) << s_EOL;

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }
    }
    else
    {
        tout << "No data found! Wrong format." << s_EOL;
    }

// **********************************************************************************************

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

void MainWindow::doConvertSPE()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert SPE data..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = convertSPE( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting SPE data was canceled" ) );

    onError( err );
}

