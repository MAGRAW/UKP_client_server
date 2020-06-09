#pragma once
#include <QtGlobal>

namespace net
{
enum class PackageType : qint32 {
    INVALID = 10,
    CONTACTS_REQUEST,
    CONTACTS_RESPONSE
};

//Для отображения текущего состояния подключения
enum ConnextionState {
    Connected,
    Disconnected,
    Connecting
};
}
