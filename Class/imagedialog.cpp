#include "imagedialog.h"
#include "ui_imagedialog.h"
#include "scalablegraphicsview.h"
ImageDialog::ImageDialog(const QPixmap &pixmap, QWidget *parent)
    : QDialog(parent), scaleFactor(1.0)
    , ui(new Ui::ImageDialog)
{
    // Set up the dialog
    setWindowTitle("Image Viewer");
    // Adjust the size to accommodate the QLabel as well
    setFixedSize(1500 + 180, 700); // Increased width by 180 pixels for the QLabel

    // Create the horizontal layout for scroll area and label
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Create the scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setGeometry(0, 0, 1500, height()); // Set the width to 1500 pixels

    // Create the graphics view and scene
    ScalableGraphicsView *view = new ScalableGraphicsView(scrollArea);
    QGraphicsScene *scene = new QGraphicsScene(view);
    view->setScene(scene);
    view->setAlignment(Qt::AlignCenter);

    // Add the pixmap to the scene
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    scene->addItem(pixmapItem);

    // Center the pixmap
    view->centerOn(pixmapItem);

    // Set the view as the widget for the scroll area
    scrollArea->setWidget(view);

    // Add the scroll area to the main layout
    mainLayout->addWidget(scrollArea);

    // if(straa!=""){
    // // Create the QLabel
    // QLabel *label = new QLabel(this);
    // label->setFixedWidth(180); // Set the width of the label to 180 pixels
    // label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding); // Fixed width, expanding height
    // label->setWordWrap(true);
    // label->setText(straa);
    // // Add the QLabel to the main layout
    // mainLayout->addWidget(label);
    // }
    // Set the main layout as the dialog's layout
    setLayout(mainLayout);
}

ImageDialog::~ImageDialog()
{
    delete ui;
}
