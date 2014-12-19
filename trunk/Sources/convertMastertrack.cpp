/* 2014-02-22                 */
/* Dr. Rainer Sieger          */

// http://en.wikipedia.org/wiki/Nautical_mile

#include <QGeoCoordinate>

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2014-02-22

int MainWindow::convertMastertrack( const QString &s_FilenameIn, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const QStringList &sl_crInput, const int i_NumOfFiles )
{
    int         i                            = 1;
    int         n                            = 0;

    int			stopProgress                 = 0;

    int         i_ExpeditionID               = 0;

    double      d_Bearing                    = 0.;
    double      d_Distance                   = 0.;
    double      d_Time                       = 0.;

    QString     s_OutputStr                  = "";

    QString     s_FilenameOut                = "";
    QString     s_FilenameImport             = "";
    QString     s_FilenameExpedition         = "";

    QString     s_LocationStart              = "";
    QString     s_LocationEnd                = "";
    QString     s_DateBegin                  = "";
    QString     s_DateEnd                    = "";
    QString     s_Expedition                 = "";
    QString     s_Basis                      = "";
    QString     s_Author                     = "";
    QString     s_Cruise                     = "";
    QString     s_CruiseReport               = "";
    QString     s_StationList                = "";
    QString     s_Mastertrack_generalized    = "";
    QString     s_Mastertrack_fullresolution = "";
    QString     s_FurtherDetails             = "";

    QString     s_EOL                        = setEOLChar( i_EOL );

    QStringList sl_Input;

    QDateTime   DateTime1( QDate( 0001, 01, 01 ) );
    QDateTime   DateTime2( QDate( 0001, 01, 01 ) );

    QGeoCoordinate  Pos1;
    QGeoCoordinate  Pos2;


// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( -10 );

// **********************************************************************************************

    s_Basis      = findBasis( s_FilenameIn );
    s_Expedition = findExpedition( s_FilenameIn, sl_crInput );

    if ( s_Basis == "ps" )
    {
        i_ExpeditionID = findExpeditionID( s_Expedition, sl_crInput );

        if ( i_ExpeditionID > 0 )
        {
            s_Cruise                     = sl_crInput.at( i_ExpeditionID ).section( "\t", 0, 0 );
            s_LocationStart              = sl_crInput.at( i_ExpeditionID ).section( "\t", 4, 4 );
            s_LocationEnd                = sl_crInput.at( i_ExpeditionID ).section( "\t", 6, 6 );
            s_DateBegin                  = sl_crInput.at( i_ExpeditionID ).section( "\t", 3, 3 );
            s_DateEnd                    = sl_crInput.at( i_ExpeditionID ).section( "\t", 5, 5 );
            s_Author                     = sl_crInput.at( i_ExpeditionID ).section( "\t", 25, 25 );
            s_CruiseReport               = sl_crInput.at( i_ExpeditionID ).section( "\t", 26, 26 );
            s_StationList                = sl_crInput.at( i_ExpeditionID ).section( "\t", 27, 27 );
            s_Mastertrack_generalized    = sl_crInput.at( i_ExpeditionID ).section( "\t", 28, 28 );
            s_Mastertrack_fullresolution = sl_crInput.at( i_ExpeditionID ).section( "\t", 29, 29 );
            s_FurtherDetails             = sl_crInput.at( i_ExpeditionID ).section( "\t", 30, 30 );

            if ( ( s_Expedition.toLower().startsWith( "ant-" ) == true ) || ( s_Expedition.toLower().startsWith( "ark-" ) == true ) )
            {
                s_Expedition = sl_crInput.at( i_ExpeditionID ).section( "\t", 0, 0 ) + "/" + sl_crInput.at( i_ExpeditionID ).section( "\t", 1, 1 ).section( "/", 1, 1 );
                s_Cruise     = sl_crInput.at( i_ExpeditionID ).section( "\t", 1, 1 );
            }
        }
    }

// **********************************************************************************************
// open output file

    QFileInfo fi( s_FilenameIn );

    s_FilenameExpedition = s_Expedition;
    s_FilenameExpedition.replace( "/", "_" );
    s_FilenameExpedition.replace( ".", "_" );

    s_FilenameOut = fi.absolutePath() + "/" + s_FilenameExpedition + "_mastertrack";

    if ( s_FilenameIn.contains( "_generalized" ) == true )
        s_FilenameOut.append( "_generalized" );

    s_FilenameOut.append( ".txt" );

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting Mastertrack data..." ), sl_Input.count() );

// **********************************************************************************************


    if ( s_FilenameIn.contains( "_generalized" ) == false )
        tout << "Date/Time (UTC)" << "\t" << "Latitude" << "\t" << "Longitude" << "\t" << "Flag for data source" << s_EOL;
    else
        tout << "Date/Time (UTC)" << "\t" << "Latitude" << "\t" << "Longitude" << s_EOL;

    while ( ( i<n ) && ( stopProgress != _APPBREAK_ ) )
    {
        DateTime1.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy-MM-dd" ) );
        DateTime1.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ss" ) );

//      DateTime1.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy/MM/dd" ) );
//      DateTime1.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ssZ" ) );

        tout << DateTime1.toString( Qt::ISODate ) << "\t" << sl_Input.at( i ).section( "\t", 1, 1 ) << "\t" << sl_Input.at( i ).section( "\t", 2, 2 )  << "\t" << sl_Input.at( i ).section( "\t", 3, 3 ) << s_EOL;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    fout.close();

// **********************************************************************************************

    compressFile( s_FilenameOut );

// **********************************************************************************************

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    if ( s_FilenameIn.contains( "_generalized" ) == false )
    {
        s_FilenameImport = fi.absolutePath() + "/" + s_FilenameExpedition + "_mastertrack_imp.txt";

        QFile fimp( s_FilenameImport );

        if ( fimp.open( QIODevice::WriteOnly | QIODevice::Text) == false )
            return( -20 );

        QTextStream timp( &fimp );

        switch ( i_CodecOutput )
        {
        case _SYSTEM_:
            break;
        case _LATIN1_:
            timp.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
            break;
        case _APPLEROMAN_:
            timp.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
            break;
        default:
            timp.setCodec( QTextCodec::codecForName( "UTF-8" ) );
            break;
        }

// **********************************************************************************************

        initProgress( i_NumOfFiles, s_FilenameIn, tr( "Creating import file..." ), sl_Input.count() );

// **********************************************************************************************
// write data description

        if ( ( s_Basis == "ps" ) && ( i_ExpeditionID > 0 ) )
        {
            timp << "/* DATA DESCRIPTION:" << s_EOL;
            timp << "Author:\t" << s_Author << s_EOL;
            timp << "Source:\t32" << s_EOL;
            timp << "Title:\tStation list and links to master tracks in different resolutions of POLARSTERN cruise " << s_Cruise << ", ";
            timp << s_LocationStart << " - " << s_LocationEnd << ", " << s_DateBegin << " - "  << s_DateEnd << s_EOL;

            if ( s_CruiseReport.isEmpty() == false )
                timp << "Reference:\t" << s_CruiseReport << " * RELATIONTYPE: 12" << s_EOL;  // Cruise report

            if ( s_StationList.isEmpty() == false )
                timp << "Reference:\t" << s_StationList << " * RELATIONTYPE: 17" << s_EOL;  // Station list

            if ( s_Mastertrack_generalized.isEmpty() == false )
                timp << "Reference:\t" << s_Mastertrack_generalized << " * RELATIONTYPE: 17" << s_EOL;  // Link to master track generalized

            if ( s_Mastertrack_fullresolution.isEmpty() == false )
                timp << "Reference:\t" << s_Mastertrack_fullresolution << " * RELATIONTYPE: 17" << s_EOL;  // Link to master track in full resolution

            if ( s_FurtherDetails.isEmpty() == false )
                timp << "Reference:\t" << s_FurtherDetails << " * RELATIONTYPE: 17" << s_EOL;  // Further details

            timp << "Export Filename:\t" << s_FilenameExpedition << "_mastertrack" << s_EOL;
            timp << "Event:\t" << s_Expedition << "-track" << s_EOL;
            timp << "PI:\t506" << s_EOL;
            timp << "Parameter:	1599 * PI: 506 * METHOD: 43 * FORMAT: yyyy-MM-dd'T'HH:mm" << s_EOL;
            timp << "\t1600 * PI: 506 * METHOD: 43 * FORMAT: ##0.00000" << s_EOL;
            timp << "\t1601 * PI: 506 * METHOD: 43 * FORMAT: ##0.00000" << s_EOL;
            timp << "\t2960 * PI: 506 * METHOD: 50 * FORMAT: ##0.000 * COMMENT: 10 min resolution" << s_EOL;
            timp << "\t21892 * PI: 506 * METHOD: 50 * FORMAT: ##0.0 * COMMENT: 10 min resolution" << s_EOL;
            timp << "Project:\t15" << s_EOL;
            timp << "Topologic Type:\t6" << s_EOL;
            timp << "Status:\t4" << s_EOL;
            timp << "Login:\t1" << s_EOL;
            timp << "*/" << s_EOL;
            timp << "Event label\t1599\t1600\t1601\t2960\t21892" << s_EOL;
        }
        else
        {
            timp << "Event label\tDate/Time\tLatitude\tLongitude\tSpeed [kn]\tHeading [deg]" << s_EOL;
        }

// **********************************************************************************************

        i = 1;

        DateTime1.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy-MM-dd" ) );
        DateTime1.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ss" ) );

//      DateTime1.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy/MM/dd" ) );
//      DateTime1.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ssZ" ) );

        i++;

        while ( ( i<n ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( ( sl_Input.at( i-1 ).section( "\t", 3, 3 ) == sl_Input.at( i ).section( "\t", 3, 3 ) )
              && ( sl_Input.at( i ).section( "\t", 3, 3 ) != "GAP" )
              && ( sl_Input.at( i ).section( "\t", 1, 1 ).contains( "e-" ) == false )
              && ( sl_Input.at( i ).section( "\t", 2, 2 ).contains( "e-" ) == false ) )
            {
                DateTime2.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy-MM-dd" ) );
                DateTime2.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ss" ) );

//              DateTime2.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy/MM/dd" ) );
//              DateTime2.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ssZ" ) );

                d_Time = (double) DateTime1.secsTo( DateTime2 );

                if ( d_Time >= 10*60 ) // 10 min
                {
                    DateTime1.setDate( QDate::fromString( sl_Input.at( i-1 ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy-MM-dd" ) );
                    DateTime1.setTime( QTime::fromString( sl_Input.at( i-1 ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ss" ) );

//                  DateTime1.setDate( QDate::fromString( sl_Input.at( i-1 ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy/MM/dd" ) );
//                  DateTime1.setTime( QTime::fromString( sl_Input.at( i-1 ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ssZ" ) );

                    d_Time = (double) DateTime1.secsTo( DateTime2 );

                    Pos1.setLatitude( sl_Input.at( i-1 ).section( "\t", 1, 1 ).toDouble() );
                    Pos1.setLongitude( sl_Input.at( i-1 ).section( "\t", 2, 2 ).toDouble() );
                    Pos2.setLatitude( sl_Input.at( i ).section( "\t", 1, 1 ).toDouble() );
                    Pos2.setLongitude( sl_Input.at( i ).section( "\t", 2, 2 ).toDouble() );

                    d_Distance = (double) Pos1.distanceTo( Pos2 ); // distance in meter
                    d_Bearing  = (double) Pos1.azimuthTo( Pos2 );  // Heading in deg

                    s_OutputStr = s_Expedition + "-track" + "\t";
                    s_OutputStr.append( DateTime2.toString( Qt::ISODate ) + "\t" + sl_Input.at( i ).section( "\t", 1, 1 ) + "\t" + sl_Input.at( i ).section( "\t", 2, 2 ) + "\t" );
                    s_OutputStr.append( QString( "%1\t%2").arg( d_Distance/d_Time * 3.6 / 1.853 ).arg( d_Bearing ) );

                    timp << s_OutputStr << s_EOL;

                    DateTime1 = DateTime2;
                    Pos1      = Pos2;
                }
            }
            else
            {
                ; // Latitude or Longitude very close to 0, e.g. 1.45e-05
            }

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }

// **********************************************************************************************

        fimp.close();

        resetProgress( i_NumOfFiles );
    }

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2014-02-23

QString MainWindow::findBasis( const QString &s_Filename )
{
    QFileInfo fi( s_Filename );

    if ( fi.baseName().toLower().startsWith( "ps" ) == true )
        return( "ps" );

    if ( fi.baseName().toLower().startsWith( "ant-" ) == true )
        return( "ps" );

    if ( fi.baseName().toLower().startsWith( "ark-" ) == true )
        return( "ps" );

    if ( fi.baseName().toLower().startsWith( "he" ) == true )
        return( "he" );

    if ( fi.baseName().toLower().startsWith( "so" ) == true )
        return( "so" );

    if ( fi.baseName().toLower().startsWith( "msm" ) == true )
        return( "msm" );

    return( "unknown" );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2014-02-23

QString MainWindow::findExpedition( const QString &s_Filename, const QStringList &sl_crInput )
{
    int         j               = 1;

    QString     s_Expedition    = "";

// **********************************************************************************************

    QFileInfo fi( s_Filename );

    if ( fi.baseName().toLower().startsWith( "ps" ) == true )
    {
        if ( fi.completeBaseName().section( "_", 0, 0 ).mid( 2 ).toFloat() < 82. )
            s_Expedition = fi.completeBaseName().section( "_", 0, 0 ) + "/" + fi.completeBaseName().section( "_", 1, 1 );
        else
            s_Expedition = fi.completeBaseName().section( "_", 0, 0 ); // PS87 or PS88.1

        return( s_Expedition );
    }

    if ( ( fi.baseName().toLower().startsWith( "ant-" ) == true ) || ( fi.baseName().toLower().startsWith( "ark-" ) == true ) )
    {
        s_Expedition = fi.baseName().section( "_", 0, 1 );
        s_Expedition.replace( "_", "/" );

        while ( j<sl_crInput.count() )
        {
            if ( sl_crInput.at( j ).section( "\t", 1, 1 ) ==  s_Expedition )
            {
                s_Expedition = sl_crInput.at( j ).section( "\t", 1, 1 );

                return( s_Expedition );
            }

            j++;
        }
    }

    if ( fi.baseName().toLower().startsWith( "he" ) == true )
    {
        s_Expedition = fi.baseName().section( "_", 0, 0 );

        return( s_Expedition );
    }

    if ( fi.baseName().toLower().startsWith( "so" ) == true )
    {
        s_Expedition = fi.baseName().section( "_", 0, 0 );

        return( s_Expedition );
    }

    return( "unknown" );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2014-02-23

int MainWindow::findExpeditionID( const QString &s_Expedition, const QStringList &sl_crInput )
{
    int         j               = 1;

// **********************************************************************************************

    while ( j<sl_crInput.count() )
    {
        if ( sl_crInput.at( j ).section( "\t", 1, 1 ) == s_Expedition )
            return( j );
        else
            j++;
    }

    return( -999 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2014-02-22

void MainWindow::doConvertMastertrack()
{
    int     i               = 0;
    int     n               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

    QStringList sl_crInput;

// **********************************************************************************************

    setStatusBar( tr( "Reading cruise reports - please wait." ) );

    QFileInfo fi( getDataLocation() );

    QFile fID( fi.absoluteFilePath() + "/" + "CruiseReports_Polarstern.txt" );

    if ( fID.open( QIODevice::WriteOnly | QIODevice::Text ) == true )
    {
        webfile m_webfile;

        m_webfile.setUrl( QLatin1String( "http://www.pangaea.de/PHP/cr/CruiseReports_Polarstern.txt" ) );

        if ( m_webfile.open() == true )
        {
            char    buffer[1024];
            qint64  nSize = 0;

            while ( ( nSize = m_webfile.read( buffer, sizeof( buffer ) ) ) > 0 )
                fID.write( buffer, nSize );

            m_webfile.close();
        }

        fID.close();
    }

    setStatusBar( tr( "Ready" ), 2 );

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( ( n = readFile( fi.absoluteFilePath() + "/" + "CruiseReports_Polarstern.txt", sl_crInput, _UTF8_ ) ) < 1 )
            err = _FILENOTEXISTS_;

        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert Mastertrack data..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = convertMastertrack( s_FilenameIn, gi_CodecInput, gi_CodecOutput, gi_EOL, sl_crInput, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting Mastertrack data was canceled" ) );

    onError( err );
}


