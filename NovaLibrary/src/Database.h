//============================================================================
// Name        : Database.h
// Copyright   : DataSoft Corporation 2011-2013
//	Nova is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   Nova is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with Nova.  If not, see <http://www.gnu.org/licenses/>.
// Description : Wrapper for adding entries to the SQL database
//============================================================================/*

#ifndef DATABASE_H_
#define DATABASE_H_

#include "Suspect.h"
#include "FeatureSet.h"

#include <string>
#include <sqlite3.h>
#include <stdexcept>

namespace Nova
{

class DatabaseException : public std::runtime_error
{
public:
	DatabaseException(const string& errorcode)
	: runtime_error("Database error code: " + errorcode) {};

};

class Database
{
public:
	Database(std::string databaseFile = "");
	~Database();

	bool Connect();
	bool Disconnect();

	void InsertSuspectHostileAlert(Suspect *suspect);

	void InsertSuspect(Suspect *suspect);
	void ResetPassword();

	static int callback(void *NotUsed, int argc, char **argv, char **azColName);


private:
	std::string m_databaseFile;

	sqlite3 *db;

	sqlite3_stmt * insertSuspect;
};

} /* namespace Nova */
#endif /* DATABASE_H_ */
