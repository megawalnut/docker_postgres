#ifndef EDIT_H
#define EDIT_H

#include <QVariantMap>

#include "icommand.h"

class Edit : public ICommand {
public:
    Edit();
    ~Edit() override;

    //method for a edit
    QVariant exec(QVariantMap&&) override;
};

#endif // EDIT_H

