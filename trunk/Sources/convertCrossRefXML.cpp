/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-06

int MainWindow::parseCrossRefXML( const QString &s_FilenameIn, const int i_CodecOutput, const int i_EOL, const int i_Extension, const int i_NumOfFiles )
{
    QString s_error					= "";
    QString s_content				= "";
    QString s_attributeContent      = "";
    QString s_EOL                   = setEOLChar( i_EOL );

    int		i_error					= 0;
    int		i_errorLine				= 0;
    int		i_errorColumn			= 0;

    QFile           fin( s_FilenameIn );
    QFileInfo       fi( s_FilenameIn );
    QDomDocument    doc;

// **********************************************************************************************
// open output file

    QFile fout( fi.absolutePath() + "/CrossRef" + setExtension( i_Extension ) );

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

    tout << "Author(s)\tYear\tTitel\tJournal\tVolume\tPages\tDOI\tISSN\tType" << s_EOL;

// **********************************************************************************************
// read file

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Parsing CrossRef XML files..." ), 2 );

    if ( doc.setContent( &fin, true, &s_error, &i_errorLine, &i_errorColumn ) == false )
        i_error = -1;

    incProgress( i_NumOfFiles, 1 );

    if ( i_error == 0 )
    {
        QDomElement root = doc.documentElement();

        if ( root.tagName() != "ERGEBNISSE" )
            i_error = -2;

        if ( i_error == 0 )
        {
            QDomNode Body = root.firstChild();

            while ( Body.isNull() == false )
            {
                int         i            = 0;

                QString     s_Result     = "";

                QDomNode    Query        = Body.firstChild();
                QDomElement Contributors = Query.firstChildElement( "contributors" );

                while ( Contributors.isNull() == false )
                {
                    QDomNode Contributor = Contributors.firstChildElement( "contributor" );

                    while ( Contributor.isNull() == false )
                    {
                        i++;

                        i_error = parseTextEntry( Contributor.firstChildElement( "surname" ), s_content );

                        if ( i == 1 )
                            s_Result.append( s_content );
                        else
                            s_Result.append( "//" + s_content );

                        i_error = parseTextEntry( Contributor.firstChildElement( "given_name" ), s_content ); s_Result.append( ", " + s_content );

                        Contributor = Contributor.nextSibling();
                    }

                    Contributors = Contributors.nextSiblingElement( "contributor" );
                }

                i_error = parseTextEntry( Query.firstChildElement( "year" ), "media_type", s_attributeContent, s_content ); s_Result.append( "\t" + s_content );
                i_error = parseTextEntry( Query.firstChildElement( "article_title" ), s_content ); s_Result.append( "\t" + s_content );
                i_error = parseTextEntry( Query.firstChildElement( "journal_title" ), s_content ); s_Result.append( "\t" + s_content );
                i_error = parseTextEntry( Query.firstChildElement( "volume" ), s_content ); s_Result.append( "\t" + s_content );
                i_error = parseTextEntry( Query.firstChildElement( "issue" ), s_content ); s_Result.append( "(" + s_content + ")" );
                i_error = parseTextEntry( Query.firstChildElement( "first_page" ), s_content ); s_Result.append( "\t" + s_content + "-" );
                i_error = parseTextEntry( Query.firstChildElement( "last_page" ), s_content ); s_Result.append( s_content );

                i_error = parseTextEntry( Query.firstChildElement( "doi" ), "type", s_attributeContent, s_content ); s_Result.append( "\tdoi:" + s_content );
                i_error = parseTextEntry( Query.firstChildElement( "issn" ), "type", s_attributeContent, s_content ); s_Result.append( "\tISSN " + s_content );
                i_error = parseTextEntry( Query.firstChildElement( "publication_type" ), s_content ); s_Result.append( "\t" + s_content );

                tout << s_Result << s_EOL;

                Body = Body.nextSibling();
            }
        }
    }

    incProgress( i_NumOfFiles, 2 );

    resetProgress( i_NumOfFiles );

    fout.close();

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doParseCrossRefXML()
{
    int         i				= 0;
    int         err				= 0;
    int         stopProgress	= 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        setWaitCursor();

        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Parsing CrossRef XML files..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = parseCrossRefXML( s_FilenameIn, gi_CodecOutput, gi_EOL, gi_Extension, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Parsing CrossRef XML files was canceled" ), false, false );

    onError( err );
}

