#include "controller.h"

Controller::Controller(MainWindow *w)
{
    this->w = w;
    canvas = w->ui->canvas;
    drawer = new Drawer(canvas);
    updater = new Updater(canvas);
    remover = new Remover(canvas);
    filer = new Filer(canvas);
    redoUndoer = new RedoUndoer(canvas);
    initConnections();
    initCashe();
}

void Controller::initConnections()
{
    // 绑定图源选项
    connect(w->ui->processElementBtn, &QPushButton::clicked, [=](){
        canvas->hideMagSizeAll();
        drawer->setPaintProcessElement();
        if(drawer->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->decisionElementBtn, &QPushButton::clicked, [=](){
        canvas->hideMagSizeAll();
        drawer->setPaintDecisionElement();
        if(drawer->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->startEndElementBtn, &QPushButton::clicked, [=](){
        canvas->hideMagSizeAll();
        drawer->setPaintStartEndElement();
        if(drawer->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->connectorElementBtn, &QPushButton::clicked, [=](){
        canvas->hideMagSizeAll();
        drawer->setPaintConnectorElement();
        if(drawer->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->dataElementBtn, &QPushButton::clicked, [=](){
        canvas->hideMagSizeAll();
        drawer->setPaintDataElement();
        if(drawer->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->subprocessElementBtn, &QPushButton::clicked, [=](){
        canvas->hideMagSizeAll();
        drawer->setPaintSubprocessElement();
        if(drawer->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });
    connect(w->ui->documentElementBtn, &QPushButton::clicked, [=](){
        canvas->hideMagSizeAll();
        drawer->setPaintDocumentElement();
        if(drawer->curPaintChart)
            mouseEventType = MOUSE_EVENT_TYPE::CREATING;
    });

    //鼠标事件
    connect(canvas, &Canvas::leftPressed, this, &Controller::on_leftPressed);
    connect(canvas, &QWidget::customContextMenuRequested, this, &Controller::showRrightClickMenu);
    connect(canvas, &Canvas::mouseMoved, this, &Controller::on_mouseMoved);
    connect(canvas, &Canvas::mouseReleased, this, &Controller::on_mouseReleased);

    //键盘按键
    connect(canvas, &Canvas::escPressed, this, &Controller::on_escPressed);
    connect(canvas, &Canvas::delPressed, this, &Controller::on_delPressed);

    //文件操作
    connect(w->ui->actionSaveFile, &QAction::triggered, this, &Controller::on_saveFile);
    connect(w->ui->actionOpenFile, &QAction::triggered, this, &Controller::on_openFile);
    connect(w->ui->actionNewFile, &QAction::triggered, this, &Controller::on_newFile);
    connect(w->ui->actionSaveAsSVG, &QAction::triggered, this, &Controller::on_saveAsSVG);

    //编辑操作
    connect(w->ui->actionRedo, &QAction::triggered, this, &Controller::on_redo);
    connect(w->ui->actionUndo, &QAction::triggered, this, &Controller::on_undo);
    connect(w->ui->actionCopy, &QAction::triggered, this, &Controller::on_copy);
    connect(w->ui->actionCut, &QAction::triggered, this, &Controller::on_cut);
    connect(w->ui->actionPaste, &QAction::triggered, this, &Controller::on_paste);
    connect(w->ui->actionSearch, &QAction::triggered, this, &Controller::on_search);
    connect(w->ui->actionReplace, &QAction::triggered, this, &Controller::on_replace);

    //帮助操作

}

void Controller::showRrightClickMenu(const QPoint &pos)
{
    if(canvas->curSelecChart == nullptr)
        return;
    QMenu menu;
    if(dynamic_cast<Line*>(canvas->curSelecChart) == nullptr)
    {
        QAction *changeFillColor = menu.addAction("更换填充颜色");
        connect(changeFillColor, &QAction::triggered, [=](){
            QColor color = QColorDialog::getColor(Qt::white, w,tr("设置填充颜色"));
            updater->setSelChartFillColor(color);
        });
    }
    QAction *changeLineColor = menu.addAction("更换线条颜色");
    connect(changeLineColor, &QAction::triggered, [=](){
        QColor color = QColorDialog::getColor(Qt::white, w,tr("设置线条颜色"));
        updater->setSelChartLineColor(color);
    });
    menu.exec(canvas->mapToGlobal(pos));
}

void Controller::on_escPressed()
{
    canvas->hideMagSizeAll();
    canvas->curSelecChart = nullptr;
    updater->clearFrameSelect();
}

void Controller::on_delPressed()
{
    if(canvas->curSelecChart)
    {
        if(canvas->curSelecChart->chartType == PaintChartType::LINE)
            remover->delLine(canvas->curSelecChart);
        else
            remover->delChart(canvas->curSelecChart);
        canvas->curSelecChart = nullptr;
        // To saveChange
        to_saveChange(redoUndoer->reNo+1);
    }
    else if(updater->isFrameSelected)
    {
        for(auto x : updater->frameSelCharts)
            remover->delChart(x);
        updater->clearFrameSelect();
        // To saveChange
        to_saveChange(redoUndoer->reNo+1);
    }
}

void Controller::on_leftPressed(QMouseEvent *event)
{
    if(updater->isFrameSelected == false && canvas->curSelecChart == nullptr)
    {
        mouseEventType = MOUSE_EVENT_TYPE::FRAME_SELECTING;
        updater->frameX = event->pos().rx();
        updater->frameY = event->pos().ry();
    }
    else if(updater->isFrameSelected == true)
    {
        if(event->pos().rx() > canvas->x1 && event->pos().rx() < canvas->x2 && event->pos().ry() > canvas->y1 && event->pos().ry() < canvas->y2)
        {
            mouseEventType = MOUSE_EVENT_TYPE::CHANGE_FRAME_POS;
            updater->curSelecFramePos = event->pos();
        }
        else
            updater->clearFrameSelect();
    }
    else
    {
        for(auto x : canvas->charts)
        {
            x->overlapChartMousePressed(event);
            if(event->isAccepted()) return;
        }
        for(auto x : canvas->line)
        {
            x->overlapChartMousePressed(event);
            if(event->isAccepted()) return;
        }
        canvas->hideMagSizeAll();
        mouseEventType = MOUSE_EVENT_TYPE::NONE;
    }
}

void Controller::on_leftClickToSelect(FlowchartElement * fce, int x, int y)
{
    updater->setSelecChart(fce, x, y);
    mouseEventType = MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS;
}

void Controller::on_mouseMoved(QMouseEvent *event)
{
    mousePos = event->pos();
    if (mouseEventType == MOUSE_EVENT_TYPE::CREATING)
    {
        on_moveToCreate(event->pos().rx(), event->pos().ry());
    }
    else if (mouseEventType == MOUSE_EVENT_TYPE::CHANGE_SIZE)
    {
        on_moveToChangeSize(event->pos().rx(), event->pos().ry());
    }
    else if (mouseEventType == MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT)
    {
        on_moveToLink(event->pos().rx(), event->pos().ry());
    }
    else if (mouseEventType == MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS)
    {
        on_moveToChangePos(event->pos().rx(), event->pos().ry());
    }
    else if (mouseEventType == MOUSE_EVENT_TYPE::FRAME_SELECTING)
    {
        updater->frameSelect(event->pos().rx(), event->pos().ry());
    }
    else if(updater->isFrameSelected)
    {
        if(mouseEventType == MOUSE_EVENT_TYPE::CHANGE_FRAME_POS)
        {
            updater->moveToChangeFramePos(event->pos().rx(), event->pos().ry());
        }
        else
        {
            if(event->pos().rx() > canvas->x1 && event->pos().rx() < canvas->x2 && event->pos().ry() > canvas->y1 && event->pos().ry() < canvas->y2)
                canvas->setCursor(QCursor(Qt::SizeAllCursor));
            else
                canvas->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
    else
    {
        for(auto it = canvas->charts.begin();it!= canvas->charts.end();it++)
        {
            (*it)->overlapChartMouseMove(event);
            if(event->isAccepted()) return;
        }
        for(auto it = canvas->line.begin();it!=canvas->line.end();it++)
        {
            (*it)->overlapChartMouseMove(event);
            if(event->isAccepted()) return;
        }
        if(canvas->curSelecChart == nullptr)
        {
            canvas->setCursor(QCursor(Qt::ArrowCursor));
            mouseEventType = MOUSE_EVENT_TYPE::NONE;
        }
    }
}
void Controller::on_moveToCreate(int x, int y)
{
    drawer->moveToCreate(x, y);
}

void Controller::on_moveToChangeSize(int x, int y)
{
    updater->moveToChangeSize(x, y);
}

void Controller::on_moveToChangePos(int x, int y)
{
    updater->moveToChangePos(x, y);
}

void Controller::on_moveToLink(int x, int y)
{
    if(drawer->moveToLink(x, y) == 1)
    {
        connect(drawer->newLineChart,&FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
        connect(drawer->newLineChart,SIGNAL(setTypeChangeSize(ORIENTION)), this,SLOT(setTypeChangeSize(ORIENTION)));
    }
}

void Controller::on_mouseReleased(QMouseEvent *event)
{
    if(mouseEventType == MOUSE_EVENT_TYPE::CREATING)
    {
        connect(drawer->curPaintChart,&FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
        connect(drawer->curPaintChart,SIGNAL(setTypeChangeSize(ORIENTION)), this, SLOT(setTypeChangeSize(ORIENTION)));
        if(drawer->curPaintChart->chartType != PaintChartType::LINE)
            connect(drawer->curPaintChart,SIGNAL(setTypeCreateMagPoint(FlowchartElement *,ORIENTION,int)),this,SLOT(setTypeCreateMagPoint(FlowchartElement *,ORIENTION,int)));
        on_doneCreate();
        to_saveChange(redoUndoer->reNo+1);
    }
    else if(mouseEventType == MOUSE_EVENT_TYPE::CHANGE_SIZE)
    {
        on_doneChangeSize();
        to_saveChange(redoUndoer->reNo+1);
    }
    else if(mouseEventType == MOUSE_EVENT_TYPE::RUNTIME_CREATE_MAGPOINT)
    {
        on_doneLink();
        to_saveChange(redoUndoer->reNo+1);
    }
    else if(mouseEventType == MOUSE_EVENT_TYPE::RUNTIME_CHANGE_POS)
        to_saveChange(redoUndoer->reNo+1);
    else if(mouseEventType == MOUSE_EVENT_TYPE::NONE)
        event->ignore();
    else if(mouseEventType == MOUSE_EVENT_TYPE::FRAME_SELECTING)
    {
        if(event->pos().rx() - 1 < updater->frameX || event->pos().ry() - 1 < updater->frameY)
            updater->clearFrameSelect();
        else
            updater->doneFrameSelect();
    }
    mouseEventType = MOUSE_EVENT_TYPE::NONE;
}

void Controller::on_doneCreate()
{
    drawer->doneCreate();
}

void Controller::on_doneChangeSize()
{
    updater->doneChangeSize();
}

void Controller::on_doneLink()
{
    drawer->doneLink();
}

void Controller::on_saveFile()
{
    QString tmpFilePath = QFileDialog::getSaveFileName(w, tr("保存文件"), "C:", tr("FY文件(*.fy)"));
    if(tmpFilePath == "") return;
    filer->saveFile(tmpFilePath);
}

void Controller::on_openFile()
{
    QString tmpFilePath = QFileDialog::getOpenFileName(w,tr("打开文件"),"C:",tr("FY文件(*.fy)"));
    if(tmpFilePath == "") return;
    remover->clear();
    filer->openFile(tmpFilePath);
    connectAll();
    to_saveChange(0);
}

void Controller::connectAll(){
    for(auto x : canvas->charts)
    {
        connect(x, &FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
        connect(x,SIGNAL(setTypeChangeSize(ORIENTION)),this,SLOT(setTypeChangeSize(ORIENTION)));
        connect(x,SIGNAL(setTypeCreateMagPoint(FlowchartElement *,ORIENTION,int)),this,SLOT(setTypeCreateMagPoint(FlowchartElement *,ORIENTION,int)));
    }
    for(auto x : canvas->line)
    {
        connect(x,&FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
        connect(x,SIGNAL(setTypeChangeSize(ORIENTION)),this,SLOT(setTypeChangeSize(ORIENTION)));
    }
}
void Controller::on_newFile(){
    if(!canvas->charts.empty())
        on_saveFile();
    remover->clear();
    to_saveChange(0);
}

void Controller::on_saveAsSVG()
{
    QString tmpFilePath = QFileDialog::getSaveFileName(w, tr("保存文件"), "C:", tr("SVG文件(*.svg)"));
    if(tmpFilePath == "") return;
    filer->saveAsSVG(tmpFilePath);
}

void Controller::on_redo(){
    if(redoUndoer->unNo < 0){
        QMessageBox msgBox;
        msgBox.setText("重写失败，没有更早！");
        msgBox.exec();
    }
    else{
        remover->clear();
        qDebug()<<666;
        redoUndoer->redo();
        qDebug()<<667;
        filer->openFile(QDir::current().filePath("../../assets/cashe/Redo/")+QString::number(redoUndoer->reNo)+".fy");
        connectAll();
        qDebug()<<668;
    }
}

void Controller::on_undo(){
    if(redoUndoer->reNo <= 0){
        QMessageBox msgBox;
        msgBox.setText("撤销失败，没有更早！");
        msgBox.exec();
    }
    else{
        remover->clear();
        redoUndoer->undo();
        filer->openFile(QDir::current().filePath("../../assets/cashe/Redo/")+QString::number(redoUndoer->reNo)+".fy");
        connectAll();
    }
}

void Controller::on_copy(){
    drawer->copy();
}

void Controller::on_cut(){
    if(canvas->curSelecChart == nullptr)
        return;
    drawer->copy();
    remover->delChart(canvas->curSelecChart);
    canvas->curSelecChart = nullptr;
    to_saveChange(redoUndoer->reNo+1);
}

void Controller::on_paste(){
    drawer->paste(mousePos.rx(), mousePos.ry());
    connect(canvas->curSelecChart, &FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
    connect(canvas->curSelecChart, SIGNAL(setTypeChangeSize(ORIENTION)),this,SLOT(setTypeChangeSize(ORIENTION)));
    connect(canvas->curSelecChart, SIGNAL(setTypeCreateMagPoint(FlowchartElement *,ORIENTION,int)),this,SLOT(setTypeCreateMagPoint(FlowchartElement *,ORIENTION,int)));
    to_saveChange(redoUndoer->reNo+1);
}

void Controller::on_search(){
    bool ok;
    QString searchText = QInputDialog::getText(w, tr("查找"),
            tr("请输入查找内容:"), QLineEdit::Normal,"", &ok);
    if (ok && !searchText.isEmpty())
        updater->search(searchText);
}

void Controller::on_replace(){
    ReplaceDialog dialog(w);

    if (dialog.exec() == QDialog::Accepted) {
        QString findText = dialog.getFindText();       // 获取查找内容
        QString replaceText = dialog.getReplaceText(); // 获取替换内容
        if (!findText.isEmpty())
            updater->replace(findText, replaceText);
    }
    to_saveChange(redoUndoer->reNo+1);
}

void Controller::to_saveChange(int now){
    if(now==0){
        remover->clearCasheRe();
        remover->clearCasheUn();
        filer->saveFile(QDir::current().filePath("../../assets/cashe/Redo/")+"0"+".fy");
        redoUndoer->reNo=0;
        redoUndoer->unNo=-1;
    }
    else{
        filer->saveFile(QDir::current().filePath("../../assets/cashe/Redo/")+QString::number(now)+".fy");
        redoUndoer->reNo+=1;
        remover->clearCasheUn();
    }
}

void Controller::initCashe(){
    to_saveChange(0);
}
