#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(465, 385);

    layout = new QGridLayout;

    createParamsArea();
    createTableArea();
    createAnswerArea();

    setLayout(layout);
}

void Widget::createParamsArea()
{
    QStringList lst;
    lst<<"Строки"<<"Столбцы";

    param_table = new QTableWidget(1,2);
    param_table->setFixedSize(217, 54);
    param_table->setHorizontalHeaderLabels(lst);
    QTableWidgetItem *item1 = new QTableWidgetItem;
    QTableWidgetItem *item2 = new QTableWidgetItem;
    item1->setText("4");
    param_table->setItem(0, 0, item1);
    item2->setText("5");
    param_table->setItem(0, 1, item2);

    accept = new QPushButton("Применить");

    layout->addWidget(param_table, 0, 0);
    layout->addWidget(accept, 1, 0);
    layout->setAlignment(Qt::AlignTop);

    connect(accept, SIGNAL(clicked()), this, SLOT(changeTable()));
}

void Widget::createTableArea()
{
    table = new QTableWidget(4, 5);
    table->setFixedSize(217,217);
    for (int i = 0; i < table->columnCount(); i++)
        table->setColumnWidth(i, 30);

    save = new QPushButton("Сохранить");
    load = new QPushButton("Загрузить");

    layout->addWidget(table, 2, 0);
    layout->addWidget(save, 3, 0);
    layout->addWidget(load, 4, 0);

    connect(save, SIGNAL(clicked()), this, SLOT(saveMatrix()));
    connect(load, SIGNAL(clicked()), this, SLOT(loadMatrix()));
}

void Widget::createAnswerArea()
{
    answer_table = new QTableWidget(table->columnCount()-1, 1);
    answer_table->setFixedSize(217,217);

    solve = new QPushButton("Решить");

    layout->addWidget(answer_table, 2, 1);
    layout->addWidget(solve, 3, 1);

    connect(solve, SIGNAL(clicked()), this, SLOT(solveSystem()));
}

void Widget::changeTable()
{
    int row = param_table->item(0,0)->text().toInt();
    int col = param_table->item(0,1)->text().toInt();
    table->setRowCount(row);
    table->setColumnCount(col);
    answer_table->setRowCount(row);

    for (int i = 0; i < table->columnCount(); i++)
        table->setColumnWidth(i, 30);
}

void Widget::saveMatrix()
{
    ofstream out;
    out.open("C:\Users\P\Dropbox\GIT\SAS");

    out<<table->rowCount()<<" "<<table->columnCount()<<std::endl;
    for (int i = 0; i < table->rowCount(); i++)
    {
        for (int j = 0; j < table->columnCount(); j++)
        {
            out<<table->item(i,j)->text().toDouble()<<" ";
        }
        out<<std::endl;
    }
    out.close();
}

void Widget::loadMatrix()
{
    ifstream in;
    in.open("C:\Users\P\Dropbox\GIT\SAS");

    int row, col;
    in>>row>>col;

    double **matrix = new double*[row];
    QString** matrix_str = new QString*[row];

    for (int i = 0; i < row; i++)
    {
        matrix[i] = new double[col];
        matrix_str[i] = new QString[col];
    }

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
        {
            in>>matrix[i][j];
            matrix_str[i][j] = QString("%1").arg(matrix[i][j]);
        }
    in.close();

    QTableWidgetItem*** item;
    item = new QTableWidgetItem**[row];
    for (int i = 0; i < row; i++)
    {
        item[i] = new QTableWidgetItem*[col];
        for (int j = 0; j < col; j++)
            item[i][j] = new QTableWidgetItem;
    }

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
        {
            item[i][j]->setText(matrix_str[i][j]);
            table->setItem(i, j, item[i][j]);
        }

}
void Widget::solveSystem()
{
    int row = table->rowCount();
    int col = table->columnCount();
    double** matrix = new double*[row];
    for (int i = 0; i < row; i++)
    {
        matrix[i] = new double[col];
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            matrix[i][j] = table->item(i,j)->text().toDouble();
        }
    }

    double *x = solveMatrix(row, matrix);

    QString* x_str;
    x_str = new QString[row];

    for (int i = 0; i < row; i++)
        x_str[i] = QString("%1").arg(x[i]);


    QTableWidgetItem** item = new QTableWidgetItem*[row];
    for (int i = 0; i < row; i++)
    {
        item[i] = new QTableWidgetItem;
        item[i]->setText(x_str[i]);
        answer_table->setItem(i, 0, item[i]);
    }
}

double* Widget::solveMatrix(int row, double **matrix)
{
    int i, j, k;

    double* x;
    x=new double [row];

    //прямой ход методом вращений
    double a,b,c,s,t;
    for(i=0;i<row;i++)
    {
        for(j=i+1;j<row;j++)
        {
            b=matrix[j][i];
            a=matrix[i][i];
            c=a/sqrt(a*a+b*b);
            s=b/sqrt(a*a+b*b);
            for(k=i;k<row+1;k++)
            {
                t=matrix[i][k];
                matrix[i][k]=c*matrix[i][k]+s*matrix[j][k];
                matrix[j][k]=-s*t+c*matrix[j][k];
            }
        }
    }

    //обратный ход
    for(i=row-1;i>=0;i--)
    {
        double s=0.0;
        for(j=i+1;j<row;j++)
            s+=matrix[i][j]*x[j];
        s=matrix[i][row]-s;
        if(matrix[i][i]==0)
        {
            QMessageBox::information(0, "Ошибка", "Матрица несовместна");
            return 0;
        }
        x[i]=s/matrix[i][i];
    }
    return x;
}

Widget::~Widget()
{

}
