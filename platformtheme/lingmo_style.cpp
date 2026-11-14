// SPDX-License-Identifier: GPL-3.0-or-later

#include "lingmo_style.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QPainter>
#include <QProgressBar>
#include <QScrollBar>
#include <QSlider>
#include <QStyleOptionButton>
#include <QStyleOptionComplex>
#include <QStyleOptionProgressBar>
#include <QStyleOptionSlider>
#include <QTabBar>
#include <QStyleFactory>

namespace {
constexpr qreal kRadius = 7;
constexpr qreal kSmallRadius = 5;
constexpr int kSpacing = 8;
}

LingmoStyle::LingmoStyle()
    : QProxyStyle(QStyleFactory::create(QStringLiteral("Fusion")))
{
}

void LingmoStyle::polish(QApplication *application)
{
    QProxyStyle::polish(application);
    application->setPalette(application->palette());
}

QColor LingmoStyle::surface(const QPalette &palette, bool enabled, bool hovered, bool pressed)
{
    QColor base = palette.color(QPalette::Button);
    if (pressed)
        base = base.darker(115);
    else if (hovered)
        base = base.lighter(112);
    if (!enabled)
        base.setAlpha(120);
    return base;
}

QColor LingmoStyle::outline(const QPalette &palette, bool enabled, bool focused, bool hovered)
{
    if (!enabled)
        return palette.color(QPalette::Mid);
    if (focused)
        return palette.color(QPalette::Highlight);
    return hovered ? palette.color(QPalette::Midlight) : palette.color(QPalette::Mid);
}

void LingmoStyle::rounded(QPainter *painter, const QRectF &rect, qreal radius, const QColor &fill,
                          const QColor &border)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(border.isValid() ? QPen(border, 1) : Qt::NoPen);
    painter->setBrush(fill);
    painter->drawRoundedRect(rect.adjusted(0.5, 0.5, -0.5, -0.5), radius, radius);
    painter->restore();
}

int LingmoStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    switch (metric) {
    case PM_DefaultFrameWidth:
        return 1;
    case PM_ButtonMargin:
        return 12;
    case PM_LayoutHorizontalSpacing:
    case PM_LayoutVerticalSpacing:
        return kSpacing;
    case PM_ScrollBarExtent:
        return 12;
    case PM_ScrollBarSliderMin:
        return 28;
    case PM_IndicatorWidth:
    case PM_IndicatorHeight:
        return 18;
    case PM_CheckBoxLabelSpacing:
        return 6;
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}

int LingmoStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget,
                           QStyleHintReturn *returnData) const
{
    switch (hint) {
    case SH_Widget_Animate:
        return 1;
    case SH_Menu_MouseTracking:
    case SH_MenuBar_MouseTracking:
        return 1;
    default:
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
}

QSize LingmoStyle::sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size,
                                    const QWidget *widget) const
{
    QSize result = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == CT_PushButton || type == CT_ToolButton)
        result.rheight() = qMax(result.height(), 32);
    if (type == CT_MenuItem)
        result.rheight() = qMax(result.height(), 32);
    return result;
}

void LingmoStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                const QWidget *widget) const
{
    if (element == PE_PanelButtonCommand || element == PE_PanelButtonTool) {
        const bool enabled = option->state & State_Enabled;
        const bool hovered = option->state & State_MouseOver;
        const bool pressed = option->state & State_Sunken;
        rounded(painter, option->rect, kRadius, surface(option->palette, enabled, hovered, pressed),
                outline(option->palette, enabled, option->state & State_HasFocus, hovered));
        return;
    }
    if (element == PE_FrameLineEdit || element == PE_Frame || element == PE_FrameGroupBox) {
        const bool enabled = option->state & State_Enabled;
        rounded(painter, option->rect, kSmallRadius, option->palette.brush(QPalette::Base).color(),
                outline(option->palette, enabled, option->state & State_HasFocus,
                        option->state & State_MouseOver));
        return;
    }
    if (element == PE_IndicatorCheckBox || element == PE_IndicatorRadioButton) {
        const bool checked = option->state & State_On;
        const bool enabled = option->state & State_Enabled;
        const QRectF box = QRectF(option->rect).adjusted(1, 1, -1, -1);
        if (element == PE_IndicatorRadioButton) {
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setPen(QPen(outline(option->palette, enabled, false, option->state & State_MouseOver), 1));
            painter->setBrush(checked ? option->palette.brush(QPalette::Highlight) : Qt::NoBrush);
            painter->drawEllipse(box);
            if (checked) {
                painter->setBrush(option->palette.brush(QPalette::HighlightedText));
                painter->drawEllipse(box.adjusted(5, 5, -5, -5));
            }
        } else {
            rounded(painter, box, 5, checked ? option->palette.brush(QPalette::Highlight).color()
                                             : option->palette.brush(QPalette::Base).color(),
                    outline(option->palette, enabled, false, option->state & State_MouseOver));
            if (checked) {
                painter->setPen(QPen(option->palette.color(QPalette::HighlightedText), 2));
                painter->drawLine(box.left() + 4, box.center().y(), box.center().x() - 1, box.bottom() - 4);
                painter->drawLine(box.center().x() - 1, box.bottom() - 4, box.right() - 3, box.top() + 4);
            }
        }
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void LingmoStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                              const QWidget *widget) const
{
    if (element == CE_MenuItem) {
        const auto *item = qstyleoption_cast<const QStyleOptionMenuItem *>(option);
        if (item && item->state & State_Selected)
            rounded(painter, option->rect.adjusted(4, 2, -4, -2), kSmallRadius,
                    option->palette.color(QPalette::Highlight).lighter(110));
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }
    if (element == CE_ProgressBar) {
        const auto *progress = qstyleoption_cast<const QStyleOptionProgressBar *>(option);
        if (progress) {
            rounded(painter, progress->rect, kSmallRadius, option->palette.color(QPalette::Base));
            QRect fill = progress->rect;
            if (progress->maximum > progress->minimum)
                fill.setWidth(int(double(fill.width()) * (progress->progress - progress->minimum)
                                  / (progress->maximum - progress->minimum)));
            rounded(painter, fill, kSmallRadius, option->palette.color(QPalette::Highlight));
            return;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

void LingmoStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                                     QPainter *painter, const QWidget *widget) const
{
    if (control == CC_ScrollBar) {
        const auto *scroll = qstyleoption_cast<const QStyleOptionSlider *>(option);
        if (scroll) {
            const QRect groove = subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget);
            const QRect handle = subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);
            rounded(painter, groove, 6, option->palette.color(QPalette::Base));
            rounded(painter, handle, 6, option->palette.color(QPalette::Midlight));
            return;
        }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}
