#ifndef SESSION_H
#define SESSION_H

#include "../model/user.h"

class Session {
public:
    static Session& instance();

    void setCurrentUser(const User& user);
    User currentUser() const;
    void logout();
    bool isAuthenticated() const;

private:
    Session() = default;
    ~Session() = default;
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    User m_currentUser;
    bool m_isAuthenticated = false;
};

#endif // SESSION_H
