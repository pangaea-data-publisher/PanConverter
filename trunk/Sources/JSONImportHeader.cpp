/* JSONImportHeader.cpp       */
/* 2015-07-07                 */
/* Dr. Rainer Sieger          */

#include "Application.h"


// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::OpenDataDescriptionHeader()
{
    QString s_OutputStr = "";

    s_OutputStr.append( tr( "// METAHEADER - data import at " ) );
    s_OutputStr.append( QDateTime::currentDateTime().toString( "yyyy-MM-ddThh:mm" ) );
    s_OutputStr.append( eol );
    s_OutputStr.append( "{" );
    s_OutputStr.append( eol );

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::CloseDataDescriptionHeader()
{
    QString s_OutputStr = "";

    s_OutputStr.append( "}" );
    s_OutputStr.append( eol );
    s_OutputStr.append( "// METAHEADER END" );
    s_OutputStr.append( eol );
    s_OutputStr.append( eol );

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::ParentID( const QString& s_ParentID )
{
    QString s_OutputStr = "";

    if ( s_ParentID.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"ParentID\": " );
        s_OutputStr.append( s_ParentID );
        s_OutputStr.append( "," );
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::DataSetID( const QString& s_DatasetID )
{
    QString s_OutputStr = "";

    if ( s_DatasetID.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"DataSetID\": " );
        s_OutputStr.append( s_DatasetID );
        s_OutputStr.append( "," );
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::AuthorIDs( const QString& s_AuthorIDs )
{
    QString s_OutputStr = "";

    if ( s_AuthorIDs.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"AuthorIDs\": " );
        s_OutputStr.append( "[ "+ s_AuthorIDs + " ]" );
        s_OutputStr.append( "," );
        s_OutputStr.append( eol );

        s_OutputStr.replace( ";", "," );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::SourceID( const QString& s_SourceID )
{
    QString s_OutputStr = "";

    if ( s_SourceID.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"SourceID\": " );
        s_OutputStr.append( s_SourceID );
        s_OutputStr.append( "," );
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::DatasetTitle( const QString& s_Text  )
{
    QString s_OutputStr = "  ";
    QString sd_Text     = s_Text;

    if ( s_Text.isEmpty() == false )
    {
        sd_Text.replace( "\"", "\\\"" );
        sd_Text.replace( "\\\\", "\\" );

        s_OutputStr.append( "\"Title\": \"" );
        s_OutputStr.append( sd_Text + "\"," );
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::Reference( const QString& s_ReferenceID, const int i_RelationTypeID, const QString& s_ReferenceType, const QString& s_EventLabel )
{
    QString s_OutputStr = "";

    if ( s_ReferenceID.isEmpty() == false )
    {
        s_OutputStr = "    ";

        if ( s_ReferenceID == "999999" )
            s_OutputStr.append( "{ \"ID\": @" + s_ReferenceType + "@" + s_EventLabel + "@" + ", \"RelationTypeID\": " + num2str( i_RelationTypeID ) + " }" );
        else
            s_OutputStr.append( "{ \"ID\": " + s_ReferenceID + ", \"RelationTypeID\": " + num2str( i_RelationTypeID ) + " }" );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::ExportFilename( const QString& s_EventLabel, const QString& s_Text )
{
    QString s_OutputStr = "";

    if ( ( s_EventLabel.isEmpty() == false ) && ( s_Text.isEmpty() == false ) )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"ExportFilename\": \"" );
        s_OutputStr.append( s_EventLabel + "_" );
        s_OutputStr.append( s_Text + "\"," );
        s_OutputStr.replace( "/", "_" );
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::EventLabel( const QString& s_EventLabel )
{
    QString s_OutputStr = "";

    if ( s_EventLabel.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"EventLabel\": \"" );
        s_OutputStr.append( s_EventLabel );
        s_OutputStr.append( "\"," );
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::Parameter( const QString& s_ParameterID, const QString& s_PIID, const QString& s_MethodID, const QString& s_Format, const QString& s_Comment )
{
    QString s_OutputStr = "";

    if ( s_ParameterID.isEmpty() == false )
    {
        s_OutputStr = "    ";
        s_OutputStr.append( "{ \"ID\": " + s_ParameterID + ", " );
        s_OutputStr.append( "\"PI_ID\": "  + s_PIID + ", " );
        s_OutputStr.append( "\"MethodID\": "  + s_MethodID );

        if ( s_Format.isEmpty() == false )
            s_OutputStr.append( ", \"Format\": \""  + s_Format + "\"" );

        if ( s_Comment.isEmpty() == false )
            s_OutputStr.append( ", \"Comment\": \""  + s_Comment + "\"" );

        s_OutputStr.append( " },");
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::Parameter( const QStringList sl_Parameter )
{
    int     NumOfParameters = sl_Parameter.count() - 1;

    QString s_OutputStr     = "";

// **********************************************************************************************

    if ( NumOfParameters + 1 > 1 )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"ParameterIDs\": [ " );
        s_OutputStr.append( eol );
        s_OutputStr.append( sl_Parameter.first() );

        for ( int i=1; i<NumOfParameters; i++ )
            s_OutputStr.append( sl_Parameter.at( i ) );

        s_OutputStr.append( sl_Parameter.last().section( "},", 0, 0 ) );
        s_OutputStr.append( "} ],");
        s_OutputStr.append( eol );
    }
    else
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"ParameterIDs\": [ " );
        s_OutputStr.append( sl_Parameter.first().section( "    ", 1, 1 ).section( "},", 0, 0 ) );
        s_OutputStr.append( "} ],");
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::DatasetComment( const QString& s_DatasetComment )
{
    QString s_OutputStr = "";
    QString sd_DatasetComment = s_DatasetComment;

    if ( s_DatasetComment.isEmpty() == false )
    {
        sd_DatasetComment.replace( "\"", "\\\"" );
        sd_DatasetComment.replace( "\\\\", "\\" );

        s_OutputStr = "  ";
        s_OutputStr.append( "\"DataSetComment\": \"" );
        s_OutputStr.append( sd_DatasetComment );
        s_OutputStr.append( "\"," );
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::ProjectIDs( const QString& s_ProjectIDs )
{
    QString s_OutputStr = "";

    if ( s_ProjectIDs.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"ProjectIDs\": " );
        s_OutputStr.append( "[ "+ s_ProjectIDs + " ]" );
        s_OutputStr.append( "," );
        s_OutputStr.append( eol );

        s_OutputStr.replace( ";", "," );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::TopologicTypeID( const QString& s_TopologicTypeID )
{
    QString s_OutputStr = "";

    if ( s_TopologicTypeID.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"TopologicTypeID\": " );
        s_OutputStr.append( s_TopologicTypeID );
        s_OutputStr.append( "," );
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::StatusID( const QString& s_StatusID )
{
    QString s_OutputStr = "";

    if ( s_StatusID.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"StatusID\": " );
        s_OutputStr.append( s_StatusID );
        s_OutputStr.append( "," );
        s_OutputStr.append( eol );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::UserIDs( const QString& s_UserIDs )
{
    QString s_OutputStr = "";

    if ( s_UserIDs.isEmpty() == false )
    {
        s_OutputStr = "  ";
        s_OutputStr.append( "\"UserIDs\": " );
        s_OutputStr.append( "[ "+ s_UserIDs + " ]" );
        s_OutputStr.append( "," );
        s_OutputStr.append( eol );

        s_OutputStr.replace( ";", "," );
    }

    return( s_OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::LoginID( const QString& s_LoginID )
{
    QString s_OutputStr = "  ";

    s_OutputStr.append( "\"LoginID\": " );
    s_OutputStr.append( s_LoginID ); // no comma at the end!
    s_OutputStr.append( eol );

    return( s_OutputStr );
}
