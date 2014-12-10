#ifndef GPSTOGPXOPTIONSDIALOG_H
#define GPSTOGPXOPTIONSDIALOG_H

#include <QDialog>

#include "ui_gpstogpxoptionsdialog.h"

class GPStoGpxOptionsDialog : public QDialog, public Ui::GPStoGpxOptionsDialog
{
Q_OBJECT

public:
    GPStoGpxOptionsDialog( QWidget *parent = 0 );
};

#endif
