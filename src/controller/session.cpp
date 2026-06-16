#include "session.h"

Session& Session::instance() {
    static Session inst;
    return inst;
}

void Session::setCurrentUser(const User& user) {
    m_currentUser = user;
    m_isAuthenticated = true;
}

User Session::currentUser() const {
    return m_currentUser;
}

void Session::logout() {
    m_currentUser = User();
    m_isAuthenticated = false;
}

bool Session::isAuthenticated() const {
    return m_isAuthenticated;
}
