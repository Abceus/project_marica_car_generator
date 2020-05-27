#pragma once

struct Version
{
    int major;
    int minor;
    int patch;
};

inline bool operator< (const Version& lhs, const Version& rhs)
{
    if(lhs.major != rhs.major) return lhs.major < rhs.major;
    if(lhs.minor != rhs.minor) return lhs.minor < rhs.minor;
    if(lhs.patch != rhs.patch) return lhs.patch < rhs.patch;
}

inline bool operator==(const Version& lhs, const Version& rhs)
{
    return lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.patch == rhs.patch;
}
