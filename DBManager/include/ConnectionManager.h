#pragma once
#include <memory> //используется для умного указателя, чтобы более корректно работать с памятью
//#include <QObject> //здесь определён Q_DISABLE_COPY

namespace db
{
class ConnectionManager
{
public:

    //Q_DISABLE_COPY(ConnectionManager) //макрос запрета копирования
    ConnectionManager(const ConnectionManager&) = delete; //удаляем генерацию определённую конструктором
    ConnectionManager& operator = (const ConnectionManager&) = delete; //удаляем оператор присваивания
    //если мы явно указали конструктор копирования, то move-конструктора не генерируется
    ~ConnectionManager();

    //будет реализовываться singletone Маерса как один из 3-возможных реализации этого паттерна
    static ConnectionManager& inctance();

private:
    ConnectionManager();
    struct ConnectionManagerPrivate;
    std::unique_ptr<ConnectionManagerPrivate> m_d;
};
}

