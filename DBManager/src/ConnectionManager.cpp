#include "ConnectionManager.h"
#include "dbtypes.h"
#include <QString> //не используем std::-шный string потому что придётся его конвертировать в QString
                   //с классами Qt рекомендуется использовать QString.
#include <QSqlDatabase>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

namespace db
{

namespace
{
class DBCloser {
    public:
    void operator()(QSqlDatabase* db)
    {
        db->close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        delete db;
    }
};
}

struct ConnectionManager::ConnectionManagerPrivate
{
    std::unique_ptr<QSqlDatabase, DBCloser> database;
    QString dbPath; //std::string dbPath;
    bool isValid {true};
    DBState state {DBState::OK};

    //Инциализируем базу:
    bool setup();
    bool setupWorkspace();
    bool setupTables();
};

ConnectionManager &ConnectionManager::inctance()
{
    //Основная проблема singleton-а кроется в использовании его в многопоточных приложениях
    //Здесь попытка сделать его потокобезопасным, но вроде как это не гарантия от повторного
    //создания объекта
    /*if (!initialized)
    {
        ConnectionManager* instance = new ConnectionManager;
        initialized = true;
    }*/
    //singleton Маерса решает эту проблему. Начиная с 11-го стандарта singleton потокобезопасен
    //Потому что статические переменные инициализируются в одном потоке. Все остальные потоки будут
    //ждать пока static не проинициализируется.
    static ConnectionManager instance;
    return instance;
}

ConnectionManager::ConnectionManager()
    : m_d {new ConnectionManagerPrivate {}}
{
    const bool setupResult {m_d->setup()};
    m_d->isValid = setupResult;
}

ConnectionManager::~ConnectionManager()
{

}

bool ConnectionManager::ConnectionManagerPrivate::setup()
{
    //Определяемся с драйвером, который будем ииспользовать
    static const QString  driver {"QSQLITE"};
    if (!QSqlDatabase::isDriverAvailable(driver)) //проверка на наличие драйвера
    {
        state = DBState::ERROR_NO_DRIVER;
        qCritical() << "Driver" << driver << "is not available";
        return false;
    }

    if (!setupWorkspace())
    {
        state = DBState::ERROR_WORKSPACE;
        qCritical() << "Workspace setup failed!";
        return false;
    }

    //auto* db = new QSqlDatabase {QSqlDatabase::addDatabase(driver)};
    database.reset(new QSqlDatabase {QSqlDatabase::addDatabase(driver)});
    //укажем имя БД
    database->setDatabaseName(dbPath);//database.setDatabaseName(dbName);

    //Открытие базы данных
    qDebug() << "Database name: " << database->databaseName();
    if (!database->open())
    {
        state = DBState::ERROR_OPENING;
        qCritical() << "Error in opening DB" << database->databaseName()
                    << "reason " << database->lastError().text();
        return false;
    }
    return setupTables();

}

bool ConnectionManager::ConnectionManagerPrivate::setupWorkspace()
{
#ifdef BUILD_TESTS
    static const QString databaseName {"TestDB"};
#else
    static const QString databaseName {"ContactsDB"};
#endif

    //определим куда мы положим БД
    const QString location {QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)};
    //Добавим информацию про мя БД и расширение исполняемого файла БД
    const QString fullPath {location + "/" + databaseName + ".db"};
    //сохраняем
    dbPath = fullPath; //dbPath = fullPath.toStdString();
    //создаём сам файл
    QDir dbDirectory {location};
    if (!dbDirectory.exists()) //проверка на наличия папки
    {
        const bool creationResult {dbDirectory.mkpath(location)};
        qInfo() << "DB directory not  exist, creating result: " << creationResult;
    }

    qDebug() << "Data path: " << fullPath;
    return dbDirectory.exists();

}

bool ConnectionManager::ConnectionManagerPrivate::setupTables()
{
    bool result {true};
    std::vector<QSqlQuery> creationQueries {
        QSqlQuery {
            "CREATE TABLE IF NOT EXISTS Contacts"
            "("
            "Name TEXT,"
            "Surname TEXT,"
            "PhoneNumber TEXT"
            "UNIQUE(Name, Surname)"
            ")"
        }
    };

    for (auto& query : creationQueries)
    {
        if (!query.exec())
        {
            result = false;
            state = DBState::ERROR_TABLES;
            qWarning() << "Table creation failed. Reason: " <<
                          query.lastError() << "; Query: " << query.lastQuery();
        }
        else {
            qInfo() << "Table successfully created! Query: \n" << query.lastQuery();
        }
    }

    return result;
}

}
