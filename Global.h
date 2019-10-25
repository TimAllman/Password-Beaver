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
#ifndef GLOBAL_H
#define GLOBAL_H

#include  <QString>

/**
 * @file Global.h
 * Contains compile time information needed throughout the program.
 */

namespace Global
{

///@{
/** Used by `QSettings` to create the configuration file and its path. */
const QString ORGANIZATION_NAME("Brasscats");
const QString ORGANIZATION_DOMAIN("brasscats.ca");
const QString APPLICATION_NAME("Password Beaver");
///@}

/// The home page for this application.
const QString APPLICATION_URL("https://github.com/TimAllman/Password-Beaver");

const int MIN_POOL_LENGTH = 8;
const int MIN_PW_LENGTH = 8;
const int MAX_PW_LENGTH = 30;
const int DEFAULT_PW_LENGTH = 16;


} // namespace

#endif // GLOBAL_H
