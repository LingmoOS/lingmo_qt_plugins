// SPDX-License-Identifier: GPL-3.0-or-later

#include "lingmo_style.h"
#include "lingmo_style_plugin.h"

QStyle *LingmoStylePlugin::create(const QString &key)
{
    return (key.compare(QStringLiteral("Lingmo"), Qt::CaseInsensitive) == 0
            || key.compare(QStringLiteral("Darkly"), Qt::CaseInsensitive) == 0)
            ? new LingmoStyle
            : nullptr;
}

