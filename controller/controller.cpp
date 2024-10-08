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
    initCache();
}

void Controller::initConnections()
{
    // 绑定图源选项
    connect(w->ui->processElementBtn, &QPushButton::clicked, [=]()
            {
        canvas->hideMagSizeAll();
        drawer->setPaintProcessElement();
        mouseEventType = MOUSE_EVENT_TYPE::CREATING; });
    connect(w->ui->decisionElementBtn, &QPushButton::clicked, [=]()
            {
        canvas->hideMagSizeAll();
        drawer->setPaintDecisionElement();
        mouseEventType = MOUSE_EVENT_TYPE::CREATING; });
    connect(w->ui->startEndElementBtn, &QPushButton::clicked, [=]()
            {
        canvas->hideMagSizeAll();
        drawer->setPaintStartEndElement();
        mouseEventType = MOUSE_EVENT_TYPE::CREATING; });
    connect(w->ui->connectorElementBtn, &QPushButton::clicked, [=]()
            {
        canvas->hideMagSizeAll();
        drawer->setPaintConnectorElement();
        mouseEventType = MOUSE_EVENT_TYPE::CREATING; });
    connect(w->ui->dataElementBtn, &QPushButton::clicked, [=]()
            {
        canvas->hideMagSizeAll();
        drawer->setPaintDataElement();
        mouseEventType = MOUSE_EVENT_TYPE::CREATING; });
    connect(w->ui->subprocessElementBtn, &QPushButton::clicked, [=]()
            {
        canvas->hideMagSizeAll();
        drawer->setPaintSubprocessElement();
        mouseEventType = MOUSE_EVENT_TYPE::CREATING; });
    connect(w->ui->documentElementBtn, &QPushButton::clicked, [=]()
            {
        canvas->hideMagSizeAll();
        drawer->setPaintDocumentElement();
        mouseEventType = MOUSE_EVENT_TYPE::CREATING; });

    // 鼠标事件
    connect(canvas, &Canvas::leftPressed, this, &Controller::on_leftPressed);
    connect(canvas, &QWidget::customContextMenuRequested, this, &Controller::showRrightClickMenu);
    connect(canvas, &Canvas::mouseMoved, this, &Controller::on_mouseMoved);
    connect(canvas, &Canvas::mouseReleased, this, &Controller::on_mouseReleased);

    // 键盘按键
    connect(canvas, &Canvas::escPressed, this, &Controller::on_escPressed);
    connect(canvas, &Canvas::delPressed, this, &Controller::on_delPressed);

    // 文件操作
    connect(w->ui->actionSaveFile, &QAction::triggered, this, &Controller::on_saveFile);
    connect(w->ui->actionOpenFile, &QAction::triggered, this, &Controller::on_openFile);
    connect(w->ui->actionNewFile, &QAction::triggered, this, &Controller::on_newFile);
    connect(w->ui->actionSaveAsSVG, &QAction::triggered, this, &Controller::on_saveAsSVG);
    connect(w->ui->actionSaveAsPNG, &QAction::triggered, this, &Controller::on_saveAsPNG);
    connect(w->ui->actionSaveAsElse, &QAction::triggered, this,&Controller::on_saveAsElse);
    connect(w->ui->actionSetBack, &QAction::triggered, this, &Controller::on_setBack);

    // 编辑操作
    connect(w->ui->actionRedo, &QAction::triggered, this, &Controller::on_redo);
    connect(w->ui->actionUndo, &QAction::triggered, this, &Controller::on_undo);
    connect(w->ui->actionCopy, &QAction::triggered, this, &Controller::on_copy);
    connect(w->ui->actionCut, &QAction::triggered, this, &Controller::on_cut);
    connect(w->ui->actionPaste, &QAction::triggered, this, &Controller::on_paste);
    connect(w->ui->actionSearch, &QAction::triggered, this, &Controller::on_search);
    connect(w->ui->actionReplace, &QAction::triggered, this, &Controller::on_replace);
    connect(w->ui->actionFillColor, &QAction::triggered, this, &Controller::on_setFillColor);
    connect(w->ui->actionLineColor, &QAction::triggered, this, &Controller::on_setLineColor);
    connect(w->ui->actionFont, &QAction::triggered, this, &Controller::on_setFontFamily);
    connect(w->ui->actionFontColor_2, &QAction::triggered, this, &Controller::on_setFontColor);

    //ToolBar
    connect(w->ui->actionToolNew,&QAction::triggered, this, &Controller::on_openFile);
    connect(w->ui->actionSaveTool,&QAction::triggered, this, &Controller::on_saveFile);

    connect(w->ui->actionFontBold, &QAction::triggered, this, &Controller::on_setFontBold);
    connect(w->ui->actionFontItalic, &QAction::triggered, this, &Controller::on_setFontItalic);
    connect(w->ui->actionFontUnder, &QAction::triggered, this, &Controller::on_setFontUnder);
    connect(w->ui->actionFontThrou, &QAction::triggered, this, &Controller::on_setFontThrou);
    connect(w->ui->actionFontColor, &QAction::triggered, this, &Controller::on_setFontColor);
    connect(w->ui->actionFontFamily,&QAction::triggered, this, &Controller::on_setFontFamily);

    connect(w->ui->actionToolRedo,&QAction::triggered, this, &Controller::on_redo);
    connect(w->ui->actionToolUndo,&QAction::triggered, this, &Controller::on_undo);

    connect(w->ui->actionToolCopy,&QAction::triggered, this, &Controller::on_copy);
    connect(w->ui->actionToolCut,&QAction::triggered, this, &Controller::on_cut);
    connect(w->ui->actionToolPaste,&QAction::triggered, this, &Controller::on_paste);
}

void Controller::showRrightClickMenu(const QPoint &pos)
{
    QMenu menu;
    if (canvas->curSelecChart == nullptr)
    {
        if (drawer->curPasteChart != nullptr)
        {
            menu.addAction(w->ui->actionPaste);
            menu.exec(canvas->mapToGlobal(pos));
        }
    }
    else
    {
        if (canvas->curSelecChart->chartType != PaintChartType::LINE)
        {
            menu.addAction(w->ui->actionCopy);
            menu.addAction(w->ui->actionCut);
            if (drawer->curPasteChart != nullptr)
                menu.addAction(w->ui->actionPaste);
            menu.addAction(w->ui->actionFillColor);
        }
        menu.addAction(w->ui->actionLineColor);
        menu.addAction(w->ui->actionFontFamily);
        menu.addAction(w->ui->actionFontColor);
        menu.exec(canvas->mapToGlobal(pos));
    }
}

void Controller::on_setFillColor()
{
    if (canvas->curSelecChart == nullptr || canvas->curSelecChart->chartType == PaintChartType::LINE)
        return;
    QColor curcolor= canvas->curSelecChart->paintChartFillPen.color();
    QColor color = QColorDialog::getColor(curcolor, w, tr("设置填充颜色"));
    updater->setSelChartFillColor(color);
    to_saveChange(redoUndoer->reNo + 1);
}

void Controller::on_setFontBold()
{
    if (canvas->curSelecChart == nullptr)
        return;
    QFont font = canvas->curSelecChart->chartText.text->font();
    font.setBold(!font.bold());  // 切换粗体状态
    canvas->curSelecChart->chartText.text->setFont(font);     // 应用新的字体状态
    to_saveChange(redoUndoer->reNo + 1);
}

void Controller::on_setFontItalic()
{
    if (canvas->curSelecChart == nullptr)
        return;
    QFont font = canvas->curSelecChart->chartText.text->font();
    font.setItalic(!font.italic());  // 切换斜体状态
    canvas->curSelecChart->chartText.text->setFont(font);     // 应用新的字体状态
    to_saveChange(redoUndoer->reNo + 1);
}

void Controller::on_setFontUnder()
{
    if (canvas->curSelecChart == nullptr)
        return;
    QFont font = canvas->curSelecChart->chartText.text->font();
    font.setUnderline(!font.underline());  // 切换下划线状态
    canvas->curSelecChart->chartText.text->setFont(font);     // 应用新的字体状态
    to_saveChange(redoUndoer->reNo + 1);
}

void Controller::on_setFontThrou()
{
    if (canvas->curSelecChart == nullptr)
        return;
    QFont font = canvas->curSelecChart->chartText.text->font();
    font.setStrikeOut(!font.strikeOut());  // 切换下划线状态
    canvas->curSelecChart->chartText.text->setFont(font);     // 应用新的字体状态
    to_saveChange(redoUndoer->reNo + 1);
}

void Controller::on_setFontColor()
{
    if (canvas->curSelecChart == nullptr)
        return;
    QColor color = QColorDialog::getColor(Qt::white, w, tr("设置字体颜色"));
    if (color.isValid()) {
        // 设置字体颜色
        QString colorStyle = QString("color: %1").arg(color.name());
        canvas->curSelecChart->chartText.text->setStyleSheet(colorStyle);
    }
    to_saveChange(redoUndoer->reNo + 1);
}

void Controller::on_setFontFamily()
{
    if (canvas->curSelecChart == nullptr)
        return;

    // 获取当前选中图形的字体
    QFont currentFont = canvas->curSelecChart->chartText.text->font();

    // 打开字体选择对话框，只修改字体种类
    bool ok;
    QFont font = QFontDialog::getFont(&ok, currentFont, w);

    if (ok)
    {
        canvas->curSelecChart->chartText.text->setFont(font);
        // 保存更改
        to_saveChange(redoUndoer->reNo + 1);
    }


}

void Controller::on_setLineColor()
{
    if (canvas->curSelecChart == nullptr)
        return;
    QColor curcolor= canvas->curSelecChart->paintChartDrawPen.color();
    QColor color = QColorDialog::getColor(curcolor, w, tr("设置线条颜色"));
    updater->setSelChartLineColor(color);
    to_saveChange(redoUndoer->reNo + 1);
}

void Controller::on_escPressed()
{
    canvas->hideMagSizeAll();
    canvas->curSelecChart = nullptr;
    updater->clearFrameSelect();
}

void Controller::on_delPressed()
{
    if (canvas->curSelecChart)
    {
        if (canvas->curSelecChart->chartType == PaintChartType::LINE)
            remover->delLine(canvas->curSelecChart);
        else
            remover->delChart(canvas->curSelecChart);
        canvas->curSelecChart = nullptr;
        to_saveChange(redoUndoer->reNo + 1);
    }
    else if (updater->isFrameSelected)
    {
        for (auto x : updater->frameSelCharts)
            remover->delChart(x);
        updater->clearFrameSelect();
        to_saveChange(redoUndoer->reNo + 1);
    }
}

void Controller::on_leftPressed(QMouseEvent *event)
{
    canvas->grabKeyboard();
    if (updater->isFrameSelected == false && canvas->curSelecChart == nullptr)
    {
        mouseEventType = MOUSE_EVENT_TYPE::FRAME_SELECTING;
        updater->frameX = event->pos().rx();
        updater->frameY = event->pos().ry();
    }
    else if (updater->isFrameSelected == true)
    {
        if (event->pos().rx() > canvas->x1 && event->pos().rx() < canvas->x2 && event->pos().ry() > canvas->y1 && event->pos().ry() < canvas->y2)
        {
            mouseEventType = MOUSE_EVENT_TYPE::CHANGE_FRAME_POS;
            updater->curSelecFramePos = event->pos();
        }
        else
            updater->clearFrameSelect();
    }
    else
    {
        for (auto x : canvas->charts)
        {
            x->overlapChartMousePressed(event);
            if (event->isAccepted())
                return;
        }
        for (auto x : canvas->line)
        {
            x->overlapChartMousePressed(event);
            if (event->isAccepted())
                return;
        }
        canvas->hideMagSizeAll();
        mouseEventType = MOUSE_EVENT_TYPE::NONE;
    }
}

void Controller::on_leftClickToSelect(FlowchartElement *fce, int x, int y)
{
    updater->setSelecChart(fce, x, y);
    mouseEventType = MOUSE_EVENT_TYPE::CHANGE_POS;
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
    else if (mouseEventType == MOUSE_EVENT_TYPE::CREATE_MAGPOINT)
    {
        on_moveToLink(event->pos().rx(), event->pos().ry());
    }
    else if (mouseEventType == MOUSE_EVENT_TYPE::CHANGE_POS)
    {
        on_moveToChangePos(event->pos().rx(), event->pos().ry());
    }
    else if (mouseEventType == MOUSE_EVENT_TYPE::FRAME_SELECTING)
    {
        updater->frameSelect(event->pos().rx(), event->pos().ry());
    }
    else if (updater->isFrameSelected)
    {
        if (mouseEventType == MOUSE_EVENT_TYPE::CHANGE_FRAME_POS)
        {
            updater->moveToChangeFramePos(event->pos().rx(), event->pos().ry());
        }
        else
        {
            if (event->pos().rx() > canvas->x1 && event->pos().rx() < canvas->x2 && event->pos().ry() > canvas->y1 && event->pos().ry() < canvas->y2)
                canvas->setCursor(QCursor(Qt::SizeAllCursor));
            else
                canvas->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
    else
    {
        for (auto it = canvas->charts.begin(); it != canvas->charts.end(); it++)
        {
            (*it)->overlapChartMouseMove(event);
            if (event->isAccepted())
                return;
        }
        for (auto it = canvas->line.begin(); it != canvas->line.end(); it++)
        {
            (*it)->overlapChartMouseMove(event);
            if (event->isAccepted())
                return;
        }
        if (canvas->curSelecChart == nullptr)
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
    if (drawer->moveToLink(x, y) == 1)
    {
        connect(drawer->newLineChart, &FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
        connect(drawer->newLineChart, &FlowchartElement::setTypeChangeSize, this, &Controller::setTypeChangeSize);
    }
}

void Controller::on_mouseReleased(QMouseEvent *event)
{
    if (mouseEventType == MOUSE_EVENT_TYPE::CREATING)
    {
        connect(drawer->curPaintChart, &FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
        connect(drawer->curPaintChart, &FlowchartElement::setTypeChangeSize, this, &Controller::setTypeChangeSize);
        if (drawer->curPaintChart->chartType != PaintChartType::LINE)
            connect(drawer->curPaintChart, &FlowchartElement::setTypeCreateMagPoint, this, &Controller::setTypeCreateMagPoint);
        on_doneCreate();
        to_saveChange(redoUndoer->reNo + 1);
    }
    else if (mouseEventType == MOUSE_EVENT_TYPE::CHANGE_SIZE)
    {
        on_doneChangeSize();
        to_saveChange(redoUndoer->reNo + 1);
    }
    else if (mouseEventType == MOUSE_EVENT_TYPE::CREATE_MAGPOINT)
    {
        on_doneLink();
        to_saveChange(redoUndoer->reNo + 1);
    }
    else if (mouseEventType == MOUSE_EVENT_TYPE::CHANGE_POS)
        to_saveChange(redoUndoer->reNo + 1);
    else if (mouseEventType == MOUSE_EVENT_TYPE::NONE)
        event->ignore();
    else if (mouseEventType == MOUSE_EVENT_TYPE::FRAME_SELECTING)
    {
        if (event->pos().rx() - 1 < updater->frameX || event->pos().ry() - 1 < updater->frameY)
            updater->clearFrameSelect();
        else
            updater->doneFrameSelect();
    }
    else if (mouseEventType == MOUSE_EVENT_TYPE::CHANGE_FRAME_POS)
    {
        to_saveChange(redoUndoer->reNo + 1);
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
    if (tmpFilePath == "")
        return;
    filer->saveFile(tmpFilePath);
}

void Controller::on_openFile()
{
    QString tmpFilePath = QFileDialog::getOpenFileName(w, tr("打开文件"), "C:", tr("FY文件(*.fy)"));
    if (tmpFilePath == "")
        return;
    remover->clear();
    filer->openFile(tmpFilePath);
    connectAll();
    to_saveChange(0);
}

void Controller::connectAll()
{
    for (auto x : canvas->charts)
    {
        connect(x, &FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
        connect(x, &FlowchartElement::setTypeChangeSize, this, &Controller::setTypeChangeSize);
        connect(x, &FlowchartElement::setTypeCreateMagPoint, this, &Controller::setTypeCreateMagPoint);
    }
    for (auto x : canvas->line)
    {
        connect(x, &FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
        connect(x, &FlowchartElement::setTypeChangeSize, this, &Controller::setTypeChangeSize);
    }
}
void Controller::on_newFile()
{
    if (!canvas->charts.empty())
        on_saveFile();
    remover->clear();
    to_saveChange(0);
}

void Controller::on_saveAsSVG()
{
    QString tmpFilePath = QFileDialog::getSaveFileName(w, tr("导出文件"), "C:", tr("SVG文件(*.svg)"));
    if (tmpFilePath == "")
        return;
    filer->saveAsSVG(tmpFilePath);
}

void Controller::on_saveAsPNG()
{
    QString tmpFilePath = QFileDialog::getSaveFileName(w, tr("导出文件"), "C:", tr("PNG文件(*.png)"));
    if (tmpFilePath == "")
        return;
    filer->saveAsPNG(tmpFilePath);
}

void Controller::on_saveAsElse()
{
    QString tmpFilePath = QFileDialog::getSaveFileName(w, tr("导出文件"), "C:", tr("Images (*.png *.jpg *.bmp *.gif *.tiff *.xpm *.jpeg)"));
    if (tmpFilePath == "")
        return;
    filer->saveAsElse(tmpFilePath);
}

void Controller::on_setBack()
{
    QString imagePath = QFileDialog::getOpenFileName(w, "选择图片", "", "Images (*.png *.jpg *.bmp *.gif *.tiff *.xpm *.jpeg)");
    if (!imagePath.isEmpty())
    {
        canvas->backgroundImage = QPixmap(imagePath);
        canvas->update();
        to_saveChange(redoUndoer->reNo + 1);
    }
}

void Controller::on_redo()
{
    updater->clearFrameSelect();
    if (redoUndoer->unNo < 0)
    {
        QMessageBox msgBox;
        msgBox.setText("重写失败，没有更早！");
        msgBox.exec();
    }
    else
    {
        remover->clear();
        redoUndoer->redo();
        filer->openFile(QDir::current().filePath("assets/cache/Redo/") + QString::number(redoUndoer->reNo) + ".fy");
        connectAll();
    }
}

void Controller::on_undo()
{
    updater->clearFrameSelect();
    if (redoUndoer->reNo <= 0)
    {
        QMessageBox msgBox;
        msgBox.setText("撤销失败，没有更早！");
        msgBox.exec();
    }
    else
    {
        remover->clear();
        redoUndoer->undo();
        filer->openFile(QDir::current().filePath("assets/cache/Redo/") + QString::number(redoUndoer->reNo) + ".fy");
        connectAll();
    }
}

void Controller::on_copy()
{
    drawer->copy();
}

void Controller::on_cut()
{
    if (canvas->curSelecChart == nullptr)
        return;
    drawer->copy();
    remover->delChart(canvas->curSelecChart);
    canvas->curSelecChart = nullptr;
    to_saveChange(redoUndoer->reNo + 1);
}

void Controller::on_paste()
{
    drawer->paste(mousePos.rx(), mousePos.ry());
    connect(canvas->curSelecChart, &FlowchartElement::sendThisClass, this, &Controller::on_leftClickToSelect);
    connect(canvas->curSelecChart, &FlowchartElement::setTypeChangeSize, this, &Controller::setTypeChangeSize);
    connect(canvas->curSelecChart, &FlowchartElement::setTypeCreateMagPoint, this, &Controller::setTypeCreateMagPoint);
    to_saveChange(redoUndoer->reNo + 1);
}

void Controller::on_search()
{
    canvas->releaseKeyboard();
    bool ok;
    QString searchText = QInputDialog::getText(w, tr("查找"),
                                               tr("请输入查找内容:"), QLineEdit::Normal, "", &ok);
    if (ok && !searchText.isEmpty())
        updater->search(searchText);
    canvas->grabKeyboard();
}

void Controller::on_replace()
{
    canvas->releaseKeyboard();
    ReplaceDialog dialog(w);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString findText = dialog.getFindText();       // 获取查找内容
        QString replaceText = dialog.getReplaceText(); // 获取替换内容
        if (!findText.isEmpty())
            updater->replace(findText, replaceText);
    }
    canvas->grabKeyboard();
    to_saveChange(redoUndoer->reNo + 1);
}

void Controller::to_saveChange(int now)
{
    if (now == 0)
    {
        remover->clearCacheRe();
        remover->clearCacheUn();
        filer->saveFile(QDir::current().filePath("assets/cache/Redo/") + "0" + ".fy");
        redoUndoer->reNo = 0;
        redoUndoer->unNo = -1;
    }
    else
    {
        filer->saveFile(QDir::current().filePath("assets/cache/Redo/") + QString::number(now) + ".fy");
        redoUndoer->reNo += 1;
        remover->clearCacheUn();
        redoUndoer->unNo = -1;
    }
}

void Controller::initCache()
{
    QString folderPath = QDir::current().filePath("assets/cache/Redo/");

    // 检查文件夹是否存在，如果不存在则创建
    QDir dir(folderPath);
    if (!dir.exists())
        dir.mkpath(".");
    QString folderPath2 = QDir::current().filePath("assets/cache/Undo/");

    // 检查文件夹是否存在，如果不存在则创建
    QDir dir2(folderPath2);
    if (!dir2.exists())
        dir2.mkpath(".");
    to_saveChange(0);
}
