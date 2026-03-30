#ifndef UPDATE_H
#define UPDATE_H

#include <QVariantMap>

#include "icommand.h"

class Update : public ICommand {
public:
    Update() = default;
    ~Update() = default;

    //method for a update
    QByteArray exec(QVariantMap&&) override;
};

#endif // UPDATE_H

