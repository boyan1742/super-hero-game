#ifndef SUPERHEROES_INDEXOUTOFRANGEEXCEPTION_H
#define SUPERHEROES_INDEXOUTOFRANGEEXCEPTION_H

#include <stdexcept>

class IndexOutOfRangeException : public std::exception
{
    char* m_msg;
public:
    IndexOutOfRangeException();
    explicit IndexOutOfRangeException(const char* msg);

    const char* what() const override;
};


#endif //SUPERHEROES_INDEXOUTOFRANGEEXCEPTION_H
