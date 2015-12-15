#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QStack>
#include <fstream>
#include <math.h>
#include <QMessageBox>

using std::ifstream;
using std::ofstream;

class Widget : public QWidget
{
    Q_OBJECT

private:
    QGridLayout* layout;

    QPushButton* accept;
    QPushButton* load;
    QPushButton* save;
    QPushButton* solve;

    QTableWidget* param_table;
    QTableWidget* table;
    QTableWidget* answer_table;

public:
    void createAnswerArea();
    void createParamsArea();
    void createTableArea();

    double* solveMatrix(int row, double** matrix);

    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void changeTable();
    void saveMatrix();
    void loadMatrix();
    void solveSystem();
};

#endif // WIDGET_H
