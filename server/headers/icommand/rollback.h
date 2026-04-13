#ifndef GET_H
#define GET_H

#include <QVariantMap>

#include "icommand.h"

class Get : public ICommand {
public:
    Get() = default;
    ~Get() = default;

    //method for a get
    QByteArray exec(QVariantMap&&) override;
};

#endif // GET_H


