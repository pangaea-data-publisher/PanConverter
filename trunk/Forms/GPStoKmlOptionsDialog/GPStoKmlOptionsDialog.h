#ifndef GPSTOKMLOPTIONSDIALOG_H
#define GPSTOKMLOPTIONSDIALOG_H

#include <QDialog>

#include "ui_gpstokmloptionsdialog.h"

class GPStoKmlOptionsDialog : public QDialog, public Ui::GPStoKmlOptionsDialog
{
Q_OBJECT

public:
    GPStoKmlOptionsDialog( QWidget *parent = 0 );
    QString getProgramDir();

private slots:
    void browseFilenameDialog();
};

#endif
