#pragma once

#include <cstdint>
#include <qhashfunctions.h>
#include <QDebug>

struct LayerId {
    uint32_t user = 0;
    uint32_t layer = 0;

    LayerId() = default;
    LayerId(uint32_t user, uint32_t layer): user(user), layer(layer) {}
};

inline bool operator<(const LayerId& a, const LayerId& b) {
    if (a.user == b.user) {
        return a.layer < b.layer;
    }
    return a.user < b.user;
}

inline bool operator==(const LayerId& a, const LayerId& b) {
    return a.user == b.user && a.layer == b.layer;
}

inline int qHash(const LayerId& layer) {
    return qHash(layer.user) + 31*qHash(layer.layer);
}

inline QDebug operator<<(QDebug stream, const LayerId& layer) {
    return stream << "{ user" << layer.user << "layer" << layer.layer << "}";
}
