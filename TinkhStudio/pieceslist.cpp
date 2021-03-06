/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "pieceslist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QLabel>
#include "BrickData.h"

PiecesList::PiecesList(QWidget *parent)
    : QListWidget(parent)
{
    setDragEnabled(true);
    //setViewMode(QListView::IconMode);
    //setIconSize(QSize(m_PieceSize, m_PieceSize));
    setSpacing(3);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setMinimumSize(200, 500);
}

void PiecesList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-brickdata"))
        event->accept();
    else
        event->ignore();
}

void PiecesList::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-brickdata")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void PiecesList::dropEvent(QDropEvent *event)
{
//    if (event->mimeData()->hasFormat("application/x-brickdata")) {
//        QByteArray pieceData = event->mimeData()->data("image/x-puzzle-piece");
//        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
//        QPixmap pixmap;
//        QPoint location;
//        dataStream >> pixmap >> location;

//        addPiece(pixmap, location);

//        event->setDropAction(Qt::MoveAction);
//        event->accept();
//    } else {
//        event->ignore();
//    }
}

void PiecesList::addPiece(QString pieceLabel)
{
    QListWidgetItem *pieceItem = new QListWidgetItem(this);

    //pieceItem->setIcon(QIcon(pixmap));
    //pieceItem->setData(Qt::UserRole, QVariant(pixmap));
    //pieceItem->setData(Qt::UserRole+1, location);

    pieceItem->setText(pieceLabel);
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
}

void PiecesList::startDrag(Qt::DropActions /*supportedActions*/)
{
    QListWidgetItem *item = currentItem();

    QLabel *label = new QLabel(item->text());
    QPixmap pixmap(QSize(200,100));
    label->render(&pixmap,QPoint(50,30));

    QPoint hotSpot = QPoint(10,15);

    //QMimeData *mimeData = new QMimeData;
    //mimeData->setText(item->text());
    //mimeData->setData("application/x-brickdata", QByteArray::number(row(item)));

    BrickData brickData(-1);
    brickData.createBrickData(item->text());
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << brickData;
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-brickdata", itemData);
    mimeData->setText(item->text());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(hotSpot);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}
