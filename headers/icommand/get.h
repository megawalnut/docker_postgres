#ifndef GET_H
#define GET_H

#include <QVariantMap>

#include "icommand.h"

class Get : public ICommand {
public:
    Get();
    ~Get() override;

    //method for a get
    void exec(QVariantMap&&) override;
};

#endif // GET_H


