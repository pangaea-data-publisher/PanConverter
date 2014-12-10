/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-12

int MainWindow::parseTreeRingXML( const QString &s_FilenameIn, const int i_CodecOutput, const int i_EOL, const int i_Extension, QStringList &sl_Filenames_events, QStringList &sl_Filenames_metadata, QStringList &sl_Filenames_data, const int i_NumOfFiles )
{
    QString s_error					= "";
    QString s_EOL                   = setEOLChar( i_EOL );

    int		i_error					= 0;
    int		i_errorLine				= 0;
    int		i_errorColumn			= 0;

    QFile           fin( s_FilenameIn );
    QFileInfo       fi( s_FilenameIn );

    QDomDocument    doc;

// **********************************************************************************************
// read file

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Parsing Tree Ring XML files..." ), 2 );

    if ( doc.setContent( &fin, true, &s_error, &i_errorLine, &i_errorColumn ) == false )
        i_error = -1;

    incProgress( i_NumOfFiles, 1 );

    if ( i_error == 0 )
    {
        QDomElement root = doc.documentElement();

        if ( root.tagName() != "tridas" )
            i_error = -2;

        if ( i_error == 0 )
        {
            QString     s_Content           = "";
            QString     s_AttributeContent  = "";

            QString     s_Category          = "";
            QString     s_Event             = "";
            QString     s_Date              = "";
            QString     s_DateEvent         = "";
            QString     s_Position          = "";
            QString     s_PI                = "";
            QString     s_ProjectTitle      = "";
            QString     s_ObjectTitle       = "";
            QString     s_ObjectType        = "";
            QString     s_ObjectComment     = "";
            QString     s_Town              = "";
            QString     s_Country           = "";

            QDomNode    Project  = root.firstChild();

            i_error = parseTextEntry( Project.firstChildElement( "title" ), s_ProjectTitle );
            i_error = parseTextEntry( Project.firstChildElement( "identifier" ), s_Event );
            i_error = parseTextEntry( Project.firstChildElement( "category" ), s_Category );
            i_error = parseTextEntry( Project.firstChildElement( "investigator" ), s_PI );
            i_error = parseTextEntry( Project.firstChildElement( "period" ), s_Date );


            QDomElement Object   = Project.firstChildElement( "object" );
            QDomElement Location = Object.firstChildElement( "location" );

            i_error = parseTextEntry( Object.firstChildElement( "title" ), s_ObjectTitle );
            i_error = parseTextEntry( Object.firstChildElement( "type" ), s_ObjectType );


            QDomElement Position = Location.firstChildElement( "locationGeometry" ).firstChildElement( "Point" );

            i_error = parseTextEntry( Position.firstChildElement( "pos" ), s_Position );


            QDomElement Address  = Location.firstChildElement( "address" );

            i_error = parseTextEntry( Address.firstChildElement( "cityOrTown" ), s_Town );
            i_error = parseTextEntry( Address.firstChildElement( "country" ), s_Country );

// **********************************************************************************************
// write event import file

            QFile fevent( fi.absolutePath() + "/event_" + s_Event + setExtension( i_Extension ) );

            sl_Filenames_events.append( fevent.fileName() );

            if ( fevent.open( QIODevice::WriteOnly | QIODevice::Text) == false )
                return( -20 );

            QTextStream tevent( &fevent );

            switch ( i_CodecOutput )
            {
            case _SYSTEM_:
                break;
            case _LATIN1_:
                tevent.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
                break;
            case _APPLEROMAN_:
                tevent.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
                break;
            default:
                tevent.setCodec( QTextCodec::codecForName( "UTF-8" ) );
                break;
            }

            s_Country.replace( "NL", "Netherlands" );
            s_Country.replace( "BE", "Belgium" );
            s_Country.replace( "GE", "Germany" );
            s_Country.replace( "FR", "France" );

            if ( s_Date == "- n/a" )
                s_DateEvent = "";
            else
                s_DateEvent = s_Date.left( 4 ) + "-" + s_Date.mid( 4,2 ) + "-" + s_Date.mid( 6,2 );

            s_ObjectComment = s_ProjectTitle.section( ", ", 0, 0 ) + "\t" + s_ProjectTitle.section( ", ", 1, 1 ) + "\t" + s_Category + "\t" + s_ObjectTitle;

            if ( s_ObjectTitle != s_ObjectType )
                s_ObjectComment.append( "\t" + s_ObjectType );

            s_ObjectComment.replace( "\tachterhuis", "\tAchterhuis" );
            s_ObjectComment.replace( "\tbegane grond", "\tBegane Grond" );
            s_ObjectComment.replace( "\thoofdhuis", "\tHoofdhuis" );
            s_ObjectComment.replace( "\tvoorhuis", "\tVoorhuis" );

            s_ObjectComment.replace( "Bauwerke", "Building" );
            s_ObjectComment.replace( "Archologie", "Archaeology" );
            s_ObjectComment.replace( "Einrichtungen", "Interior equipment" );

            s_ObjectComment.replace( "unbekannt", "" );
            s_ObjectComment.replace( "\t-", "" );

            tevent << "Campaign\tArea\tLabelEvent\tGear\tDateEvent\tLatitudeEvent\tLongitudeEvent\tTown\tProject title\tCategory\tObject title\tObject type" << s_EOL;

            tevent << "not_given" << "\t" << s_Town << ", " << s_Country << "\t" << s_Event << "\t" << "DDRILL" << "\t";
            tevent << s_DateEvent << "\t";
            tevent << s_Position.section( " ", 0, 0 ) << "\t" << s_Position.section( " ", 1, 1 ) << "\t";
            tevent << s_ObjectComment << s_EOL;

            fevent.close();

            incProgress( i_NumOfFiles, 1 );

// **********************************************************************************************
// open metadata file

            QFile fmetadata( fi.absolutePath() + "/metadata_" + s_Event + ".rdb" );

            sl_Filenames_metadata.append( fmetadata.fileName() );

            if ( fmetadata.open( QIODevice::WriteOnly | QIODevice::Text) == false )
                return( -20 );

            QTextStream tmetadata( &fmetadata );

            switch ( i_CodecOutput )
            {
            case _SYSTEM_:
                break;
            case _LATIN1_:
                tmetadata.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
                break;
            case _APPLEROMAN_:
                tmetadata.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
                break;
            default:
                tmetadata.setCodec( QTextCodec::codecForName( "UTF-8" ) );
                break;
            }

// **********************************************************************************************
// open data import file

            QFile fdata( fi.absolutePath() + "/imp_" + s_Event + setExtension( i_Extension ) );

            sl_Filenames_data.append( fdata.fileName() );

            if ( fdata.open( QIODevice::WriteOnly | QIODevice::Text) == false )
                return( -20 );

            QTextStream tdata( &fdata );

            switch ( i_CodecOutput )
            {
            case _SYSTEM_:
                break;
            case _LATIN1_:
                tdata.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
                break;
            case _APPLEROMAN_:
                tdata.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
                break;
            default:
                tdata.setCodec( QTextCodec::codecForName( "UTF-8" ) );
                break;
            }

            tdata << "Event label\tORDINAL NUMBER []\tAGE [ka BP]\tAge [a AD]\tRing width [1/100 mm]" << s_EOL;

// **********************************************************************************************

            QDomElement Element  = Object.firstChildElement( "element" );

            while ( Element.isNull() == false )
            {
                int     i_nrOfSapwoodRings  = 0;

                QString s_Taxon             = "";
                QString s_Pith              = "";
                QString s_Heartwood         = "";
                QString s_Sapwood           = "";
                QString s_lastRingUnderBark = "";

                i_error = parseTextEntry( Element.firstChildElement( "title" ), s_Content );
                i_error = parseTextEntry( Element.firstChildElement( "taxon" ), s_Taxon );

                QDomElement WoodCompleteness = Element.firstChildElement( "sample" ).firstChildElement( "radius" ).firstChildElement( "woodCompleteness" );

                if ( WoodCompleteness.isNull() == false )
                {
                    i_error = parseTextEntry( WoodCompleteness.firstChildElement( "pith" ), "presence", s_Pith, s_Content );
                    i_error = parseTextEntry( WoodCompleteness.firstChildElement( "heartwood" ), "presence", s_Heartwood, s_Content );
                    i_error = parseTextEntry( WoodCompleteness.firstChildElement( "sapwood" ), "presence", s_Sapwood, s_Content );

                    QDomElement Sapwood = Element.firstChildElement( "sample" ).firstChildElement( "radius" ).firstChildElement( "woodCompleteness" ).firstChildElement( "sapwood" );

                    if ( Sapwood.isNull() == false )
                    {
                        i_error = parseNumericEntry( Sapwood.firstChildElement( "nrOfSapwoodRings" ), i_nrOfSapwoodRings );
                        i_error = parseTextEntry( Sapwood.firstChildElement( "lastRingUnderBark" ), "presence", s_lastRingUnderBark, s_Content );
                    }
                }

                QDomElement Data = Element.firstChildElement( "sample" ).firstChildElement( "radius" ).firstChildElement( "measurementSeries" );

                if ( Data.isNull() == false )
                {
                    QStringList sl_Data;

                    QString     s_Sample        = "";
                    QString     s_SampleLabel   = "";
                    QString     s_EventLabel    = "";

                    int         i_Value         = -1;
                    int         i_firstYear     = -1;
                    int         i_deathYear     = -1;
                    int         i_YearAD        = -1;
                    int         i               = 1;

                    i_error = parseTextEntry( Data.firstChildElement( "title" ), s_Sample );
                    i_error = parseTextEntry( Data.firstChildElement( "measuringMethod" ), s_Content );

                    s_EventLabel = s_Event + "@" + s_Sample.section( " ", 1, 1 );

                    QDomElement Age = Data.firstChildElement( "interpretation" );

                    if ( Age.isNull() == false )
                    {
                        i_error = parseNumericEntry( Age.firstChildElement( "firstYear" ), "suffix", s_AttributeContent, i_firstYear );
                        i_error = parseNumericEntry( Age.firstChildElement( "deathYear" ), "suffix", s_AttributeContent, i_deathYear );

                        i_YearAD = i_firstYear + 1; // + 1: first year is birth year!
                    }

                    QDomNode Values = Data.firstChildElement( "values" ).firstChild();

                    while ( Values.isNull() == false )
                    {
                        if ( Values.toElement().tagName() == "variable" )
                            i_error = parseTextEntry( Values.toElement(), s_Content );
                        else if ( Values.toElement().tagName() == "unit" )
                            i_error = parseTextEntry( Values.toElement(), s_Content );
                        else if ( Values.toElement().tagName() == "value" )
                            i_Value = parseValueEntry( Values.toElement() );

                        if ( i_Value > 0 )
                        {
                            if ( i_YearAD > 0 )
                            {
                                sl_Data.append( QString( "%1\t\t%2\t%3\t%4" ).arg( s_EventLabel ).arg( (1950-(float) i_YearAD)/1000. ).arg( i_YearAD ).arg( i_Value ) );
                                i_YearAD++;
                            }
                            else
                            {
                                sl_Data.append( QString( "%1\t%2\t\t\t%3" ).arg( s_EventLabel ).arg( i ).arg( i_Value ) );
                                i++;
                            }
                        }

                        Values = Values.nextSibling();
                    }

// **********************************************************************************************
// write metadata

                    s_EventLabel = s_Event + "-" + s_Sample.section( " ", 1, 1 );

                    if ( s_Taxon.contains( " " ) == false )
                        s_Taxon.append( " species" );

                    s_Taxon.replace( "Fagus syl.", "Fagus sylvatica" );
                    s_Taxon.replace( "n/a species", "not given" );

                    s_SampleLabel = s_Sample.section( " ", 1, 1 ).left( 5 ) + "/" + s_Sample.section( " ", 1, 1 ).mid( 5 );

                    if ( s_Taxon == "not given" )
                        tmetadata << "@D@" << s_EventLabel << "@" << "\t" << "Tree-ring width from historical object sample " << s_SampleLabel << ", " << s_Country << ", " << s_Town << s_EOL;
                    else
                        tmetadata << "@D@" << s_EventLabel << "@" << "\t" << "Tree-ring width of " << s_Taxon << " from historical object sample " << s_SampleLabel << ", " << s_Country << ", " << s_Town << s_EOL;

                    if ( s_PI.startsWith( "erhard pre", Qt::CaseInsensitive ) == true )
                        s_PI = "21462";

                    tmetadata << "@A@" << s_EventLabel << "@" << "\t" << s_PI << s_EOL;
                    tmetadata << "@GP@" << s_EventLabel << "@" << "\t" << s_PI << s_EOL;
                    tmetadata << "@PP@" << s_EventLabel << "@" << "\t" << s_PI << s_EOL;

                    tmetadata << "@I@" << s_EventLabel << "@" << "\t" << "DataSet ID" << s_EOL;

                    tmetadata << "@O@" << s_EventLabel << "@" << "\t" << "http://store.pangaea.de/documentation/TreeRing/data/" << s_Event << ".xml" << s_EOL;
                    tmetadata << "@C@" << s_EventLabel << "@" << "\t" << "Species: " << s_Taxon << "; No of rings: " << sl_Data.count();

                    if ( i_YearAD > 0 )
                        tmetadata << "; first year: " << i_firstYear << "; tree-death year: " << i_deathYear;
                    else
                        tmetadata << "; Age not determined";

                    tmetadata << "; Pith: " << s_Pith;
                    tmetadata << "; Heartwood: " << s_Heartwood;
                    tmetadata << "; Sapwood: " << s_Sapwood;
                    tmetadata << "; Number of sapwood rings: " << i_nrOfSapwoodRings;
                    tmetadata << "; Last ring under bark: " << s_lastRingUnderBark << s_EOL;

// **********************************************************************************************
// write data

                    for ( int i=0; i<sl_Data.count(); i++ )
                        tdata << sl_Data.at( i ) << s_EOL;
                }

                Element = Element.nextSiblingElement();
            }

            fmetadata.close();
            fdata.close();
        }
    }

    incProgress( i_NumOfFiles, 2 );

    resetProgress( i_NumOfFiles );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-05

int MainWindow::parseValueEntry( const QDomElement &element )
{
    int i_Value = -1;

    if ( element.attribute( "value" ).isEmpty() == false )
        i_Value = element.attribute( "value" ).toInt();

    return ( i_Value );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-04

int MainWindow::parseNumericEntry( const QDomElement &element, int &i_Content )
{
    if ( element.text().isEmpty() == true )
        return( _ERROR_ );

    i_Content = element.text().toInt();

    return ( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-04

int MainWindow::parseNumericEntry( const QDomElement &element, const QString &s_Attribute, QString &s_AttributeContent, int &i_Content )
{
    if ( ( element.attribute( s_Attribute ).isEmpty() == true ) && ( element.text().isEmpty() == true ) )
        return( _ERROR_ );

    s_AttributeContent = element.attribute( s_Attribute );
    i_Content          = element.text().toInt();

    return ( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-04

int MainWindow::parseTextEntry( const QDomElement &element, QString &s_Content )
{
    if ( element.text().isEmpty() == true )
        return( _ERROR_ );

    s_Content = element.text();

    return ( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-04

int MainWindow::parseTextEntry( const QDomElement &element, const QString &s_Attribute, QString &s_AttributeContent, QString &s_Content )
{
    int err = _NOERROR_;

    if ( ( element.attribute( s_Attribute ).isEmpty() == true ) && ( element.text().isEmpty() == true ) )
        return( _ERROR_ );

    s_AttributeContent = element.attribute( s_Attribute );
    s_Content          = element.text();

    return ( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doParseTreeRingXML()
{
    int         i				= 0;
    int         err				= 0;
    int         stopProgress	= 0;

    QString     s_FilenameIn    = "";
    QString     s_FilenameOut   = "";

    QStringList sl_Filenames_data;
    QStringList sl_Filenames_events;
    QStringList sl_Filenames_metadata;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        setWaitCursor();

        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Parsing Tree Ring XML files..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = parseTreeRingXML( s_FilenameIn, gi_CodecOutput, gi_EOL,  gi_Extension, sl_Filenames_events, sl_Filenames_metadata, sl_Filenames_data, gsl_FilenameList.count() );

                stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
            }
            else
            {
                err = _FILENOTEXISTS_;
            }
        }

        resetFileProgress( gsl_FilenameList.count() );

        setNormalCursor();
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    if ( ( err == _NOERROR_ ) && ( gsl_FilenameList.count() > 1 ) )
    {
        QFileInfo fi( gsl_FilenameList.at( 0 ) );

        err = concatenateFilesByLines( -1, gs_FilenameFormat, gi_Extension, sl_Filenames_events, fi.absolutePath() + "/Events" + setExtension( gi_Extension ), gi_CodecOutput, gi_CodecOutput, gi_EOL, 1, false, false, false, true );
        err = concatenateFilesByLines( -1, gs_FilenameFormat, gi_Extension, sl_Filenames_data, fi.absolutePath() + "/Data" + setExtension( gi_Extension ), gi_CodecOutput, gi_CodecOutput, gi_EOL, 1, false, false, false, true );
        err = concatenateFilesByLines( -1, gs_FilenameFormat, gi_Extension, sl_Filenames_metadata, fi.absolutePath() + "/Metadata.rdb", gi_CodecOutput, gi_CodecOutput, gi_EOL, 0, false, false, false, true );
    }

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Parsing Tree Ring XML was canceled" ), false, false );

    onError( err );
}

