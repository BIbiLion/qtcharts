#include "qscatterseries.h"
#include "scatterseries_p.h"
#include "qchart.h"

/*!
    \class QScatterSeries
    \brief QtCommercial Chart series API for showing scatter series.

    \mainclass

    Example on how to create a chart with scatter series:
    \snippet ../example/scatter/main.cpp 1

    The example code would result the following:

    \image scatter_example1.jpg
*/

/*!
    \enum QScatterSeries::MarkerShape

    This enum describes the shape used when rendering marker items.

    \value MarkerShapeDefault
    \value MarkerShapePoint
    \value MarkerShapeX
    \value MarkerShapeRectangle
    \value MarkerShapeTiltedRectangle
    \value MarkerShapeTriangle
    \value MarkerShapeCircle
*/

/*!
    \fn QChartSeriesType QScatterSeries::type() const
    \brief Returns QChartSeries::SeriesTypeScatter.
*/

/*!
    \fn void QScatterSeries::clicked()
    \brief TODO
*/

/*!
    \fn void QScatterSeries::hoverEnter()
    \brief TODO
*/

/*!
    \fn void QScatterSeries::hoverLeave()
    \brief TODO
*/

/*!
    \fn void QScatterSeries::changed()
    \brief TODO
*/

QTCOMMERCIALCHART_BEGIN_NAMESPACE

QScatterSeriesPrivate::QScatterSeriesPrivate() :
    m_data(QList<QPointF>()),
    m_markerPen(QPen()),
    m_markerBrush(QBrush()),
    m_markerShape(QScatterSeries::MarkerShapeDefault)
{
    // Initialize pen color to invalid to use a theme color by default
    m_markerPen.setColor(QColor::Invalid);
    m_markerBrush.setColor(QColor::Invalid);
}

/*!
    Constructs a series object which is a child of \a parent.
*/
QScatterSeries::QScatterSeries(QObject *parent) :
    QChartSeries(parent),
    d(new QScatterSeriesPrivate())
{
}

/*!
    Destroys the object. Note that adding series to QChart transfers the ownership to the chart.
*/
QScatterSeries::~QScatterSeries()
{
    delete d;
}

/*!
    Add single data point with \a value to the series.
    For example:
    \snippet ../example/scatter/main.cpp 2
*/
void QScatterSeries::addData(QPointF value)
{
    d->m_data.append(value);
    emit changed();
}

/*!
    Add list of \a points to the series.
*/
void QScatterSeries::addData(QList<QPointF> points)
{
    d->m_data.append(data);
    emit changed();
}

/*!
    Stream operator for adding a data point with \a value to the series.
    \sa addData()

    For example:
    \snippet ../example/scatter/main.cpp 3
*/
QScatterSeries& QScatterSeries::operator << (const QPointF &value)
{
    d->m_data.append(value);
    emit changed();
    return *this;
}

/*!
    Stream operator for adding a list of points to the series.
    \sa addData()
*/
QScatterSeries& QScatterSeries::operator << (QList<QPointF> value)
{
    d->m_data.append(value);
    emit changed();
    return *this;
}

/*!
    Replaces the data of the series with the given list of data \a points.
*/
void QScatterSeries::setData(QList<QPointF> points)
{
    d->m_data = data;
    emit changed();
}

/*!
    Returns the current list of data points of the series.
*/
QList<QPointF> QScatterSeries::data()
{
    return d->m_data;
}

/*!
    Overrides the default pen used for drawing a marker item with a user defined \a pen. The
    default pen is defined by chart theme setting.

    For example:
    \snippet ../example/scatter/main.cpp 5

    Would present your scatter markers with an opaque, uglyish green outlines instead of the
    beatiful markers defined by the chart's theme:
    \image scatter_example_pen.jpg

    \sa setMarkerBrush()
    \sa QChart::setChartTheme()
*/
void QScatterSeries::setMarkerPen(QPen pen)
{
    d->m_markerPen = pen;
}

/*!
    Returns the pen used for drawing markers.
*/
QPen QScatterSeries::markerPen()
{
    return d->m_markerPen;
}

/*!
    Overrides the default brush of the marker items with a user defined \a brush. The default brush
    is defined by chart theme setting.

    For example:
    \snippet ../example/scatter/main.cpp 4

    Would fill your scatter markers with an opaque red color:
    \image scatter_example_brush.jpg

    \sa setMarkerPen()
    \sa QChart::setChartTheme()
*/
void QScatterSeries::setMarkerBrush(QBrush brush)
{
    d->m_markerBrush = brush;
}

/*!
    Returns the brush used for drawing markers.
*/
QBrush QScatterSeries::markerBrush()
{
    return d->m_markerBrush;
}

/*!
    Overrides the default shape of the marker items with a user defined \a shape. The default shape
    is defined by chart theme setting.

    For example:
    \snippet ../example/scatter/main.cpp 6

    Would make your scatter marker items rectangle:
    \image scatter_example_shape.jpg
*/
void QScatterSeries::setMarkerShape(MarkerShape shape)
{
    d->m_markerShape = shape;
}

/*!
    Returns the shape used for drawing markers.
*/
QScatterSeries::MarkerShape QScatterSeries::markerShape()
{
    return (QScatterSeries::MarkerShape) d->m_markerShape;
}

#include "moc_qscatterseries.cpp"

QTCOMMERCIALCHART_END_NAMESPACE
