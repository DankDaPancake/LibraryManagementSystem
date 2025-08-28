#pragma once
#include "AppState.hpp"
#include "core/User.hpp"
#include "services/LibraryManager.hpp"

struct UIContext {
    AppState state = AppState::Login;
    User     curUser;
    LibraryManager& mgr = LibraryManager::getInstance();
};
