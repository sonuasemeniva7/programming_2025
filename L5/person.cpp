//person.cpp
#include "person.h"
#include "form.h"

void RuP::Craft(int idx, QWidget* parent)
{
    Form* f = new Form(this, idx, parent);
    f->setAttribute(Qt::WA_DeleteOnClose);
    f->show();
}

void AmP::Craft(int idx, QWidget* parent)
{
    Form* f = new Form(this, idx, parent);
    f->setAttribute(Qt::WA_DeleteOnClose);
    f->show();
}
