// ============================================================================
// 
//         Author:  Dale Lukas Peterson (dlp), hazelnusse@gmail.com
// 
//    Description:  Implementation of QMainWindow subclass
// 
// ============================================================================

#include <QStatusBar>
#include <QInputDialog>
//#incl
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    plot_{new QCustomPlot(this)}
{
    setCentralWidget(plot_);
    setGeometry(400, 250, 542, 390);

    plot_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                           QCP::iSelectLegend | QCP::iSelectPlottables);
    plot_->axisRect()->setupFullAxesBox();
    plot_->plotLayout()->insertRow(0);
    plot_->plotLayout()->addElement(0, 0, new QCPPlotTitle(plot_, "Interaction Example"));
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i = 0; i < 101; ++i) {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i];  // let's plot a quadratic function
    }
    plot_->addGraph();
    plot_->graph(0)->setData(x, y);
    plot_->graph(0)->setName("y = x^2");
    plot_->xAxis->setLabel("x Axis");
    plot_->yAxis->setLabel("y Axis");
    plot_->xAxis->setRange(-1, 1);
    plot_->yAxis->setRange(0, 1);
    plot_->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    plot_->legend->setFont(legendFont);
    plot_->legend->setSelectedFont(legendFont);
    // box shall not be selectable, only legend items
    plot_->legend->setSelectableParts(QCPLegend::spItems);

    statusBar()->showMessage(tr("Ready"));

    // Connect SIGNALS and SLOTS
    connect(plot_,
            SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),
            this,
            SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(plot_, SIGNAL(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)),
            this, SLOT(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)));
    connect(plot_, SIGNAL(selectionChangedByUser()),
            this, SLOT(selectionChanged()));
    connect(plot_, SIGNAL(mousePress(QMouseEvent*)),
            this, SLOT(mousePress()));
    connect(plot_, SIGNAL(mouseWheel(QWheelEvent*)),
            this, SLOT(mouseWheel()));
    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(plot_, SIGNAL(plottableClick(QCPAbstractPlottable *, QMouseEvent *)),
            this, SLOT(graphClicked(QCPAbstractPlottable *)));
    connect(plot_,
            SIGNAL(legendDoubleClick(QCPLegend *, QCPAbstractLegendItem *, QMouseEvent *)),
            this,
            SLOT(legendDoubleClick(QCPLegend *, QCPAbstractLegendItem *)));

}

MainWindow::~MainWindow()
{
//    plot_->savePdf("test.pdf");
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable)
{
    statusBar()->showMessage(QString("Graph '%1' selected.").arg(plottable->name()));
}

void MainWindow::legendDoubleClick(QCPLegend *, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item

  // only react if item was clicked (user could have clicked on border padding
  // of legend where there is no item, then item is 0)
  if (item) {
      QCPPlottableLegendItem * plItem = qobject_cast<QCPPlottableLegendItem *>(item);
      bool ok;
      QString newName = QInputDialog::getText(this, "QCustomPlot example",
              "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
      if (ok) {
        plItem->plottable()->setName(newName);
        plot_->replot();
      }
  }
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  
  if (plot_->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot_->axisRect()->setRangeDrag(plot_->xAxis->orientation());
  else if (plot_->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot_->axisRect()->setRangeDrag(plot_->yAxis->orientation());
  else
    plot_->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  
  if (plot_->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot_->axisRect()->setRangeZoom(plot_->xAxis->orientation());
  else if (plot_->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot_->axisRect()->setRangeZoom(plot_->yAxis->orientation());
  else
    plot_->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable
   separately, but we want the user only to be able to select the axis as a
   whole, so we tie the selected states of the tick labels and the axis base
   line together. However, the axis label shall be selectable individually.
   
   The selection state of the left and right axes shall be synchronized as well
   as the state of the bottom and top axes.
   
   Further, we want to synchronize the selection of the graphs with the
   selection state of the respective legend item belonging to that graph. So
   the user can select a graph by either clicking on the graph itself or on its
   legend item.
  */
  
  // make top and bottom axes be selected synchronously, and handle axis and
  // tick labels as one selectable object:
  if (plot_->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || plot_->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      plot_->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || plot_->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    plot_->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    plot_->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (plot_->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || plot_->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      plot_->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || plot_->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    plot_->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    plot_->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  
  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i = 0; i < plot_->graphCount(); ++i)
  {
    QCPGraph *graph = plot_->graph(i);
    QCPPlottableLegendItem *item = plot_->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelected(true);
    }
  }
}

void MainWindow::titleDoubleClick(QMouseEvent *, QCPPlotTitle * title)
{
  // Set the plot title by double clicking on it
  bool ok;
  QString newTitle = QInputDialog::getText(this, "QCustomPlot example",
          "New plot title:", QLineEdit::Normal, title->text(), &ok);
  if (ok) {
    title->setText(newTitle);
    plot_->replot();
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  // only react when the actual axis label is clicked, not tick label or axis backbone
  if (part == QCPAxis::spAxisLabel) {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example",
            "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok) {
      axis->setLabel(newLabel);
      plot_->replot();
    }
  }
}
