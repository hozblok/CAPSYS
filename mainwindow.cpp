#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    projs_dialog = new ProjectsDialog(this);
    ui->setupUi(this);

    ui->capsListWidget->addItem(QString::number(3));
    ui->sourcesListWidget->addItem(QString::number(2));

    connect(ui->pushButton_delCap, SIGNAL(clicked()), this, SLOT(deleteCapId_clicked()));
    connect(ui->pushButton_delSource, SIGNAL(clicked()), this, SLOT(deleteSourceId_clicked()));
    connect(ui->pushButton_addCap, SIGNAL(clicked()), this, SLOT(addCapId_clicked()));
    connect(ui->pushButton_addSource, SIGNAL(clicked()), this, SLOT(addSourceId_clicked()));

    connect(ui->comboBox_dbtables, SIGNAL(activated(QString)), projs_dialog, SLOT(slotSetTable(QString)));
    connect(ui->pushButton_TestTableBD, SIGNAL(clicked()), projs_dialog, SLOT(exec()));

    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(startSpread()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    try
    {
        REAL temp(arg1.toLocal8Bit().constData(), NUMBITS);
        temp = temp / PlanckConstant;
        ui->lineEdit_2->setText(QString::fromStdString(getString(temp, true)));
        REAL temp2(arg1.toLocal8Bit().constData(), NUMBITS);
        temp2 = SpeedOfLight * PlanckConstant / temp2;
        ui->lineEdit_3->setText(QString::fromStdString(getString(temp2, true)));
    }
    catch(...)
    {
        ui->lineEdit_2->setText("NaN");
        ui->lineEdit_3->setText("NaN");
    }
}

void MainWindow::on_lineEdit_2_textEdited(const QString &arg1)
{
    try
    {
        REAL temp(arg1.toLocal8Bit().constData(), NUMBITS);
        temp = temp * PlanckConstant;
        ui->lineEdit->setText(QString::fromStdString(getString(temp, true)));
        REAL temp2(arg1.toLocal8Bit().constData(), NUMBITS);
        temp2 = SpeedOfLight / temp2;
        ui->lineEdit_3->setText(QString::fromStdString(getString(temp2, true)));
    }
    catch(...)
    {
        ui->lineEdit->setText("NaN");
        ui->lineEdit_3->setText("NaN");
    }
}

void MainWindow::on_lineEdit_3_textEdited(const QString &arg1)
{
    try
    {
        REAL temp(arg1.toLocal8Bit().constData(), NUMBITS);
        temp = SpeedOfLight * PlanckConstant / temp;
        ui->lineEdit->setText(getString(temp, true).data());
        REAL temp2(arg1.toLocal8Bit().constData(), NUMBITS);
        temp2 = SpeedOfLight / temp2;
        ui->lineEdit_2->setText(getString(temp2, true).data());
    }
    catch(...)
    {
        ui->lineEdit->setText("NaN");
        ui->lineEdit_2->setText("NaN");
    }
}

// 1 0 0 1
// 0 1 0 -2
// 0 0 1 3
//
// 1 0 1 4
// 1 1 0 -1
// 0 0 1 3
//
// 1 0 1 4
// 1 1 0 -1
// 1 0 2 7
//
// 1 0 1 0.4
// 1 1 0 -0.1
// 1 0 2 0.7

int cout(VEC_VEC_R & matrix, int sizeRows = 3, int size_col = 3)
{
#if USE_MPF_CLASS
    for(int i = 0; i < sizeRows; ++i)
    {
        QString temp;
        mp_exp_t exp;
        for(int j = 0; j < size_col; ++j)
        {
            QString significand = QString::fromStdString(matrix[i][j].get_str(exp));
            temp = temp + " " + significand + " (" + QString::number(exp) + ")";
        }
        qDebug() << temp;
    }
#else
    if (size_col == 3)
    {
        for(int i = 0; i < sizeRows; ++i)
        {
            qDebug() << matrix[i][0] << " " << matrix[i][1] << " " << matrix[i][2];
        }
    }
#endif //USE_MPF_CLASS

    return 0;
}

int cout(VEC_R & vec, int size = 3)
{
#if USE_MPF_CLASS
    QString temp;
    mp_exp_t exp;
    for(int j = 0; j < size; ++j)
    {
        QString significand = QString::fromStdString(vec[j].get_str(exp));
        temp = temp + " " + significand + " (" + QString::number(exp) + ")";
    }
    qDebug() << temp;
#else
    if (size == 3)
    {
        qDebug() << vec[0] << " " << vec[1] << " " << vec[2];
    }
#endif //USE_MPF_CLASS

    return 0;
}

int cout(VEC_I & vec, int size = 3)
{
    if (size == 3)
    {
        qDebug() << vec[0] << " " << vec[1] << " " << vec[2];
    }
    return 0;
}

#if USE_MPF_CLASS
int test_linal_gmp()
{
    LinAlg ll;
//    const int size_of_double_bit = sizeof(double) * 8; //64 bit
//    mpf_set_default_prec(size_of_double_bit * 8); //512 bit
//    qDebug() << "mpf_get_default_prec()" << mpf_get_default_prec();

    const QString test0[][sizeColumns] = {
        { "1000000000000000000000000.1", "0", "1000000000000000000000000.1" },
        { "1000000000000000000000000.1", "1000000000000000000000000.1", "0" },
        { "1000000000000000000000000.1", "0", "2000000000000000000000000.2" }
    };

    VEC_VEC_R mass(ll.get_size_str());
    for (int i = 0; i < ll.get_size_str(); ++i) {
        mass[i].resize(ll.get_size_col());
        for (int j = 0; j < ll.get_size_col(); ++j) {
            mass[i][j].set_prec(512);
            mass[i][j] = test0[i][j].toUtf8().data();
        }
    }

    qDebug() << "### mass:";
    cout(mass);
    int signum = 1;

    VEC_I p(ll.get_size_str());
    ll.LU_decomposition(mass, p, &signum);
    qDebug() << "### LU - mass:";
    cout(mass);
    VEC_R x(ll.get_size_col());
    VEC_R b(ll.get_size_col());
    x[0].set_prec(512); x[1].set_prec(512); x[2].set_prec(512);
    b[0].set_prec(512); b[1].set_prec(512); b[2].set_prec(512);
    b[0] = "4000000000000000000000000.8";
    b[1] = "-1000000000000000000000000.2";
    b[2] = "7000000000000000000000001.4";
    ll.LU_solve(mass, p, b, x);
    qDebug() << "### LU solve - mass:";
    cout(mass);
    qDebug() << "### otvet p:";
    cout(p);
    qDebug() << "### otvet b:";
    cout(b);
    qDebug() << "### otvet x:";
    cout(x);
//    double a = 100000000000000000000011.125; // 1.234 cannot be stored in a double exactly, try "1.234"
//    mpf_class fff;
//    fff.get_d()
//    fff = "-1.5";
//    mpf_class f(1.5, size_of_double_bit * 8);
//    f.set_str("-111111100000000000000011.125", 10);
//    mp_exp_t exp;
//    qDebug() << size_of_double_bit;
//    qDebug() << f.get_prec() << fff.get_prec();
//    std::string significand = f.get_str(exp);
//    qDebug() << significand.data();
//    qDebug() << exp;
//    std::string significand_ = fff.get_str(exp);
//    qDebug() << significand_.data();
//    qDebug() << exp;
    return 0;
}
#endif

void MainWindow::on_pushButton_5_clicked()
{
//    int bit_of_double = sizeof(double) * 8; //64 bit
#if USE_MPF_CLASS
//    mpf_set_default_prec(size_of_double_bit * 8); //512 bit
//    mpfr_set_default_prec(bit_of_double * 8); //512 bit
    test_linal_gmp();
#else
    test_linal_double();
#endif //USE_MPF_CLASS

    double _a = 0.0;
    if (_a) {
        qDebug() << "0.0true";
    }
    else
    {
        qDebug() << "0.0false";
    }
    _a = -0.0;
    if (_a) {
        qDebug() << "-0.0true";
    }
    else
    {
        qDebug() << "-0.0false";
    }
    _a = -0.0000065;
    if (_a) {
        qDebug() << "-0.0000065true";
    }
    else
    {
        qDebug() << "-0.0000065false";
    }
    _a = -13;
    if (_a) {
        qDebug() << "-13true";
    }
    else
    {
        qDebug() << "-13false";
    }
    _a = 111;
    if (_a) {
        qDebug() << "111true";
    }
    else
    {
        qDebug() << "111false";
    }
    _a = 1.000000000000000000000000000000000000000005 - 1;
    if (_a) {
        qDebug() << "0.00000000000000005;true";
    }
    else
    {
        qDebug() << "0.00000000000000005;false";
    }

    {
    REAL _a;
    _a = "0.0";
    REAL _aa;
    _aa = "0.0";
    if((_a)-(_a)==_aa) qDebug() << "ok";
    if (_a.get_d()) {
        qDebug() << "0.0true";
    }
    else
    {
        qDebug() << "0.0false";
    }
    _a = -0.0;
    if((_a)-(_a)==_aa) qDebug() << "ok";
    if (_a.get_d()) {
        qDebug() << "-0.0true";
    }
    else
    {
        qDebug() << "-0.0false";
    }
    _a = -0.0000065;
    if((_a)-(_a)==_aa) qDebug() << "ok";
    if (_a.get_d()) {
        qDebug() << "-0.0000065true";
    }
    else
    {
        qDebug() << "-0.0000065false";
    }
    _a = -13;
    if((_a)-(_a)==_aa) qDebug() << "ok";
    if (_a.get_d()) {
        qDebug() << "-13true";
    }
    else
    {
        qDebug() << "-13false";
    }
    _a = 111;
    if((_a)-(_a)==_aa) qDebug() << "ok";
    if (_a.get_d()) {
        qDebug() << "111true";
    }
    else
    {
        qDebug() << "111false";
    }
    mpf_class b = 1.000000000000000000000000000000000000000005;
    mpf_class c = 1;
    _a = b - c;
    if((_a)-(_a)==_aa) qDebug() << "ok";
//    if((_a)&&(_a)) qDebug() << "@@ok";
    if (_a.get_d()) {
        qDebug() << "0.00000000000000005;true";
    }
    else
    {
        qDebug() << "0.00000000000000005;false";
    }

    }
    mpf_class e, ZERO;
    ZERO = "0.0";
    e = "3.14";
//    e = sin(e);
    qDebug() << "BUY";




    mpf_class xxx(2.25), test;
    mpf_class yyy(1.5);
    qDebug() << "BUY2";
    Eval asdfasdf("3*x*x+y*y");
    qDebug() << "BUY3";
    test = asdfasdf.getValue(xxx, yyy);

    qDebug() << "BUY4";
    qDebug() << test.get_d();
    test = asdfasdf.getValueDerivative('x',xxx);
    qDebug() << test.get_d();
    qDebug() << "BUY5";

    qDebug() << "mpf_get_default_prec()" << mpf_get_default_prec();
//    qDebug() << "mpfr_get_default_prec()" << mpfr_get_default_prec();

//    mpf_class x;
//    x = "0.5";
//    mpf_class y;
//    qDebug()  << x.get_prec() << y.get_prec();
////    x /= 2;
//    mpfr_t xx;
//    mpfr_init_set_f(xx, x.get_mpf_t(), MPFR_RNDN);

//    mpfr_t yy;
//    mpfr_init2(yy, 512);
//    mpfr_asin(yy, xx, MPFR_RNDN);
//    qDebug()  << y.get_prec();
//    mpfr_get_f(y.get_mpf_t(), yy, MPFR_RNDN);

//    mp_exp_t expex;
//    std::string aaaaaaaa = y.get_str(expex);
//    qDebug() << aaaaaaaa.data() << expex << y.get_prec();


}

void MainWindow::on_pushButton_7_clicked()
{
    qDebug() << "### случайное число:";
    REAL ran("0.0", NUMBITS), ONE("1.0", NUMBITS), ZERO("0.0", NUMBITS);
//    gmp_randclass rr(gmp_randinit_mt);

    ran = utils.getRandom(ZERO, ONE);
    long int random=ran.get_ui();

    mp_exp_t exp;
    std::string str = ran.get_str(exp);
    qDebug() << str.data() << exp << "ran.get_ui():" << random;
    qDebug() << getString(ran, true).data();
    qDebug() << getString(ran).data();
    qDebug() << "###";
}

void MainWindow::on_pushButton_8_clicked()
{
    XRays x(this);
    VEC_I capId;
    VEC_I sourcId;
    if(ui->capsListWidget->count() < 1 || ui->sourcesListWidget->count() < 1)
    {
        return;
    }
    //??
    capId.push_back(ui->capsListWidget->item(0)->text().toInt());
    //??
    sourcId.push_back(ui->sourcesListWidget->item(0)->text().toInt());
    qDebug() << "DEBUG: countRays: " << ui->spinBox_countRays->value();
    for(int i = 0; i < ui->spinBox_countRays->value(); ++i)
    {
        x.send_ray(capId, sourcId, ui->checkBox_useNotCaptured->isChecked());
        qDebug() << i;
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    qDebug() << "# тест класса eval:";
    REAL x__("0.0", NUMBITS), y__("0.0", NUMBITS), z__("0.0", NUMBITS);
    REAL x2__("0.0", NUMBITS), y2__("0.0", NUMBITS), z2__("0.0", NUMBITS);
    qDebug() << "accuracy: 1e-40";
    REAL acc__("1e-40", NUMBITS);
    qDebug() << "x*x + y*y - 1";
    Eval eval("x*x + y*y - 1");

    qDebug() << "берём очень близкие две точки:";
    x__ = "0.964367251971326110233821374689097736910415068718459853808852145918551902769426009554425151260210135458925188462366986082194303245712819671263722070631165609";
    y__ = "0.264567200017826882082853084557167348338202330802380576391265070383561244044468260334165579374961611339519015770215498372184709208058946615580610007478927605";
    z__ = "2123.27046053112661935856519223762197859422259482060988449219629583097909446212488203201765335045537071370260910633566863566636088013292277012911814450724548";

    x2__ = "0.964367251971326110233821374689097736910434856462063201505785572387915962672068664914220093792229732723354455560702249385789846946656150953671193682497866458";
    y2__ = "0.264567200017826882082853084557167348338207660367486137102687152701716218187097382432135909653526586370872909924638967624032593042647356783492681666800579975";
    z2__ = "2123.27046053112661935856519223762197859401332251115990494482413200040000817275545595224787134687987568663626223094315434388173745239204791504028590045822281";

    qDebug() << "f(x1,y1,z1)" <<getString(eval.getValue(x__,y__,z__)).data()
             << "f(x2,y2,z2)" << getString(eval.getValue(x2__,y2__,z2__)).data()
             << "f(x1,y1,z1) < acc__" << (eval.getValue(x__,y__,z__) < acc__)
             << "f(x2,y2,z2) < acc__" << (eval.getValue(x2__,y2__,z2__) < acc__);
    acc__ = z2__ - z__;
    qDebug() << "z2 - z1" << getString(acc__).data();
    qDebug() << "#";
}

void MainWindow::on_pushButton_10_clicked()
{
    qDebug() << "###";
    REAL xxx(2.25, NUMBITS), test("0.0", NUMBITS);
    mpf_class yyy(1.5, NUMBITS); // mpf_class === REAL
    qDebug() << "Eval: f(x,y)='3*x*x + y*y'";
    Eval eval_1("3*x*x + y*y");
    test = eval_1.getValue(xxx, yyy);

    qDebug() << "f(2.25, 1.5):" << test.get_d();
    test = eval_1.getValueDerivative('x', xxx);
    qDebug() << "f'x(2.25):" << test.get_d();
    qDebug() << "Eval f(x)='sin(x)'";
    Eval eval_2("sin(x)");
    qDebug() << "f(2.25):";
    test = eval_2.getValue(xxx);
    qDebug() << getString(test).data();
    qDebug() << "теперь получим 2 * asin(1) без использования eval:";
    QByteArray aa;
    test = 2 * _asin(ONE, ZERO, aa);
    qDebug() << getString(test).data();

    qDebug() << "mpf_get_default_prec()" << mpf_get_default_prec();
    qDebug() << "###";
}

void MainWindow::addCapId_clicked()
{
    QList<QListWidgetItem *> itemsFound = ui->capsListWidget->findItems(QString::number(ui->spinBox_cap->value()), Qt::MatchExactly);
    if(itemsFound.isEmpty())
    {
        ui->capsListWidget->addItem(QString::number(ui->spinBox_cap->value()));
    }
}

void MainWindow::addSourceId_clicked()
{
    QList<QListWidgetItem *> itemsFound = ui->sourcesListWidget->findItems(QString::number(ui->spinBox_source->value()), Qt::MatchExactly);
    if(itemsFound.isEmpty())
    {
        ui->sourcesListWidget->addItem(QString::number(ui->spinBox_source->value()));
    }
}

void MainWindow::deleteCapId_clicked()
{
    QListWidgetItem * item = ui->capsListWidget->currentItem();
    int row = ui->capsListWidget->row(item);
    ui->capsListWidget->takeItem(row);
    delete item;
}

void MainWindow::deleteSourceId_clicked()
{
    QListWidgetItem * item = ui->sourcesListWidget->currentItem();
    int row = ui->sourcesListWidget->row(item);
    ui->sourcesListWidget->takeItem(row);
    delete item;
}

void MainWindow::on_pushButton_clicked()
{
    REAL x(ui->lineEdit_x->text().toLocal8Bit().constData(), NUMBITS), y(ui->lineEdit_y->text().toLocal8Bit().constData(), NUMBITS), z(ui->lineEdit_z->text().toLocal8Bit().constData(), NUMBITS);
    Eval privet(ui->lineEdit_eval->text().toLocal8Bit());
    qDebug() << getString(privet.getValue(x,y,z)).data();
}

void MainWindow::on_pushButton_2_clicked()
{
    REAL x(ui->lineEdit_x->text().toLocal8Bit().constData(), NUMBITS),
         y(ui->lineEdit_y->text().toLocal8Bit().constData(), NUMBITS),
         z(ui->lineEdit_z->text().toLocal8Bit().constData(), NUMBITS);
//    Eval privet(ui->lineEdit_eval->text().toLocal8Bit());
//    QByteArray error;
//    qDebug() << getQString(_sqrt(x, ZERO, error)*_sqrt(x, ZERO, error));

//    complex_mpf aaa(x, y);
//    z = _sin(MPF_PI/(ONE+ONE+ONE+ONE), ZERO, error) * (ONE+ONE);
//    qDebug() << "##" << getQString(z);
//    aaa.show();
//    aaa = aaa.sqrt_compl();
//    aaa.show();
//    aaa = aaa.pow2();
//    aaa.show();

    XRays xrays(this);
    qDebug() << "xrays getCoefOfRefl:";
    z = xrays.getCoefOfRefl(x,y);

    qDebug() << "result:" << getString(z).data() << xrays.getCoefOfRefl(ui->lineEdit_x->text().toLocal8Bit().toDouble(), ui->lineEdit_y->text().toLocal8Bit().toDouble());

//    qDebug() << getQString(_pow(x, y, error));
    qDebug() << "###";
}

void MainWindow::startSpread()
{
    Project proj(ui->spinBox_idProjs->value(), this);
    proj.startModelling();
}

void MainWindow::on_pushButton_3_clicked()
{
    REAL a("1.1", NUMBITS);
    REAL b("2.3", NUMBITS);
    REAL c("3.5", NUMBITS);

    REAL * array[] = {&a, &b, &c};

    qDebug() << QString::fromStdString(getString(*array[1])) << QString::fromStdString(getString(*array[2]));
}
