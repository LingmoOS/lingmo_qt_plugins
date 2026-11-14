// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QStylePlugin>

class LingmoStylePlugin final : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "lingmo-style.json")

public:
    QStringList keys() const { return {QStringLiteral("Lingmo"), QStringLiteral("Darkly")}; }
    QStyle *create(const QString &key) override;
};
