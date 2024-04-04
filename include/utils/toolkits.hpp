#ifndef MUSE_SIMULATOR_TOOLKITS_HPP
#define MUSE_SIMULATOR_TOOLKITS_HPP

#ifndef _WIN32
    #define SIMULATOR_CPP_WIN_API
#else
    #ifdef simulator_EXPORTS
        #define SIMULATOR_CPP_WIN_API __declspec(dllexport)   //库项目调用
    #else
        #define SIMULATOR_CPP_WIN_API __declspec(dllimport)  //调用库库项目调用
    #endif
#endif

#endif //MUSE_SIMULATOR_TOOLKITS_HPP
