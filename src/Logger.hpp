#pragma once

// #define DEBUG

#ifdef DEBUG
    #define log_out std::cerr
#else
    #include <ostream>
    #include <sstream>

    // Creating empty buffer
    class MyBuf : public std::stringbuf
    {
        public:
            virtual int sync() { return 0; }
    };
    static MyBuf buffer;

    static std::ostream log_out(&buffer); 
    // #define log_out {}
#endif  // DEBUG
