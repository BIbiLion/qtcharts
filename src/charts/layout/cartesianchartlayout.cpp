/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt Enterprise Charts Add-on.
**
** $QT_BEGIN_LICENSE$
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
** $QT_END_LICENSE$
**
****************************************************************************/

#include <private/cartesianchartlayout_p.h>
#include <private/chartpresenter_p.h>
#include <private/chartaxiselement_p.h>
#include <QtCore/QDebug>

QT_CHARTS_BEGIN_NAMESPACE

static const qreal maxAxisPortion = 0.4;

CartesianChartLayout::CartesianChartLayout(ChartPresenter *presenter)
    : AbstractChartLayout(presenter)
{
}

CartesianChartLayout::~CartesianChartLayout()
{
}

QRectF CartesianChartLayout::calculateAxisGeometry(const QRectF &geometry, const QList<ChartAxisElement *> &axes) const
{
    QSizeF left(0,0);
    QSizeF minLeft(0,0);
    QSizeF right(0,0);
    QSizeF minRight(0,0);
    QSizeF bottom(0,0);
    QSizeF minBottom(0,0);
    QSizeF top(0,0);
    QSizeF minTop(0,0);
    QSizeF labelExtents(0,0);
    int leftCount = 0;
    int rightCount = 0;
    int topCount = 0;
    int bottomCount = 0;

    foreach (ChartAxisElement *axis , axes) {

        if (!axis->isVisible())
            continue;


        QSizeF size = axis->effectiveSizeHint(Qt::PreferredSize);
        //this is used to get single thick font size
        QSizeF minSize = axis->effectiveSizeHint(Qt::MinimumSize);

        switch (axis->axis()->alignment()) {
        case Qt::AlignLeft:
           left.setWidth(left.width()+size.width());
           left.setHeight(qMax(left.height(),size.height()));
           minLeft.setWidth(minLeft.width()+minSize.width());
           minLeft.setHeight(qMax(minLeft.height(),minSize.height()));
           labelExtents.setHeight(qMax(size.height(), labelExtents.height()));
           leftCount++;
           break;
        case Qt::AlignRight:
            right.setWidth(right.width()+size.width());
            right.setHeight(qMax(right.height(),size.height()));
            minRight.setWidth(minRight.width()+minSize.width());
            minRight.setHeight(qMax(minRight.height(),minSize.height()));
            labelExtents.setHeight(qMax(size.height(), labelExtents.height()));
            rightCount++;
            break;
        case Qt::AlignTop:
            top.setWidth(qMax(top.width(),size.width()));
            top.setHeight(top.height()+size.height());
            minTop.setWidth(qMax(minTop.width(),minSize.width()));
            minTop.setHeight(minTop.height()+minSize.height());
            labelExtents.setWidth(qMax(size.width(), labelExtents.width()));
            topCount++;
            break;
        case Qt::AlignBottom:
            bottom.setWidth(qMax(bottom.width(), size.width()));
            bottom.setHeight(bottom.height() + size.height());
            minBottom.setWidth(qMax(minBottom.width(),minSize.width()));
            minBottom.setHeight(minBottom.height() + minSize.height());
            labelExtents.setWidth(qMax(size.width(), labelExtents.width()));
            bottomCount++;
            break;
        default:
            qWarning()<<"Axis is without alignment !";
            break;
        }
    }

    qreal totalVerticalAxes = leftCount + rightCount;
    qreal leftSqueezeRatio = 1.0;
    qreal rightSqueezeRatio = 1.0;
    qreal vratio = 0;

    if (totalVerticalAxes > 0)
        vratio = (maxAxisPortion * geometry.width()) / totalVerticalAxes;

    if (leftCount > 0) {
        int maxWidth = vratio * leftCount;
        if (left.width() > maxWidth) {
            leftSqueezeRatio = maxWidth / left.width();
            left.setWidth(maxWidth);
        }
    }
    if (rightCount > 0) {
        int maxWidth = vratio * rightCount;
        if (right.width() > maxWidth) {
            rightSqueezeRatio = maxWidth / right.width();
            right.setWidth(maxWidth);
        }
    }

    qreal totalHorizontalAxes = topCount + bottomCount;
    qreal topSqueezeRatio = 1.0;
    qreal bottomSqueezeRatio = 1.0;
    qreal hratio = 0;

    if (totalHorizontalAxes > 0)
        hratio = (maxAxisPortion * geometry.height()) / totalHorizontalAxes;

    if (topCount > 0) {
        int maxHeight = hratio * topCount;
        if (top.height() > maxHeight) {
            topSqueezeRatio = maxHeight / top.height();
            top.setHeight(maxHeight);
        }
    }
    if (bottomCount > 0) {
        int maxHeight = hratio * bottomCount;
        if (bottom.height() > maxHeight) {
            bottomSqueezeRatio = maxHeight / bottom.height();
            bottom.setHeight(maxHeight);
        }
    }

    qreal minHeight = qMax(minLeft.height(),minRight.height()) + 1;
    qreal minWidth = qMax(minTop.width(),minBottom.width()) + 1;

    // Ensure that there is enough space for first and last tick labels.
    left.setWidth(qMax(labelExtents.width(), left.width()));
    right.setWidth(qMax(labelExtents.width(), right.width()));
    top.setHeight(qMax(labelExtents.height(), top.height()));
    bottom.setHeight(qMax(labelExtents.height(), bottom.height()));

    QRectF chartRect = geometry.adjusted(qMax(left.width(),minWidth/2), qMax(top.height(), minHeight/2),-qMax(right.width(),minWidth/2),-qMax(bottom.height(),minHeight/2));

    qreal leftOffset = 0;
    qreal rightOffset = 0;
    qreal topOffset = 0;
    qreal bottomOffset = 0;

    foreach (ChartAxisElement *axis , axes) {

        if (!axis->isVisible())
            continue;

        QSizeF size = axis->effectiveSizeHint(Qt::PreferredSize);

        switch (axis->axis()->alignment()){
        case Qt::AlignLeft:{
            qreal width = size.width();
            if (leftSqueezeRatio < 1.0)
                width *= leftSqueezeRatio;
            leftOffset+=width;
            axis->setGeometry(QRect(chartRect.left()-leftOffset, geometry.top(),width, geometry.bottom()),chartRect);
            break;
        }
        case Qt::AlignRight:{
            qreal width = size.width();
            if (rightSqueezeRatio < 1.0)
                width *= rightSqueezeRatio;
            axis->setGeometry(QRect(chartRect.right()+rightOffset,geometry.top(),width,geometry.bottom()),chartRect);
            rightOffset+=width;
            break;
        }
        case Qt::AlignTop: {
            qreal height = size.height();
            if (topSqueezeRatio < 1.0)
                height *= topSqueezeRatio;
            axis->setGeometry(QRect(geometry.left(), chartRect.top() - topOffset - height, geometry.width(), height), chartRect);
            topOffset += height;
            break;
        }
        case Qt::AlignBottom:
            qreal height = size.height();
            if (bottomSqueezeRatio < 1.0)
                height *= bottomSqueezeRatio;
            axis->setGeometry(QRect(geometry.left(), chartRect.bottom() + bottomOffset, geometry.width(), height), chartRect);
            bottomOffset += height;
            break;
        }
    }

    return chartRect;
}

QRectF CartesianChartLayout::calculateAxisMinimum(const QRectF &minimum, const QList<ChartAxisElement *> &axes) const
{
    QSizeF left;
    QSizeF right;
    QSizeF bottom;
    QSizeF top;

    foreach (ChartAxisElement *axis, axes) {
        QSizeF size = axis->effectiveSizeHint(Qt::MinimumSize);

        if (!axis->isVisible())
            continue;

        switch (axis->axis()->alignment()) {
        case Qt::AlignLeft:
            left.setWidth(left.width() + size.width());
            left.setHeight(qMax(left.height() * 2, size.height()));
            break;
        case Qt::AlignRight:
            right.setWidth(right.width() + size.width());
            right.setHeight(qMax(right.height() * 2, size.height()));
            break;
        case Qt::AlignTop:
            top.setWidth(qMax(top.width(), size.width()));
            top.setHeight(top.height() + size.height());
            break;
        case Qt::AlignBottom:
            bottom.setWidth(qMax(bottom.width(), size.width()));
            bottom.setHeight(bottom.height() + size.height());
            break;
        }
    }
    return minimum.adjusted(0, 0, left.width() + right.width() + qMax(top.width(), bottom.width()), top.height() + bottom.height() + qMax(left.height(), right.height()));
}

QT_CHARTS_END_NAMESPACE