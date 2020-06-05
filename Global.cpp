/*
 * Copyright © 2019 Tim Allman
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Global.h"

///@{
/** Used by `QSettings` to create the configuration file and its path. */
QString const Global::ORGANIZATION_NAME = "Brasscats";
QString const Global::ORGANIZATION_DOMAIN = "brasscats.ca";
QString const Global::APPLICATION_NAME = "Password Beaver";
///@}

/// The home page for this application.
QString const Global::APPLICATION_URL = "https://github.com/TimAllman/Password-Beaver";

/// Minimum character pool length.
int const Global::MIN_POOL_LENGTH = 8;

int const Global::MIN_PW_LENGTH = 8;    ///< Minimum password length.
int const Global::MAX_PW_LENGTH = 99;   ///< Maximum password length.

/// Default password length.
int const Global::DEFAULT_PW_LENGTH = 16;
