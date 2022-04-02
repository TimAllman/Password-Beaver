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
 * Contains information needed throughout the program.
 */

struct Global
{
///@{
/// Version tag. Be sure to set this for each release.
    static QString const VERSION;

/** Used by `QSettings` to create the configuration file and its path. */
    static QString const ORGANIZATION_NAME;
    static QString const ORGANIZATION_DOMAIN;
    static QString const APPLICATION_NAME;
///@}

/// The home page for this application.
    static QString const APPLICATION_URL;

/// Minimum character pool length.
    static int const MIN_POOL_LENGTH;
    static int const MIN_PW_LENGTH;    ///< Minimum password length.
    static int const MAX_PW_LENGTH;   ///< Maximum password length.

/// Default password length.
    static int const DEFAULT_PW_LENGTH;

};

#endif // GLOBAL_H
