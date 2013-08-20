#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
private slots:
  void axisLabelDoubleClick(QCPAxis * axis, QCPAxis::SelectablePart part);
  void titleDoubleClick(QMouseEvent *, QCPPlotTitle * title);
  void graphClicked(QCPAbstractPlottable * plottable);
  void legendDoubleClick(QCPLegend * legend, QCPAbstractLegendItem * item);
  void mousePress();
  void mouseWheel();
  void selectionChanged();
  
private:
  QCustomPlot * plot_;
};

#endif // MAINWINDOW_H

