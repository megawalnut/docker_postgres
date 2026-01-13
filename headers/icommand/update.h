#ifndef UPDATE_H
#define UPDATE_H

#include <QVariantMap>

#include "icommand.h"

class Update : public ICommand {
public:
    Update();
    ~Update() override;

    //method for a update
    QVariant exec(QVariantMap&&) override;
};

#endif // UPDATE_H
