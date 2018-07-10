#include "newdialog.h"
#include "ui_newdialog.h"

NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::NewDialog)
{
    ui->setupUi(this);
    ui->actionBtnsContainer->setAlignment(Qt::AlignTop);

    initPresetCombo();
    initDimensionCombos();
    initSignalSlots();
}

NewDialog::~NewDialog()
{
    delete ui;
}

void NewDialog::initSignalSlots()
{
    connect(ui->presetCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(switchPreset(int)));

    connect(ui->widthUnitCombo, SIGNAL(currentIndexChanged(int)),
            ui->heightUnitCombo, SLOT(setCurrentIndex(int)));

    connect(ui->heightUnitCombo, SIGNAL(currentIndexChanged(int)),
            ui->widthUnitCombo, SLOT(setCurrentIndex(int)));

    connect(ui->sizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(displaySize(QString)));

    connect(ui->widthTxt, SIGNAL(textEdited(const QString&)),
            this, SLOT(switchToCustomPreset(const QString&)));

    connect(ui->heightTxt, SIGNAL(textEdited(const QString&)),
            this, SLOT(switchToCustomPreset(const QString&)));
}

void NewDialog::switchToCustomPreset(const QString &string)
{
    ui->presetCombo->setCurrentIndex(Preset::CUSTOM);
}

void NewDialog::initPresetCombo()
{
    QMap<Preset, QString> presets = getPresetList();
    ui->presetCombo->addItems(QStringList(presets.values()));
    ui->presetCombo->setCurrentIndex(Preset::DEFAULT);
}

void NewDialog::initDimensionCombos()
{
    QMap<Canvas::DimensionUnit, QString> unitMap = getDimensionUnitList();

    ui->widthUnitCombo->addItems(QStringList(unitMap.values()));
    ui->widthUnitCombo->setCurrentIndex(Canvas::DimensionUnit::PIXELS);

    ui->heightUnitCombo->addItems(QStringList(unitMap.values()));
    ui->heightUnitCombo->setCurrentIndex(Canvas::DimensionUnit::PIXELS);
}

QMap<Canvas::DimensionUnit, QString> NewDialog::getDimensionUnitList()
{
    QMap<Canvas::DimensionUnit, QString> units;

    units.insert(Canvas::DimensionUnit::PIXELS, "Pixels");
    units.insert(Canvas::DimensionUnit::INCHES, "Inches");
    units.insert(Canvas::DimensionUnit::CENTIMETERS, "Centimeters");
    units.insert(Canvas::DimensionUnit::MILLIMETERS, "Millimeters");
    units.insert(Canvas::DimensionUnit::POINTS, "Points");
    units.insert(Canvas::DimensionUnit::PICAS, "Picas");

    return units;
}

QMap<NewDialog::Preset, QString> NewDialog::getPresetList()
{
    QMap<Preset, QString> presets;

    presets.insert(Preset::DEFAULT, "Fixture Default");
    presets.insert(Preset::INTERNATIONAL, "International");
    presets.insert(Preset::US_PAPER, "US Paper");
    presets.insert(Preset::CUSTOM, "Custom");

    return presets;
}

QMap<QString, NewDialog::PageSize> NewDialog::getInternationalList()
{
    QMap<QString, PageSize> stdSizes;

    stdSizes.insert("A1", {23.39, 33.11, Canvas::INCHES});
    stdSizes.insert("A2", {16.54, 23.39, Canvas::INCHES});
    stdSizes.insert("A3", {11.69, 16.54, Canvas::INCHES});
    stdSizes.insert("A4", {8.27, 11.69, Canvas::INCHES});
    stdSizes.insert("A5", {5.83, 8.27, Canvas::INCHES});

    return stdSizes;
}

QMap<QString, NewDialog::PageSize> NewDialog::getUSPaperList()
{
    QMap<QString, PageSize> stdSizes;

    stdSizes.insert("Letter", {8.5, 11, Canvas::INCHES});
    stdSizes.insert("Legal", {8.5, 14, Canvas::INCHES});
    stdSizes.insert("Ledger", {11, 17, Canvas::INCHES});
    stdSizes.insert("Tabloid", {17, 11, Canvas::INCHES});
    stdSizes.insert("Executive", {7.25, 10.55, Canvas::INCHES});

    return stdSizes;
}

void NewDialog::switchPreset(int index)
{
    ui->sizeCombo->setDisabled(false);

    switch (index) {

    case Preset::INTERNATIONAL:
        ui->sizeCombo->clear();
        _currSize = getInternationalList();
        ui->sizeCombo->addItems(QStringList(_currSize.keys()));
        break;

    case Preset::US_PAPER:
        ui->sizeCombo->clear();
        _currSize = getUSPaperList();
        ui->sizeCombo->addItems(QStringList(_currSize.keys()));
        break;

    case Preset::DEFAULT:
        ui->sizeCombo->clear();
        ui->sizeCombo->setDisabled(true);
        displaySizeContents(NewDialog::Default);
        break;

    default:
        ui->sizeCombo->setDisabled(true);

     }

}

void NewDialog::displaySize(QString presetKey)
{
    PageSize size = _currSize.value(presetKey);
    displaySizeContents(size);
}

void NewDialog::displaySizeContents(NewDialog::PageSize size)
{
    ui->widthTxt->setText(QString::number(size.width));
    ui->heightTxt->setText(QString::number(size.height));
    ui->widthUnitCombo->setCurrentIndex(size.unit);
    ui->heightUnitCombo->setCurrentIndex(size.unit);
}

void NewDialog::on_actionOk_clicked()
{
    // Needs a validation layer
    try {
        _dimensionUnit = (Canvas::DimensionUnit) ui->widthUnitCombo->currentIndex();
        _resolution = (int) getDoubleValue(ui->resTxt);

        ui->heightTxt->setFocus();
        int height = getPixelValue(ui->heightTxt);

        ui->widthTxt->setFocus();
        int width = getPixelValue(ui->widthTxt);

        QString docName = ui->docNameVal->text();

        Canvas *canvas = createCanvas(docName, width, height, _dimensionUnit, _resolution, Canvas::PPI);
        emit canvasAvailable(canvas);
        this->close();

    } catch (const QString msg) {
        showZeroErrorMessage(msg);
        return;
    }
}

/**
 * @brief NewDialog::createCanvas Creates a new canvas with the given params
 *
 * Processes the dimensions with specified units
 * @param docName
 * @param width
 * @param height
 * @param dimUnit
 * @param resolution
 * @param resUnit
 * @return
 */
Canvas* NewDialog::createCanvas(QString docName, double width, double height,
                                 Canvas::DimensionUnit dimUnit, double resolution,
                                 Canvas::ResolutionUnit resUnit) const
{

    return new Canvas(docName,width,height,
                        dimUnit,resolution,resUnit);
}

void NewDialog::on_actionCancel_clicked()
{
    this->close();
}

void NewDialog::on_widthTxt_editingFinished()
{
    try {
        checkDimensionValidity(getDoubleValue(ui->widthTxt));
    }
    catch (const QString msg) {
        ui->widthTxt->setFocus();
        showZeroErrorMessage(msg);
    }
}

void NewDialog::showZeroErrorMessage(QString errMsg)
{
    QMessageBox errDialogbox;
    errDialogbox.setText(errMsg);
    errDialogbox.exec();
}

void NewDialog::on_heightTxt_editingFinished()
{
    try {       
        checkDimensionValidity(getDoubleValue(ui->heightTxt));
    }
    catch (const QString msg) {        
        ui->heightTxt->setFocus();
        showZeroErrorMessage(msg);
    }
}

// This part needs to be heavily improved.
void NewDialog::checkDimensionValidity(double fieldVal)
{
    if (fieldVal < 1) {
        QString error = "Cannot set dimension value less than 1";
        throw error;
    }
}

double NewDialog::getDoubleValue(QLineEdit *fieldVal)
{
    QString fieldStr = fieldVal->text();
    return fieldStr.toDouble();
}

int NewDialog::getPixelValue(QLineEdit *field)
{
    double val = getDoubleValue(field);

    switch(_dimensionUnit) {

    case Canvas::DimensionUnit::INCHES:
        return val * _resolution;

    case Canvas::DimensionUnit::CENTIMETERS:
        return val * _resolution / 2.54;

    case Canvas::DimensionUnit::MILLIMETERS:
        return val * _resolution / 25.4;

    case Canvas::DimensionUnit::POINTS:
        return val * _resolution / 72;

    case Canvas::DimensionUnit::PICAS:
        return val * _resolution / 16;

    default:
        return val;

    }
}
