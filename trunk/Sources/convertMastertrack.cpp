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

    QString     s_Expedition                 = "";
    QString     s_ExpeditionOptional         = "";

    QString     s_FilenameOut                = "";
    QString     s_FilenameZip                = "";
    QString     s_FilenameRefImp             = "";
    QString     s_FilenameExpedition         = "";

    QString     s_FileSize                   = "";

    QString     s_EOL                        = setEOLChar( i_EOL );

    QStringList sl_Input;

//  QDateTime   DateTime( QDate( 0001, 01, 01 ) );
    QDateTime   DateTime = QDateTime().toUTC();

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( -10 );

// **********************************************************************************************
// open output file

    QFileInfo fi( s_FilenameIn );

    s_Expedition = findExpedition( fi.completeBaseName(), sl_crInput );

    if ( s_Expedition.startsWith( "unknown_" ) == true )
    {
        s_FilenameOut    = s_Expedition.section( "unknown_", 1, 1 );
        s_FilenameOut    = fi.absolutePath() + "/" + s_FilenameOut.replace( "/", "_" ) + "_mastertrack";
        s_FilenameRefImp = s_Expedition.section( "unknown_", 1, 1 );
        s_FilenameRefImp = fi.absolutePath() + "/" + s_FilenameRefImp.replace( "/", "_" ) + "_ref_imp.txt";
        s_FilenameZip    = s_Expedition.section( "unknown_", 1, 1 );
        s_FilenameZip    = fi.absolutePath() + "/" + s_FilenameZip.replace( "/", "_" ) + "_mastertrack.zip";
    }
    else
    {
        s_FilenameOut    = s_Expedition;
        s_FilenameOut    = fi.absolutePath() + "/" + s_FilenameOut.replace( "/", "_" ) + "_mastertrack";
        s_FilenameRefImp = s_Expedition;
        s_FilenameRefImp = fi.absolutePath() + "/" + s_FilenameRefImp.replace( "/", "_" ) + "_ref_imp.txt";
        s_FilenameZip    = s_Expedition;
        s_FilenameZip    = fi.absolutePath() + "/" + s_FilenameZip.replace( "/", "_" ) + "_mastertrack.zip";
    }

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
// open references import file

    QFile fref( s_FilenameRefImp );

    if ( fref.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tref( &fref );

    switch ( i_CodecOutput )
    {
    case _SYSTEM_:
        break;
    case _LATIN1_:
        tref.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case _APPLEROMAN_:
        tref.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tref.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting Mastertrack data..." ), sl_Input.count() );

// **********************************************************************************************

    if ( sl_Input.at( 0 ).section( "\t", 0, 0 ) == "DATE/TIME [UTC]" ) // is FIELAX format
    {
        if ( s_FilenameIn.contains( "_generalized" ) == false )
            tout << "Date/Time (UTC)" << "\t" << "Latitude" << "\t" << "Longitude" << "\t" << "Flag for data source" << s_EOL;
        else
            tout << "Date/Time (UTC)" << "\t" << "Latitude" << "\t" << "Longitude" << s_EOL;
    }
    else
    {
        if ( s_FilenameIn.contains( "_generalized" ) == false )
            tout << "DATE/TIME [UTC]" << "\t" << "LAT [DEG]" << "\t" << "LON [DEG]" << "\t" << "NAV [VALUE]" << s_EOL;
        else
            tout << "DATE/TIME [UTC]" << "\t" << "LAT [DEG]" << "\t" << "LON [DEG]" << s_EOL;
    }

    while ( ( i<n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).section( "\t", 0, 0 ).isEmpty() == false )
        {
            DateTime.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy-MM-dd" ) );
            DateTime.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ss" ) );
        }
        else
        {
            // Recover time during daylight saving hour in March
            DateTime = DateTime.addSecs( 1 );
        }

        tout << DateTime.toString( "yyyy-MM-ddThh:mm:ss" ) << "\t" << sl_Input.at( i ).section( "\t", 1, 1 ) << "\t" << sl_Input.at( i ).section( "\t", 2, 2 )  << "\t" << sl_Input.at( i ).section( "\t", 3, 3 ) << s_EOL;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    fout.close();

// **********************************************************************************************

    compressFile( s_FilenameOut, _ZIP_, findZip( _ZIP_ ) );

    QFileInfo fzip( s_FilenameZip );

    if ( fzip.size() > 1000*1024 )
        s_FileSize = QString( "%1 MB" ).arg( (double) fzip.size()/1024./1024., 0, 'f', 1 );
    else if ( fzip.size() > 1024 )
        s_FileSize = QString( "%1 kBytes" ).arg( fzip.size()/1024 );
    else if ( fzip.size() > 0 )
        s_FileSize = QString( "%1 Bytes" ).arg( fzip.size() );
    else
        s_FileSize = "file not found";

// **********************************************************************************************
// create ref import file

    i_ExpeditionID = findExpeditionID( fi.completeBaseName(), sl_crInput );

    if ( i_ExpeditionID > 1000 )
        i_ExpeditionID -= 1000;

    tref << "Title" << "\t" << "URI" << s_EOL;

    if ( i_ExpeditionID > 0 )
    {
        s_Expedition         = sl_crInput.at( i_ExpeditionID ).section( "\t", 1, 1 );
        s_ExpeditionOptional = sl_crInput.at( i_ExpeditionID ).section( "\t", 0, 0 );

        if ( s_Expedition.startsWith( "PS" ) == true )
        {
            s_FilenameExpedition = sl_crInput.at( i_ExpeditionID ).section( "\t", 1, 1 ).replace( "/", "_" );

            tref << "Master track of POLARSTERN cruise " << s_Expedition << " (" << s_ExpeditionOptional << ") in 1 sec resolution ";
            tref << "(zipped, " << s_FileSize << ")" << "\t";
            tref << "http://hs.pangaea.de/nav/mastertrack/ps/" << s_FilenameExpedition << "_mastertrack.zip" << s_EOL;

            tref << "Generalized master track of POLARSTERN cruise " << s_Expedition << " (" << s_ExpeditionOptional << ")" << "\t";
            tref << "http://hs.pangaea.de/nav/mastertrack/ps/" << s_FilenameExpedition << "_mastertrack_generalized.zip" << s_EOL;

            tref << "Station list of POLARSTERN cruise " << s_Expedition << " (" << s_ExpeditionOptional << ")" << "\t";
            tref << "https://pangaea.de/ddi/" << s_FilenameExpedition << ".tab?retr=events/Polarstern/" << s_FilenameExpedition;
            tref << ".retr&conf=events/CruiseReportText.conf&format=textfile" << s_EOL;

            tref << "Trackline map and processing report for navigation sensors from POLARSTERN cruise " << s_Expedition << " (" << s_ExpeditionOptional << ")" << "\t";
            tref << "hdl:10013/epic." << s_Expedition << s_EOL;
        }

        if ( ( s_Expedition.startsWith( "ANT-" ) == true ) || ( s_Expedition.startsWith( "ARK-" ) == true ) )
        {
            s_FilenameExpedition = findExpedition( fi.completeBaseName(), sl_crInput ).replace( "/", "_" );

            tref << "Master track of POLARSTERN cruise " << s_Expedition << " in 1 sec resolution ";
            tref << "(zipped, " << s_FileSize << ")" << "\t";
            tref << "http://hs.pangaea.de/nav/mastertrack/ps/" << s_FilenameExpedition << "_mastertrack.zip" << s_EOL;

            tref << "Generalized master track of POLARSTERN cruise " << s_Expedition << "\t";
            tref << "http://hs.pangaea.de/nav/mastertrack/ps/" << s_FilenameExpedition << "_mastertrack_generalized.zip" << s_EOL;

            s_FilenameExpedition = sl_crInput.at( i_ExpeditionID ).section( "\t", 1, 1 ).replace( "/", "_" );

            tref << "Station list of POLARSTERN cruise " << s_Expedition << "\t";
            tref << "https://pangaea.de/ddi/" << s_FilenameExpedition << ".tab?retr=events/Polarstern/" << s_FilenameExpedition;
            tref << ".retr&conf=events/CruiseReportText.conf&format=textfile" << s_EOL;

            tref << "Trackline map and processing report for navigation sensors from POLARSTERN cruise " << s_Expedition << "\t";
            tref << "hdl:10013/epic." << s_FilenameExpedition << s_EOL;
        }
    }

    if ( s_Expedition.startsWith( "HE" ) == true )
    {
        tref << "Master track of HEINCKE cruise " << s_Expedition << " in 1 sec resolution ";
        tref << "(zipped, " << s_FileSize << ")" << "\t";
        tref << "http://hs.pangaea.de/nav/mastertrack/he/" << s_Expedition << "_mastertrack.zip" << s_EOL;

        tref << "Generalized master track of HEINCKE cruise " << s_Expedition << "\t";
        tref << "http://hs.pangaea.de/nav/mastertrack/he/" << s_Expedition << "_mastertrack_generalized.zip" << s_EOL;

        tref << "Station list of HEINCKE cruise " << s_Expedition << "\t";
        tref << "https://pangaea.de/ddi/" << s_Expedition << ".tab?retr=events/Heincke/" << s_Expedition;
        tref << ".retr&conf=events/CruiseReportText.conf&format=textfile" << s_EOL;

        tref << "Trackline map and processing report for navigation sensors from HEINCKE cruise " << s_Expedition << "\t";
        tref << "hdl:10013/epic." << s_Expedition << s_EOL;
    }

    if ( ( s_Expedition.startsWith( "MSM" ) == false ) && ( s_Expedition.startsWith( "M" ) == true ) )
    {
        tref << "Master track of METEOR cruise " << s_Expedition << " in 1 sec resolution ";
        tref << "(zipped, " << s_FileSize << ")" << "\t";
        tref << "http://hs.pangaea.de/nav/mastertrack/m/" << s_Expedition << "_mastertrack.zip" << s_EOL;

        tref << "Generalized master track of METEOR cruise " << s_Expedition << "\t";
        tref << "http://hs.pangaea.de/nav/mastertrack/m/" << s_Expedition << "_mastertrack_generalized.zip" << s_EOL;

        tref << "Station list of METEOR cruise " << s_Expedition << "\t";
        tref << "https://pangaea.de/ddi/" << s_Expedition << ".tab?retr=events/Meteor_1986/" << s_Expedition;
        tref << ".retr&conf=events/CruiseReportText.conf&format=textfile" << s_EOL;

        tref << "Trackline map and processing report for navigation sensors from METEOR cruise " << s_Expedition << "\t";
        tref << "hdl:10013/epic." << s_Expedition << s_EOL;
    }

    if ( s_Expedition.startsWith( "unknown" ) == true )
    {
        tref << "Master track of unknown cruise " << s_Expedition.section( "unknown_", 1, 1 ) << " in 1 sec resolution ";
        tref << "(zipped, " << s_FileSize << ")" << "\t";
        tref << "http://hs.pangaea.de/nav/mastertrack/unknown/" << s_Expedition.section( "unknown_", 1, 1 ) << "_mastertrack.zip" << s_EOL;

        tref << "Generalized master track of unknown cruise " << s_Expedition.section( "unknown_", 1, 1 ) << "\t";
        tref << "http://hs.pangaea.de/nav/mastertrack/unknown/" << s_Expedition.section( "unknown_", 1, 1 ) << "_mastertrack_generalized.zip" << s_EOL;

        tref << "Station list of unknown cruise " << s_Expedition.section( "unknown_", 1, 1 ) << "\t";
        tref << "https://pangaea.de/ddi/" << s_Expedition.section( "unknown_", 1, 1 ) << ".tab?retr=events/unknown/" << s_Expedition.section( "unknown_", 1, 1 );
        tref << ".retr&conf=events/CruiseReportText.conf&format=textfile" << s_EOL;

        tref << "Trackline map and processing report for navigation sensors from unknown cruise " << s_Expedition.section( "unknown_", 1, 1 ) << "\t";
        tref << "hdl:10013/epic." << s_Expedition.section( "unknown_", 1, 1 ) << s_EOL;
    }

    fref.close();

// **********************************************************************************************

    resetProgress( i_NumOfFiles );

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2015-01-10

int MainWindow::createMastertrackImportFile( const QString &s_FilenameIn, const int i_CodecInput, const int i_CodecOutput, const QStringList &sl_crInput, const int i_Resolution, const int i_NumOfFiles )
{
    int         i                            = 1;
    int         n                            = 0;

    int			stopProgress                 = 0;

    int         i_ExpeditionID               = 0;

    double      d_Bearing                    = 0.;
    double      d_Distance                   = 0.;
    double      d_Time                       = 0.;
    double      d_TimeResolution             = 0.;

    QString     qs                           = "  \"";
    QString     qe                           = "\": ";

    QString     s_OutputStr                  = "";

    QString     s_FilenameOut                = "";
    QString     s_FilenameImport             = "";
    QString     s_FilenameExpedition         = "";

    QString     s_LocationStart              = "";
    QString     s_LocationEnd                = "";
    QString     s_DateBegin                  = "";
    QString     s_DateEnd                    = "";
    QString     s_EventLabel                 = "";
    QString     s_Expedition                 = "";
    QString     s_Basis                      = "";
    QString     s_Author                     = "";
    QString     s_CruiseReport               = "";
    QString     s_StationList                = "";
    QString     s_Mastertrack_generalized    = "";
    QString     s_Mastertrack_fullresolution = "";
    QString     s_TracklineMap               = "";
    QString     s_DatasetTitle               = "";
    QString     s_DatasetComment             = "";
    QString     s_DatasetID                  = "";
    QString     s_ParameterComment           = "";
    QString     s_TimeFormat                 = "";

    QStringList sl_Parameter;
    QStringList sl_Reference;

    QStringList sl_Input;

//  QDateTime   DateTime1( QDate( 0001, 01, 01 ) );
//  QDateTime   DateTime2( QDate( 0001, 01, 01 ) );

    QDateTime   DateTime1 = QDateTime().toUTC();
    QDateTime   DateTime2 = QDateTime().toUTC();

    QGeoCoordinate  Pos1;
    QGeoCoordinate  Pos2;

// **********************************************************************************************

    switch( i_Resolution )
    {
    case _R30SEC_:
        s_ParameterComment = tr( "30-sec average");
        d_TimeResolution   = 30.;
        s_TimeFormat       = tr( "yyyy-MM-dd'T'HH:mm:ss" );
        break;
    case _R1MIN_:
        s_ParameterComment = tr( "1-min average" );
        d_TimeResolution   = 60.;
        s_TimeFormat       = tr( "yyyy-MM-dd'T'HH:mm" );
        break;
    default:
        s_ParameterComment = tr( "10-min average" );
        d_TimeResolution   = 600.;
        s_TimeFormat       = tr( "yyyy-MM-dd'T'HH:mm" );
        break;
    }

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( -10 );

// **********************************************************************************************

    QFileInfo fi( s_FilenameIn );

    s_Basis       = findBasis( fi.completeBaseName() );
    s_Expedition  = findExpedition( fi.completeBaseName(), sl_crInput );

    if ( s_Expedition.startsWith( "unknown_" ) == true )
    {
        s_FilenameOut = s_Expedition.section( "unknown_", 1, 1 );
        s_FilenameOut = fi.absolutePath() + "/" + s_FilenameOut.replace( "/", "_" ) + "_mastertrack";
    }
    else
    {
        s_FilenameOut = s_Expedition;
        s_FilenameOut = fi.absolutePath() + "/" + s_FilenameOut.replace( "/", "_" ) + "_mastertrack";
    }

    i_ExpeditionID = findExpeditionID( fi.completeBaseName(), sl_crInput );

    if ( i_ExpeditionID > 1000 )
        i_ExpeditionID -= 1000;

    if ( i_ExpeditionID > 0 )
    {
        if ( sl_crInput.at( i_ExpeditionID ).section( "\t", 0, 0 ).startsWith( "PS" ) == true )
        {
            s_EventLabel = sl_crInput.at( i_ExpeditionID ).section( "\t", 0, 0 ) + "/" + sl_crInput.at( i_ExpeditionID ).section( "\t", 1, 1 ).section( "/", 1, 1 ); // PS*-track or HE
            s_Expedition = sl_crInput.at( i_ExpeditionID ).section( "\t", 1, 1 ); // ANT-* or ARK-*
        }
        else
        {
            s_EventLabel = sl_crInput.at( i_ExpeditionID ).section( "\t", 1, 1 );
            s_Expedition = sl_crInput.at( i_ExpeditionID ).section( "\t", 0, 0 );
        }

        s_LocationStart              = sl_crInput.at( i_ExpeditionID ).section( "\t", 4, 4 );
        s_LocationEnd                = sl_crInput.at( i_ExpeditionID ).section( "\t", 6, 6 );
        s_DateBegin                  = sl_crInput.at( i_ExpeditionID ).section( "\t", 3, 3 );
        s_DateEnd                    = sl_crInput.at( i_ExpeditionID ).section( "\t", 5, 5 );
        s_Author                     = sl_crInput.at( i_ExpeditionID ).section( "\t", 25, 25 );
        s_CruiseReport               = sl_crInput.at( i_ExpeditionID ).section( "\t", 26, 26 );
        s_StationList                = sl_crInput.at( i_ExpeditionID ).section( "\t", 27, 27 );
        s_Mastertrack_generalized    = sl_crInput.at( i_ExpeditionID ).section( "\t", 28, 28 );
        s_Mastertrack_fullresolution = sl_crInput.at( i_ExpeditionID ).section( "\t", 29, 29 );
        s_TracklineMap               = sl_crInput.at( i_ExpeditionID ).section( "\t", 30, 30 );
    }
    else
    {
        s_EventLabel                 = s_Expedition.section( "unknown_", 1, 1 );

        s_Author                     = "@A@" + s_EventLabel + "@";
        s_Mastertrack_fullresolution = "@OR@" + s_EventLabel + "@";
        s_Mastertrack_generalized    = "@FR@" + s_EventLabel + "@";
    }

// **********************************************************************************************

    s_FilenameExpedition = s_EventLabel;
    s_FilenameExpedition.replace( "/", "_" );

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

    if ( s_FilenameIn.contains( "_generalized" ) == false )
    {
        initProgress( i_NumOfFiles, s_FilenameIn, tr( "Creating import file..." ), sl_Input.count() );

        if ( s_Author.isEmpty() == true )
            s_Author = tr( "22483" ); // Anonymous

        if ( s_StationList.isEmpty() == false )
        {
             if ( s_Basis == "ps" )
                s_DatasetTitle = tr( "Station list and links to master tracks in different resolutions of POLARSTERN cruise " );

            if ( s_Basis == "he" )
                s_DatasetTitle = tr( "Station list and links to master tracks in different resolutions of HEINCKE cruise " );

            if ( s_Basis == "m" )
                s_DatasetTitle = tr( "Station list and links to master tracks in different resolutions of METEOR cruise " );

            if ( s_Basis == "msm" )
                s_DatasetTitle = tr( "Station list and links to master tracks in different resolutions of Maria S. Merian cruise " );
        }
        else
        {
            if ( s_Basis == "ps" )
               s_DatasetTitle = tr( "Links to master tracks in different resolutions of POLARSTERN cruise " );

           if ( s_Basis == "he" )
               s_DatasetTitle = tr( "Links to master tracks in different resolutions of HEINCKE cruise " );

           if ( s_Basis == "m" )
               s_DatasetTitle = tr( "Links to master tracks in different resolutions of METEOR cruise " );

           if ( s_Basis == "msm" )
               s_DatasetTitle = tr( "Links to master tracks in different resolutions of Maria S. Merian cruise " );
        }

        if ( sl_crInput.at( i_ExpeditionID ).section( "\t", 0, 0 ).startsWith( "PS" ) == true )
        {
            s_DatasetTitle.append( s_Expedition );
        }
        else
        {
            if ( s_DatasetTitle.isEmpty() == false )
            {
                if ( s_Expedition.isEmpty() == false )
                    s_DatasetTitle.append( s_EventLabel ).append( " (" ).append( s_Expedition ).append( ")" );
                else
                    s_DatasetTitle.append( s_EventLabel );
            }
            else
            {
                s_DatasetTitle = "@D@" + s_EventLabel + "@";
            }
        }

        if ( ( s_LocationStart.isEmpty() == false ) && ( s_LocationEnd.isEmpty() == false )  && ( s_DateBegin.isEmpty() == false )  && ( s_DateEnd.isEmpty() == false ) )
            s_DatasetTitle.append( ", " + s_LocationStart + " - " + s_LocationEnd + ", " + s_DateBegin + " - "  + s_DateEnd );

        if ( s_CruiseReport.isEmpty() == false )
            sl_Reference.append( Reference( s_CruiseReport, _RELATEDTO_ ) );                        // Cruise report (Related to)

        if ( s_StationList.isEmpty() == false )
            sl_Reference.append( Reference( s_StationList, _FURTHERDETAILS_ ) );                    // Link to Station list (Further details)

        if ( s_TracklineMap.isEmpty() == false )
            sl_Reference.append( Reference( s_TracklineMap, _FURTHERDETAILS_ ) );                   // Link to Trackline map (Further details)

        if ( s_Mastertrack_fullresolution.isEmpty() == false )
            sl_Reference.append( Reference( s_Mastertrack_fullresolution, _OTHERVERSION_ ) );       // Link to master track in full resolution (Other version)

        if ( s_Mastertrack_generalized.isEmpty() == false )
            sl_Reference.append( Reference( s_Mastertrack_generalized, _FURTHERDETAILS_ ) );        // Link to master track generalized (Further details)

        sl_Parameter.append( Parameter( num2str( 1599 ), num2str( 506 ), num2str( 43 ), s_TimeFormat ) );
        sl_Parameter.append( Parameter( num2str( 1600 ), num2str( 506 ), num2str( 43 ), tr( "###0.00000" ) ) );
        sl_Parameter.append( Parameter( num2str( 1601 ), num2str( 506 ), num2str( 43 ), tr( "###0.00000" ) ) );
        sl_Parameter.append( Parameter( num2str( 2960 ), num2str( 506 ), num2str( 50 ), tr( "###0.000" ), s_ParameterComment ) );
        sl_Parameter.append( Parameter( num2str( 2961 ), num2str( 506 ), num2str( 50 ), tr( "###0.0" ), s_ParameterComment ) );

        if ( sl_Parameter.count() > 1 )
        {
            timp << OpenDataDescriptionHeader();
            timp << DataSetID( s_DatasetID );
//          timp << DataSetID( "@I@" + s_EventLabel + "@" );
            timp << AuthorIDs( s_Author );

            if ( s_Basis == "ps" )
                timp << SourceID( num2str( 32 ) );

            if ( s_Basis == "he" )
                timp << SourceID( num2str( 32 ) );

            if ( s_Basis == "m" )
                timp << SourceID( num2str( 4 ) );

            if ( s_Basis == "msm" )
                timp << SourceID( num2str( 4 ) );

            timp << DatasetTitle( s_DatasetTitle );

            if ( sl_Reference.count() > 0 )
            {
                timp << qs << tr( "ReferenceIDs" ) << qe << "[ " << eol;

                for ( int i=0; i<sl_Reference.count()-1; i++ )
                    timp << sl_Reference.at( i ) << "," << eol;

                timp << sl_Reference.at( sl_Reference.count()-1 ) << " ]," << eol;
            }

            timp << ExportFilename( s_EventLabel, tr( "link-to-mastertrack" ) );

            if (s_Expedition.startsWith( "unknown_" ) == true )
                timp << EventLabel( s_EventLabel );
            else
                timp << EventLabel( s_EventLabel + tr( "-track" ) );

            timp << Parameter( sl_Parameter );
            timp << DatasetComment( s_DatasetComment );
            timp << ProjectIDs( num2str( 15 ) );
            timp << TopologicTypeID( num2str( 6 ) );
            timp << StatusID( num2str( 4 ) );
            timp << LoginID( num2str( 1 ) );
            timp << CloseDataDescriptionHeader();
        }

// **********************************************************************************************

        timp << "1599\t1600\t1601\t2960\t2961" << eol;

        i = 1;

        DateTime1.setDate( QDate::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy-MM-dd" ) );
        DateTime1.setTime( QTime::fromString( sl_Input.at( i ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ss" ) );

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

                d_Time = (double) DateTime1.secsTo( DateTime2 );

                if ( ( d_Time >= d_TimeResolution ) || ( i == n-1 ) ) // 10 min or last line
                {
                    DateTime1.setDate( QDate::fromString( sl_Input.at( i-1 ).section( "\t", 0, 0 ).section( "T", 0, 0 ), "yyyy-MM-dd" ) );
                    DateTime1.setTime( QTime::fromString( sl_Input.at( i-1 ).section( "\t", 0, 0 ).section( "T", 1, 1 ), "hh:mm:ss" ) );

                    d_Time = (double) DateTime1.secsTo( DateTime2 );

                    Pos1.setLatitude( sl_Input.at( i-1 ).section( "\t", 1, 1 ).toDouble() );
                    Pos1.setLongitude( sl_Input.at( i-1 ).section( "\t", 2, 2 ).toDouble() );
                    Pos2.setLatitude( sl_Input.at( i ).section( "\t", 1, 1 ).toDouble() );
                    Pos2.setLongitude( sl_Input.at( i ).section( "\t", 2, 2 ).toDouble() );

                    d_Distance = (double) Pos1.distanceTo( Pos2 ); // distance in meter
                    d_Bearing  = (double) Pos1.azimuthTo( Pos2 );  // course in deg

                    s_OutputStr.clear();
                    s_OutputStr.append( DateTime2.toString( "yyyy-MM-ddThh:mm:ss" ) + "\t" + sl_Input.at( i ).section( "\t", 1, 1 ) + "\t" + sl_Input.at( i ).section( "\t", 2, 2 ) + "\t" );
                    s_OutputStr.append( QString( "%1\t%2").arg( d_Distance/d_Time * 3.6 / 1.853 ).arg( d_Bearing ) );

                    timp << s_OutputStr << eol;

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

        resetProgress( i_NumOfFiles );
    }
    else
    {
        timp << "This routine can't create a master track from a generalized master track." << eol;
        timp << "Please choose " << s_Expedition << "_nav.txt and try again" << eol;
    }

    fimp.close();

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

    if ( fi.baseName().toLower().startsWith( "m" ) == true )
        return( "m" );

    return( "unknown" );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2014-02-23

QString MainWindow::findExpedition( const QString &s_Filename, const QStringList &sl_crInput )
{
    int     i_ExpeditionID = 0;

    QString s_Expedition   = "";

// **********************************************************************************************

    if ( s_Filename.toLower().startsWith( "he" ) == true )
    {
        s_Expedition = s_Filename.section( "_nav", 0, 0 ).replace( "_", "/" );
        return( s_Expedition );
    }

    if ( s_Filename.toLower().startsWith( "m" ) == true )
    {
        s_Expedition = s_Filename.section( "_nav", 0, 0 ).replace( "_", "/" );
        return( s_Expedition );
    }

    if ( s_Filename.toLower().startsWith( "msm" ) == true )
    {
        s_Expedition = s_Filename.section( "_nav", 0, 0 ).replace( "_", "/" );
        return( s_Expedition );
    }

    if ( s_Filename.toLower().startsWith( "so" ) == true )
    {
        s_Expedition = s_Filename.section( "_nav", 0, 0 ).replace( "_", "/" );
        return( s_Expedition );
    }

    if ( s_Filename.toLower().startsWith( "ps" ) == true ) // can't handle PS88.1
    {
        s_Expedition = s_Filename.section( "_nav", 0, 0 ).replace( "_", "/" );
        return( s_Expedition );
    }

    if ( ( s_Filename.toLower().startsWith( "ant-" ) == true ) || ( s_Filename.toLower().startsWith( "ark-" ) == true ) )
    {
        s_Expedition = s_Filename.section( "_nav", 0, 0 ).replace( "_", "/" );

        i_ExpeditionID = findExpeditionID( s_Expedition, sl_crInput );

        if ( i_ExpeditionID > 1000 )
            return ( sl_crInput.at( i_ExpeditionID-1000 ).section( "\t", 0, 0 ) + "/" + sl_crInput.at( i_ExpeditionID-1000 ).section( "\t", 1, 1 ).section( "/", 1, 1 ) );

        if ( i_ExpeditionID > 0 )
            return( sl_crInput.at( i_ExpeditionID ).section( "\t", 1, 1 ) );

        return( s_Expedition );
    }

    return( "unknown_" + s_Filename.section( "_nav", 0, 0 ) );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2014-02-23

int MainWindow::findExpeditionID( const QString &s_Expedition, const QStringList &sl_crInput )
{
    int     j             = 1;

    QString sd_Expedition = "";

// **********************************************************************************************

    sd_Expedition = s_Expedition.section( "_nav", 0, 0 ).replace( "_", "/" );

    j = 1;

    while ( j<sl_crInput.count() )
    {
        if ( sl_crInput.at( j ).section( "\t", 0, 0 ) == sd_Expedition )
            return( j );
        else if ( sl_crInput.at( j ).section( "\t", 1, 1 ) == sd_Expedition )
            return( 1000 + j );
        else
            j++;
    }

// **********************************************************************************************

    sd_Expedition = s_Expedition.section( "_", 0, 0 );

    j = 1;

    while ( j<sl_crInput.count() )
    {
        if ( sl_crInput.at( j ).section( "\t", 0, 0 ) == sd_Expedition )
            return( j );
        else if ( sl_crInput.at( j ).section( "\t", 1, 1 ) == sd_Expedition )
            return( 1000 + j );
        else
            j++;
    }

// **********************************************************************************************

    return( 0 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2014-02-22

void MainWindow::doConvertMastertrack()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_Curl          = findCurl();

    QString s_Basis         = "unknown";

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

    QStringList sl_crInput;

// **********************************************************************************************
// download CruiseReports_xxx.txt

    err = downloadFile( s_Curl, QLatin1String( "https://pangaea.de/PHP/cr/CruiseReports_Polarstern.txt" ), getDataLocation() + "/" + QLatin1String( "CruiseReports_Polarstern.txt" ) );
    err = downloadFile( s_Curl, QLatin1String( "https://pangaea.de/PHP/cr/CruiseReports_Heincke.txt" ), getDataLocation() + "/" + QLatin1String( "CruiseReports_Heincke.txt" ) );
    err = downloadFile( s_Curl, QLatin1String( "https://pangaea.de/PHP/cr/CruiseReports_Meteor_1986.txt" ), getDataLocation() + "/" + QLatin1String( "CruiseReports_Meteor_1986.txt" ) );
    err = downloadFile( s_Curl, QLatin1String( "https://pangaea.de/PHP/cr/CruiseReports_Merian.txt" ), getDataLocation() + "/" + QLatin1String( "CruiseReports_Merian.txt" ) );

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert Mastertrack data..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                s_Basis = findBasis( s_FilenameIn );

                if ( s_Basis == "ps" )
                    readFile( getDataLocation() + "/" + QLatin1String( "CruiseReports_Polarstern.txt" ), sl_crInput, _UTF8_ );

                if ( s_Basis == "he" )
                    readFile( getDataLocation() + "/" + QLatin1String( "CruiseReports_Heincke.txt" ), sl_crInput, _UTF8_ );

                if ( s_Basis == "m" )
                    readFile( getDataLocation() + "/" + QLatin1String( "CruiseReports_Meteor_1986.txt" ), sl_crInput, _UTF8_ );

                if ( s_Basis == "msm" )
                    readFile( getDataLocation() + "/" + QLatin1String( "CruiseReports_Merian.txt" ), sl_crInput, _UTF8_ );

                if ( sl_crInput.count() == 0 )
                    err = _FILENOTEXISTS_;

                if ( err == _NOERROR_ )
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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting Mastertrack data was canceled" ), true, false );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2015-01-10

int MainWindow::createMastertrackImportFile( const int i_Resolution )
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_Basis         = "unknown";

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";
    QString s_Curl          = findCurl();

    QStringList sl_Filenames;
    QStringList sl_crInput;

// **********************************************************************************************
// download CruiseReports_xxx.txt

    err = downloadFile( s_Curl, QLatin1String( "https://pangaea.de/PHP/cr/CruiseReports_Polarstern.txt" ), getDataLocation() + "/" + QLatin1String( "CruiseReports_Polarstern.txt" ) );
    err = downloadFile( s_Curl, QLatin1String( "https://pangaea.de/PHP/cr/CruiseReports_Heincke.txt" ), getDataLocation() + "/" + QLatin1String( "CruiseReports_Heincke.txt" ) );
    err = downloadFile( s_Curl, QLatin1String( "https://pangaea.de/PHP/cr/CruiseReports_Meteor_1986.txt" ), getDataLocation() + "/" + QLatin1String( "CruiseReports_Meteor_1986.txt" ) );
    err = downloadFile( s_Curl, QLatin1String( "https://pangaea.de/PHP/cr/CruiseReports_Merian.txt" ), getDataLocation() + "/" + QLatin1String( "CruiseReports_Merian.txt" ) );
    err = downloadFile( s_Curl, QLatin1String( "https://pangaea.de/PHP/cr/CruiseReports_Polar_5.txt" ), getDataLocation() + "/" + QLatin1String( "CruiseReports_Polar_5.txt" ) );
    err = downloadFile( s_Curl, QLatin1String( "https://pangaea.de/PHP/cr/CruiseReports_Polar_6.txt" ), getDataLocation() + "/" + QLatin1String( "CruiseReports_Polar_6.txt" ) );

    sl_Filenames.append( getDataLocation() + "/" + QLatin1String( "CruiseReports_Polar_5.txt" ) );
    sl_Filenames.append( getDataLocation() + "/" + QLatin1String( "CruiseReports_Polar_6.txt" ) );

    err = concatenateFilesByLines( -1, gs_FilenameFormat, gi_Extension, sl_Filenames, getDataLocation() + "/" + QLatin1String( "CruiseReports.txt" ), gi_CodecOutput, gi_CodecOutput, gi_EOL, 1, false, false, false, false );

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Create Mastertrack import file..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                sl_crInput.clear();

                s_Basis = findBasis( s_FilenameIn );

                if ( s_Basis == "ps" )
                    readFile( getDataLocation() + "/" + QLatin1String( "CruiseReports_Polarstern.txt" ), sl_crInput, _UTF8_ );

                if ( s_Basis == "he" )
                    readFile( getDataLocation() + "/" + QLatin1String( "CruiseReports_Heincke.txt" ), sl_crInput, _UTF8_ );

                if ( s_Basis == "m" )
                    readFile( getDataLocation() + "/" + QLatin1String( "CruiseReports_Meteor_1986.txt" ), sl_crInput, _UTF8_ );

                if ( s_Basis == "msm" )
                    readFile( getDataLocation() + "/" + QLatin1String( "CruiseReports_Merian.txt" ), sl_crInput, _UTF8_ );

                if ( s_Basis == "unknown" )
                    readFile( getDataLocation() + "/" + QLatin1String( "CruiseReports.txt" ), sl_crInput, _UTF8_ );

                if ( sl_crInput.count() == 0 )
                    err = _FILENOTEXISTS_;

                if ( err == _NOERROR_ )
                    err = createMastertrackImportFile( s_FilenameIn, gi_CodecInput, gi_CodecOutput, sl_crInput, i_Resolution, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Creating Mastertrack import file was canceled" ), true, false );

    return( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2015-12-22

void MainWindow::doCreateMastertrackImportFile_30sec()
{
    onError( createMastertrackImportFile( _R30SEC_ ) );
}

void MainWindow::doCreateMastertrackImportFile_1min()
{
    onError( createMastertrackImportFile( _R1MIN_ ) );
}

void MainWindow::doCreateMastertrackImportFile_10min()
{
    onError( createMastertrackImportFile( _R10MIN_ ) );
}
