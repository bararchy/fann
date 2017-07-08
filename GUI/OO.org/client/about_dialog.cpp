#include <QRect>
#include <QPixmap>
#include <QString>

#include <cstdlib>

#include "about_dialog.h"

const char* const fannImage = "fann/fann_logo.png";

AboutDialog::AboutDialog(QWidget *parent, Qt::WindowFlags flags) :
  QDialog(parent, flags), Ui::AboutDialog()
{
    setupUi(this);
    fannLogo->setGeometry(QRect(240, 10, 145, 49));
    fannLogo->setPixmap(QPixmap(QString(std::getenv("OFFICE_HOME")) + fannImage));
}
