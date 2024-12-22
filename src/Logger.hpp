#pragma once

// #define DEBUG

#ifdef DEBUG
    #define log_out std::cerr
#else
    #include <ostream>
    #include <sstream>

    class MyBuf : public std::stringbuf
    {
    public:
        virtual int sync() {
            // add this->str() to database here
            // (optionally clear buffer afterwards)
        }
    };
    static MyBuf buffer;

    static std::ostream log_out(&buffer); 
    // #define log_out {}
#endif  // DEBUG