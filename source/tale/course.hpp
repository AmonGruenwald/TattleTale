#ifndef TALE_COURSE_H
#define TALE_COURSE_H

#include <string>

namespace tale
{
    /**
     * @brief Course managing actors
     */
    class Course
    {
    public:
        std::string name_;
        const size_t id_;
        Course(size_t id, std::string name_);
    };

} // namespace tale
#endif // TALE_COURSE_H