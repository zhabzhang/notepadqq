#define NQQ_DECLARE_EXTENSION_METHOD(method_name) \
    StubReturnValue method_name(const QJsonArray &); \
    bool __nqqmeta__##method_name = registerMethod(QString(#method_name), \
              [&](const QJsonArray &args) -> Stub::StubReturnValue { \
        return method_name(args); \
    });

#define NQQ_DEFINE_EXTENSION_METHOD(class_name, method_name, args_name) \
    Stub::StubReturnValue class_name::method_name(const QJsonArray &args_name)

#define NQQ_STUB_NAME(stub_name) \
    inline static QString stubName() { return stub_name; } \
    inline QString stubName_() const { return stubName(); }

#ifndef EXTENSIONS_STUBS_STUB_H
#define EXTENSIONS_STUBS_STUB_H

#include <QObject>
#include <QWeakPointer>
#include <QJsonValue>
#include <functional>
#include <QHash>

namespace Extensions {

    class RuntimeSupport;

    namespace Stubs {

        class Stub : public QObject
        {
            Q_OBJECT
        public:
            explicit Stub(RuntimeSupport *rts);
            explicit Stub(const QWeakPointer<QObject> &object, RuntimeSupport *rts);
            explicit Stub(const QSharedPointer<QObject> &object, RuntimeSupport *rts);
            explicit Stub(QObject *object, RuntimeSupport *rts);
            virtual ~Stub() = 0;

            struct StubReturnValue {
                QJsonValue result;
                QJsonValue error;
            };

            enum class PointerType {
                DETACHED,
                WEAK_POINTER,
                SHARED_POINTER,
                UNMANAGED_POINTER
            };

            bool invoke(const QString &method, StubReturnValue &ret, const QJsonArray &args);
            virtual QString stubName_() const = 0;

            bool operator ==(const Stub &other) const;
            bool operator !=(const Stub &other) const;
        signals:

        public slots:

        protected:
            QWeakPointer<QObject> objectWeakPtr();
            QSharedPointer<QObject> objectSharedPtr();
            QObject *objectUnmanagedPtr();
            PointerType pointerType();

            bool registerMethod(const QString &methodName, std::function<StubReturnValue (const QJsonArray &)> method);
            RuntimeSupport *runtimeSupport();

        private:
            RuntimeSupport *m_rts;
            PointerType m_pointerType;
            QWeakPointer<QObject> m_weakPointer;
            QSharedPointer<QObject> m_sharedPointer;
            QObject *m_unmanagedPointer = nullptr;
            QHash<QString, std::function<StubReturnValue (const QJsonArray &)>> m_methods;
        };

    }
}

#endif // EXTENSIONS_STUBS_STUB_H
