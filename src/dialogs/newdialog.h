#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

#include "../model/canvas.h"

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDialog(QWidget *parent = 0);
    ~NewDialog();

signals:
    void canvasAvailable(const Canvas *canvas);

private slots:
    void on_actionOk_clicked();
    void on_actionCancel_clicked();
    void on_widthTxt_editingFinished();
    void on_heightTxt_editingFinished();

    void switchToCustomPreset(const QString &string);
    void switchPreset(int index);
    void displaySize(QString presetKey);
private:
    struct PageSize
    {
        double width;
        double height;
        Canvas::DimensionUnit unit;
    }
    Default { 600, 600, Canvas::DimensionUnit::PIXELS};

    enum Preset
    {
        DEFAULT, INTERNATIONAL, US_PAPER, CUSTOM
    };
    Canvas *createCanvas(QString docName, double width, double height,
                          Canvas::DimensionUnit dimUnit, double resolution,
                          Canvas::ResolutionUnit resUnit) const;
    Ui::NewDialog *ui;
    int _resolution;
    Canvas::DimensionUnit _dimensionUnit;
    double getDoubleValue(QLineEdit *fieldVal);
    int getPixelValue(QLineEdit *field);
    void showZeroErrorMessage(QString fieldName);
    void checkDimensionValidity(double fieldVal);
    void displaySizeContents(NewDialog::PageSize size);
    QMap<QString, PageSize> _currSize;

    void initPresetCombo();
    void initDimensionCombos();
    void initSignalSlots();


    QMap<QString, PageSize> getInternationalList();
    QMap<QString, PageSize> getUSPaperList();
    QMap<Canvas::DimensionUnit, QString> getDimensionUnitList();
    QMap<Preset, QString> getPresetList();

};

#endif // NEWDIALOG_H
