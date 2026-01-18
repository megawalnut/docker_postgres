#ifndef PULL_H
#define PULL_H

#include <QVariantMap>

#include "icommand.h"

class Pull : public ICommand {
public:
    Pull();
    ~Pull() override;

    //method for a pull
    QByteArray exec(QVariantMap&&) override;
};

#endif // PULL_H

