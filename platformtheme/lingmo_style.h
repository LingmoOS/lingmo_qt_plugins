// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QProxyStyle>

class LingmoStyle final : public QProxyStyle
{
public:
    LingmoStyle();

    void polish(QApplication *application) override;
    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr,
                    const QWidget *widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption *option = nullptr,
                  const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size,
                           const QWidget *widget) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter,
                            const QWidget *widget = nullptr) const override;

private:
    static QColor surface(const QPalette &palette, bool enabled, bool hovered, bool pressed);
    static QColor outline(const QPalette &palette, bool enabled, bool focused, bool hovered);
    static void rounded(QPainter *painter, const QRectF &rect, qreal radius, const QColor &fill,
                        const QColor &border = {});
};
